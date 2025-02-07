#include "order.h"

#include <format>
#include <stdexcept>

Order::Order(const OrderType &orderType, const Side &side, const std::uint32_t &orderId, const std::uint32_t &price, const std::uint32_t quantity) :
    orderType_(orderType), side_(side), orderId_(orderId), price_(price), quantity_(quantity) {}

OrderType Order::GetOrderType() const {
    return orderType_;
}

Side Order::GetSide() const {
    return side_;
}

std::uint32_t Order::GetPrice() const {
    return price_;
}

std::uint32_t Order::GetQuantity() const {
    return quantity_;
}

std::uint32_t Order::GetOrderId() const {
    return orderId_;
}

void Order::Fill(const std::uint32_t &quantity) {
    if (quantity > GetQuantity()) {
        throw std::logic_error(std::format("Order {} cannot be filled for more than its remaining quantity", GetOrderId()));
    }

    quantity_ -= quantity;
}

bool Order::IsFilled() const {
    return quantity_ == 0;
}

