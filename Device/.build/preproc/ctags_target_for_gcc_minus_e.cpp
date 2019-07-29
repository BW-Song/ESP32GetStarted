# 1 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino"
# 1 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino"
/**

 * A simple Azure IoT example for sending telemetry to Iot Hub.

 */
# 5 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino"
# 6 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 2
# 7 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 2
# 8 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 2
# 9 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 2
# 10 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 2
# 11 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 2

#define INTERVAL 10000
#define MESSAGE_MAX_LEN 256

WiFiManager wifiManager;
char* connectionString;
Preferences preferences;
WiFiManagerParameter conStr("ConnectionString","ConnectionString",connectionString,40);

const char *messageData = "{\"messageId\":%d, \"Temperature\":%f, \"Humidity\":%f}";
static bool hasIoTHub = false;
static bool hasWifi = false;
int messageCount = 1;
static bool messageSending = true;
static uint64_t send_interval_ms;

static void SendConfirmationCallback(IOTHUB_CLIENT_CONFIRMATION_RESULT result)
{
  if (result == IOTHUB_CLIENT_CONFIRMATION_OK)
  {
    Serial.println("Send Confirmation Callback finished.");
  }
}

static void MessageCallback(const char* payLoad, int size)
{
  Serial.println("Message callback:");
  Serial.println(payLoad);
}

static void DeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, const unsigned char *payLoad, int size)
{
  char *temp = (char *)malloc(size + 1);
  if (temp == 
# 44 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 3 4
             __null
# 44 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino"
                 )
  {
    return;
  }
  memcpy(temp, payLoad, size);
  temp[size] = '\0';
  // Display Twin message.
  Serial.println(temp);
  free(temp);
}

static int DeviceMethodCallback(const char *methodName, const unsigned char *payload, int size, unsigned char **response, int *response_size)
{
  do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 57 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 3 4
 __null
# 57 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino"
 ) l(AZ_LOG_INFO, "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino", __func__, 57, 0x01, "Try to invoke method %s", methodName); }; }while((void)0,0);
  const char *responseMessage = "\"Successfully invoke device method\"";
  int result = 200;

  if (strcmp(methodName, "start") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 63 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 3 4
   __null
# 63 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino", __func__, 63, 0x01, "Start sending temperature and humidity data"); }; }while((void)0,0);
    messageSending = true;
  }
  else if (strcmp(methodName, "stop") == 0)
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 68 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 3 4
   __null
# 68 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino", __func__, 68, 0x01, "Stop sending temperature and humidity data"); }; }while((void)0,0);
    messageSending = false;
  }
  else
  {
    do{{ LOGGER_LOG l = xlogging_get_log_function(); if (l != 
# 73 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino" 3 4
   __null
# 73 "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino"
   ) l(AZ_LOG_INFO, "c:\\Users\\t-bosong\\Documents\\GitHub\\ESP32GetStarted\\Device\\device.ino", __func__, 73, 0x01, "No method %s found", methodName); }; }while((void)0,0);
    responseMessage = "\"No method found\"";
    result = 404;
  }

  *response_size = strlen(responseMessage) + 1;
  *response = (unsigned char *)strdup(responseMessage);

  return result;
}

void saveConfigCallback()
{
  // Serial.println(wifiManager.getSSID());
  // Serial.println(wifiManager.getPassword());
  // Serial.println(conStr.getValue());
  preferences.putString("WiFi_SSID",wifiManager.getSSID());
  preferences.putString("WiFi_Password",wifiManager.getPassword());
  preferences.putString("ConStr",String(conStr.getValue()));
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Device");
  Serial.println("Initializing...");

  Serial.println(" > WiFi");
  Serial.println("Starting connecting WiFi.");

  preferences.begin("Connections");
  wifiManager.resetSettings();
  wifiManager.setDebugOutput(false);
  wifiManager.addParameter(&conStr);
  wifiManager.setSaveParamsCallback(saveConfigCallback);
  wifiManager.autoConnect("ESP32-WiFiConfig", "AzureSet");
  preferences.end();
  hasWifi = true;

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(" > IoT Hub");
  if (!Esp32MQTTClient_Init((const uint8_t*)connectionString, true))
  {
    hasIoTHub = false;
    Serial.println("Initializing IoT hub failed.");
    return;
  }
  hasIoTHub = true;
  Esp32MQTTClient_SetSendConfirmationCallback(SendConfirmationCallback);
  Esp32MQTTClient_SetMessageCallback(MessageCallback);
  Esp32MQTTClient_SetDeviceTwinCallback(DeviceTwinCallback);
  Esp32MQTTClient_SetDeviceMethodCallback(DeviceMethodCallback);
  Serial.println("Start sending events.");
  randomSeed(analogRead(0));
  send_interval_ms = millis();

}

void loop() {
if (hasWifi && hasIoTHub)
  {
    if (messageSending &&
        (int)(millis() - send_interval_ms) >= 10000)
    {
      // Send teperature data
      char messagePayload[256];
      float temperature = (float)random(0,500)/10;
      float humidity = (float)random(0, 1000)/10;
      snprintf(messagePayload, 256, messageData, messageCount++, temperature, humidity);
      Serial.println(messagePayload);
      EVENT_INSTANCE* message = Esp32MQTTClient_Event_Generate(messagePayload, MESSAGE);
      Esp32MQTTClient_SendEventInstance(message);
      send_interval_ms = millis();
    }
    else
    {
      Esp32MQTTClient_Check();
    }
  }
  delay(10);
}
