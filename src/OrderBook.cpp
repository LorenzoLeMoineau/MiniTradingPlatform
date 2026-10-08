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
    // Highest price first; stable_sort keeps time priority between equal prices
    std::stable_sort(buyOrders.begin(), buyOrders.end(), [](Order* a, Order* b) {
        return a->getPrice() > b->getPrice();
    });
}

void OrderBook::sortSellOrders() {
    // Lowest price first; stable_sort keeps time priority between equal prices
    std::stable_sort(sellOrders.begin(), sellOrders.end(), [](Order* a, Order* b) {
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

        if (!buy->isActive())  { delete buy;  buyOrders.erase(buyOrders.begin()); continue; }
        if (!sell->isActive()) { delete sell; sellOrders.erase(sellOrders.begin()); continue; }

        if (buy->getPrice() >= sell->getPrice()) {
            int qty = std::min(buy->getRemainingQty(), sell->getRemainingQty());
            double tradePrice = sell->getPrice();

            Trade* trade = new Trade(
                buy->getClient(), sell->getClient(),
                buy->getInstrument(), qty, tradePrice
            );
            trades.push_back(trade);

            // Realized P/L for the seller, computed before the position is reduced
            double avgCost = sell->getClient()->getPortfolio().getAvgPrice(sell->getInstrument()->getSymbol());
            sell->getClient()->addRealizedPnL((tradePrice - avgCost) * qty);

            // Update cash and portfolios
            double total = qty * tradePrice;
            buy->getClient()->debit(total);
            sell->getClient()->credit(total);

            buy->getClient()->getPortfolio().addPosition(buy->getInstrument(), qty, tradePrice);
            sell->getClient()->getPortfolio().removePosition(sell->getInstrument(), qty);

            // The last traded price becomes the instrument's market price
            buy->getInstrument()->setMarketPrice(tradePrice);

            buy->fill(qty);
            sell->fill(qty);

            if (!buy->isActive())  { delete buy;  buyOrders.erase(buyOrders.begin()); }
            if (!sell->isActive()) { delete sell; sellOrders.erase(sellOrders.begin()); }
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
