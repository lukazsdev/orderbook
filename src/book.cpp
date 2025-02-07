#include "book.h"

#include <iostream>

Book::Book() : running_(false) {}

Book::~Book() {
    StopEngine();
}

void Book::StartEngine() {
    running_ = true;
    matchingThread_ = std::thread(&Book::ProcessOrders, this);
}

void Book::StopEngine() {
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        running_ = false;
    }
    cv_.notify_one();

    if (matchingThread_.joinable()) {
        matchingThread_.join();
    }
}

void Book::SubmitOrder(const std::shared_ptr<Order> &order) {
    {
        std::lock_guard<std::mutex> lock(queueMutex_);
        orderQueue_.push(order);
    }
    ++pendingOrders_;
    cv_.notify_one();
}

void Book::ProcessOrders() {
    while (true) {
        std::vector<std::shared_ptr<Order>> batch;

        {
            std::unique_lock<std::mutex> lock(queueMutex_);
            cv_.wait(lock, [this] { return !orderQueue_.empty() || !running_; });

            if (!running_ && orderQueue_.empty()) {
                break;
            }

            while (!orderQueue_.empty() && batch.size() < 100) {
                batch.push_back(orderQueue_.front());
                orderQueue_.pop();
            }
        }

        for (auto& order : batch) {
            AddOrder(order);
        }

        if (pendingOrders_.fetch_sub(batch.size()) == batch.size()) {
            cvFinished_.notify_one();
        }
    }
}

// Wait for all orders to be processed (used in tests)
void Book::WaitForProcessing() {
    std::unique_lock<std::mutex> lock(queueMutex_);
    cvFinished_.wait(lock, [this] { return pendingOrders_.load() == 0; });
}


std::vector<Trade> Book::MatchOrders() {
    std::vector<Trade> trades;

    while (true) {
        if (bids_.empty() || asks_.empty()) {
            break;
        }

        auto& [bestBidPrice, bestBids] = *bids_.begin();
        auto& [bestAskPrice, bestAsks] = *asks_.begin();

        if (bestBidPrice < bestAskPrice) {
            break;
        }

        while (!bestBids.empty() && !bestAsks.empty()) {
            const auto bid = bestBids.front();
            const auto ask = bestAsks.front();

            auto quantity = std::min(bid->GetQuantity(), ask->GetQuantity());

            bid->Fill(quantity);
            ask->Fill(quantity);

            if (bid->IsFilled()) {
                bestBids.pop_front();
                orders_.erase(bid->GetOrderId());
            }

            if (ask->IsFilled()) {
                bestAsks.pop_front();
                orders_.erase(ask->GetOrderId());
            }

            trades.emplace_back(
                TradeData{bid->GetOrderId(), bid->GetPrice(), quantity},
                TradeData{ask->GetOrderId(), ask->GetPrice(), quantity}
            );
        }

        if (bestBids.empty()) {
            bids_.erase(bestBidPrice);
        }

        if (bestAsks.empty()) {
            asks_.erase(bestAskPrice);
        }
    }

    return trades;
}

std::vector<Trade> Book::AddOrder(const std::shared_ptr<Order>& order) {
    if (orders_.contains(order->GetOrderId())) {
        return {};
    }

    std::list<std::shared_ptr<Order>>::iterator it;

    if (order->GetSide() == Side::Buy) {
        auto& orders = bids_[order->GetPrice()];
        orders.push_back(order);
        it = std::next(orders.end(), -1);
    }
    else {
        auto& orders = asks_[order->GetPrice()];
        orders.push_back(order);
        it = std::next(orders.end(), -1);
    }

    orders_.insert({order->GetOrderId(), OrderEntry{order, it}});
    return MatchOrders();
}

std::size_t Book::Size() const {
    return orders_.size();
}


void Book::Print() const {
    std::cout << "Orderbook:\n";
    std::cout << "Bids:\n";
    for (const auto& [bidPrice, bids] : bids_) {
        for (const auto& orderPtr : bids) {
            std::cout << "id-" << orderPtr->GetOrderId() << ": " << orderPtr->GetQuantity() << " @ $" << bidPrice << "\n";
        }
    }

    std::cout << "\nAsks:\n";
    for (const auto& [askPrice, asks] : asks_) {
        for (const auto& orderPtr : asks) {
            std::cout << "id-" << orderPtr->GetOrderId() << ": " << orderPtr->GetQuantity() << " @ $" << askPrice << "\n";
        }
    }

    std::cout << "\n";
}


