#include "LimitBuyOrder.h"

LimitBuyOrder::LimitBuyOrder(Client* client, Instrument* instrument, int quantity, double limitPrice)
    : LimitOrder(client, instrument, OrderSide::BUY, quantity, limitPrice) {}

std::string LimitBuyOrder::getType() const { return "LIMIT BUY"; }
