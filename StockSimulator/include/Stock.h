// 股票資訊

#ifndef STOCK_H
#define STOCK_H

#include <string>
class Stock {
public:
    Stock();
    Stock(const std::string symbol, double price);
    std::string getSymbol() const; // 取得股票代號
    double getPrice() const; // 取得價格
    void updatePrice(double newPrice); // 更新股價

private:
    std::string symbol; // 股票代號
    double price; // 當前股價
};

#endif