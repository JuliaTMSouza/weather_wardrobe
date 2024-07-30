#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>
#include "json.hpp"      // json handling
#include "mqtt/client.h" // paho mqtt
#include <iomanip>
#include <fstream>

#define QOS 1
#define BROKER_ADDRESS "tcp://localhost:1883"
#define DEFAULT_TIMESPAN 3

#include <iostream>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>

class SensorConfig
{
public:
    std::string sensor_id;
    std::string data_type;
    int data_interval;
};

bool getConfigFileActive(std::string sensor_id)
{
    std::ifstream file("sensors_config.json");

    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de configuração." << std::endl;
        return true;
    }

    nlohmann::json config;
    file >> config;
    file.close();

    for (const auto &obj : config)
    {
        std::string id = obj["id"];
        if (id == sensor_id)
        {
            bool active = obj["active"];
            return active;
        }
    }
    return true;
}

int getConfigFileTimespan(std::string sensor_id)
{
    std::ifstream file("sensors_config.json");

    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de configuração." << std::endl;
        return DEFAULT_TIMESPAN;
    }

    nlohmann::json config;
    file >> config;
    file.close();

    for (const auto &obj : config)
    {
        std::string id = obj["id"];
        if (id == sensor_id)
        {
            int timespan = obj["timespan"];
            return timespan;
        }
    }
    return DEFAULT_TIMESPAN;
}

bool isDiskSensorWorking = getConfigFileActive("diskUsage");
bool isRamSensorWorking = getConfigFileActive("ramMemoryUsage");

double getDiskUsagePercentage(const std::string &path)
{
    struct statvfs64 fsInfo;

    if (statvfs64(path.c_str(), &fsInfo) != 0)
    {
        isDiskSensorWorking = false;
        std::cerr << "Erro ao obter informações do sistema de arquivos." << std::endl;
        return 0.0;
    }

    isDiskSensorWorking = true;
    unsigned long long totalSpace = fsInfo.f_blocks * fsInfo.f_frsize;
    unsigned long long freeSpace = fsInfo.f_bfree * fsInfo.f_frsize;
    unsigned long long usedSpace = totalSpace - freeSpace;

    double usagePercentage = (usedSpace / static_cast<double>(totalSpace)) * 100.0;

    return usagePercentage;
}

double getMemoryUsagePercentage()
{
    struct sysinfo memInfo;
    if (sysinfo(&memInfo) != 0)
    {
        isRamSensorWorking = false;
        std::cerr << "Falha ao obter informações de memória." << std::endl;
        return 0.0;
    }

    isRamSensorWorking = true;
    unsigned long totalMemory = memInfo.totalram;
    unsigned long freeMemory = memInfo.freeram;
    unsigned long usedMemory = totalMemory - freeMemory;
    double usagePercentage = (usedMemory / (double)totalMemory) * 100;

    return usagePercentage;
}

void sendMessage(mqtt::client &client, std::string sensor_id, double value)
{
    // Get the unique machine identifier, in this case, the hostname.
    char hostname[1024];
    gethostname(hostname, 1024);
    std::string machineId(hostname);

    // Get the current time in ISO 8601 format.
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *now_tm = std::localtime(&now_c);
    std::stringstream ss;
    ss << std::put_time(now_tm, "%FT%TZ");
    std::string timestamp = ss.str();

    // Construct the JSON message.
    nlohmann::json j;
    j["timestamp"] = timestamp;
    j["value"] = value;

    // Publish the JSON message to the appropriate topic.
    std::string topic = "/sensors/" + machineId + "/" + sensor_id;
    mqtt::message msg(topic, j.dump(), QOS, false);
    std::clog << "message published - topic: " << topic << " - message: " << j.dump() << std::endl;
    client.publish(msg);
}

void initialMessage(mqtt::client &client)
{
    while (1)
    {
        int timespan = getConfigFileTimespan("initialMessage");
        std::vector<SensorConfig> sensorArray;
        // Get the unique machine identifier, in this case, the hostname.
        char hostname[1024];
        gethostname(hostname, 1024);
        std::string machineId(hostname);
        if (isDiskSensorWorking)
        {
            SensorConfig diskSensor;
            diskSensor.sensor_id = "diskUsage";
            diskSensor.data_type = "double";
            diskSensor.data_interval = getConfigFileTimespan("diskUsage");
            sensorArray.push_back(diskSensor);
        }
        if (isRamSensorWorking)
        {
            SensorConfig ramSensor;
            ramSensor.sensor_id = "ramMemoryUsage";
            ramSensor.data_type = "double";
            ramSensor.data_interval = getConfigFileTimespan("ramMemoryUsage");
            sensorArray.push_back(ramSensor);
        }

        // Construct the JSON message.
        nlohmann::json j;
        j["machine_id"] = machineId;
        j["sensors"] = nlohmann::json::array();
        for (const auto &sensor : sensorArray)
        {
            nlohmann::json sensorJson;
            sensorJson["sensor_id"] = sensor.sensor_id;
            sensorJson["data_type"] = sensor.data_type;
            sensorJson["data_interval"] = sensor.data_interval;
            j["sensors"].push_back(sensorJson);
        }

        // Publish the JSON message to the appropriate topic.
        std::string topic = "/sensor_monitors";
        mqtt::message msg(topic, j.dump(), QOS, false);
        std::clog << "message published - topic: " << topic << " - message: " << j.dump() << std::endl;
        client.publish(msg);
        std::this_thread::sleep_for(std::chrono::seconds(timespan));
    }
}

void diskSensor(mqtt::client &client)
{
    while (1)
    {
        if (getConfigFileActive("diskUsage"))
        {
            int timespan = getConfigFileTimespan("diskUsage");
            std::string path = "/"; // Diretório raiz
            double diskUsage = getDiskUsagePercentage(path);
            sendMessage(client, "diskUsage", diskUsage);
            std::this_thread::sleep_for(std::chrono::seconds(timespan));
        }
    }
}

void ramSensor(mqtt::client &client)
{
    while (1)
    {
        if (getConfigFileActive("ramMemoryUsage"))
        {
            int timespan = getConfigFileTimespan("ramMemoryUsage");
            double memoryUsage = getMemoryUsagePercentage();
            sendMessage(client, "ramMemoryUsage", memoryUsage);
            std::this_thread::sleep_for(std::chrono::seconds(timespan));
        }
    }
}

int main(int argc, char *argv[])
{

    std::string clientId = "sensor-monitor";
    mqtt::client client(BROKER_ADDRESS, clientId);

    // Connect to the MQTT broker.
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try
    {
        client.connect(connOpts);
    }
    catch (mqtt::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::clog << "connected to the broker" << std::endl;

    std::thread initialMsgThread(&initialMessage, std::ref(client));
    std::thread diskThread(&diskSensor, std::ref(client));
    std::thread ramThread(&ramSensor, std::ref(client));

    diskThread.join();
    ramThread.join();
    client.disconnect();
    return EXIT_SUCCESS;
}