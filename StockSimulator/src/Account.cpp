#include "Account.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Account::Account(double initialBalance): balance(initialBalance) {}


bool Account::buy(std::string symbol, int qty, double price) {
    if (qty <= 0) {
        return false;
    }

    if (balance < price * qty) {
        return false;
    }
    // 紀錄總股票數與總購買金額
    avgPriceData[symbol].first += qty;
    avgPriceData[symbol].second += qty * price;

    // 紀錄買入資料
    Trade trade(symbol, qty, price, "BUY");

    // 將買入時間轉為字串
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&now_c);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    trade.setTime(ss.str());

    tradeHistory.push_back(trade);

    // 買入資料計算
    balance -= (price * qty);
    holdings[symbol] += qty;
    return true;
}

bool Account::sell(std::string symbol, int qty, double price) {
    if (qty <= 0) {
        return false;
    }

    if (holdings[symbol] - qty < 0) {
        return false;
    }

    // 紀錄賣出資料
    Trade trade(symbol, qty, price, "SELL");

    // 將賣出時間轉為字串
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&now_c);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    trade.setTime(ss.str());

    tradeHistory.push_back(trade);
    
    // 賣出資料計算
    balance += (price * qty);
    holdings[symbol] -= qty;
    return true;
}

void Account::showPortfolio() const {

    if (!holdings.empty()) {
        std::cout << "======= Shareholding Information =======" << std::endl;
        for (auto holding = holdings.begin(); holding != holdings.end(); holding++) {
            auto data = avgPriceData.at(holding->first);
            double avgPrice = data.second / data.first;
            std::cout << "Stock Code: " << holding->first << std::endl;
            std::cout << "Shares Held: " << holding->second << " shares" << std::endl;
            std::cout << "Average Purchase Price: " << std::fixed << std::setprecision(2)
                      << avgPrice << std::endl;
            if (holding != prev(holdings.end())) std::cout << std::endl;
        }
        std::cout << "========================================" << std::endl;
    } else {
        std::cout << "Shareholding Information: No shares currently held" << std::endl;
    }
}

void Account::showTradeHistory() const {
    if (!tradeHistory.empty()) {
        std::cout << "=============== Transaction Record ===============" << std::endl;
        for (Trade trade : tradeHistory) {
            std::cout << "[" << trade.getTime() << "]" << " ";
            std::cout << trade.getType() << " ";
            std::cout << trade.getSymbol() << " ";
            std::cout << trade.getQuantity() << " shares ";
            std::cout << "@ " << std::fixed << std::setprecision(2)
                      << trade.getPrice() << " NTD" << std::endl;
        }
        std::cout << "==================================================" << std::endl;

    } else {
        std::cout << "No transaction records available" << std::endl;
    }
}