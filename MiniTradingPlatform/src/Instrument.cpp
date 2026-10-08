#include "Instrument.h"

Instrument::Instrument(const std::string& symbol, const std::string& companyName, double marketPrice)
    : symbol(symbol), companyName(companyName), marketPrice(marketPrice) {}

const std::string& Instrument::getSymbol() const { return symbol; }
const std::string& Instrument::getCompanyName() const { return companyName; }
double Instrument::getMarketPrice() const { return marketPrice; }
void Instrument::setMarketPrice(double price) { marketPrice = price; }

bool Instrument::operator==(const Instrument& other) const {
    return symbol == other.symbol;
}

bool Instrument::operator<(const Instrument& other) const {
    return symbol < other.symbol;
}

std::ostream& operator<<(std::ostream& os, const Instrument& instr) {
    os << "[" << instr.symbol << "] " << instr.companyName << " @ $" << instr.marketPrice;
    return os;
}
