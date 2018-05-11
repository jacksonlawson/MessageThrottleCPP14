
#include "Throttle.hpp"

void testRun(int throttle_rate, int interval) {
    std::vector<int64_t> timestamps;

    Throttle t(throttle_rate, interval);

    for (int i = 0; i < throttle_rate * 10; ++i) {
        while (!t.trySend()) {
            auto timeInMS = std::chrono::system_clock::now().time_since_epoch();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(timeInMS).count() << " -> waiting" << std::endl;
            t.wait();
            timeInMS = std::chrono::system_clock::now().time_since_epoch();
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(timeInMS).count() << "-> done waiting" << std::endl;

            // std::this_thread::sleep_for(std::chrono::duration<long, std::milli>(1000));
        }
        auto timeInMS = std::chrono::system_clock::now().time_since_epoch();
        auto durams = std::chrono::duration_cast<std::chrono::milliseconds>(timeInMS).count();
        // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(timeInMS).count() << " -> consumed" << std::endl;
        timestamps.push_back(durams);
    }

    int count = 0;
    int64_t startts = 0, starttsms = 0;
    for (auto iter = timestamps.begin(); iter != timestamps.end(); ++iter) {
        auto ts = *iter / 1000;
        if (startts == 0) {
            starttsms = *iter;
            startts = ts;
        }

        if (*iter- starttsms < interval * 1000) {
            count++;
        }
        else {
            std::cout << count << "-> orders sent in interval - " << starttsms << " : " << *iter << std::endl;
            startts = ts;
            starttsms = *iter;
            count = 1;
        }
    }
    if (count > 0) {
        std::cout << count << "-> orders sent" << std::endl;
    }
}

int main() 
{
    testRun(10, 1);
    std::cout << std::endl;
    
    testRun(20, 1);
    std::cout << std::endl;

    testRun(35, 1);
    std::cout << std::endl;

    testRun(50, 1);
    std::cout << std::endl;

    testRun(100, 1);


    getchar();
    return 0;
}