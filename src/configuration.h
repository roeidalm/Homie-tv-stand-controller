
#if defined(ESP32)
int IR_RECEIVE_PIN = 15;
#else
int IR_RECEIVE_PIN = 11;
#endif

const int PIN_OpenSensor = 21;  //the pin number like d21
const int PIN_CloseSensor = 13; //D13
const int LED_PIN = 2;

//SEND:
// #define IR_SEND_PIN 4 //if you want to use you pinOut so uncomment this  --the default is D4