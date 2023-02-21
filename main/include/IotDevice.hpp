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
  LedOutput onboardLed{this, "light_0", 2};
public:
  IotDevice(MyClient &_client);
  MyClient* getClient(){return &client;};
  void cVoidCallback();
  void cMessageReceivedCallback(const string &topicStr, const string &message);
  void publish(string topic, string message, bool retain);
  void process();
  int  getStatus();
  string getMainTopic(){return mainTopic;};
};
