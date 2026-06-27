#include "threadpool.h"
#include <iostream>

ThreadPool::ThreadPool(unsigned int threadCount)
	: numThreads{threadCount}
	, running{true}
{
	for (int i = 0; i < numThreads; i++) {
		std::thread thread(&ThreadPool::superTask, this);
		threads.push_back(std::move(thread));
	}
}

ThreadPool::~ThreadPool()
{
	joinAll();
}

void ThreadPool::enqueueTask(std::function<void()> &&task)
{
	std::unique_lock lock(mut);
	if (!running) {
		std::cout << "Failed to enqueue because threadpool is not running" << std::endl;
		return;
	}
	tasks.push(std::move(task));
	cv_run.notify_one();
}

void ThreadPool::joinAll(bool completeTasks)
{
	std::unique_lock lock(mut);
	if (completeTasks) {
		cv_join.wait(lock, [this](){ return tasks.empty(); });
	}
	running = false;
	lock.unlock();
	cv_run.notify_all();

	for (std::thread &thread : threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
}

void ThreadPool::clearTasks()
{
	std::unique_lock lock(mut);
	tasks = {};
}

void ThreadPool::superTask()
{
	do {
		std::unique_lock lock(mut);
		if (!tasks.empty()) {
			const std::function<void()> task = tasks.front();
			tasks.pop();
			lock.unlock();
			task();
		} else {
			cv_join.notify_one();
			cv_run.wait(lock, [this](){ return !running || !tasks.empty(); });
		}
	} while(running); //NOTE: `running` is not protected by the `mut` mutex and may cause problems
}

