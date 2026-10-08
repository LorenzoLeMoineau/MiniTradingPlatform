#include "MarketBuyOrder.h"

MarketBuyOrder::MarketBuyOrder(Client* client, Instrument* instrument, int quantity)
    : MarketOrder(client, instrument, OrderSide::BUY, quantity) {}

std::string MarketBuyOrder::getType() const { return "MARKET BUY"; }
