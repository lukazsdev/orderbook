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