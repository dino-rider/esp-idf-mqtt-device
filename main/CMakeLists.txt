idf_component_register(SRCS "main.cpp" "IotDevice.cpp" "MqttClient.cpp" "Peripherals.cpp" 
                       INCLUDE_DIRS "./include")
target_compile_options(${COMPONENT_LIB} PRIVATE "-std=gnu++17")
