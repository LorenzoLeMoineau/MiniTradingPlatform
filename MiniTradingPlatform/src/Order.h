#pragma once
#include "Client.h"
#include "Instrument.h"
#include <string>
#include <iostream>

enum class OrderSide { BUY, SELL };
enum class OrderStatus { PENDING, PARTIAL, FILLED, CANCELLED };

class Order {
protected:
    static int nextId;
    int id;
    Client* client;
    Instrument* instrument;
    OrderSide side;
    int quantity;
    int remainingQty;
    OrderStatus status;

public:
    Order(Client* client, Instrument* instrument, OrderSide side, int quantity);
    virtual ~Order() = default;

    int getId() const;
    Client* getClient() const;
    Instrument* getInstrument() const;
    OrderSide getSide() const;
    int getQuantity() const;
    int getRemainingQty() const;
    OrderStatus getStatus() const;

    void fill(int qty);
    bool isActive() const;

    virtual double getPrice() const = 0;
    virtual std::string getType() const = 0;
    virtual void display() const;

    bool operator<(const Order& other) const;
    bool operator==(const Order& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
};
