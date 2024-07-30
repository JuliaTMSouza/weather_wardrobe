#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <bson.h>
#include <mongoc.h>
#include "json.hpp"
#include "mqtt/client.h"
#include <fstream>

#define QOS 1
#define BROKER_ADDRESS "tcp://localhost:1883"

class Activity
{
public:
    std::string sensor_id;
    std::string time_limit;
    Activity(const std::string &id, const std::string &limit) : sensor_id(id), time_limit(limit) {}
};

Activity activityDisk("diskUsage", "");
Activity activityRam("ramMemoryUsage", "");
double moveMeanDisk = NULL;
double moveMeanRam = NULL;
void insert_document(mongoc_collection_t *collection, std::string machine_id, std::string timestamp_str, int value, double moveMean)
{
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc;

    std::tm tm{};
    std::istringstream ss{timestamp_str};
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    std::time_t time_t_timestamp = std::mktime(&tm);

    doc = bson_new();
    BSON_APPEND_UTF8(doc, "machine_id", machine_id.c_str());
    BSON_APPEND_TIME_T(doc, "timestamp", time_t_timestamp);
    BSON_APPEND_INT32(doc, "value", value);
    BSON_APPEND_DOUBLE(doc, "moveMean", moveMean);

    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error))
    {
        std::cerr << "Failed to insert doc: " << error.message << std::endl;
    }

    bson_destroy(doc);
}

void insert_alarms(mongoc_collection_t *alarmCollection, std::string sensor_id, std::string machine_id, std::string description)
{
    bson_error_t error;
    bson_t *doc;

    doc = bson_new();
    BSON_APPEND_UTF8(doc, "sensor_id", sensor_id.c_str());
    BSON_APPEND_UTF8(doc, "machine_id", machine_id.c_str());
    BSON_APPEND_UTF8(doc, "description", description.c_str());

    if (!mongoc_collection_insert_one(alarmCollection, doc, NULL, NULL, &error))
    {
        std::cerr << "Failed to insert doc: " << error.message << std::endl;
    }

    bson_destroy(doc);
}

std::vector<std::string> split(const std::string &str, char delim)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delim))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void save_initial_message(nlohmann::json msg)
{
    std::ofstream file("initial_message.json", std::ofstream::trunc);

    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de mensagem inicial." << std::endl;
    }

    file << msg.dump();

    file.close();
}

bool set_activity_alarm(Activity &sensorActivity, time_t newTimeT)
{
    std::tm tm = {};
    std::istringstream ss(sensorActivity.time_limit);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (ss.fail())
    {
        std::cerr << "Erro ao converter a string para time_t." << std::endl;
        return false;
    }

    std::time_t time = std::mktime(&tm);

    if (newTimeT >= time)
    {
        activityDisk.time_limit = "";
        return true;
    }
    else
    {
        // Converte o novo tempo para uma representação de string
        std::tm *newTimeTM = std::gmtime(&newTimeT);
        std::stringstream ss;
        ss << std::put_time(newTimeTM, "%Y-%m-%d %H:%M:%S");
        std::string newTimeString = ss.str();
        activityDisk.time_limit = newTimeString;
        return false;
    }
}

void verify_activity(mongoc_collection_t *alarmCollection, std::string sensor_id, std::string machine_id)
{
    auto currentTime = std::chrono::system_clock::now();

    std::ifstream file("initial_message.json");

    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de configuração." << std::endl;
    }

    nlohmann::json config;
    file >> config;
    file.close();

    int timespan;
    for (const auto &obj : config["sensors"])
    {
        std::string id = obj["sensor_id"];
        if (id == sensor_id)
        {
            timespan = obj["data_interval"];
        }
    }

    // Adiciona os segundos ao tempo atual
    auto newTime = currentTime + std::chrono::seconds(timespan * 10);
    std::time_t newTimeT = std::chrono::system_clock::to_time_t(newTime);
    std::string description = "Sensor inativo por dez períodos de tempo previstos";
    if (sensor_id == "diskUsage")
    {
        int isAlarm = set_activity_alarm(activityDisk, newTimeT);
        if (isAlarm)
        {
            insert_alarms(alarmCollection, sensor_id, machine_id, description);
            mongoc_collection_destroy(alarmCollection);
        }
    }
    else if (sensor_id == "ramMemoryUsage")
    {
        int isAlarm = set_activity_alarm(activityRam, newTimeT);
        if (isAlarm)
        {
            insert_alarms(alarmCollection, sensor_id, machine_id, description);
            mongoc_collection_destroy(alarmCollection);
        }
    }
}

