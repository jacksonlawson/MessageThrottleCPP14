
#ifndef __THROTTLE_CPP_14__H__
#define __THROTTLE_CPP_14__H__

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>
#include <cstdint>
#include <iostream>

class Throttle
{
public:
    Throttle(int psize, int pinterval) {
        length = psize;
        start = 0; 
        end = 0;
        startTS = endTS = 0;
        interval = pinterval * 1000;
    }

    bool trySend() {
        bool result { false };
        auto duration = std::chrono::system_clock::now().time_since_epoch();
        startTS = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        if (endTS < startTS) {
            used = 0;
            endTS = startTS + interval;
        }

        if (used + 1 <= length) {
            result = true;
            used++;
        }
        
        return result;
    }

    bool send() {
        bool result { trySend() };
        while (!result) {
            wait();
            result = trySend();
        }
        return result;
    }

    void wait() {
        auto pendingMS = endTS - startTS;
        if (pendingMS <= 0) {
            pendingMS = 1;
        }
        std::this_thread::sleep_for(std::chrono::duration<long, std::milli>(pendingMS));
    }

private:
    std::atomic<std::int32_t> start, end, length, used;
    int64_t startTS, endTS;
    int interval;
};

#endif