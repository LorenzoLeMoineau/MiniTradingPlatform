#pragma once
#include <string>
#include <iostream>

class Instrument {
private:
    std::string symbol;
    std::string companyName;
    double marketPrice;

public:
    Instrument(const std::string& symbol, const std::string& companyName, double marketPrice);

    const std::string& getSymbol() const;
    const std::string& getCompanyName() const;
    double getMarketPrice() const;
    void setMarketPrice(double price);

    bool operator==(const Instrument& other) const;
    bool operator<(const Instrument& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Instrument& instr);
};
