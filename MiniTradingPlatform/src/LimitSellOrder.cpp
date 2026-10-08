#include "LimitSellOrder.h"

LimitSellOrder::LimitSellOrder(Client* client, Instrument* instrument, int quantity, double limitPrice)
    : LimitOrder(client, instrument, OrderSide::SELL, quantity, limitPrice) {}

std::string LimitSellOrder::getType() const { return "LIMIT SELL"; }
