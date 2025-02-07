#include "src/book.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>

void RunBenchmark(int numOrders, int numThreads) {
    Book orderBook;
    orderBook.StartEngine();

    std::vector<std::thread> threads;
    auto startTime = std::chrono::high_resolution_clock::now();

    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back([&orderBook, t, numOrders, numThreads]() {
            for (int i = 0; i < numOrders / numThreads; ++i) {
                auto order = std::make_shared<Order>(
                    OrderType::Limit,
                    (t % 2 == 0 ? Side::Buy : Side::Sell), // Alternate buy/sell per thread
                    t * (numOrders / numThreads) + i,
                    100,
                    1);
                orderBook.SubmitOrder(order);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    orderBook.WaitForProcessing();
    orderBook.StopEngine();

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;

    double ordersPerSecond = numOrders / elapsed.count();

    std::cout << "Benchmark Results:\n";
    std::cout << "Total Orders: " << numOrders << "\n";
    std::cout << "Elapsed Time: " << elapsed.count() << " seconds\n";
    std::cout << "Orders per Second: " << ordersPerSecond << "\n";
}

int main() {
    constexpr int NUM_ORDERS = 1'000'000;
    constexpr int NUM_THREADS = 10;

    std::cout << "Running order book benchmark...\n";
    RunBenchmark(NUM_ORDERS, NUM_THREADS);

    return 0;
}