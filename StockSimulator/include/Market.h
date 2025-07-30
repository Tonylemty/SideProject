// 股票市場管理

#ifndef MARKET_H
#define MARKET_H

#include <string>
#include <map>
#include "Stock.h"

class Market {
public:
    void addStock(std::string symbol, double price); // 新增股票到市場
    void updatePrice(std::string symbol, double newPrice); // 更新特定股票價格
    Stock* getStock(std::string symbol); // 取得某支股票
    void showAllStocks() const; // 印出所有股票現價
    void simulatePriceChanges(); // 模擬隨機漲跌 (進階)

private:
    std::map<std::string, Stock> stocks; // 股票物件
};

#endif