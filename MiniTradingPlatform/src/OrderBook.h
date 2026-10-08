#pragma once
#include "Order.h"
#include "Trade.h"
#include <vector>
#include <string>
#include <iostream>

class OrderBook {
private:
    std::string symbol;
    std::vector<Order*> buyOrders;
    std::vector<Order*> sellOrders;

    void sortBuyOrders();
    void sortSellOrders();

public:
    explicit OrderBook(const std::string& symbol);
    ~OrderBook();

    const std::string& getSymbol() const;

    void addOrder(Order* order);
    std::vector<Trade*> matchOrders();

    void display() const;
    friend std::ostream& operator<<(std::ostream& os, const OrderBook& book);
};
