#pragma once
#include "Position.h"
#include <map>
#include <string>

class Portfolio {
private:
    std::map<std::string, Position> positions; // symbol -> Position

public:
    void addPosition(Instrument* instrument, int qty, double price);
    void removePosition(Instrument* instrument, int qty);
    bool hasPosition(const std::string& symbol) const;
    int getQuantity(const std::string& symbol) const;

    double getTotalMarketValue() const;
    double getTotalUnrealizedPnL() const;

    const std::map<std::string, Position>& getPositions() const;
    friend std::ostream& operator<<(std::ostream& os, const Portfolio& portfolio);
};
