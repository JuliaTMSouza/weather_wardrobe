#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>
#include "json.hpp" // json handling
#include "mqtt/client.h" // Paho MQTT
#include <iomanip>
#include <fstream>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <curl/curl.h> // CURL for HTTP requests

#define QOS 1
#define BROKER_ADDRESS "tcp://localhost:1883"
#define INIT_MESSAGE_INTERVAL 60
#define API_KEY "452ed16bb510843623b6b138b616351f"
#define CITY_ID "3470127"
#define DEFAULT_TIMESPAN 3

class SensorConfig {
public:
    std::string sensor_id;
    std::string data_type;
    int data_interval;
};

bool getConfigFileActive(const std::string& sensor_id) {
    std::ifstream file("sensors_config.json");
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de configuração." << std::endl;
        return true;
    }

    nlohmann::json config;
    file >> config;
    file.close();

    for (const auto& obj : config) {
        std::string id = obj["id"];
        if (id == sensor_id) {
            return obj["active"];
        }
    }
    return true;
}

int getConfigFileTimespan(const std::string& sensor_id) {
    std::ifstream file("sensors_config.json");
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de configuração." << std::endl;
        return DEFAULT_TIMESPAN;
    }

    nlohmann::json config;
    file >> config;
    file.close();

    for (const auto& obj : config) {
        std::string id = obj["id"];
        if (id == sensor_id) {
            return obj["timespan"];
        }
    }
    return DEFAULT_TIMESPAN;
}

bool isDiskSensorWorking = getConfigFileActive("diskUsage");
bool isRamSensorWorking = getConfigFileActive("ramMemoryUsage");

double getDiskUsagePercentage(const std::string& path) {
    struct statvfs64 fsInfo;
    if (statvfs64(path.c_str(), &fsInfo) != 0) {
        isDiskSensorWorking = false;
        std::cerr << "Erro ao obter informações do sistema de arquivos." << std::endl;
        return 0.0;
    }

    isDiskSensorWorking = true;
    unsigned long long totalSpace = fsInfo.f_blocks * fsInfo.f_frsize;
    unsigned long long freeSpace = fsInfo.f_bfree * fsInfo.f_frsize;
    unsigned long long usedSpace = totalSpace - freeSpace;
    return (usedSpace / static_cast<double>(totalSpace)) * 100.0;
}

double getMemoryUsagePercentage() {
    struct sysinfo memInfo;
    if (sysinfo(&memInfo) != 0) {
        isRamSensorWorking = false;
        std::cerr << "Falha ao obter informações de memória." << std::endl;
        return 0.0;
    }

    isRamSensorWorking = true;
    unsigned long totalMemory = memInfo.totalram;
    unsigned long freeMemory = memInfo.freeram;
    unsigned long usedMemory = totalMemory - freeMemory;
    return (usedMemory / static_cast<double>(totalMemory)) * 100;
}

void sendMessage(mqtt::client& client, const std::string& sensor_id, double value) {
    char hostname[1024];
    gethostname(hostname, 1024);
    std::string machineId(hostname);

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::gmtime(&now_c);
    std::stringstream ss;
    ss << std::put_time(now_tm, "%FT%TZ");
    std::string timestamp = ss.str();

    nlohmann::json j;
    j["timestamp"] = timestamp;
    j["value"] = value;

    std::string topic = "/sensors/" + machineId + "/" + sensor_id;
    mqtt::message msg(topic, j.dump(), QOS, false);
    std::clog << "message published - topic: " << topic << " - message: " << j.dump() << std::endl;
    client.publish(msg);
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch (std::bad_alloc& e) {
        return 0;
    }
    return newLength;
}

nlohmann::json getWeatherData() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "http://api.openweathermap.org/data/2.5/weather?id=" + std::string(CITY_ID) + "&appid=" + std::string(API_KEY) + "&units=metric";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            return nlohmann::json::parse(readBuffer);
        }
    }
    return nullptr;
}

