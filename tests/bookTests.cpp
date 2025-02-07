#include "gtest/gtest.h"

#include "book.h"

TEST(OrderBookTests, AddLimitOrderTests) {
    Book book;

    const std::vector<std::shared_ptr<Order>> initialOrders = {
        std::make_shared<Order>(OrderType::Limit, Side::Buy, 1, 50, 100),
        std::make_shared<Order>(OrderType::Limit, Side::Sell, 2, 51, 100),
        std::make_shared<Order>(OrderType::Limit, Side::Buy, 3, 50, 200)
    };

    for (const auto& order : initialOrders) {
        auto trades = book.AddOrder(order);
        ASSERT_EQ(trades.size(), 0);
    }

    ASSERT_EQ(book.Size(), 3);

    const std::vector<std::shared_ptr<Order>> sellOrders = {
        std::make_shared<Order>(OrderType::Limit, Side::Sell, 4, 49, 50),
        std::make_shared<Order>(OrderType::Limit, Side::Sell, 5, 49, 150),
        std::make_shared<Order>(OrderType::Limit, Side::Sell, 6, 49, 250)
    };

    const std::vector<std::vector<Trade>> expectedTrades = {
        {Trade{TradeData{1, 50, 50}, TradeData{4, 49, 50}}},
        {Trade{TradeData{1, 50, 50}, TradeData{5, 49, 50}},
            Trade{TradeData{3, 50, 100}, TradeData{5, 49, 100}}},
           {Trade{TradeData{3, 50, 100}, TradeData{6, 49, 100}}}
    };

    for (auto i = 0; i < sellOrders.size(); i++) {
        const auto trades = book.AddOrder(sellOrders[i]);
        ASSERT_EQ(trades, expectedTrades[i]);
    }

    ASSERT_EQ(book.Size(), 2);
}

/*

#include <iostream>

#include "src/book.h"

int main() {
    Book book;

    auto order1 = std::make_shared<Order>(OrderType::Limit, Side::Buy, 1, 50, 100);
    auto order2 = std::make_shared<Order>(OrderType::Limit, Side::Sell, 2, 51, 100);
    auto order3 = std::make_shared<Order>(OrderType::Limit, Side::Buy, 3, 50, 200);

    book.AddOrder(order1);
    book.AddOrder(order2);
    book.AddOrder(order3);

    book.Print();

    auto order4 = std::make_shared<Order>(OrderType::Limit, Side::Sell, 4, 49, 50);
    book.AddOrder(order4);

    book.Print();

    auto order5 = std::make_shared<Order>(OrderType::Limit, Side::Sell, 5, 49, 150);
    book.AddOrder(order5);

    book.Print();

    auto order6 = std::make_shared<Order>(OrderType::Limit, Side::Sell, 6, 49, 250);
    book.AddOrder(order6);

    book.Print();
}
 */