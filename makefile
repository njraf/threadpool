all: static shared

sources = threadpool.cpp

static: 
	@ar rcs libthreadpool.a $(sources)

shared:
	@g++ -shared -fPIC $(sources) -o libthreadpool.so


clean:
	@rm libthreadpool.a
	@rm libthreadpool.so
