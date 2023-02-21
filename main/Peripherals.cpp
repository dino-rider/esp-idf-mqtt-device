#include "Peripherals.hpp"
#include "IotDevice.hpp"

void Peripheral::publish(const string message, bool retain)
{
 device->publish(topic, message, retain);
}

void Peripheral::subscribe()
{
  device->getClient()->usubscribe(topic, static_cast<MqttCaller*>(this), 0);
}

void Peripheral::storeState()
{

}

void Peripheral::readState()
{

}

Peripheral::Peripheral(IotDevice *_device, string _topic): device(_device)
{
  topic = device->getMainTopic()+"/"+_topic;
}

string Peripheral::read()
{
  return "test";
}

void Peripheral::setTopic(string _topic)
{
  topic = device->getMainTopic()+"/"+_topic;
}

string Peripheral::getTopic()
{
 return topic;
}

void Peripheral::cMessageReceivedCallback(const string &topicStr, const string &message)
{

}

Output::Output(IotDevice *_device, string _topic): Peripheral(_device, _topic)
{

}

void Output::cMessageReceivedCallback(const string &topicStr, const string &message)
{
  execute(message);
}

LedOutput::LedOutput(IotDevice *_device, string _topic, int _ledPin): Output(_device,_topic) ,ledPin(_ledPin), state(false)
{
  // pinMode(ledPin, OUTPUT);
  printf("onBoardLed topic:");
  printf(topic.c_str());
}

void LedOutput::execute(string &command)
{
  if (command == "ON")
  {
    printf("Onboard LED ON");
    // digitalWrite(ledPin, HIGH);
    return;
  }
  if (command == "OFF")
  {
    printf("Onboard LED OFF");
    // digitalWrite(ledPin, LOW);
    return;
  }
}