void initialMessage(mqtt::client& client) {
    while (true) {
        int timespan = getConfigFileTimespan("initialMessage");
        std::vector<SensorConfig> sensorArray;

        char hostname[1024];
        gethostname(hostname, 1024);
        std::string machineId(hostname);

        if (isDiskSensorWorking) {
            SensorConfig diskSensor;
            diskSensor.sensor_id = "diskUsage";
            diskSensor.data_type = "double";
            diskSensor.data_interval = getConfigFileTimespan("diskUsage");
            sensorArray.push_back(diskSensor);
        }
        if (isRamSensorWorking) {
            SensorConfig ramSensor;
            ramSensor.sensor_id = "ramMemoryUsage";
            ramSensor.data_type = "double";
            ramSensor.data_interval = getConfigFileTimespan("ramMemoryUsage");
            sensorArray.push_back(ramSensor);
        }

        nlohmann::json j;
        j["machine_id"] = machineId;
        j["sensors"] = nlohmann::json::array();
        for (const auto& sensor : sensorArray) {
            nlohmann::json sensorJson;
            sensorJson["sensor_id"] = sensor.sensor_id;
            sensorJson["data_type"] = sensor.data_type;
            sensorJson["data_interval"] = sensor.data_interval;
            j["sensors"].push_back(sensorJson);
        }

        std::string topic = "/sensor_monitors";
        mqtt::message msg(topic, j.dump(), QOS, false);
        std::clog << "message published - topic: " << topic << " - message: " << j.dump() << std::endl;
        client.publish(msg);
        std::this_thread::sleep_for(std::chrono::seconds(timespan));
    }
}

void diskSensor(mqtt::client& client) {
    while (true) {
        if (getConfigFileActive("diskUsage")) {
            int timespan = getConfigFileTimespan("diskUsage");
            std::string path = "/"; // Root directory
            double diskUsage = getDiskUsagePercentage(path);
            sendMessage(client, "diskUsage", diskUsage);
            std::this_thread::sleep_for(std::chrono::seconds(timespan));
        }
    }
}

void ramSensor(mqtt::client& client) {
    while (true) {
        if (getConfigFileActive("ramMemoryUsage")) {
            int timespan = getConfigFileTimespan("ramMemoryUsage");
            double memoryUsage = getMemoryUsagePercentage();
            sendMessage(client, "ramMemoryUsage", memoryUsage);
            std::this_thread::sleep_for(std::chrono::seconds(timespan));
        }
    }
}

void weatherSensor(mqtt::client& client) {
    while (true) {
        std::string timestamp = getCurrentTimestamp();
        nlohmann::json weatherData = getWeatherData();
        if (!weatherData.is_null()) {
            float temperature = weatherData["main"]["temp"];
            float humidity = weatherData["main"]["humidity"];

            nlohmann::json tempMsg;
            tempMsg["timestamp"] = timestamp;
            tempMsg["value"] = temperature;

            nlohmann::json humidityMsg;
            humidityMsg["timestamp"] = timestamp;
            humidityMsg["value"] = humidity;

            char hostname[1024];
            gethostname(hostname, 1024);
            std::string machineId(hostname);

            std::string tempTopic = "/sensors/" + machineId + "/temperature";
            mqtt::message tempMessage(tempTopic, tempMsg.dump(), QOS, false);
            client.publish(tempMessage);
            std::clog << "Temperature message published - topic: " << tempTopic << " - message: " << tempMsg.dump() << std::endl;

            std::string humidityTopic = "/sensors/" + machineId + "/humidity";
            mqtt::message humidityMessage(humidityTopic, humidityMsg.dump(), QOS, false);
            client.publish(humidityMessage);
            std::clog << "Humidity message published - topic: " << humidityTopic << " - message: " << humidityMsg.dump() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char* argv[]) {
    std::string clientId = "sensor-monitor";
    mqtt::client client(BROKER_ADDRESS, clientId);

    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try {
        client.connect(connOpts);
    } catch (mqtt::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::clog << "Connected to the broker" << std::endl;

    std::thread initialMsgThread(&initialMessage, std::ref(client));
    std::thread diskThread(&diskSensor, std::ref(client));
    std::thread ramThread(&ramSensor, std::ref(client));
    std::thread weatherThread(&weatherSensor, std::ref(client));

    initialMsgThread.join();
    diskThread.join();
    ramThread.join();
    weatherThread.join();
    client.disconnect();
    return EXIT_SUCCESS;
}