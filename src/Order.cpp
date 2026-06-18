#include "Order.h"

int Order::nextId = 1;

Order::Order(Client* client, Instrument* instrument, OrderSide side, int quantity)
    : id(nextId++), client(client), instrument(instrument),
      side(side), quantity(quantity), remainingQty(quantity), status(OrderStatus::PENDING) {}

int Order::getId() const { return id; }
Client* Order::getClient() const { return client; }
Instrument* Order::getInstrument() const { return instrument; }
OrderSide Order::getSide() const { return side; }
int Order::getQuantity() const { return quantity; }
int Order::getRemainingQty() const { return remainingQty; }
OrderStatus Order::getStatus() const { return status; }

void Order::fill(int qty) {
    remainingQty -= qty;
    if (remainingQty <= 0) {
        remainingQty = 0;
        status = OrderStatus::FILLED;
    } else {
        status = OrderStatus::PARTIAL;
    }
}

bool Order::isActive() const {
    return status == OrderStatus::PENDING || status == OrderStatus::PARTIAL;
}

bool Order::operator<(const Order& other) const {
    return id < other.id;
}

bool Order::operator==(const Order& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    order.display();
    return os;
}

void Order::display() const {
    std::string sideStr = (side == OrderSide::BUY) ? "BUY" : "SELL";
    std::string statusStr;
    switch (status) {
        case OrderStatus::PENDING:   statusStr = "PENDING"; break;
        case OrderStatus::PARTIAL:   statusStr = "PARTIAL"; break;
        case OrderStatus::FILLED:    statusStr = "FILLED"; break;
        case OrderStatus::CANCELLED: statusStr = "CANCELLED"; break;
    }
    std::cout << "Order #" << id << " [" << getType() << "] " << sideStr
              << " " << remainingQty << "/" << quantity
              << " " << instrument->getSymbol()
              << " @ $" << getPrice()
              << " | Client: " << client->getName()
              << " | Status: " << statusStr;
}
