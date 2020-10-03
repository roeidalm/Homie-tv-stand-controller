#include <Homie.h>
#include <IRremote.h>
#include "secTask.h"
#include "TVStand.h"
#include "sensorData.h"

#if defined(ESP32)
#include "bleRemote.h"
#define bleremote true
#else
#define bleremote false
#endif

void onHomieEvent(const HomieEvent &event)
{
  switch (event.type)
  {
  case HomieEventType::ABOUT_TO_RESET:
    reset();
    break;
  default:
    break;
  }
}

void loopHandler()
{

  checkSensorStatus();

  if ((RELAY_OPEN && getOpenSensorState()) || (RELAY_CLOSE && getCloseSensorState()))
  {
    if (RELAY_OPEN)
    {
      sendIrCmd(getTvStandOpenCmd());
    }
    else
    {
      sendIrCmd(getTvStandCloseCmd());
    }
  }
  else if ((RELAY_OPEN && !getOpenSensorState()) || (RELAY_CLOSE && !getCloseSensorState()))
  {
    if (RELAY_OPEN)
    {
      RELAY_OPEN = false;
    }
    else
    {
      RELAY_CLOSE = false;
    }
  }
}

void advertiseSetup()
{
  sensorAdvertiseSetup();
  TVStandAdvertiseSetup();
  if (bleremote)
  {
    //TODO Set the advertise or the function for this
  }
}
void setup()
{
  EEPROM.begin(16);

  Serial.begin(115200);
  Serial << endl
         << endl;

  xTaskCreatePinnedToCore(
      Task1code, /* Function to implement the task */
      "Task1",   /* Name of the task */
      10000,     /* Stack size in words */
      NULL,      /* Task input parameter */
      0,         /* Priority of the task */
      &Task1,    /* Task handle. */
      0);        /* Core where the task should run */

  Homie.disableResetTrigger();
  sensorSetup();
  if (bleremote)
    bleRemoteSetup();
  Homie_setFirmware("my-sensors", "1.0.0");
  Homie.setLoopFunction(loopHandler);
  Homie.onEvent(onHomieEvent);
  advertiseSetup();
  Homie.setup();
}

void loop()
{
  Homie.loop();
}
