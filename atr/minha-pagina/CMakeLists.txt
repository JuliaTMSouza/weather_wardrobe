cmake_minimum_required(VERSION 3.14) # Recomendado para FetchContent
project(my_project)

# Definir o padrão C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adicionar os arquivos fonte
set(SOURCES
    data_processor.cpp
    sensor_monitor.cpp
)

# Adicionar o executável
add_executable(my_program ${SOURCES})

# Usar FetchContent para bibliotecas externas

# Biblioteca nlohmann_json
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

# Biblioteca Paho MQTT
FetchContent_Declare(
    paho_mqttpp3
    GIT_REPOSITORY https://github.com/eclipse/paho.mqtt.cpp.git
    GIT_TAG v1.1.1
)
FetchContent_MakeAvailable(paho_mqttpp3)

# Biblioteca libcurl
FetchContent_Declare(
    libcurl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG curl-8_0_1
)
FetchContent_MakeAvailable(libcurl)

# Linkar as bibliotecas
target_link_libraries(my_program PRIVATE
    nlohmann_json::nlohmann_json
    PahoMqttCpp::paho-mqttpp3
    CURL::libcurl
)

# Incluir diretórios de cabeçalhos se necessário
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/path/to/your/headers)
