#include "Peripherals.hpp"
#include "IotDevice.hpp"

void Peripheral::publish(const string message, mqtt::QoS qos, bool retain)
{
 device->publish(topic, message, qos, retain);
}

void Peripheral::subscribe()
{
  // device->getClient()->usubscribe(topic, static_cast<MqttCaller*>(this), 0);
  device->getClient()->usubscribe(topic, this, 0);
}

void Peripheral::storeSettings()
{

}

void Peripheral::readSettings()
{

}

Peripheral::Peripheral(IotDevice *_device, string _topic): device(_device)
{
  topic = device->getMainTopic()+"/"+_topic;
}

void Peripheral::read()
{
  
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

Sensor::Sensor(IotDevice *_device, string _topic): Peripheral(_device, _topic)
{

}

void Sensor::Sensor::cMessageReceivedCallback(const string &topicStr, const string &message)
{

}



Output::Output(IotDevice *_device, string _topic): Peripheral(_device, _topic)
{

}

void Output::cMessageReceivedCallback(const string &topicStr, const string &message)
{
  execute(message);
}


WifiStrengthSensor::WifiStrengthSensor(IotDevice *_device, string _topic): Sensor(_device,_topic)
{
  printf("wifi signal topic: %s \n", topic.c_str());
   
}

void WifiStrengthSensor::read()
{
  esp_wifi_sta_get_ap_info(&ap); 
  int reading = ap.rssi;
  if (abs(value-reading)>5)
  {
    value = reading;
    onChange();
  }
}

void WifiStrengthSensor::onChange()
{
  publish(std::to_string(value),mqtt::QoS::AtMostOnce ,true);
}


LedOutput::LedOutput(IotDevice *_device, string _topic, int _ledPin): Output(_device,_topic) ,ledPin(_ledPin), gpio_0(GPIO_Output(GPIONum(ledPin)))
{
  printf("onBoardLed topic: %s \n", topic.c_str());
}

void LedOutput::execute(string command)
{
    printf("LED command: %s \n", command.c_str());

  if (command == "ON")
  {
    printf("Onboard LED ON\n");
    gpio_0.set_high();
    return;
  }
  if (command == "OFF")
  {
    printf("Onboard LED OFF\n");
    gpio_0.set_low();
    return;
  }
}