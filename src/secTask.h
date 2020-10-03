
#include <Arduino.h>
TaskHandle_t Task1;

#if defined(ESP32)
int IR_RECEIVE_PIN = 15;
#else
int IR_RECEIVE_PIN = 11;
#endif
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;
HomieNode irRecvNode("irRecv", "IrReceiver", "reader");

void Task1code(void *parameter)
{
    irrecv.enableIRIn(); // Start the receiver

    irRecvNode.advertise("irRead_Cmd").setName("IRRead_Cmd").setDatatype("float");
    irRecvNode.advertise("ReadCmd_decode_type").setName("ReadCmd_decode_type").setDatatype("string");
    for (;;)
    {
        if (irrecv.decode(&results))
        {
            Homie.getLogger() << "recieve IR: " << results.value << endl;
            String res = "";
            switch (results.decode_type)
            {
            case NEC:
                res = "NEC";
                break;
            case SONY:
                res = "SONY";
                break;
            case RC5:
                res = "RC5";
                break;
            case RC6:
                res = "RC6";
                break;
            default:
                res = "UNKNOWN";
                break;
            }
            irRecvNode.setProperty("irRead_Cmd").send(String(results.value));
            irRecvNode.setProperty("readCmd_decode_type").send(res);

            irrecv.resume(); // Receive the next value
        }
        delay(100);
    }
}
