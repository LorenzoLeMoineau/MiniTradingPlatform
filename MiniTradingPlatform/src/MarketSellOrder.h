#pragma once
#include "MarketOrder.h"

class MarketSellOrder : public MarketOrder {
public:
    MarketSellOrder(Client* client, Instrument* instrument, int quantity);
    std::string getType() const override;
};
