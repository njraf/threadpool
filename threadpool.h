#pragma once

#include <condition_variable>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <functional>
#include <vector>

/** Creates a pool of reusable threads to avoid the overhead of creating a thread.
 * Threads are automatically cleaned up in the destructor when the ThreadPool object is destroyed.
 * The destructor will hang if a thread is not able to join.
 */
class ThreadPool {
public:
	ThreadPool(unsigned int threadCount);
	~ThreadPool();

	/** Enqueues tasks that the thread pool will execute in order as threads become free.
	 *
	 * @param task A function to execute in a thread
	 */
	void enqueueTask(std::function<void()> &&task);

	/** Sets the ThreadPool running state to false and waits for all active threads to be joined.
	 *
	 * @param completeTasks Whether to wait for the task queue to empty before joining all threads
	 */
	void joinAll(bool completeTasks = false);

	/** Clears the task queue.
	 */
	void clearTasks();

private:
	unsigned int numThreads;
	std::atomic<bool> running; //NOTE: condition_variable requires atomic variables to be changed behind a mutex. this could be non-atomic and still be correct.
	std::mutex mut;
	std::condition_variable cv_run;
	std::condition_variable cv_join;
	std::queue<std::function<void()>> tasks;
	std::vector<std::thread> threads;

	/** A function responsible for waiting for and running tasks. It is run in each thread created in the constructor.
	 */
	void superTask();
};
