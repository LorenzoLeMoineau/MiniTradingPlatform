#pragma once
#include "MarketOrder.h"

class MarketBuyOrder : public MarketOrder {
public:
    MarketBuyOrder(Client* client, Instrument* instrument, int quantity);
    std::string getType() const override;
};
