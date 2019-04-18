#pragma once
#include <chrono>
#include <mutex>

template<typename T>
class msg_queue
{
public:
	~msg_queue() {}

	void post( T&& msg )
	{
		std::lock_guard<std::mutex> lock( mtx );
		queue.push_front( std::move( msg ) );
		cond.notify_one();
	}

	bool wait_for( T& msg, std::chrono::milliseconds time )
	{
		std::unique_lock<std::mutex> lock( mtx );
		bool b = cond.wait_for( lock, time, [ this ] { return !queue.empty(); } );
		if ( b )
		{
			msg = std::move( queue.back() );
			queue.pop_back();
			return true;
		}
		else
		{
			return false;
		}
	}

	unsigned int size()
	{
		std::lock_guard<std::mutex> lock( mtx );
		return queue.size();
	}

	void clear()
	{
		std::lock_guard<std::mutex> lock( mtx );
		queue.clear();
	}

private:
	std::deque<T> queue;
	std::condition_variable cond;
	std::mutex mtx;
};
