#pragma once
#ifndef EVENTENGINE_H
#define EVENTENGINE_H

#include<string>
#include<queue>
#include<mutex>
#include<memory>
#include<condition_variable>
#include<map>
#include<thread>
#include<atomic>
#include<chrono>
#include<time.h>
#include "common_fuction.hpp"

template<typename EVENT>
class SynQueue
{
public:
    void push(std::shared_ptr<EVENT>Event) 
	{
        std::unique_lock<std::recursive_mutex>lck(mutex);
        queue.push(Event);
        cv.notify_all();
    }
    std::shared_ptr<Event> take() 
	{
        std::unique_lock<std::recursive_mutex>lck(mutex);
        while (queue.empty()) {
            cv.wait(lck);
        }
        std::shared_ptr<Event>e = queue.front();
        queue.pop();
        return e;
    }

private:
    std::recursive_mutex mutex;
    std::queue<std::shared_ptr<EVENT>> queue;
    std::condition_variable_any cv;
};


class EventEngine
{
public:
    EventEngine();
    ~EventEngine();
    void startEngine();
    void stopEngine();
    void regEvent(const std::string &eventtype,const std::function<void(std::shared_ptr<Event>)> &task);
    void unregEvent(const std::string &eventtype);
    void doTask();
    void put(std::shared_ptr<Event>e);
    void timer();
private:
    std::mutex mutex;
    std::condition_variable cv;
    SynQueue<Event>*event_queue = nullptr;
    std::vector<std::thread*>*task_pool = nullptr;
    std::multimap<std::string, std::function<void(std::shared_ptr<Event>)>>*task_map = nullptr;
    std::thread* timer_thread = nullptr;;
    std::atomic<bool>active;
};

#endif
