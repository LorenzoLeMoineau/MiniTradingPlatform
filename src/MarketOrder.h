#pragma once
#include "Order.h"

class MarketOrder : public Order {
public:
    MarketOrder(Client* client, Instrument* instrument, OrderSide side, int quantity);
    double getPrice() const override;
    std::string getType() const override;
};
