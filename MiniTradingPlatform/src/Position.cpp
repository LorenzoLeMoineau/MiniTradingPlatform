#include "Position.h"
#include "Exceptions.h"

Position::Position(Instrument* instrument, int quantity, double avgPrice)
    : instrument(instrument), quantity(quantity), avgAcquisitionPrice(avgPrice) {}

Instrument* Position::getInstrument() const { return instrument; }
int Position::getQuantity() const { return quantity; }
double Position::getAvgAcquisitionPrice() const { return avgAcquisitionPrice; }

double Position::getMarketValue() const {
    return quantity * instrument->getMarketPrice();
}

double Position::getUnrealizedPnL() const {
    return quantity * (instrument->getMarketPrice() - avgAcquisitionPrice);
}

void Position::addShares(int qty, double price) {
    double totalCost = avgAcquisitionPrice * quantity + price * qty;
    quantity += qty;
    avgAcquisitionPrice = totalCost / quantity;
}

void Position::removeShares(int qty) {
    if (qty > quantity)
        throw InsufficientSharesException("Cannot remove more shares than owned.");
    quantity -= qty;
}

Position& Position::operator+=(int qty) {
    quantity += qty;
    return *this;
}

Position& Position::operator-=(int qty) {
    if (qty > quantity)
        throw InsufficientSharesException("Cannot remove more shares than owned.");
    quantity -= qty;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    os << *pos.instrument << " | Qty: " << pos.quantity
       << " | Avg: $" << pos.avgAcquisitionPrice
       << " | Market Value: $" << pos.getMarketValue()
       << " | Unrealized P/L: $" << pos.getUnrealizedPnL();
    return os;
}
