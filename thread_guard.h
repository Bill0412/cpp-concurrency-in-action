//
// Created by Fenghe Xu on 8/26/21.
//

// One of the move support of std::thread is that you can build on the thread_guard class like the following

#ifndef CPP_CONCURRENCY_IN_ACTION_THREAD_GUARD_H
#define CPP_CONCURRENCY_IN_ACTION_THREAD_GUARD_H

#include <thread>

namespace thread_guard {
    class scoped_thread {
        std::thread t;
    public:
        explicit scoped_thread(std::thread t_):
            t(std::move(t_))
        {
            if(!t.joinable())
                throw std::logic_error("No thread");
        }

        ~scoped_thread()
        {
            t.join();
        }

        scoped_thread(scoped_thread const&) = delete;
        scoped_thread& operator=(scoped_thread const&)=delete;
    };


    // The output should be like
    // using a and b inside the main thread
    // using a and b in the scoped thread
    void f() {
        int a = 1, b = 2;
        scoped_thread t(std::thread([&]() {
            int c = 0;
            for(int i = 0; i < 10000; ++i) {
                c = a + b;
            }
            std::cout << "using a and b in the scoped thread\n";
            return a + b;
        }));
        std::cout << "using a and b inside the main thread\n";
    }
}


#endif //CPP_CONCURRENCY_IN_ACTION_THREAD_GUARD_H
