#pragma once
#include "LimitOrder.h"

class LimitBuyOrder : public LimitOrder {
public:
    LimitBuyOrder(Client* client, Instrument* instrument, int quantity, double limitPrice);
    std::string getType() const override;
};
