#include "Portfolio.h"
#include "Exceptions.h"

void Portfolio::addPosition(Instrument* instrument, int qty, double price) {
    auto it = positions.find(instrument->getSymbol());
    if (it != positions.end()) {
        it->second.addShares(qty, price);
    } else {
        positions.emplace(instrument->getSymbol(), Position(instrument, qty, price));
    }
}

void Portfolio::removePosition(Instrument* instrument, int qty) {
    auto it = positions.find(instrument->getSymbol());
    if (it == positions.end())
        throw InsufficientSharesException("No position for " + instrument->getSymbol());
    it->second.removeShares(qty);
    if (it->second.getQuantity() == 0)
        positions.erase(it);
}

bool Portfolio::hasPosition(const std::string& symbol) const {
    return positions.count(symbol) > 0;
}

int Portfolio::getQuantity(const std::string& symbol) const {
    auto it = positions.find(symbol);
    return (it != positions.end()) ? it->second.getQuantity() : 0;
}

double Portfolio::getTotalMarketValue() const {
    double total = 0;
    for (const auto& [sym, pos] : positions)
        total += pos.getMarketValue();
    return total;
}

double Portfolio::getTotalUnrealizedPnL() const {
    double total = 0;
    for (const auto& [sym, pos] : positions)
        total += pos.getUnrealizedPnL();
    return total;
}

const std::map<std::string, Position>& Portfolio::getPositions() const {
    return positions;
}

std::ostream& operator<<(std::ostream& os, const Portfolio& portfolio) {
    if (portfolio.positions.empty()) {
        os << "  (empty portfolio)";
        return os;
    }
    for (const auto& [sym, pos] : portfolio.positions)
        os << "  " << pos << "\n";
    os << "  Total Market Value: $" << portfolio.getTotalMarketValue()
       << " | Total Unrealized P/L: $" << portfolio.getTotalUnrealizedPnL();
    return os;
}
