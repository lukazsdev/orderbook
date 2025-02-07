#ifndef BOOK_H
#define BOOK_H

#include <list>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "order.h"
#include "trade.h"

struct OrderEntry {
    std::shared_ptr<Order> order_;
    std::list<std::shared_ptr<Order>>::iterator it_;
};

class Book {
public:
    std::vector<Trade> MatchOrders();
    std::vector<Trade> AddOrder(const std::shared_ptr<Order>& order);
    std::size_t Size() const;
    void Print() const;

private:
    // Maps price to vector of orders
    std::map<std::uint32_t, std::list<std::shared_ptr<Order>>, std::greater<>> bids_;
    std::map<std::uint32_t, std::list<std::shared_ptr<Order>>> asks_;

    // Maps OrderId -> OrderEntry
    std::unordered_map<uint32_t, OrderEntry> orders_;
};



#endif //BOOK_H
