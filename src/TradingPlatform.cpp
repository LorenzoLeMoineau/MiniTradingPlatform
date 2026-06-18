#include "TradingPlatform.h"
#include "MarketBuyOrder.h"
#include "MarketSellOrder.h"
#include "LimitBuyOrder.h"
#include "LimitSellOrder.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <map>
#include <algorithm>

TradingPlatform::TradingPlatform() : nextClientId(1) {}

TradingPlatform::~TradingPlatform() {
    for (auto& [id, c] : clients) delete c;
    for (auto& [sym, i] : instruments) delete i;
    for (auto& [sym, ob] : orderBooks) delete ob;
    for (Trade* t : trades) delete t;
}

Client* TradingPlatform::findClient(int id) {
    auto it = clients.find(id);
    if (it == clients.end()) throw ClientNotFoundException(std::to_string(id));
    return it->second;
}

Instrument* TradingPlatform::findInstrument(const std::string& symbol) {
    auto it = instruments.find(symbol);
    if (it == instruments.end()) throw InstrumentNotFoundException(symbol);
    return it->second;
}

OrderBook* TradingPlatform::getOrCreateOrderBook(const std::string& symbol) {
    if (orderBooks.find(symbol) == orderBooks.end())
        orderBooks[symbol] = new OrderBook(symbol);
    return orderBooks[symbol];
}

void TradingPlatform::createClient(const std::string& name) {
    clients[nextClientId] = new Client(nextClientId, name);
    std::cout << "Client created: #" << nextClientId << " - " << name << "\n";
    ++nextClientId;
}

void TradingPlatform::listClients() const {
    if (clients.empty()) { std::cout << "No clients.\n"; return; }
    for (const auto& [id, c] : clients) std::cout << *c << "\n";
}

void TradingPlatform::depositCash(int clientId, double amount) {
    Client* c = findClient(clientId);
    c->deposit(amount);
    std::cout << "Deposited $" << amount << " to " << c->getName()
              << ". New balance: $" << c->getCashBalance() << "\n";
}

void TradingPlatform::showClientReport(int clientId) const {
    auto it = clients.find(clientId);
    if (it == clients.end()) throw ClientNotFoundException(std::to_string(clientId));
    const Client* c = it->second;
    double portfolioValue = c->getPortfolio().getTotalMarketValue();
    std::cout << "=== Client Report ===\n";
    std::cout << *c << "\n";
    std::cout << "Portfolio Value: $" << portfolioValue << "\n";
    std::cout << "Total Account Value: $" << (c->getCashBalance() + portfolioValue) << "\n";
    std::cout << "Portfolio:\n" << c->getPortfolio() << "\n";
}

void TradingPlatform::createInstrument(const std::string& symbol, const std::string& name, double price) {
    if (instruments.count(symbol)) { std::cout << "Instrument already exists.\n"; return; }
    instruments[symbol] = new Instrument(symbol, name, price);
    std::cout << "Instrument created: " << *instruments[symbol] << "\n";
}

void TradingPlatform::listInstruments() const {
    if (instruments.empty()) { std::cout << "No instruments.\n"; return; }
    for (const auto& [sym, i] : instruments) std::cout << *i << "\n";
}

void TradingPlatform::placeMarketBuyOrder(int clientId, const std::string& symbol, int qty) {
    Client* c = findClient(clientId);
    Instrument* instr = findInstrument(symbol);
    double cost = qty * instr->getMarketPrice();
    if (c->getCashBalance() < cost)
        throw InsufficientFundsException("Need $" + std::to_string(cost));
    Order* order = new MarketBuyOrder(c, instr, qty);
    getOrCreateOrderBook(symbol)->addOrder(order);
    std::cout << "Market Buy Order placed: " << qty << " " << symbol << "\n";
}

