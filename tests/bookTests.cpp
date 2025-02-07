#include "gtest/gtest.h"

#include "book.h"

TEST(OrderBookTests, BasicMatchingTest) {
    Book orderBook;
    orderBook.StartEngine();

    auto buyOrder = std::make_shared<Order>(OrderType::Limit, Side::Buy, 1, 100, 10);
    auto sellOrder = std::make_shared<Order>(OrderType::Limit, Side::Sell, 2, 100, 10);

    orderBook.SubmitOrder(buyOrder);
    orderBook.SubmitOrder(sellOrder);

    orderBook.WaitForProcessing(); // Ensure all orders are processed

    EXPECT_EQ(orderBook.Size(), 0); // Both orders should be matched and removed
}

TEST(OrderBookTests, PartialMatchingTest) {
    Book orderBook;
    orderBook.StartEngine();

    auto buyOrder = std::make_shared<Order>(OrderType::Limit, Side::Buy, 1, 100, 10);
    auto sellOrder = std::make_shared<Order>(OrderType::Limit, Side::Sell, 2, 100, 5); // Only 5 available

    orderBook.SubmitOrder(buyOrder);
    orderBook.SubmitOrder(sellOrder);

    orderBook.WaitForProcessing();

    EXPECT_EQ(orderBook.Size(), 1); // Remaining buy order should still be in book
    EXPECT_EQ(buyOrder->GetQuantity(), 5); // 5 should be unfilled
}

TEST(OrderBookTests, MultithreadedSubmissionTest) {
    Book orderBook;
    orderBook.StartEngine();

    constexpr int numThreads = 10;
    constexpr int ordersPerThread = 10;

    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back([&orderBook, t]() {
            for (int i = 0; i < ordersPerThread; ++i) {
                auto order = std::make_shared<Order>(
                    OrderType::Limit,
                    (t % 2 == 0 ? Side::Buy : Side::Sell), // Alternate buy/sell per thread
                    t * ordersPerThread + i,
                    (t % 2 == 0 ? 100 : 101), // Buys at 100, Sells at 101 (no immediate match)
                    1);
                orderBook.SubmitOrder(order);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    orderBook.WaitForProcessing();

    // Since Buy 100 does NOT match Sell 101, all orders remain in the book
    EXPECT_EQ(orderBook.Size(), numThreads * ordersPerThread);
}