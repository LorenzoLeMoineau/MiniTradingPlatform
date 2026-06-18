#include "OrderBook.h"
#include <algorithm>
#include <iostream>

OrderBook::OrderBook(const std::string& symbol) : symbol(symbol) {}

OrderBook::~OrderBook() {
    for (Order* o : buyOrders) delete o;
    for (Order* o : sellOrders) delete o;
}

const std::string& OrderBook::getSymbol() const { return symbol; }

void OrderBook::sortBuyOrders() {
    // Highest price first
    std::sort(buyOrders.begin(), buyOrders.end(), [](Order* a, Order* b) {
        return a->getPrice() > b->getPrice();
    });
}

void OrderBook::sortSellOrders() {
    // Lowest price first
    std::sort(sellOrders.begin(), sellOrders.end(), [](Order* a, Order* b) {
        return a->getPrice() < b->getPrice();
    });
}

void OrderBook::addOrder(Order* order) {
    if (order->getSide() == OrderSide::BUY) {
        buyOrders.push_back(order);
        sortBuyOrders();
    } else {
        sellOrders.push_back(order);
        sortSellOrders();
    }
}

std::vector<Trade*> OrderBook::matchOrders() {
    std::vector<Trade*> trades;

    while (!buyOrders.empty() && !sellOrders.empty()) {
        Order* buy  = buyOrders.front();
        Order* sell = sellOrders.front();

        if (!buy->isActive())  { buyOrders.erase(buyOrders.begin()); continue; }
        if (!sell->isActive()) { sellOrders.erase(sellOrders.begin()); continue; }

        if (buy->getPrice() >= sell->getPrice()) {
            int qty = std::min(buy->getRemainingQty(), sell->getRemainingQty());
            double tradePrice = sell->getPrice();

            Trade* trade = new Trade(
                buy->getClient(), sell->getClient(),
                buy->getInstrument(), qty, tradePrice
            );
            trades.push_back(trade);

            // Update cash and portfolios
            double total = qty * tradePrice;
            buy->getClient()->debit(total);
            sell->getClient()->credit(total);

            buy->getClient()->getPortfolio().addPosition(buy->getInstrument(), qty, tradePrice);
            sell->getClient()->getPortfolio().removePosition(sell->getInstrument(), qty);

            // Realized P/L for seller
            double avgCost = sell->getClient()->getPortfolio().hasPosition(sell->getInstrument()->getSymbol())
                ? 0 : 0; // already removed, simplified
            sell->getClient()->addRealizedPnL(total);

            buy->fill(qty);
            sell->fill(qty);

            if (!buy->isActive())  buyOrders.erase(buyOrders.begin());
            if (!sell->isActive()) sellOrders.erase(sellOrders.begin());
        } else {
            break; // No more matches possible
        }
    }

    return trades;
}

void OrderBook::display() const {
    std::cout << "=== Order Book: " << symbol << " ===\n";
    std::cout << "-- BUY ORDERS --\n";
    if (buyOrders.empty()) std::cout << "  (none)\n";
    for (Order* o : buyOrders) { o->display(); std::cout << "\n"; }

    std::cout << "-- SELL ORDERS --\n";
    if (sellOrders.empty()) std::cout << "  (none)\n";
    for (Order* o : sellOrders) { o->display(); std::cout << "\n"; }
}

std::ostream& operator<<(std::ostream& os, const OrderBook& book) {
    book.display();
    return os;
}
