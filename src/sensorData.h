HomieNode SensorDataNode("sensorData", "SensorData", "endstop");
/*
Bounce debouncer_OpenSensor = Bounce();  // Bounce is built into Homie, so you can use it without including it first
Bounce debouncer_CloseSensor = Bounce(); // Bounce is built into Homie, so you can use it without including it first
*/
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
    /*
    debouncer_OpenSensor.attach(PIN_OpenSensor);
    debouncer_CloseSensor.attach(PIN_CloseSensor);
    debouncer_OpenSensor.interval(50);
    debouncer_CloseSensor.interval(50);
*/
}

void checkOpenSensorState()
{
    int OpenSensorValue = digitalRead(PIN_OpenSensor); //debouncer_OpenSensor.read();

    if (OpenSensorValue != lastOpenSensorValue)
    {
        Homie.getLogger() << "OpenSensor is now " << (OpenSensorValue ? "closed" : "open") << endl;

        SensorDataNode.setProperty("open").send(OpenSensorValue ? "false" : "true");
        lastOpenSensorValue = OpenSensorValue;
    }
}
void checkCloseSensorState()
{
    int closeSensorValue = digitalRead(PIN_CloseSensor); //debouncer_CloseSensor.read();

    if (closeSensorValue != lastCloseSensorValue)
    {
        Homie.getLogger() << "CloseSensor sensor state is " << (closeSensorValue ? "closed" : "open") << endl;

        SensorDataNode.setProperty("close").send(closeSensorValue ? "false" : "true");
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
    SensorDataNode.advertise("open");
    SensorDataNode.advertise("clear");
}