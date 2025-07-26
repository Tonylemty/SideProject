#include "Stock.h"


Stock::Stock(const std::string symbol, double price): symbol(symbol), price(price) {}

std::string Stock::getSymbol() const {
    return symbol;
}

double Stock::getPrice() const {
    return price;
}

void Stock::updatePrice(double newPrice) {
    price = newPrice;
}