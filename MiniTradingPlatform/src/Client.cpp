#include "Client.h"
#include "Exceptions.h"

Client::Client(int id, const std::string& fullName, double initialCash)
    : id(id), fullName(fullName), cashBalance(initialCash), realizedPnL(0.0) {}

int Client::getId() const { return id; }
const std::string& Client::getName() const { return fullName; }
double Client::getCashBalance() const { return cashBalance; }
double Client::getRealizedPnL() const { return realizedPnL; }
Portfolio& Client::getPortfolio() { return portfolio; }
const Portfolio& Client::getPortfolio() const { return portfolio; }

void Client::deposit(double amount) {
    if (amount <= 0) throw InvalidOrderException("Deposit amount must be positive.");
    cashBalance += amount;
}

void Client::debit(double amount) {
    if (amount > cashBalance)
        throw InsufficientFundsException("Cannot debit $" + std::to_string(amount));
    cashBalance -= amount;
}

void Client::credit(double amount) {
    cashBalance += amount;
}

void Client::addRealizedPnL(double amount) {
    realizedPnL += amount;
}

bool Client::operator==(const Client& other) const {
    return id == other.id;
}

std::ostream& operator<<(std::ostream& os, const Client& client) {
    os << "Client #" << client.id << " | " << client.fullName
       << " | Cash: $" << client.cashBalance
       << " | Realized P/L: $" << client.realizedPnL;
    return os;
}