void TradingPlatform::placeMarketSellOrder(int clientId, const std::string& symbol, int qty) {
    Client* c = findClient(clientId);
    Instrument* instr = findInstrument(symbol);
    if (c->getPortfolio().getQuantity(symbol) < qty)
        throw InsufficientSharesException("Not enough shares of " + symbol);
    Order* order = new MarketSellOrder(c, instr, qty);
    getOrCreateOrderBook(symbol)->addOrder(order);
    std::cout << "Market Sell Order placed: " << qty << " " << symbol << "\n";
}

void TradingPlatform::placeLimitBuyOrder(int clientId, const std::string& symbol, int qty, double limitPrice) {
    Client* c = findClient(clientId);
    Instrument* instr = findInstrument(symbol);
    double cost = qty * limitPrice;
    if (c->getCashBalance() < cost)
        throw InsufficientFundsException("Need $" + std::to_string(cost));
    Order* order = new LimitBuyOrder(c, instr, qty, limitPrice);
    getOrCreateOrderBook(symbol)->addOrder(order);
    std::cout << "Limit Buy Order placed: " << qty << " " << symbol << " @ $" << limitPrice << "\n";
}

void TradingPlatform::placeLimitSellOrder(int clientId, const std::string& symbol, int qty, double limitPrice) {
    Client* c = findClient(clientId);
    Instrument* instr = findInstrument(symbol);
    if (c->getPortfolio().getQuantity(symbol) < qty)
        throw InsufficientSharesException("Not enough shares of " + symbol);
    Order* order = new LimitSellOrder(c, instr, qty, limitPrice);
    getOrCreateOrderBook(symbol)->addOrder(order);
    std::cout << "Limit Sell Order placed: " << qty << " " << symbol << " @ $" << limitPrice << "\n";
}

void TradingPlatform::displayOrderBook(const std::string& symbol) const {
    auto it = orderBooks.find(symbol);
    if (it == orderBooks.end()) { std::cout << "No order book for " << symbol << "\n"; return; }
    it->second->display();
}

void TradingPlatform::executeMatching(const std::string& symbol) {
    auto it = orderBooks.find(symbol);
    if (it == orderBooks.end()) { std::cout << "No order book for " << symbol << "\n"; return; }
    auto newTrades = it->second->matchOrders();
    if (newTrades.empty()) { std::cout << "No matches found for " << symbol << "\n"; return; }
    for (Trade* t : newTrades) {
        std::cout << "TRADE EXECUTED: " << *t << "\n";
        trades.push_back(t);
    }
}

void TradingPlatform::executeAllMatching() {
    for (auto& [sym, ob] : orderBooks) {
        auto newTrades = ob->matchOrders();
        for (Trade* t : newTrades) {
            std::cout << "TRADE EXECUTED: " << *t << "\n";
            trades.push_back(t);
        }
    }
}

void TradingPlatform::displayTrades() const {
    if (trades.empty()) { std::cout << "No trades.\n"; return; }
    std::cout << "=== Trade History ===\n";
    for (const Trade* t : trades) std::cout << *t << "\n";
}

void TradingPlatform::displayTradingReport() const {
    std::cout << "=== Trading Report ===\n";
    std::cout << "Total trades: " << trades.size() << "\n";
    double volume = 0;
    std::map<std::string, int> volumeBySymbol;
    for (const Trade* t : trades) {
        volume += t->getValue();
        volumeBySymbol[t->getInstrument()->getSymbol()] += t->getQuantity();
    }
    std::cout << "Total volume: $" << volume << "\n";
    if (!volumeBySymbol.empty()) {
        auto most = std::max_element(volumeBySymbol.begin(), volumeBySymbol.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; });
        std::cout << "Most traded instrument: " << most->first
                  << " (" << most->second << " shares)\n";
    }
}

void TradingPlatform::displayAllPortfolios() const {
    for (const auto& [id, c] : clients) {
        std::cout << "--- " << c->getName() << " (Cash: $" << c->getCashBalance() << ") ---\n";
        std::cout << c->getPortfolio() << "\n\n";
    }
}

