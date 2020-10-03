#include <EEPROM.h>

// HomieNode shuttersNode("shutters", "Shutters", "shutters");
HomieNode tvStandOpenNode("tvStandOpenNode", "TVStandOpenNode", "switch");
HomieNode tvStandCloseNode("tvStandCloseNode", "TVStandCloseNode", "switch");
HomieNode abortNode("abortNode", "AbortNode", "switch");

bool RELAY_OPEN = false;
bool RELAY_CLOSE = false;

const byte OPEN_IR_COMMANE_EEPROM = 0;
const byte CLOSE_IR_COMMANE_EEPROM = 8;
uint64_t openCmd = -1;
uint64_t closeCmd = -1;

//SEND:
// #define IR_SEND_PIN 4 //if you want to use you pinOut so uncomment this  --the default is D4
IRsend irsend;
uint64_t getTvStandOpenCmd()
{
    openCmd = openCmd == -1 ? EEPROM.readLong64(OPEN_IR_COMMANE_EEPROM) : openCmd;
    return openCmd;
}

uint64_t getTvStandCloseCmd()
{
    closeCmd = closeCmd == -1 ? EEPROM.readLong64(CLOSE_IR_COMMANE_EEPROM) : openCmd;
    return closeCmd;
}

void tvStandSetState(int addr, uint64_t irCmd)
{
    EEPROM.write(addr, irCmd);
    EEPROM.commit();
    if (addr == OPEN_IR_COMMANE_EEPROM)
    {
        openCmd = irCmd;
    }
    else
    {
        closeCmd = irCmd;
    }
}

void sendIrCmd(uint64_t irCmd, int nbitsToSend = 32)
{
    Homie.getLogger() << "send IR Cmd " << endl;
    irsend.sendNEC(irCmd, nbitsToSend);
}
void tvStandHalt()
{
    RELAY_OPEN = false;
    RELAY_CLOSE = false;
}

void reset()
{
    tvStandHalt();
    //TODO
}

bool RelayOpenOnHandler(const HomieRange &range, const String &value)
{
    if (value != "true" && value != "false")
        return false;

    RELAY_OPEN = (value == "true");
    tvStandOpenNode.setProperty("on").send(value);
    Homie.getLogger() << "RELAY_OPEN " << (RELAY_OPEN ? "on" : "off") << endl;

    return true;
}
bool RelayCloseOnHandler(const HomieRange &range, const String &value)
{
    if (value != "true" && value != "false")
        return false;

    RELAY_CLOSE = (value == "true");
    tvStandCloseNode.setProperty("on").send(value);
    Homie.getLogger() << "RELAY_CLOSE " << (RELAY_CLOSE ? "on" : "off") << endl;

    return true;
}
bool AbortRelayOnHandler(const HomieRange &range, const String &value)
{
    if (value != "true" && value != "false")
        return false;

    if (value == "true")
    {
        tvStandHalt();
        tvStandOpenNode.setProperty("on").send(value);
        tvStandCloseNode.setProperty("on").send(value);
        Homie.getLogger() << "!!!ABORT!!!" << endl;
    }

    return true;
}

bool tvStandOpenSetStateOnHandler(const HomieRange &range, const String &value)
{
    long longValue = atol(value.c_str());
    tvStandSetState(OPEN_IR_COMMANE_EEPROM, longValue);
    Homie.getLogger() << "tvStand Open cmd is now: " << longValue << endl;
    return true;
}
bool tvStandCloseSetStateOnHandler(const HomieRange &range, const String &value)
{
    long longValue = atol(value.c_str());
    tvStandSetState(CLOSE_IR_COMMANE_EEPROM, longValue);
    Homie.getLogger() << "tvStand Close cmd is now: " << longValue << endl;
    return true;
}

void TVStandAdvertiseSetup()
{
    tvStandOpenNode.advertise("openCmd").setName("Open Cmd").setDatatype("boolean").settable(RelayOpenOnHandler);
    tvStandOpenNode.advertise("openEEPROMCmd").setName("Open EEPROM Cmd").setDatatype("float").settable(tvStandOpenSetStateOnHandler);
    tvStandCloseNode.advertise("closeCmd").setName("close Cmd").setDatatype("boolean").settable(RelayCloseOnHandler);
    tvStandOpenNode.advertise("closeEEPROMCmd").setName("close EEPROM Cmd").setDatatype("float").settable(tvStandCloseSetStateOnHandler);
    abortNode.advertise("abortCmd").setName("Abort Cmd").setDatatype("boolean").settable(AbortRelayOnHandler);
}