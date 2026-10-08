#pragma once
#include "Client.h"
#include "Instrument.h"
#include "OrderBook.h"
#include "Trade.h"
#include <map>
#include <vector>
#include <string>

class TradingPlatform {
private:
    std::map<int, Client*> clients;
    std::map<std::string, Instrument*> instruments;
    std::map<std::string, OrderBook*> orderBooks;
    std::vector<Trade*> trades;
    int nextClientId;

    Client* findClient(int id);
    Instrument* findInstrument(const std::string& symbol);
    OrderBook* getOrCreateOrderBook(const std::string& symbol);

public:
    TradingPlatform();
    ~TradingPlatform();

    // Client management
    void createClient(const std::string& name);
    void listClients() const;
    void depositCash(int clientId, double amount);
    void depositShares(int clientId, const std::string& symbol, int qty);
    void showClientReport(int clientId) const;

    // Instrument management
    void createInstrument(const std::string& symbol, const std::string& name, double price);
    void listInstruments() const;

    // Order placement
    void placeMarketBuyOrder(int clientId, const std::string& symbol, int qty);
    void placeMarketSellOrder(int clientId, const std::string& symbol, int qty);
    void placeLimitBuyOrder(int clientId, const std::string& symbol, int qty, double limitPrice);
    void placeLimitSellOrder(int clientId, const std::string& symbol, int qty, double limitPrice);

    // Order book & matching
    void displayOrderBook(const std::string& symbol) const;
    void executeMatching(const std::string& symbol);
    void executeAllMatching();

    // Trades & reports
    void displayTrades() const;
    void displayTradingReport() const;
    void displayAllPortfolios() const;

    // Menu
    void runMenu();
};
