
#if defined(ESP32)
int IR_RECEIVE_PIN = 15;
#else
// int IR_RECEIVE_PIN = 11; //def
int IR_RECEIVE_PIN = 23;
#endif

const int PIN_OpenSensor = 27;  //the pin number like D12
const int PIN_CloseSensor = 14; //D13
const int LED_PIN = 13;

//SEND:
// #define IR_SEND_PIN 4 //if you want to use you pinOut so uncomment this  --the default is D4