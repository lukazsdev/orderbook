#ifndef ORDER_H
#define ORDER_H

#include "types.h"
#include <cstdint>

class Order {
public:
    Order(const OrderType& orderType, const Side& side, const std::uint32_t& orderId, const std::uint32_t& price, std::uint32_t quantity);

    [[nodiscard]] OrderType GetOrderType() const;
    [[nodiscard]] Side GetSide() const;
    [[nodiscard]] std::uint32_t GetOrderId() const;
    [[nodiscard]] std::uint32_t GetPrice() const;
    [[nodiscard]] std::uint32_t GetQuantity() const;

    void Fill(const std::uint32_t& quantity);
    bool IsFilled() const;

private:
    OrderType orderType_;
    Side side_;
    std::uint32_t orderId_;
    std::uint32_t price_;
    std::uint32_t quantity_;
};

#endif //ORDER_H