// ---- MENU ----
static int readInt(const std::string& prompt) {
    int v;
    std::cout << prompt;
    while (!(std::cin >> v)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, try again: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}

static double readDouble(const std::string& prompt) {
    double v;
    std::cout << prompt;
    while (!(std::cin >> v)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, try again: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return v;
}

static std::string readString(const std::string& prompt) {
    std::string v;
    std::cout << prompt;
    std::getline(std::cin, v);
    return v;
}

void TradingPlatform::runMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========== Mini Trading Platform ==========\n";
        std::cout << " 1.  Create client\n";
        std::cout << " 2.  List clients\n";
        std::cout << " 3.  Deposit cash\n";
        std::cout << " 4.  Create instrument\n";
        std::cout << " 5.  List instruments\n";
        std::cout << " 6.  Place Market Buy Order\n";
        std::cout << " 7.  Place Market Sell Order\n";
        std::cout << " 8.  Place Limit Buy Order\n";
        std::cout << " 9.  Place Limit Sell Order\n";
        std::cout << " 10. Display Order Book\n";
        std::cout << " 11. Execute Matching (one instrument)\n";
        std::cout << " 12. Execute All Matching\n";
        std::cout << " 13. Display Trades\n";
        std::cout << " 14. Display All Portfolios\n";
        std::cout << " 15. Client Report\n";
        std::cout << " 16. Trading Report\n";
        std::cout << " 0.  Exit\n";
        std::cout << ">>> ";

        choice = readInt("");

        try {
            switch (choice) {
                case 1: {
                    std::string name = readString("Client name: ");
                    createClient(name);
                    break;
                }
                case 2: listClients(); break;
                case 3: {
                    int id = readInt("Client ID: ");
                    double amt = readDouble("Amount: ");
                    depositCash(id, amt);
                    break;
                }
                case 4: {
                    std::string sym = readString("Symbol (e.g. AAPL): ");
                    std::string name = readString("Company name: ");
                    double price = readDouble("Market price: ");
                    createInstrument(sym, name, price);
                    break;
                }
                case 5: listInstruments(); break;
                case 6: {
                    int id = readInt("Client ID: ");
                    std::string sym = readString("Symbol: ");
                    int qty = readInt("Quantity: ");
                    placeMarketBuyOrder(id, sym, qty);
                    break;
                }
                case 7: {
                    int id = readInt("Client ID: ");
                    std::string sym = readString("Symbol: ");
                    int qty = readInt("Quantity: ");
                    placeMarketSellOrder(id, sym, qty);
                    break;
                }
                case 8: {
                    int id = readInt("Client ID: ");
                    std::string sym = readString("Symbol: ");
                    int qty = readInt("Quantity: ");
                    double price = readDouble("Limit price: ");
                    placeLimitBuyOrder(id, sym, qty, price);
                    break;
                }
                case 9: {
                    int id = readInt("Client ID: ");
                    std::string sym = readString("Symbol: ");
                    int qty = readInt("Quantity: ");
                    double price = readDouble("Limit price: ");
                    placeLimitSellOrder(id, sym, qty, price);
                    break;
                }
                case 10: {
                    std::string sym = readString("Symbol: ");
                    displayOrderBook(sym);
                    break;
                }
                case 11: {
                    std::string sym = readString("Symbol: ");
                    executeMatching(sym);
                    break;
                }
                case 12: executeAllMatching(); break;
                case 13: displayTrades(); break;
                case 14: displayAllPortfolios(); break;
                case 15: {
                    int id = readInt("Client ID: ");
                    showClientReport(id);
                    break;
                }
                case 16: displayTradingReport(); break;
                case 0: std::cout << "Goodbye!\n"; break;
                default: std::cout << "Unknown option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "[ERROR] " << e.what() << "\n";
        }
    }
}
