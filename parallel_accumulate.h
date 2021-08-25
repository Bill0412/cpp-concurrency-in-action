//
// Created by Fenghe Xu on 8/26/21.
//

#ifndef CPP_CONCURRENCY_IN_ACTION_PARALLEL_ACCUMULATE_H
#define CPP_CONCURRENCY_IN_ACTION_PARALLEL_ACCUMULATE_H

#include <numeric>
#include <thread>
#include <algorithm>
#include <vector>
#include <chrono>
#include <iostream>

namespace parallel_accumulate {

    template<typename Iterator, typename T>
    struct accumulate_block
    {
        void operator()(Iterator first, Iterator last, T& result)
        {
            result = std::accumulate(first, last, result);
        }
    };

    template<typename Iterator, typename T>
    T parallel_accumulate(Iterator first, Iterator last, T init)
    {
        unsigned long const length = std::distance(first, last);
        if(!length)
            return init;
        unsigned long long const min_per_thread = 25;
        unsigned long long const max_threads = (length + min_per_thread - 1) / min_per_thread;
        unsigned long const hardware_threads = std::thread::hardware_concurrency();
        unsigned long long const num_threads = std::min((unsigned long long)((hardware_threads!=0)?hardware_threads:2), max_threads);
        unsigned long const block_size = length / num_threads;
        std::vector<T> results(num_threads);
        std::vector<std::thread> threads(num_threads - 1);
        Iterator block_start = first;

        for(unsigned long i = 0; i < (num_threads-1); ++i) {
            Iterator block_end = block_start;
            std::advance(block_end, block_size);
            threads[i] = std::thread(accumulate_block<Iterator, T>(),block_start, block_end, std::ref(results[i]));
            block_start = block_end;
        }

        accumulate_block<Iterator, T>()(block_start, last, results[num_threads-1]);

        for(auto& entry: threads) {
            entry.join();
        }

        return std::accumulate(results.begin(), results.end(), init);
    }

    // To call this function in the main function
    //    parallel_accumulate::test();
    void test() {
        std::vector<int> v;

        for (int i = 0; i < 1e8; ++i) {
            v.push_back(10);
        }
        using namespace std::chrono;
        auto start = high_resolution_clock::now();
        parallel_accumulate(v.begin(), v.end(), 0);
        auto duration = high_resolution_clock::now() - start;
        std::cout << "parallel_accumulate: " << duration.count() / 1000000 << " seconds" << std::endl;

        start = high_resolution_clock::now();
        std::accumulate(v.begin(), v.end(), 0);
        duration = high_resolution_clock::now() - start;
        std::cout << "std::accumulate: " << duration.count() / 1000000 << " seconds" << std::endl;
    }
}


#endif //CPP_CONCURRENCY_IN_ACTION_PARALLEL_ACCUMULATE_H
