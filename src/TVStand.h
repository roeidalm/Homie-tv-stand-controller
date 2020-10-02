#include <EEPROM.h>

// HomieNode shuttersNode("shutters", "Shutters", "shutters");
HomieNode tvStandOpenNode("tvStandOpenNode", "TVStandOpenNode", "switch");
HomieNode tvStandCloseNode("tvStandCloseNode", "TVStandCloseNode", "switch");
HomieNode abortNode("abortNode", "AbortNode", "switch");

bool RELAY_OPEN = false;
bool RELAY_CLOSE = false;

const byte OPEN_IR_COMMANE_EEPROM = 0;
const byte CLOSE_IR_COMMANE_EEPROM = 8;

uint64_t tvStandOpenGetState()
{
    return EEPROM.readLong64(OPEN_IR_COMMANE_EEPROM);
}

uint64_t tvStandCloseGetState()
{
    return EEPROM.readLong64(CLOSE_IR_COMMANE_EEPROM);
}

void tvStandSetState(int addr, uint64_t state)
{
    EEPROM.write(addr, state);
    EEPROM.commit();
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