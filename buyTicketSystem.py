from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from webdriver_manager.chrome import ChromeDriverManager
import prettytable as pt
import os


# 設定瀏覽器選項
options = Options()
# options.add_argument('--headless')  # 啟用無頭模式
# options.add_argument('--disable-gpu')  # 禁用 GPU（防止某些環境中啟用時出錯） 
# options.add_argument('--no-sandbox')  # 避免在某些環境中因為沙箱設定出錯
options.add_argument("--log-level=3") # 降低 ChromeDriver 本身的日誌輸出
options.add_experimental_option("detach", True)  # 瀏覽器不會自動關閉

# 啟動 Selenium 瀏覽器
service = Service(ChromeDriverManager().install())
driver = webdriver.Chrome(service=service, options=options)
driver.get('https://www.railway.gov.tw/tra-tip-web/tip')

# 輸入查詢條件
startStation = input('請輸入起始站名（如：新左營）：')
endStation = input('請輸入到達站名（如：臺北）：')
date = input('請輸入時間（如：YYYYMMDD）：')
time = input('請輸入起迄時間（如：00:00）：')

driver.find_element(By.ID, "startStation").send_keys(startStation)
driver.find_element(By.ID, "endStation").send_keys(endStation)

date_input = driver.find_element(By.ID, "rideDate")
date_input.clear()
date_input.send_keys(date)

driver.find_element(By.ID, "startTime").send_keys(time)
driver.find_element(By.XPATH, '//*[@id="queryForm"]/div[6]/input').click()

# 等待結果表格載入
try:
    WebDriverWait(driver, 10).until(
        EC.presence_of_element_located((By.XPATH, '//*[@id="pageContent"]/div/table/tbody'))  # 結果表格的 XPath
    )
    print("\n台鐵時刻表（目標時間後的所有資料）")
    table_rows = driver.find_elements(By.CLASS_NAME, 'trip-column')
    tb = pt.PrettyTable()
    tb.field_names = ['車種車次（始發站 → 終點站）', '出發時間', '抵達時間', '行駛時間', '全票', '孩童票', '敬老票', '訂票']

    if not table_rows:
        tb.add_row(['查無資料'] * len(tb.field_names))
    else:
        for row in table_rows:
            columns = row.find_elements(By.TAG_NAME, 'td')
            if len(columns) >= 9:
                train_type_no = columns[0].text.strip()
                departure_time = columns[1].text.strip()
                arrival_time = columns[2].text.strip()
                through_time = columns[3].text.strip()
                full_price = columns[6].text.strip()
                child_price = columns[7].text.strip()
                senior_price = columns[8].text.strip()
                if (columns[9].text.strip() == '訂票'):
                    book_ticket = '✔'
                else:
                    book_ticket = ''
                tb.add_row([train_type_no, departure_time, arrival_time, through_time, full_price, child_price, senior_price, book_ticket])
    print(tb)

except Exception as e:
    print(f"發生錯誤: {e}")

# 進入訂票頁面
train_number = input('請輸入目標車次：')
for row in table_rows:
    col = row.find_elements(By.TAG_NAME, "td")
    if col[0].text.find(train_number) != -1:
        col[9].click()
        print('已進入訂票頁面...\n')
        break
    
# 獲取所有分頁的句柄和切換到最新打開的分頁
all_tabs = driver.window_handles
driver.switch_to.window(all_tabs[-1])

# 輸入身份證字號
id = input('請輸入你的身分證字號：')
try:
    identify = driver.find_element(By.ID, 'pid')
    identify.send_keys(id)
    print("身份證字號輸入成功！")
except Exception as e:
    print(f"無法定位身份證欄位: {e}")

# 處理驗證碼，等待並手動完成驗證
# 可能會出現座位不足
input('請手動勾選完 "我不是機器人" 後按下 Enter')
try:
    WebDriverWait(driver, 10).until(
        EC.element_to_be_clickable((By.CLASS_NAME, "btn-3d"))
    )
    driver.find_element(By.CLASS_NAME, "btn-3d").click()
    print("驗證完成，進入下一步！")
except Exception as e:
    print(f"驗證失敗: {e}")
    driver.quit()

# 印出購票資訊
try:
    WebDriverWait(driver, 20).until(
        EC.presence_of_element_located((By.CLASS_NAME, 'order-head'))  # 結果表格的 XPath
    )
    
    card_id = driver.find_element(By.CLASS_NAME, "font18")

    header_elements = driver.find_element(By.CLASS_NAME, 'order-head').find_elements(By.TAG_NAME, 'th')
    headers = ['訂票代碼', header_elements[0].text.strip(), header_elements[1].text.replace("\n", " ").strip(), header_elements[4].text.strip()]

    ticket_table = pt.PrettyTable()
    ticket_table.field_names = headers

    row_elements = driver.find_element(By.CLASS_NAME, 'passTr').find_elements(By.TAG_NAME, 'td')
    row_data = [card_id.text.strip(), row_elements[0].text.strip(), row_elements[1].text.strip(), row_elements[4].text.strip()]
    ticket_table.add_row(row_data)


    print("\n購票資訊")
    print(ticket_table, '\n')

    print('購票成功!!!')
    # driver.quit()

except Exception as e:
    print(f"發生錯誤: {e}")