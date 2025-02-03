from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
from webdriver_manager.chrome import ChromeDriverManager
import prettytable as pt

# 設定瀏覽器選項
options = Options()
options.add_argument('--headless')  # 啟用無頭模式
options.add_argument('--disable-gpu')  # 禁用 GPU（防止某些環境中啟用時出錯）
options.add_argument('--no-sandbox')  # 避免在某些環境中因為沙箱設定出錯
options.add_experimental_option("detach", True)  # 瀏覽器不會自動關閉

url = 'https://www.railway.gov.tw/tra-tip-web/tip'

# 啟動 Selenium 瀏覽器
service = Service(ChromeDriverManager().install())
driver = webdriver.Chrome(service=service, options=options)
driver.get(url)
driver.maximize_window()

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
    print("結果表格已載入！")
    
    # 爬取表格內容
    table_rows = driver.find_elements(By.CLASS_NAME, 'trip-column')  # 表格每一行
    tb = pt.PrettyTable()
    tb.field_names = ['車種車次', '出發時間', '抵達時間', '行駛時間', '全票', '孩童票', '敬老票']

    for row in table_rows:
        columns = row.find_elements(By.TAG_NAME, 'td')
        if len(columns) >= 8:  # 確保至少有 8 欄資料
            train_type_no = columns[0].text.strip()  # 車種車次
            departure_time = columns[1].text.strip()  # 出發時間
            arrival_time = columns[2].text.strip()  # 抵達時間
            through_time = columns[3].text.strip() # 行駛時間
            full_price = columns[6].text.strip()  # 全票
            child_price = columns[7].text.strip()  # 孩童票
            senior_price = columns[8].text.strip()  # 敬老票

            tb.add_row([train_type_no, departure_time, arrival_time, through_time, full_price, child_price, senior_price])

    # 輸出結果
    print(tb)

except Exception as e:
    print(f"發生錯誤: {e}")

# driver.quit()  # 如需手動檢查結果，可註解掉這一行以保持瀏覽器開啟
