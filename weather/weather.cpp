#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <map>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class Weather {
private:

    map<string, map<string, string>> translation = {

        {"en", {{"City", "City"}, {"Temperature", "Temperature"}, {"MaxTemp", "Max Temp"},
                {"MinTemp", "Min Temp"}, {"Pressure", "Pressure"}, {"Humidity", "Humidity"}}},
        {"zh_tw", {{"City", "城市"}, {"Temperature", "溫度"}, {"MaxTemp", "最高溫"},
                {"MinTemp", "最低溫"}, {"Pressure", "氣壓"}, {"Humidity", "濕度"}}},
        {"zh_cn", {{"City", "城市"}, {"Temperature", "温度"}, {"MaxTemp", "最高温"},
                {"MinTemp", "最低温"}, {"Pressure", "气压"}, {"Humidity", "湿度"}}},
        {"fr", {{"City", "Ville"}, {"Temperature", "Température"}, {"MaxTemp", "Temp max"},
                {"MinTemp", "Temp min"}, {"Pressure", "Pression"}, {"Humidity", "Humidité"}}},
        {"de", {{"City", "Stadt"}, {"Temperature", "Temperatur"}, {"MaxTemp", "Höchsttemp"},
                {"MinTemp", "Mindesttemp"}, {"Pressure", "Druck"}, {"Humidity", "Feuchtigkeit"}}},
        {"ja", {{"City", "都市"}, {"Temperature", "気温"}, {"MaxTemp", "最高気温"},
                {"MinTemp", "最低気温"}, {"Pressure", "気圧"}, {"Humidity", "湿度"}}},
        {"kr", {{"City", "도시"}, {"Temperature", "온도"}, {"MaxTemp", "최고 온도"},
                {"MinTemp", "최저 온도"}, {"Pressure", "기압"}, {"Humidity", "습도"}}},
        {"es", {{"City", "Ciudad"}, {"Temperature", "Temperatura"}, {"MaxTemp", "Temp. máx."},
                {"MinTemp", "Temp. mín."}, {"Pressure", "Presión"}, {"Humidity", "Humedad"}}},
        
    };
    
    string api_key;
    string url;
    string city_name;
    string weather_condition;
    string language_code;

    int temp;
    int humidity;
    int pressure;
    int temp_max;
    int temp_min;

    // 處理 API 回應的函式
    static size_t WriteCallback(void* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append((char*)ptr, size * nmemb);
        return size * nmemb;
    }

    // 獲取 API 數據
    string fetchData() {
        CURL* curl = curl_easy_init();
        string response;

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);  // 允許 HTTP 轉向

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                cerr << "CURL error: " << curl_easy_strerror(res) << endl;
            }

            curl_easy_cleanup(curl);
        }

        return response;
    }
public:
    Weather(const string &key, const string &city, const string &lang): api_key(key), city_name(city), language_code(lang) {
        url = "http://api.openweathermap.org/data/2.5/weather?q=" + city_name + "&appid=" + api_key + "&units=metric&lang="
              + language_code;
    }

    void run() {
        bool success = parseWeatherData();
        if (success) {
            outputData();
            saveHistory();
        }
    }

    bool parseWeatherData() {
        string jsonData = fetchData();
        json data = json::parse(jsonData);

        // json 原始碼
        // cout << "Raw json data: " << jsonData << endl;

        if (data.contains("message")) {
            std::cerr << "Error from API: " << data["message"] << std::endl;
            return false;
        }

        if (data.contains("weather") && data.contains("main") && data.contains("sys")) {
            weather_condition = data["weather"][0]["description"];
            temp = data["main"]["temp"];
            temp_max = data["main"]["temp_max"];
            temp_min = data["main"]["temp_min"];
            pressure = data["main"]["pressure"];
            humidity = data["main"]["humidity"];
            return true;
        } else {
            cout << "Invalid JSON structure" << endl;
        }
        return false;
    }

    void saveHistory() {
        char save;
        cout << "Do you want to save ? (y/n): ";
        cin >> save;

        if (save == 'y') {
            fstream file;
            time_t now = time(0);
            string current_time = ctime(&now);

            file.open("weather_history.txt", ios::app);

            if (!file.is_open()) {
                cout << "Error opening file for writing" << endl;
                return;
            }

        file << current_time;
        file << translation[language_code]["City"]<< ": " << city_name << endl;
        file << translation[language_code]["Temperature"]<< ": " << temp << "°C" << endl;
        file << translation[language_code]["MaxTemp"]<< ": " << temp_max << "°C" << endl;
        file << translation[language_code]["MinTemp"]<< ": " << temp_min << "°C" << endl;
        file << translation[language_code]["Pressure"]<< ": " << pressure << "hPa" << endl;
        file << translation[language_code]["Humidity"]<< ": " << humidity << "%" << endl << endl;

            file.close();
        }

    }

    void getHistoryData() {
        fstream file;
        file.open("weather_history.txt", ios::in);

        if (!file.is_open()) {
            cout << "Failed to open the file" << endl;
            return;
        }

        string line;
        string num;
        cout << "How many pieces of data do you want (or type all): ";
        cin >> num;
        cout << "========= Weather History =========" << endl;

        if (num == "all") {
            while (getline(file, line)) {
                cout << line << endl;
            }

        } else {
            int count = 0;
            int targetCount = stoi(num);
            while (count < targetCount && getline(file, line)) {
                
                cout << line << endl;
                
                if (line.empty()) {
                    count++;
                }
            }
        }

        file.close();
    }

    void outputData() {
        
        cout << translation[language_code]["City"]<< ": " << city_name << endl;
        cout << translation[language_code]["Temperature"]<< ": " << temp << "°C" << endl;
        cout << translation[language_code]["MaxTemp"]<< ": " << temp_max << "°C" << endl;
        cout << translation[language_code]["MinTemp"]<< ": " << temp_min << "°C" << endl;
        cout << translation[language_code]["Pressure"]<< ": " << pressure << "hPa" << endl;
        cout << translation[language_code]["Humidity"]<< ": " << humidity << "%" << endl << endl;
        
    }
};

int main() {
    system("chcp 65001 > nul");
    map<int, string> langs = {
        {1, "en"}, {2, "zh_tw"},
        {3, "zh_cn"}, {4, "fr"},
        {5, "de"}, {6, "ja"},
        {7, "kr"}, {8, "es"}
    };

    string city;
    string selected_lang;
    string API_KEY = "2872c1495047301be6e0ea08b32aa1c0";
    char save;
    int code;

    cout << "Select language by number (1.English 2.繁體中文 3.简体中文 4.français 5.Deutsch 6.日本語 7.한국어 8.Español): ";
    (cin >> code).get();

    if (langs.find(code) != langs.end()) {
        selected_lang = langs[code];
    } else {
        selected_lang = "en";
    }
    
    cout << "Enter city name (or type 'history' to get history data): ";
    getline(cin, city);

    if (city == "history") {
        Weather getHistory(API_KEY, "", "");
        getHistory.getHistoryData();

    } else {
        Weather Client(API_KEY, city, selected_lang);
        Client.run();        
    }

    return 0;
}