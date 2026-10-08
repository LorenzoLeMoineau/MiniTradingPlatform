#include "MarketOrder.h"

MarketOrder::MarketOrder(Client* client, Instrument* instrument, OrderSide side, int quantity)
    : Order(client, instrument, side, quantity) {}

double MarketOrder::getPrice() const {
    return instrument->getMarketPrice();
}

std::string MarketOrder::getType() const {
    return "MARKET";
}
