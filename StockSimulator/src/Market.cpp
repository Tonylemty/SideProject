#include "Market.h"
#include <iostream>
#include <iomanip>
#include <random>

void Market::addStock(std::string symbol, double price) {

    if (stocks.count(symbol)) {
        std::cout << "The stock already exists";
        return;
    }

    if (price <= 0) {
        std::cout << "The price cannot be less than or equal to 0";
        return;
    }

    if (symbol.empty()) {
        std::cout << "The symbol cannot be empty";
        return;
    }

    Stock stock(symbol, price);
    stocks.emplace(symbol, stock);
}

void Market::updatePrice(std::string symbol, double newPrice) {
    
    if (!stocks.count(symbol)) {
        std::cout << "The " << symbol << "does not exist";
        return;
    }

    if (newPrice <= 0) {
        std::cout << "The price cannot be less than or equal to 0";
        return;
    }

    if (symbol.empty()) {
        std::cout << "The symbol cannot be empty";
        return;
    }

    stocks[symbol].updatePrice(newPrice);
}

Stock* Market::getStock(std::string symbol) {

    if (!stocks.count(symbol)) {
        std::cout << "The " << symbol << "does not exist";
        return nullptr;
    }
    return &stocks[symbol];
}

void Market::showAllStocks() const {

    std::cout << "========= Stock Listings =========" << std::endl;
    std::cout << "Symbol      Price" << std::endl;
    std::cout << "--------------------" << std::endl;

    for (auto &stock : stocks) {
        std::cout << stock.first << "      $" << std::fixed 
                  << std::setprecision(2) << stock.second.getPrice()
                  << std::endl;
    }

    std::cout << "==================================" << std::endl;
}

void Market::simulatePriceChanges() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-10.0, 10.0);
    
    for (auto &stock : stocks) {
        double spread = dist(gen);
        double newPrice = stock.second.getPrice() + spread;

        if (newPrice <= 0) {
            stock.second.updatePrice(1.00);
        } else {
            stock.second.updatePrice(newPrice);
        }
    }
}