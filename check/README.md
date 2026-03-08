## **Zuvio 自動簽到與課程監控工具操作說明**
---

### **目錄**
* Python 安裝教學
* Python 模組安裝教學
* 程式配置說明
* 程式操作說明

### **Python 安裝教學**
##### 安裝步驟：
1. 先至 [Python 官網](https://www.python.org/downloads/)下載 Python 程式
2. 依據自己的電腦系統去點選對應的按鈕
3. 安裝完成後，去運行 Python 的執行檔
4. 先將 「Add Python 3.14.3 to PATH」 勾選起來，接著點選 Customize installation
5. 接著持續點擊下一步
6. 當要進行安裝時，記得將「Install for all users」的選項勾選起來，在進行安裝
##### Python 下載教學的相關影片：
1. [Python 安裝教程](https://reurl.cc/RLoVWZ)
2. [怎麼正確下載安裝 Python + Pycharm？如何配置環境？](https://reurl.cc/WAoZ87)


### **Python 模組安裝教學**

##### 安裝步驟：
1. 先至 windows 搜尋欄中打上 CMD
2. 打上指令「pip install 模組名稱」
3. 等待模組安裝

##### 安裝模組名稱：
1. *requests*
2. *beautifulsoup4*
3. *configparser*

### **程式配置說明**
本程式會自動生成 `config.ini` 檔案來儲存您的登入資訊與課程座標，避免重複輸入

##### 一、首次操作：
第一次執行程式時，終端機會要求您輸入：
* 學號：自動補全為學校信箱格式（若非本校學生，需要至程式碼第 51 行自行修改）
* 密碼：您的學校帳號的登入密碼
* Discord Webhook：用於接收簽到成功與新問題的即時通知

##### 二、座標設定
由於 Zuvio 簽到通常有地理位置限制，您可以在 `config.ini` 中的 `[course_locations]` 區塊手動增加課程及其對應的經緯度：
```ini
[course_locations]
機器學習 = 緯度,經度
無線網路 = 25.03396,121.56447
```
* **注意**：課程名稱需包含在 Zuvio 上的課程名稱關鍵字內

### **功能介紹與操作**

##### 操作流程：
1. 啟動程式：執行 `python check.py`（或您的檔名）
2. 自動登入：程式會自動取得 `accessToken` 與 `user_id`
3. 即時監控：
   * 自動簽到：偵測到簽到任務時，若已設定座標，程式會模擬 WEB 端位置發送簽到請求，並將簽到成功的通知發送到 Discord
   * 問答偵測：若老師發布「問答題」，程式會擷取題目內容並發送至 Discord
4. 狀態檢查：程式每隔 3-6 秒會隨機更新一次狀態，避免過於規律的請求被伺服器偵測