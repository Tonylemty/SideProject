#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class Weather {
private:
    string api_key;
    string url;
    string city_name;
    string weather_condition;
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
    Weather(const string &key, const string &city): api_key(key), city_name(city) {
        url = "http://api.openweathermap.org/data/2.5/weather?q=" + city_name + "&appid=" + api_key + "&units=metric";
    }

    // void run() {
    //     parseWeatherData();
    //     outputData();
    // }

    void parseWeatherData() {
        string jsonData = fetchData();
        json data = json::parse(jsonData);

        // json 原始碼
        cout << "Raw json data: " << jsonData << endl;

        if (data.contains("message")) {
            std::cerr << "Error from API: " << data["message"] << std::endl;
            return;
        }

        if (data.contains("weather") && data.contains("main") && data.contains("sys")) {
            weather_condition = data["weather"][0]["description"];
            temp = data["main"]["temp"];
            temp_max = data["main"]["temp_max"];
            temp_min = data["main"]["temp_min"];
            pressure = data["main"]["pressure"];
            humidity = data["main"]["humidity"];  
        } else {
            cout << "Invalid JSON structure" << endl;
        }
    }

    void saveHistory() {
        fstream file;
        time_t now = time(0);
        string current_time = ctime(&now);

        file.open("weather_history", ios::app);

        if (!file.is_open()) {
            cout << "Error opening file for writing" << endl;
            return;
        }

        file << current_time << endl;
        file << "City: " << city_name << endl;
        file << "Temperature: " << temp << "°C" << endl;
        file << "Max temperature: " << temp_max << "°C" << endl;
        file << "Min temperature: " << temp_min << "°C" << endl;
        file << "Pressure: " << pressure << "hPa" << endl;
        file << "Humidity: " << humidity << "%" << endl;

        file.close();

    }

    void getHistoryData() {
        fstream file;
        file.open("weather_history", ios::in);

        if (!file.is_open()) {
            cout << "Failed to open the file" << endl;
            return;
        }

        string line;
        cout << "========= Weather History =========" << endl;
        while (getline(file, line)) {
            cout << line << endl;
        }

        file.close();
    }

    void outputData() {

        cout << "The following is " << city_name << " city weather data" << endl;
        
        cout << setw(19) << left << "Temperature"
             << setw(19) << "Max temperature"
             << setw(19) << "Min temperature" << endl;
    

        cout << setw(20) << left << to_string(temp) + "°C"
             << setw(20) << to_string(temp_max) + "°C"
             << setw(20) << to_string(temp_min) + "°C" << endl;

        cout << "----------------------------------------------------" << endl;

        cout << setw(20) << left << "Weather"
             << setw(20) << "Pressure"
             << setw(20) << "Humidity" << endl;

        cout << setw(20) << left << weather_condition
             << setw(20) << to_string(pressure) + "hPa"
             << setw(20) << to_string(humidity) + "%" << endl;
    }
};

int main() {
    string city;
    string API_KEY = "2872c1495047301be6e0ea08b32aa1c0";
    cout << "Enter city name: ";
    getline(cin, city);

    system("chcp 65001 > nul");
    Weather Client(API_KEY, city);

    return 0;
}