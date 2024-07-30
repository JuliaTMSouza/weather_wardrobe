#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <thread>
#include <unistd.h>
#include "json.hpp" // Biblioteca para manipulação de JSON (nlohmann::json)
#include "mqtt/client.h" // Biblioteca Paho MQTT para comunicação MQTT
#include <iomanip>
#include <curl/curl.h> // Biblioteca CURL para realizar requisições HTTP

#define QOS 1 // Qualidade do Serviço (QoS) para as mensagens MQTT
#define BROKER_ADDRESS "tcp://localhost:1883" // Endereço do broker MQTT
#define INIT_MESSAGE_INTERVAL 60 // Intervalo para publicar a mensagem inicial em segundos
#define API_KEY "452ed16bb510843623b6b138b616351f" // Chave da API do OpenWeatherMap
#define CITY_ID "3470127" // ID da cidade do OpenWeatherMap

// Função para obter o timestamp atual no formato ISO 8601
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::gmtime(&now_c);
    std::stringstream ss;
    ss << std::put_time(now_tm, "%FT%TZ");
    return ss.str();
}

// Callback para escrever a resposta da requisição HTTP no buffer
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch(std::bad_alloc &e) {
        // Tratar problema de memória
        return 0;
    }
    return newLength;
}

// Função para obter dados climáticos da API do OpenWeatherMap
nlohmann::json getWeatherData() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init(); // Inicializa a sessão CURL
    if(curl) {
        std::string url = "http://api.openweathermap.org/data/2.5/weather?id=" + std::string(CITY_ID) + "&appid=" + std::string(API_KEY) + "&units=metric";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // Define a URL da requisição
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Define o callback para escrever os dados recebidos
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // Define o buffer para armazenar a resposta
        res = curl_easy_perform(curl); // Executa a requisição

        // Limpa a sessão CURL
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            return nlohmann::json::parse(readBuffer); // Parsea a resposta JSON e retorna
        }
    }
    return nullptr;
}

nlohmann::json getUvIndexData() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init(); // Inicializa a sessão CURL
    if(curl) {
        std::string url = "http://api.openweathermap.org/data/2.5/onecall?lat=LATITUDE&lon=LONGITUDE&exclude=hourly,daily&appid=" + std::string(API_KEY);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // Define a URL da requisição
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Define o callback para escrever os dados recebidos
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); // Define o buffer para armazenar a resposta
        res = curl_easy_perform(curl); // Executa a requisição

        // Limpa a sessão CURL
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            return nlohmann::json::parse(readBuffer); // Parsea a resposta JSON e retorna
        }
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    std::string clientId = "sensor-monitor"; // ID do cliente MQTT
    mqtt::client client(BROKER_ADDRESS, clientId);

    // Conecta ao broker MQTT
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20); // Intervalo de keep alive
    connOpts.set_clean_session(true); // Inicia uma sessão limpa

    try {
        client.connect(connOpts); // Tenta conectar ao broker
    } catch (mqtt::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::clog << "Connected to the broker" << std::endl;

    // Obtém o identificador único da máquina, neste caso, o hostname.
    char hostname[1024];
    gethostname(hostname, 1024);
    std::string machineId(hostname);

    // Mensagem inicial de configuração
    auto lastInitMessageTime = std::chrono::system_clock::now() - std::chrono::seconds(INIT_MESSAGE_INTERVAL);

    while (true) {
        auto now = std::chrono::system_clock::now();

        // Publica a mensagem inicial de configuração a cada INIT_MESSAGE_INTERVAL segundos.
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastInitMessageTime).count() >= INIT_MESSAGE_INTERVAL) {
            nlohmann::json initMsg;
            initMsg["machine_id"] = machineId;
            initMsg["sensors"] = {
                { {"sensor_id", "temperature"}, {"data_type", "float"}, {"data_interval", 1000} },
                { {"sensor_id", "humidity"}, {"data_type", "float"}, {"data_interval", 1000} }
                { {"sensor_id", "uv_index"}, {"data_type", "float"}, {"data_interval", 1000} }
            };

            mqtt::message initMessage("/sensor_monitors", initMsg.dump(), QOS, false);
            client.publish(initMessage);
            std::clog << "Initial configuration message published: " << initMsg.dump() << std::endl;

            lastInitMessageTime = now;
        }

        // Obtém o timestamp atual no formato ISO 8601.
        std::string timestamp = getCurrentTimestamp();

        // Obtém os dados climáticos
        nlohmann::json weatherData = getWeatherData();
        float temperature = weatherData["main"]["temp"];
        float humidity = weatherData["main"]["humidity"];

        // Obtenha o índice UV
        nlohmann::json uvIndexData = getUvIndexData();
        float uvIndex = uvIndexData["current"]["uvi"];

        // Constrói as mensagens JSON.
        nlohmann::json tempMsg;
        tempMsg["timestamp"] = timestamp;
        tempMsg["value"] = temperature;

        nlohmann::json humidityMsg;
        humidityMsg["timestamp"] = timestamp;
        humidityMsg["value"] = humidity;


        nlohmann::json uvIndexMsg;
        uvIndexMsg["timestamp"] = timestamp;
        uvIndexMsg["value"] = uvIndex;

        // Publica as mensagens JSON nos tópicos apropriados.
        std::string tempTopic = "/sensors/" + machineId + "/temperature";
        mqtt::message tempMessage(tempTopic, tempMsg.dump(), QOS, false);
        client.publish(tempMessage);
        std::clog << "Temperature message published - topic: " << tempTopic << " - message: " << tempMsg.dump() << std::endl;

        std::string humidityTopic = "/sensors/" + machineId + "/humidity";
        mqtt::message humidityMessage(humidityTopic, humidityMsg.dump(), QOS, false);
        client.publish(humidityMessage);
        std::clog << "Humidity message published - topic: " << humidityTopic << " - message: " << humidityMsg.dump() << std::endl;

        std::string uvIndexTopic = "/sensors/" + machineId + "/uv_index";
        mqtt::message uvIndexMessage(uvIndexTopic, uvIndexMsg.dump(), QOS, false);
        client.publish(uvIndexMessage);
        std::clog << "UV Index message published - topic: " << uvIndexTopic << " - message: " << uvIndexMsg.dump() << std::endl;

        // Dorme por algum tempo.
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return EXIT_SUCCESS;
}
