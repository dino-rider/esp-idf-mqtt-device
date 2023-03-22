#include "Node.hpp"
#include "IotDevice.hpp"

void Node::publish(const string message, mqtt::QoS qos, bool retain)
{
 device->publish(topic, message, qos, retain);
}

void Node::init()
{
  // device->getClient()->usubscribe(topic, static_cast<MqttCaller*>(this), 0);
  device->getClient()->usubscribe(topic, this, 0);
}

void Node::storeSettings()
{

}

void Node::readSettings()
{

}

Node::Node(IotDevice *_device, string _topic): device(_device)
{
  setTopic(_topic);
}

void Node::read()
{

}

void Node::setTopic(string _topic)
{
  topic = device->getMainTopic()+"/"+_topic;
}

string Node::getTopic()
{
 return topic;
}

void Node::cMessageReceivedCallback(const string &topicStr, const string &message)
{

}

// WifiStrengthSensor::WifiStrengthSensor(IotDevice *_device, string _topic): Sensor(_device,_topic)
// {
//   printf("wifi signal topic: %s \n", topic.c_str());
// }

// int WifiStrengthSensor::clamp(int value, int min, int max)
// {
//   if (value<min) return min;
//   if (value>max) return max;
//   return value;
// }

// void WifiStrengthSensor::read()
// {
//   esp_wifi_sta_get_ap_info(&ap);
//   int reading = ap.rssi;
//   if (abs(value-reading)>5)
//   {
//     value = reading;
//     percent = clamp(2*(100 + value),0,100);
//     onChange();
//   }
// }

// void WifiStrengthSensor::onChange()
// {
//   publish(std::to_string(percent),mqtt::QoS::AtMostOnce ,true);
// }


// LedOutput::LedOutput(IotDevice *_device, string _topic, int _ledPin): Output(_device,_topic) ,ledPin(_ledPin), gpio_0(GPIO_Output(GPIONum(ledPin)))
// {
//   printf("onBoardLed topic: %s \n", topic.c_str());
// }

// void LedOutput::execute(string command)
// {
//     printf("LED command: %s \n", command.c_str());

//   if (command == "ON")
//   {
//     printf("Onboard LED ON\n");
//     gpio_0.set_high();
//     return;
//   }
//   if (command == "OFF")
//   {
//     printf("Onboard LED OFF\n");
//     gpio_0.set_low();
//     return;
//   }
// }