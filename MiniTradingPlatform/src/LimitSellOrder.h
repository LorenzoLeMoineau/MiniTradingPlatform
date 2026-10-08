#pragma once
#include "LimitOrder.h"

class LimitSellOrder : public LimitOrder {
public:
    LimitSellOrder(Client* client, Instrument* instrument, int quantity, double limitPrice);
    std::string getType() const override;
};
