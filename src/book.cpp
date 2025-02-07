#include "book.h"

#include <iostream>

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


