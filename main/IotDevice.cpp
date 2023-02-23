#include "IotDevice.hpp"

IotDevice::IotDevice(MyClient &_client): client(_client)
{
  // client = _client;
  // use credentials here
  // client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  // client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  // client.enableLastWillMessage("den-k-89-house/test-device/status", "lost", true);  // You can activate the retain flag by setting the third parameter to true
  
  //adding peripherials to vectors
  
}

void IotDevice::addOutput(Output *_output)
{
  outputs.push_back(_output);
}

void IotDevice::addSensor(Sensor *_sensor)
{
  sensors.push_back(_sensor);
}

void IotDevice::cOnConnectCallback()
{
  printf("Connected");
  // client.usubscribe("den-k-89-house/test-device/#", static_cast<MqttCaller*>(this), 0);
  // for (Sensor* sensor: sensors)
  // {
  //   sensor->subscribe();
  // }
  for (Output* output: outputs)
  {
    output->subscribe();
  }
  client.upublish("den-k-89-house/test-device/status", "ready", idf::mqtt::QoS::AtLeastOnce, true);
}

void IotDevice::cMessageReceivedCallback(const string &topicStr, const string &message)
{
}

void IotDevice::publish(string topic, string message, idf::mqtt::QoS qos, bool retain)
{
  client.upublish(topic, message, qos, retain);
}


void IotDevice::process()
{
  // printf("reading input\n");
  for (Sensor* sensor: sensors)
  {
    sensor->read();
  }
}
