#include "MqttClient.hpp"

void MyClient::usubscribe(const string topicStr, MqttCaller* caller, int qos)
{
    mqtt::Filter f{topicStr.c_str()};
    subscribe(f.get());
    topic_callbacks[topicStr] = caller;
}

void MyClient::upublish(const string &topicStr,  const string &message, mqtt::QoS qos, bool retain)
{
    printf("Publishing -- Topic: %s, Message: %s \n", topicStr.c_str(), message.c_str());
    publish<string>(topicStr, mqtt::Message<string>{message , qos , retain ? mqtt::Retain::Retained : mqtt::Retain::NotRetained});
}

void MyClient::on_connected(esp_mqtt_event_handle_t const event)
    {
        using mqtt::QoS;
        connected_flag = true;
        device->cOnConnectCallback();
    }

void MyClient::on_data(esp_mqtt_event_handle_t const event)
    {
        string topicStr;
        string dataStr;
        topicStr.assign(event->topic,event->topic_len);
        dataStr.assign(event->data, event->data_len);
        printf("Topic: %s, Message: %s \n", topicStr.c_str(), dataStr.c_str());
        if (topic_callbacks.find(topicStr)!=topic_callbacks.end())
        {
            printf("Topic: %s is matching to device \n", topicStr.c_str());
            topic_callbacks[topicStr]->cMessageReceivedCallback(topicStr, dataStr);
        }
    }

void MyClient::on_disconnected(esp_mqtt_event_handle_t const event)
    {
        connected_flag = false;
    }
