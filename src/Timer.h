//
// Created by baoyixing on 3/29/21.
//

#ifndef CODE_TIMER_H
#define CODE_TIMER_H

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <thread>


class Timer {
public:
	Timer();
	Timer(Timer& timer);
	~Timer();
	void start(int interval, const std::function<void()>& task);
	static void startOnce(int delay, const std::function<void()>& task);
	void stop();

private:
	std::atomic<bool> _expired {true};
	std::atomic<bool> _try_to_expire {false};
	std::mutex _mutex;
	std::condition_variable _expired_cond;
};


#endif	  //CODE_TIMER_H
