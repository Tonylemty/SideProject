from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from selenium.common.exceptions import TimeoutException
from webdriver_manager.chrome import ChromeDriverManager
import prettytable as pt
import os

# 設定瀏覽器選項
OPTIONS = Options()
OPTIONS.add_argument("--log-level=3") # 降低 ChromeDriver 本身的日誌輸出
OPTIONS.add_experimental_option("detach", True)  # 瀏覽器不會自動關閉

# 啟動 Selenium 瀏覽器
SERVICE = Service(ChromeDriverManager().install())

class BuyTicketSystem:
    def __init__(self):
        self.driver = webdriver.Chrome(service=SERVICE, options=OPTIONS)
        self.driver.get('https://www.railway.gov.tw/tra-tip-web/tip')
        self.driver.minimize_window()
        self.table_rows = []

    def runSystem(self):
        self.inputSearchInformation()
        self.checkWord()
        self.sendSearchInformation()
        if not self.buildTimeSchedule():
            return
        if not self.enterBuyTicketPage():
            return
        self.switchToNewPage()
        if not self.sendID():
            return
        if not self.verify():
            return
        self.printTicketInformation()

    # 輸入查詢條件
    def inputSearchInformation(self):
        self.startStation = input('請輸入起始站名（如：新左營）：')
        self.endStation = input('請輸入到達站名（如：臺北）：')
        self.date = input('請輸入日期（如：YYYYMMDD）：')
        self.time = input('請輸入起迄時間，以 30 分鐘為單位（如：00:00）：')
    
    # 修改使用者輸入的站名
    def checkWord(self):
        if (self.startStation.find('台') != -1):
            self.startStation = self.startStation.replace('台', '臺')
        elif (self.endStation.find('台') != -1):
            self.endStation = self.endStation.replace('台', '臺')

    #  送出搜尋資訊
    def sendSearchInformation(self):
        self.driver.find_element(By.ID, "startStation").send_keys(self.startStation)
        self.driver.find_element(By.ID, "endStation").send_keys(self.endStation)

        date_input = self.driver.find_element(By.ID, "rideDate")
        date_input.clear()
        date_input.send_keys(self.date)

        self.driver.find_element(By.ID, "startTime").send_keys(self.time)
        self.driver.find_element(By.XPATH, '//*[@id="queryForm"]/div[6]/input').click()
    
    # 建立火車時刻表
    def buildTimeSchedule(self):
        try:
            WebDriverWait(self.driver, 10).until(
                EC.presence_of_element_located((By.CSS_SELECTOR, 'div.bk_3_list.columns'))
            )
            print("\n台鐵時刻表（目標時間後的所有資料）")
            self.table_rows = self.driver.find_elements(By.CSS_SELECTOR, 'div.bk_3_list.columns')
            tb = pt.PrettyTable()
            tb.field_names = ['車種車次（始發站 → 終點站）', '出發時間', '抵達時間', '行駛時間', '全票', '孩童票', '訂票']

            if not self.table_rows:
                tb.add_row(['查無資料'] * len(tb.field_names))
            else:
                for row in self.table_rows:
                    fields = self._readRowFields(row)
                    if fields is None:
                        continue
                    train_type_no, departure_time, arrival_time, through_time, _route, full_price, child_price, booking_cell = fields

                    book_ticket = '✔' if booking_cell.text.strip() else ''
                    tb.add_row([train_type_no, departure_time, arrival_time, through_time, full_price, child_price, book_ticket])
            print(tb)
            return True

        except TimeoutException:
            print('查無對應時刻表，請確認出發站、到達站、日期是否正確')
            return False
        except Exception as e:
            print(f'檢索發生錯誤：{e}')
            return False

    # 依序取出卡片中 8 個 ts_ 欄位方塊，取每個方塊的第二個 div（即實際數值）
    def _readRowFields(self, row):
        boxes = row.find_elements(By.CSS_SELECTOR, ':scope > div[class^="ts_"]')
        if len(boxes) < 8:
            return None
        values = []
        for box in boxes[:8]:
            inner_divs = box.find_elements(By.TAG_NAME, 'div')
            values.append(inner_divs[1] if len(inner_divs) >= 2 else box)
        train_type_no = values[0].text.strip()
        departure_time = values[1].text.strip()
        arrival_time = values[2].text.strip()
        through_time = values[3].text.strip()
        route = values[4].text.strip()
        full_price = values[5].text.strip()
        child_price = values[6].text.strip()
        booking_cell = values[7]
        return train_type_no, departure_time, arrival_time, through_time, route, full_price, child_price, booking_cell

    # 進入訂票頁面
    def enterBuyTicketPage(self):
        train_number = input('請輸入目標車次：')
        for row in self.table_rows:
            fields = self._readRowFields(row)
            if fields is None:
                continue
            train_type_no = fields[0]
            booking_cell = fields[7]
            if train_type_no.find(train_number) != -1:
                clickable = booking_cell.find_elements(By.TAG_NAME, 'a') or booking_cell.find_elements(By.TAG_NAME, 'button')
                if not clickable:
                    print('此班次目前尚未開放訂票（可能未在可售票區間內），請確認日期是否在開賣範圍內。')
                    return False
                clickable[0].click()
                print('已進入訂票頁面...\n')
                return True
        print('查無此車次')
        return False

    # 獲取所有分頁的句柄和切換到最新打開的分頁
    def switchToNewPage(self):
        all_tabs = self.driver.window_handles
        self.driver.switch_to.window(all_tabs[-1])
        self.driver.minimize_window()

    # 輸入身份證字號
    def sendID(self):
        id = input('請輸入你的身分證字號：')
        try:
            identify = self.driver.find_element(By.ID, 'pid')
            identify.send_keys(id)
            print("身份證字號輸入成功！")
            return True

        except Exception as e:
            print(f"無法定位身份證欄位：{e}")
            self.driver.quit()
            return False

    # 處理驗證碼，等待並手動完成驗證
    # 可能會出現座位不足
    def verify(self):
        self.driver.maximize_window()  # 要手動勾驗證碼了，這時才把瀏覽器叫到前景
        input('請手動輸入驗證碼後按下 Enter')
        try:
            WebDriverWait(self.driver, 10).until(
                EC.element_to_be_clickable((By.CLASS_NAME, "btn-3d"))
            )
            self.driver.find_element(By.CLASS_NAME, "btn-3d").click()
            print("驗證完成！\n")
            return True
        except Exception as e:
            print(f"驗證失敗：{e}")
            self.driver.quit()
            return False
    
    # 印出購票資訊
    def printTicketInformation(self):
        try:
            WebDriverWait(self.driver, 20).until(
                EC.presence_of_element_located((By.XPATH, '//h5[contains(text(), "訂票成功")]'))
            )
            print('訂票成功！請前往台鐵官網進行付款')
            self.driver.quit()
            return True

        except TimeoutException:
            print('訂票失敗')
            return False


if __name__ == '__main__':
    system = BuyTicketSystem()
    system.runSystem()