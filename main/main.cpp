#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "IotDevice.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

bool connected_flag = false;
bool button_flag = false;

// xQueueHandle interputQueue;

static void IRAM_ATTR gpio_interrupt_handler(void *args)
{
    button_flag = true;
    // printf("button pressed");
    // xQueueSendFromISR(interputQueue, &pinNumber, NULL);
}

// onboard LED
GPIO_Output gpio(GPIONum(2));

// Blink LED when no connection
// works on disconnect too
void Blink_led(void *arg)
{
    for(;;)
    {
    while (!connected_flag)
    {
        gpio.set_high();
        vTaskDelay(500/ portTICK_RATE_MS);
        gpio.set_low();
        vTaskDelay(500/ portTICK_RATE_MS);
    }
    while (connected_flag)
    {
    vTaskDelay(500/ portTICK_RATE_MS);
    }
    }
    vTaskDelete(NULL);
}

void Read_sensors(void *device)
{
    for(;;)
    {
        static_cast<IotDevice*>(device)->heartbeat();
        vTaskDelay(10000/ portTICK_RATE_MS);
    }
    vTaskDelete(NULL);
}

TaskHandle_t ReadTaskHandle = NULL;
TaskHandle_t BlinkTaskHandle = NULL;

extern "C" void app_main(void)
{

  gpio_pad_select_gpio(0);
  gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
  gpio_pullup_en(GPIO_NUM_0);
  gpio_pulldown_dis(GPIO_NUM_0);
  gpio_set_intr_type(GPIO_NUM_0, GPIO_INTR_NEGEDGE);

  // interputQueue = xQueueCreate(10, sizeof(int));
  gpio_install_isr_service(0);
  gpio_isr_handler_add(GPIO_NUM_0, gpio_interrupt_handler, (void *)NULL);

  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  esp_log_level_set("*", ESP_LOG_INFO);
  esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
  esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_ERROR);
  esp_log_level_set("TRANSPORT_TCP", ESP_LOG_ERROR);
  esp_log_level_set("TRANSPORT_SSL", ESP_LOG_ERROR);
  esp_log_level_set("TRANSPORT", ESP_LOG_ERROR);
  esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);

  ESP_ERROR_CHECK(nvs_flash_init());
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  xTaskCreatePinnedToCore(Blink_led, "Blink_led", 4096, NULL, 10, &BlinkTaskHandle, 1);
  std::string lastwilltopic = (std::string{CONFIG_MQTT_USERNAME}+"/sweet-home/"+std::string{CONFIG_MQTT_DEVICE_ID}+"/$state");
  mqtt::BrokerConfiguration broker{
      .address = {mqtt::URI{std::string{CONFIG_MQTT_BROKER_URL}},
                .port = CONFIG_MQTT_BROKER_PORT},
      .security = mqtt::Insecure{}
      };
  mqtt::ClientCredentials credentials{};
    credentials.client_id = std::string{CONFIG_MQTT_USERNAME};
    credentials.username = std::string{CONFIG_MQTT_USERNAME};
    credentials.authentication = mqtt::Password{std::string{CONFIG_MQTT_PASSWORD}};
  mqtt::Configuration config{};
  mqtt::Session session{};
  mqtt::LastWill lastwill{
    .lwt_topic = lastwilltopic,
    .lwt_msg = "lost",
    .lwt_qos = 1,
    .lwt_retain = 1,
    .lwt_msg_len = 4,
  };
  session.last_will = lastwill;
  session.keepalive = 10;
  session.disable_clean_session = 0;
  config.session = session;

  MyClient* client = new MyClient(broker, credentials, config);
  IotDevice device {(*client)};
  Property* test_property = new Property("test_property","prop_id", &device, OPTION, true, true, "integer", "");

  Node* test_node =  new Node{&device, "test_node", "sensor"};
  ButtonNotificationProperty* button = new ButtonNotificationProperty("button","button_id", test_node, SENSOR, false, true, "boolean", "");
  button->setFlag(&button_flag);
  ValidationTestProperty* validation_test = new ValidationTestProperty("validate","validate_id", test_node, SENSOR, true, true, "integer", "1:3");

  client->setDevice(&device);

  ESP_ERROR_CHECK(example_connect()); // probaply will not use it in a real firmware
  xTaskCreatePinnedToCore(Read_sensors, "Read_sensors", 4096, &device, 15, &ReadTaskHandle, 0);


  while (true)
  {
    device.process();
    // if (button_flag)
    // {
    // printf("button was pressed\n");
    // button_flag = false;
    // }
    // constexpr TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    vTaskDelay(500/ portTICK_RATE_MS);
  }
}