void verify_alarms(mongoc_collection_t *alarmCollection, std::string sensor_id, std::string machine_id, int value)
{
    std::string description;
    bool setAlarm = false;
    if (sensor_id == "ramMemoryUsage")
    {
        if (value > 85)
        {
            description = "Porcentagem de memória RAM utilizada está acima do ideal";
            setAlarm = true;
        }
        else if (value < 65)
        {
            description = "Porcentagem de memória RAM utilizada está abaixo do ideal";
            setAlarm = true;
        }
    }
    else if (sensor_id == "diskUsage")
    {
        if (value > 80)
        {
            description = "Porcentagem de uso de disco está acima do ideal";
            setAlarm = true;
        }
    }
    if (setAlarm)
    {
        insert_alarms(alarmCollection, sensor_id, machine_id, description);
        mongoc_collection_destroy(alarmCollection);
    }
}

double moveMeanCalc(std::string sensor_id, double value)
{
    double result = value;
    if (sensor_id == "diskUsage")
    {
        if(moveMeanDisk == NULL){
            return result;
        }
        else
        {
            result = ((moveMeanDisk + value)/2);
            moveMeanDisk = result;
            return result;
        }
    }
    else if (sensor_id == "ramMemoryUsage")
    {
        if(moveMeanRam == NULL){
            return result;
        }
        else
        {
            result = ((moveMeanRam + value)/2);
            moveMeanRam = result;
            return result;
        }
    }
}

int main(int argc, char *argv[])
{
    std::string clientId = "clientId";
    mqtt::client client(BROKER_ADDRESS, clientId);

    // Initialize MongoDB client and connect to the database.
    mongoc_init();
    mongoc_client_t *mongodb_client = mongoc_client_new("mongodb://localhost:27017");
    mongoc_database_t *database = mongoc_client_get_database(mongodb_client, "sensors_data"); // replace with your database name

    // Create an MQTT callback.
    class callback : public virtual mqtt::callback
    {
        mongoc_database_t *db;

    public:
        callback(mongoc_database_t *db) : db(db) {}

        void message_arrived(mqtt::const_message_ptr msg) override
        {
            auto j = nlohmann::json::parse(msg->get_payload());

            std::string topic = msg->get_topic();
            if (topic == "/sensor_monitors")
            {
                save_initial_message(j);
            }
            else
            {
                auto topic_parts = split(topic, '/');
                std::string machine_id = topic_parts[2];
                std::string sensor_id = topic_parts[3];
                std::string timestamp = j["timestamp"];
                int value = j["value"];
                double moveMean = moveMeanCalc(sensor_id, j["value"]);

                // Get collection and persist the document.
                mongoc_collection_t *collection = mongoc_database_get_collection(db, sensor_id.c_str());
                std::string alarm = "alarms";
                mongoc_collection_t *alarmCollection = mongoc_database_get_collection(db, alarm.c_str());
                verify_activity(alarmCollection, sensor_id, machine_id);
                verify_alarms(alarmCollection, sensor_id, machine_id, value);
                insert_document(collection, machine_id, timestamp, value, moveMean);
                mongoc_collection_destroy(collection);
            }
        }
    };

    callback cb(database);
    client.set_callback(cb);

    // Connect to the MQTT broker.
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    try
    {
        client.connect(connOpts);
        client.subscribe("/sensor_monitors", QOS);
        client.subscribe("/sensors/#", QOS);
    }
    catch (mqtt::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Cleanup MongoDB resources
    mongoc_database_destroy(database);
    mongoc_client_destroy(mongodb_client);
    mongoc_cleanup();

    return EXIT_SUCCESS;
}