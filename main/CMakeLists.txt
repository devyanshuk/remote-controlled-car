set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPONENT_SRCS "main.cpp" "src/arduinointerface.cpp" "src/driver.cpp" "src/wifi.cpp" "src/server.cpp" "src/rest_server_handler.cpp" "src/spiffs.cpp")
set(COMPONENT_ADD_INCLUDEDIRS "." "include/")

register_component()

spiffs_create_partition_image(UI ../UI FLASH_IN_PROJECT)