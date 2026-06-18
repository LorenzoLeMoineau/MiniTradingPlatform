#pragma once
#include <stdexcept>
#include <string>

class InsufficientFundsException : public std::runtime_error {
public:
    explicit InsufficientFundsException(const std::string& msg)
        : std::runtime_error("Insufficient funds: " + msg) {}
};

class InsufficientSharesException : public std::runtime_error {
public:
    explicit InsufficientSharesException(const std::string& msg)
        : std::runtime_error("Insufficient shares: " + msg) {}
};

class InvalidOrderException : public std::runtime_error {
public:
    explicit InvalidOrderException(const std::string& msg)
        : std::runtime_error("Invalid order: " + msg) {}
};

class ClientNotFoundException : public std::runtime_error {
public:
    explicit ClientNotFoundException(const std::string& msg)
        : std::runtime_error("Client not found: " + msg) {}
};

class InstrumentNotFoundException : public std::runtime_error {
public:
    explicit InstrumentNotFoundException(const std::string& msg)
        : std::runtime_error("Instrument not found: " + msg) {}
};
