#ifndef TRADE_H
#define TRADE_H

#include <cstdint>

struct TradeData {
    std::uint32_t orderId_;
    std::uint32_t price_;
    std::uint32_t quantity_;
};

class Trade {
public:
    Trade(const TradeData& bidTrade, const TradeData& askTrade);

    [[nodiscard]] const TradeData& GetBidTrade() const;
    [[nodiscard]] const TradeData& GetAskTrade() const;

private:
    TradeData bidTrade_;
    TradeData askTrade_;
};

#endif //TRADE_H
