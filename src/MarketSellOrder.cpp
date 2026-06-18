#include "MarketSellOrder.h"

MarketSellOrder::MarketSellOrder(Client* client, Instrument* instrument, int quantity)
    : MarketOrder(client, instrument, OrderSide::SELL, quantity) {}

std::string MarketSellOrder::getType() const { return "MARKET SELL"; }
