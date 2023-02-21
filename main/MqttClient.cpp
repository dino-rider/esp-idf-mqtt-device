#include "MqttClient.hpp"

void MyClient::usubscribe(const string &topicStr, MqttCaller* caller, int qos)
{

}

void MyClient::upublish(const string &topicStr,  const string &message, bool retain)
{

}


void MyClient::on_connected(esp_mqtt_event_handle_t const event)
    {
        using mqtt::QoS;
        connected_flag = true;
        subscribe(messages.get());
        subscribe(sent_load.get(), QoS::AtMostOnce);
    }

void MyClient::on_data(esp_mqtt_event_handle_t const event)
    {
        if (messages.match(event->topic, event->topic_len)) {
            printf("Received in the messages topic");
        }
    }

void MyClient::on_disconnected(esp_mqtt_event_handle_t const event)
    {
        connected_flag = false;
    }
