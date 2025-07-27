// 使用者帳戶（資金與持股）

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <map>
#include <string>
#include <vector>
#include "Trade.h"

class Account {
public:
    Account(double initialBalance);
    bool buy(std::string symbol, int qty, double price); // 買進功能
    bool sell(std::string symbol, int qty, double price); // 賣出
    void showPortfolio() const; // 印出持股資訊
    void showTradeHistory() const; // 顯示交易歷史


private:
    double balance; // 現金餘額
    std::map<std::string, int> holdings; // 股票代碼, 持有數量
    std::vector<Trade> tradeHistory; //  所有交易紀錄
    std::map<std::string, std::pair<int, double>> avgPriceData; // 累積購買股數, 總花費金額
};


#endif