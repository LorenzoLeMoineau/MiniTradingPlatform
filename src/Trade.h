#pragma once
#include "Client.h"
#include "Instrument.h"
#include <ctime>
#include <string>
#include <iostream>

class Trade {
private:
    static int nextId;
    int id;
    Client* buyer;
    Client* seller;
    Instrument* instrument;
    int quantity;
    double price;
    std::time_t timestamp;

public:
    Trade(Client* buyer, Client* seller, Instrument* instrument, int quantity, double price);

    int getId() const;
    Client* getBuyer() const;
    Client* getSeller() const;
    Instrument* getInstrument() const;
    int getQuantity() const;
    double getPrice() const;
    double getValue() const;
    std::time_t getTimestamp() const;

    bool operator==(const Trade& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Trade& trade);
};
