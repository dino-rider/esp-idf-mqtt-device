#pragma once
#include <vector>
#include <string>
#include "MqttClient.hpp"
#include "Peripherals.hpp"

constexpr auto *TAG = "MQTT_DEVICE";

using namespace idf;
using namespace std;

class MyClient;
class Sensor;
class Output;
class LedOutput;

class IotDevice: public MqttCaller
{
private:
  string mainTopic = "den-k-89-house/test-device";
  MyClient &client;
  std::vector< Sensor* > sensors;
  std::vector< Output* > outputs;

  // set up peripherials
  

public:
  IotDevice(MyClient &_client);
  MyClient* getClient(){return &client;};
  void addSensor(Sensor *_sensor);
  void addOutput(Output *_output);
  void cOnConnectCallback();
  void cMessageReceivedCallback(const string &topicStr, const string &message);
  void publish(string topic, string message, idf::mqtt::QoS qos, bool retain);
  void process();
  int  getStatus();
  string getMainTopic(){return mainTopic;};
};
