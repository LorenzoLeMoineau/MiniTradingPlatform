#pragma once
#include "Instrument.h"

class Position {
private:
    Instrument* instrument;
    int quantity;
    double avgAcquisitionPrice;

public:
    Position(Instrument* instrument, int quantity, double avgPrice);

    Instrument* getInstrument() const;
    int getQuantity() const;
    double getAvgAcquisitionPrice() const;
    double getMarketValue() const;
    double getUnrealizedPnL() const;

    Position& operator+=(int qty);
    Position& operator-=(int qty);
    friend std::ostream& operator<<(std::ostream& os, const Position& pos);

    void addShares(int qty, double price);
    void removeShares(int qty);
};
