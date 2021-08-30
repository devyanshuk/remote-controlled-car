set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(include_dirs include/ )

set(srcs
    "main.cpp"
    "src/arduinointerface.cpp"
    "src/driver.cpp"
    "src/wifi.cpp"
    "src/server.cpp"
    "src/rest_server_handler.cpp"
    "src/spiffs.cpp"
    "src/servo.cpp"
    "src/ultrasonic.cpp"
    "src/autonomous.cpp"
)

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}")

spiffs_create_partition_image(UI ../UI FLASH_IN_PROJECT)