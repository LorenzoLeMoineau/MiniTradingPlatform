#include "LimitOrder.h"

LimitOrder::LimitOrder(Client* client, Instrument* instrument, OrderSide side, int quantity, double limitPrice)
    : Order(client, instrument, side, quantity), limitPrice(limitPrice) {}

double LimitOrder::getPrice() const {
    return limitPrice;
}

std::string LimitOrder::getType() const {
    return "LIMIT";
}
