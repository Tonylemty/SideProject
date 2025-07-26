# 股票模擬系統 (Stock Simulator)

本專案使用 C++ 製作一個簡易的股票模擬交易系統，目的是加強資料結構與物件導向設計的實作能力，並作為大學推甄作品。

## 📁 專案結構
```
StockSimulator/
├── main.cpp
├── Makefile
├── include/
│ ├── Stock.h
│ ├── Account.h
│ └── Trade.h
├── src/
│ ├── Stock.cpp
│ ├── Account.cpp
│ └── Trade.cpp
├── build/ # 編譯後的 .o 檔案
└── README.md
```

## 🚀 功能說明

- 模擬股票價格的變化
- 使用者帳戶資金管理（買入／賣出）
- 交易紀錄功能
- 支援多支股票管理
- 策略機制（未來可擴充）

## 🛠️ 編譯方式

請先確認已安裝 `g++` 與 `make`，在根目錄執行：

```bash
mingw32-make        # Windows 使用者
make                # macOS / Linux 使用者
mingw32-make clean  # 若要清除編譯產物
```

## 開發工具
* C++17
* Visual Studio Code
* Git + GitHub
* make / mingw32-make