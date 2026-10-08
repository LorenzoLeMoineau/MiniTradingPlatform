#pragma once
#include "Order.h"

class LimitOrder : public Order {
protected:
    double limitPrice;

public:
    LimitOrder(Client* client, Instrument* instrument, OrderSide side, int quantity, double limitPrice);
    double getPrice() const override;
    std::string getType() const override;
};
