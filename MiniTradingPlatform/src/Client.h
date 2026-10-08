#pragma once
#include "Portfolio.h"
#include <string>
#include <iostream>

class Client {
private:
    int id;
    std::string fullName;
    double cashBalance;
    Portfolio portfolio;
    double realizedPnL;

public:
    Client(int id, const std::string& fullName, double initialCash = 0.0);

    int getId() const;
    const std::string& getName() const;
    double getCashBalance() const;
    double getRealizedPnL() const;
    Portfolio& getPortfolio();
    const Portfolio& getPortfolio() const;

    void deposit(double amount);
    void debit(double amount);
    void credit(double amount);
    void addRealizedPnL(double amount);

    bool operator==(const Client& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Client& client);
};
