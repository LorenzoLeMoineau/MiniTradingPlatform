#include "Trade.h"
#include <iomanip>
#include <sstream>

int Trade::nextId = 1;

Trade::Trade(Client* buyer, Client* seller, Instrument* instrument, int quantity, double price)
    : id(nextId++), buyer(buyer), seller(seller),
      instrument(instrument), quantity(quantity), price(price),
      timestamp(std::time(nullptr)) {}

int Trade::getId() const { return id; }
Client* Trade::getBuyer() const { return buyer; }
Client* Trade::getSeller() const { return seller; }
Instrument* Trade::getInstrument() const { return instrument; }
int Trade::getQuantity() const { return quantity; }
double Trade::getPrice() const { return price; }
double Trade::getValue() const { return quantity * price; }
std::time_t Trade::getTimestamp() const { return timestamp; }

bool Trade::operator==(const Trade& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Trade& trade) {
    char buf[20];
    std::tm* tm_info = std::localtime(&trade.timestamp);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    os << "Trade #" << trade.id
       << " | " << trade.instrument->getSymbol()
       << " | Qty: " << trade.quantity
       << " @ $" << trade.price
       << " | Buyer: " << trade.buyer->getName()
       << " | Seller: " << trade.seller->getName()
       << " | Total: $" << trade.getValue()
       << " | " << buf;
    return os;
}
