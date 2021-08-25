//
// Created by Fenghe Xu on 8/26/21.
//

#ifndef CPP_CONCURRENCY_IN_ACTION_THREAD_IN_CONTAINERS_H
#define CPP_CONCURRENCY_IN_ACTION_THREAD_IN_CONTAINERS_H

#include <thread>
#include <vector>

namespace thread_in_containers {
    int do_work(int a, int b, int k) {
        int ret = 0;
        for(int i = a; i < b; ++i) {
            ret = (ret + i) % k;
        }
        return ret;
    }

    void test() {
        std::vector<std::thread> threads;

        for(unsigned i = 0; i < 20; ++i) {
            threads.push_back(std::move(std::thread(do_work, 1, 1, 1)));
            threads.emplace_back(do_work, 1, 1, 1000);
        }

        for(auto& entry: threads)
            entry.join();
    }
}

#endif //CPP_CONCURRENCY_IN_ACTION_THREAD_IN_CONTAINERS_H
