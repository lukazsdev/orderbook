#ifndef BOOK_H
#define BOOK_H

#include <condition_variable>
#include <list>
#include <map>
#include <memory>
#include <queue>
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
    Book();
    ~Book();

    void StartEngine();
    void StopEngine();
    void SubmitOrder(const std::shared_ptr<Order>& order); // thread-safe order submitting
    void ProcessOrders(); // background method that processes queue orders (runs in separate thread)
    void WaitForProcessing(); // Wait for all orders to be processed (for testing)

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

    // Thread-safe order queue
    std::queue<std::shared_ptr<Order>> orderQueue_;

    // Synchronization
    mutable std::mutex queueMutex_; // protect access to orderQueue
    std::condition_variable cv_;
    bool running_;
    std::thread matchingThread_;

    std::atomic<std::size_t> pendingOrders_{0}; // atomic counter
    std::condition_variable cvFinished_;
};



#endif //BOOK_H
