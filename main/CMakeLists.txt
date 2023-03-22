idf_component_register(SRCS "Node.cpp" "main.cpp" "IotDevice.cpp" "MqttClient.cpp" 
                       INCLUDE_DIRS "./include")
target_compile_options(${COMPONENT_LIB} PRIVATE "-std=gnu++17")
