#include "Trade.h"


Trade::Trade(std::string symbol, int qty, double price, std::string type):
    symbol(symbol), quantity(qty), price(price), type(type) {}
void Trade::setTime(const std::string newTime) {
    time = newTime;
}

std::string Trade::getTime() const {
    return time;
}

std::string Trade::getSymbol() const {
    return symbol;
}

int Trade::getQuantity() const {
    return quantity;
}

double Trade::getPrice() const {
    return price;
}

std::string Trade::getType() const {
    return type;
}