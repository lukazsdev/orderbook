#include "trade.h"

Trade::Trade(const TradeData &bidTrade, const TradeData &askTrade) : bidTrade_(bidTrade), askTrade_(askTrade) {}

const TradeData &Trade::GetBidTrade() const {
    return bidTrade_;
}

const TradeData &Trade::GetAskTrade() const {
    return askTrade_;
}
