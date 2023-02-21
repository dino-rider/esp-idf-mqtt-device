#include "IotDevice.hpp"

// void onMessage(const string &topicStr, const string &message);
void onConnectionEstablished();

IotDevice::IotDevice(MyClient &_client): client(_client)
{
  // client = _client;
  // use credentials here
  // client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  // client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
  // client.enableLastWillMessage("den-k-89-house/test-device/status", "lost", true);  // You can activate the retain flag by setting the third parameter to true
  
  //adding peripherials to vectors
  outputs.push_back(&onboardLed);
}

void IotDevice::cVoidCallback()
{
  printf("Connected");
  client.usubscribe("den-k-89-house/test-device/#", static_cast<MqttCaller*>(this), 0);
  for (Sensor* sensor: sensors)
  {
    (*sensor).subscribe();
  }
  for (Output* output: outputs)
  {
    (*output).subscribe();
  }
  client.upublish("den-k-89-house/test-device/status", "ready", true);
}

void IotDevice::cMessageReceivedCallback(const string &topicStr, const string &message)
{
  printf(topicStr.c_str());
  printf(message.c_str());
}

void IotDevice::publish(string topic, string message, bool retain)
{
  client.upublish(topic, message, retain);
}

void IotDevice::process()
{
  // client.loop();
}
