#include "trade.h"

Trade::Trade(const TradeData &bidTrade, const TradeData &askTrade) : bidTrade_(bidTrade), askTrade_(askTrade) {}

const TradeData &Trade::GetBidTrade() const {
    return bidTrade_;
}

const TradeData &Trade::GetAskTrade() const {
    return askTrade_;
}

bool Trade::operator==(const Trade& other) const {
    return bidTrade_ == other.bidTrade_ &&
           askTrade_ == other.askTrade_;
}

bool TradeData::operator==(const TradeData& other) const {
    return orderId_ == other.orderId_ &&
           price_ == other.price_ &&
           quantity_ == other.quantity_;
}