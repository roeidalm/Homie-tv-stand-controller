HomieNode openSensorNode("openSensor", "OpenSensor", "endstop");
HomieNode closeSensorNode("closeSensor", "CloseSensor", "endstop");

const int PIN_OpenSensor = 21;  //the pin number like d21
const int PIN_CloseSensor = 13; //D13
const int LED_PIN = 2;

Bounce debouncer_OpenSensor = Bounce();  // Bounce is built into Homie, so you can use it without including it first
Bounce debouncer_CloseSensor = Bounce(); // Bounce is built into Homie, so you can use it without including it first

int lastOpenSensorValue = -1;
int lastCloseSensorValue = -1;

void sensorSetup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    pinMode(PIN_OpenSensor, INPUT);
    pinMode(PIN_CloseSensor, INPUT);
    digitalWrite(PIN_OpenSensor, HIGH);
    digitalWrite(PIN_CloseSensor, HIGH);
    debouncer_OpenSensor.attach(PIN_OpenSensor);
    debouncer_CloseSensor.attach(PIN_CloseSensor);
    debouncer_OpenSensor.interval(50);
    debouncer_CloseSensor.interval(50);
}

void checkOpenSensorState()
{
    int OpenSensorValue = digitalRead(PIN_OpenSensor); //debouncer_OpenSensor.read();
    // Homie.getLogger() << "OpenSensorValue " << OpenSensorValue << endl;

    if (OpenSensorValue != lastOpenSensorValue)
    {
        Homie.getLogger() << "OpenSensor is now " << (OpenSensorValue ? "open" : "closed") << endl;

        openSensorNode.setProperty("open").send(OpenSensorValue ? "true" : "false");
        lastOpenSensorValue = OpenSensorValue;
    }
}
void checkCloseSensorState()
{
    int closeSensorValue = digitalRead(PIN_CloseSensor);

    if (closeSensorValue != lastCloseSensorValue)
    {
        Homie.getLogger() << "CloseSensor sensor state is " << (closeSensorValue ? "Leak Detected" : "clear") << endl;

        closeSensorNode.setProperty("clear").send(closeSensorValue ? "true" : "false");
        lastCloseSensorValue = closeSensorValue;
    }
}
void checkSensorStatus()
{
    checkOpenSensorState();
    checkCloseSensorState();
}

bool getOpenSensorState()
{
    return lastOpenSensorValue;
}

bool getCloseSensorState()
{
    return lastCloseSensorValue;
}

void sensorAdvertiseSetup()
{
    openSensorNode.advertise("open");
    closeSensorNode.advertise("clear");
}