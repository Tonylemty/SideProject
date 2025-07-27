// 一次交易紀錄

#ifndef TRADE_H
#define TRADE_H

#include <string>
#include <chrono>

class Trade {
public:
    Trade(std::string symbol, int qty, double price, std::string type);
    void setTime(const std::string time);
    std::string getTime() const;
    std::string getSymbol() const;
    int getQuantity() const;
    double getPrice() const;
    std::string getType() const;
    void print() const; // 輸出交易內容（for test）

private:
    std::string symbol; // 股票代號
    int quantity; // 買賣數量
    double price; // 成交價
    std::string type; // 交易類型 buy or sell
    std::string time; // 紀錄時間
};

#endif