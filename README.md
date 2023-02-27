Mqtt Iot device firmware (framework).

Features:

 с++ mqtt client, automatically reconnects to WiFi and MQTT broker
 
  Object oriented extandable framework, devices know their sub-topics, and directly recieve   	callback onMessage thanks to inheriting MqttCaller interface.
 
  Separate xTask blinks onboard LED when connection is lost.
 
  Implemented wifi signal strength collection and onChange publishing using the Sensor base class.

  Main topic can be configured in menu MQTT_TEST_SETUP.
 
  Wifi credentials can be configured in Example_Connection_Configuration.
 
  Device subtopics:
 
   onBoardLed: [main_topic]/light_0 (commands: "ON", "OFF")
   
   Wifi rssi : [main_topic]/wifisignal (it publishes when the value changes by 5 or more points in 2 seconds]
   
   Device status: [main_topic]/status - it publishes "ready" on connection, and leaves a "lost" message on ungraceful disconnect thanks to last will message.

How to install:
1. Clone repository to esp folder.
2. run  "idf.py menuconfig"
3. Configure Wifi and MQTT settings. !!! Enable COMPILER_CXX_EXCEPTIONS !!!
4. run "idf.py build"
5. run "idf.py -p [device port] flash
6. pray that it compiles (it should, it's using unmodified esp-idf 4.4)
7. if it doesn't - try again after "idf.py fullclean"

Thank's in advance for your input and directions to make it better.
