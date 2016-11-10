#include "../include/arduinopcstrings.h"

namespace ArduinoPCStrings
{
#if defined(ARDUINO_AVR_UNO)
    const char *ARDUINO_TYPE{"arduino_uno"};
    const char *VALID_ANALOG_PIN_IDENTIFIERS[]{"A0", "A1", "A2", "A3", "A4", "A5", "14", "15", "16", "17", "18", "19", "fish"};
    #if defined(__HAVE_CAN_BUS__)
        const char *VALID_PWM_PIN_IDENTIFIERS[]{"3", "5", "6", "10", "11", "fish"};
        const char *VALID_PIN_IDENTIFIERS[]{"2", "3", "4", "5", "6", "7", "8", "10", "11", "12", "13", "14", 
                                                "15", "16", "17", "18", "19", "A0", "A1", "A2", "A3", "A4", "A5", "fish"};
    #else
        const char *VALID_PWM_PIN_IDENTIFIERS[]{"3", "5", "6", "9", "10", "11", "fish"};
        const char *VALID_PIN_IDENTIFIERS[]{"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", 
                                            "15", "16", "17", "18", "19", "A0", "A1", "A2", "A3", "A4", "A5", "fish"};
    #endif
#elif defined(ARDUINO_AVR_NANO)
    const char *ARDUINO_TYPE{"arduino_nano"};
    const char *VALID_ANALOG_PIN_IDENTIFIERS[]{"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "14", "15", "16", "17", "18", "19", "20", "21", "fish"};
    #if defined(__HAVE_CAN_BUS__)
        const char *VALID_PWM_PIN_IDENTIFIERS[]{"3", "5", "6", "10", "11", "fish"};
        const char *VALID_PIN_IDENTIFIERS[]{"2", "3", "4", "5", "6", "7", "8", "10", "11", "12", "13", "14", 
                                                "15", "16", "17", "18", "19", "20", "21", "A0", "A1", "A2", 
                                                "A3", "A4", "A5", "A6", "A7", "fish"};
    #else
        const char *VALID_PWM_PIN_IDENTIFIERS[]{"3", "5", "6", "9", "10", "11", "fish"};
        const char *VALID_PIN_IDENTIFIERS[]{"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", 
                                            "15", "16", "17", "18", "19", "20", "21", "A0", "A1", "A2", "A3", "A4", 
                                            "A5", "A6", "A7", "fish"};
    #endif
#elif defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560)
    const char *ARDUINO_TYPE{"arduino_mega"};
    const char *VALID_ANALOG_PIN_IDENTIFIERS[]{"A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "A10",
                                               "A11", "A12", "A13", "A14", "A15", "54", "55", "56", "57", "58", "59", "60",
                                               "61", "62", "63", "64", "65", "66", "67", "68", "69", "fish"};
    #if defined(__HAVE_CAN_BUS__)
        const char *VALID_PWM_PIN_IDENTIFIERS[]{"2", "3", "4", "5", "6", "7", "8", "10", "11", "12", "44", "45", "46", "fish"};
        const char *VALID_PIN_IDENTIFIERS[]{"2", "3", "4", "5", "6", "7", "8", "10", "11", "12", "13", "14", "15",
                                            "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28",
                                            "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41",
                                            "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54",
                                            "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67",
                                            "68", "69", "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", 
                                            "A10", "A11", "A12", "A13", "A14", "A15", "fish"};
    #else
        const char *VALID_PWM_PIN_IDENTIFIERS[]{"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "44", "45", "46", "fish"};
        const char *VALID_PIN_IDENTIFIERS[]{"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15",
                                            "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28",
                                            "29", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "40", "41",
                                            "42", "43", "44", "45", "46", "47", "48", "49", "50", "51", "52", "53", "54",
                                            "55", "56", "57", "58", "59", "60", "61", "62", "63", "64", "65", "66", "67",
                                            "68", "69", "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", 
                                            "A10", "A11", "A12", "A13", "A14", "A15", "fish"};
    #endif
#endif

#if defined(__HAVE_CAN_BUS__)
    const char *CLEAR_NEGATIVE_CAN_MASKS_HEADER{"{clearncanmasks"};
    const char *CURRENT_NEGATIVE_CAN_MASKS_HEADER{"{curncanmasks"};
    const char *CURRENT_POSITIVE_CAN_MASKS_HEADER{"{curpcanmasks"};
    const char *CLEAR_ALL_CAN_MASKS_HEADER{"{clearallcanmasks"};
    const char *CAN_INIT_HEADER{"{caninit"};
    const char *CAN_READ_HEADER{"{canread"};
    const char *CAN_WRITE_HEADER{"{canwrite"};
    const char *CAN_WRITE_ONCE_HEADER{"{canwriteo"};
    const char *CAN_LIVE_UPDATE_HEADER{"{canlup"};
    const char *CLEAR_CAN_MESSAGES_HEADER{"{clearcanmsgs"};
    const char *CLEAR_CAN_MESSAGE_BY_ID_HEADER{"{clearcanmsgid"};
    const char *CURRENT_CAN_MESSAGES_HEADER{"{curcanmsgs"};
    const char *CURRENT_CAN_MESSAGE_BY_ID_HEADER{"{curcanmsgid}"};
    const char *CLEAR_POSITIVE_CAN_MASKS_HEADER{"{clearpcanmasks"};
    
    const char *ADD_POSITIVE_CAN_MASK_HEADER{"{addpcanmask"};
    const char *ADD_NEGATIVE_CAN_MASK_HEADER{"{addncanmask"};
    const char *ALL_CURRENT_CAN_MASKS_HEADER{"{allcanmasks"};
    
    const char *REMOVE_POSITIVE_CAN_MASK_HEADER{"{rempcanmask"};
    const char *REMOVE_NEGATIVE_CAN_MASK_HEADER{"{remncanmask"};
#endif

    const char *ARDUINO_TYPE_HEADER{"{ardtype"};
    const char *ANALOG_READ_HEADER{"{aread"};
    const char *ANALOG_WRITE_HEADER{"{awrite"};
    const char *CHANGE_A_TO_D_THRESHOLD_HEADER{"{atodchange"};
    const char *CURRENT_A_TO_D_THRESHOLD_HEADER{"{atodthresh"};
    const char *ADD_SHORT_WATCHDOG_HEADER{"{addshortwd"};
    
    const char *CAN_BUS_ENABLED_HEADER{"{canbus"};
    
    const char *DIGITAL_READ_HEADER{"{dread"};
    const char *DIGITAL_WRITE_HEADER{"{dwrite"};
    
    const char *HEARTBEAT_HEADER{"{heartbeat"};
    
    const char *IO_REPORT_HEADER{"{ioreport"};
    
    const char *LOAD_SYSTEM_STATE_HEADER{"{loadstate"};
    
    const char *PIN_TYPE_HEADER{"{ptype"};
    const char *PIN_TYPE_CHANGE_HEADER{"{ptchange"};
    
    const char *REMOVE_SHORT_WATCHDOG_HEADER{"{remshortwd"};
    
    const char *SOFT_DIGITAL_READ_HEADER{"{sdread"};
    const char *SOFT_ANALOG_READ_HEADER{"{saread"};
    const char *STORE_SYSTEM_STATE_HEADER{"{storestate"};

    const char *FIRMWARE_VERSION_HEADER{"{version"};
    
    const char *SHORT_EVENT_DETECTED_HEADER{"{shortevent"};
    const char *STORE_SYSTEM_STATE_END_HEADER{"{storestateend"};
    const char *CHAR_ARRAY_TERMINATOR{"fish"};
    const char *DIGITAL_STATE_HIGH_IDENTIFIERS[]{"1", "high", "true", "on", "fish"};
    const char *DIGITAL_STATE_LOW_IDENTIFIERS[]{"0", "low", "false", "off", "fish"};
    const char *VALID_PIN_TYPE_IDENTIFIERS[]{"dout", "din", "aout", "ain", "dinpup", "fish"};
    const char *IO_REPORT_END_HEADER{"{ioreportend"};
    const char *LOAD_SYSTEM_STATE_END_HEADER{"{loadstateend"};
    const char *DIGITAL_INPUT_IDENTIFIER{"din"};
    const char *DIGITAL_OUTPUT_IDENTIFIER{"dout"};
    const char *DIGITAL_INPUT_PULLUP_IDENTIFIER{"dinpup"};
    const char *ANALOG_INPUT_IDENTIFIER{"ain"};
    const char *ANALOG_OUTPUT_IDENTIFIER{"aout"};
    const char *FAILSAFE_INTERIM_HEADER{":failsafe:"};
    const char *INVALID_LENGTH_EXCEEDED_HEADER{"{invalid:length_exceeded}"};
    const char *UNSPECIFIED_IO_TYPE_IDENTIFIER{"unspecified"};
    const char *CAN_BUS_ENABLED_STRING{"1"};
    const char *DIGITAL_STATE_HIGH_STRING{"1"};
    const char *DIGITAL_STATE_LOW_STRING{"0"};
    const char *INVALID_HEADER{"{invalid"};
    const char *CAN_BUS_DISABLED_STRING{"0"};
    const char *FIRMWARE_VERSION{"0.50"};
    const char *EMPTY_STRING{""};
    const char *OPERATION_FAILURE_STRING{"-1"};
    const char *OPERATION_SUCCESS_STRING{"1"};

    const char *A0_STRING{"A0"};
    const char *A1_STRING{"A1"};
    const char *A2_STRING{"A2"};
    const char *A3_STRING{"A3"};
    const char *A4_STRING{"A4"};
    const char *A5_STRING{"A5"};
    
    #if defined(ARDUINO_AVR_UNO)
        const char *A0_EQUIVALENT_STRING{"14"};
        const char *A1_EQUIVALENT_STRING{"15"};
        const char *A2_EQUIVALENT_STRING{"16"};
        const char *A3_EQUIVALENT_STRING{"17"};
        const char *A4_EQUIVALENT_STRING{"18"};
        const char *A5_EQUIVALENT_STRING{"19"};
    #elif defined(ARDUINO_AVR_NANO)
        const char *A0_EQUIVALENT_STRING{"14"};
        const char *A1_EQUIVALENT_STRING{"15"};
        const char *A2_EQUIVALENT_STRING{"16"};
        const char *A3_EQUIVALENT_STRING{"17"};
        const char *A4_EQUIVALENT_STRING{"18"};
        const char *A5_EQUIVALENT_STRING{"19"};
        const char *A6_EQUIVALENT_STRING{"20"};
        const char *A7_EQUIVALENT_STRING{"21"};
        const char *A6_STRING{"A6"};
        const char *A7_STRING{"A7"};
    #elif defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560)
        const char *A0_EQUIVALENT_STRING{"54"};
        const char *A1_EQUIVALENT_STRING{"55"};
        const char *A2_EQUIVALENT_STRING{"56"};
        const char *A3_EQUIVALENT_STRING{"57"};
        const char *A4_EQUIVALENT_STRING{"58"};
        const char *A5_EQUIVALENT_STRING{"59"};
        const char *A6_EQUIVALENT_STRING{"60"};
        const char *A7_EQUIVALENT_STRING{"61"};
        const char *A8_EQUIVALENT_STRING{"62"};
        const char *A9_EQUIVALENT_STRING{"63"};
        const char *A10_EQUIVALENT_STRING{"64"};
        const char *A11_EQUIVALENT_STRING{"65"};
        const char *A12_EQUIVALENT_STRING{"66"};
        const char *A13_EQUIVALENT_STRING{"67"};
        const char *A14_EQUIVALENT_STRING{"68"};
        const char *A15_EQUIVALENT_STRING{"69"};
        const char *A6_STRING{"A6"};
        const char *A7_STRING{"A7"};
        const char *A8_STRING{"A8"};
        const char *A9_STRING{"A9"};
        const char *A10_STRING{"A10"};
        const char *A11_STRING{"A11"};
        const char *A12_STRING{"A12"};
        const char *A13_STRING{"A13"};
        const char *A14_STRING{"A14"};
        const char *A15_STRING{"A15"};
    #endif
    
}
