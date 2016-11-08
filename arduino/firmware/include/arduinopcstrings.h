#ifndef ARDUINOPC_ARDUINOPCSTRINGS_H
#define ARDUINOPC_ARDUINOPCSTRINGS_H

#include "arduinoinit.h"

namespace ArduinoPCStrings
{
    extern const char *ARDUINO_TYPE;
    extern const char *VALID_ANALOG_PIN_IDENTIFIERS[];
    extern const char *VALID_PWM_PIN_IDENTIFIERS[];
    extern const char *VALID_PIN_IDENTIFIERS[];

#if defined(__HAVE_CAN_BUS__)
    extern const char *CAN_INIT_HEADER;
    extern const char *CAN_READ_HEADER;
    extern const char *CAN_WRITE_HEADER;
    extern const char *CAN_WRITE_ONCE_HEADER;
    extern const char *CAN_LIVE_UPDATE_HEADER;

    extern const char *CLEAR_CAN_MESSAGES_HEADER;
    extern const char *CLEAR_CAN_MESSAGE_BY_ID_HEADER;
    extern const char *CURRENT_CAN_MESSAGES_HEADER;
    extern const char *CURRENT_CAN_MESSAGE_BY_ID_HEADER;
    
    extern const char *ADD_POSITIVE_CAN_MASK_HEADER;
    extern const char *REMOVE_POSITIVE_CAN_MASK_HEADER;
    extern const char *CURRENT_POSITIVE_CAN_MASKS_HEADER;
    extern const char *CLEAR_POSITIVE_CAN_MASKS_HEADER;

    extern const char *ADD_NEGATIVE_CAN_MASK_HEADER;
    extern const char *REMOVE_NEGATIVE_CAN_MASK_HEADER;
    extern const char *CURRENT_NEGATIVE_CAN_MASKS_HEADER;
    extern const char *CLEAR_NEGATIVE_CAN_MASKS_HEADER;

    extern const char *ALL_CURRENT_CAN_MASKS_HEADER;
    extern const char *CLEAR_ALL_CAN_MASKS_HEADER;
#endif

    extern const char *FAILSAFE_INTERIM_HEADER;
    extern const char *ADD_SHORT_WATCHDOG_HEADER;
    extern const char *REMOVE_SHORT_WATCHDOG_HEADER;
    extern const char *SHORT_EVENT_DETECTED_HEADER;
    extern const char *CHAR_ARRAY_TERMINATOR;
    extern const char *DIGITAL_STATE_HIGH_IDENTIFIERS[];
    extern const char *DIGITAL_STATE_LOW_IDENTIFIERS[];
    extern const char *VALID_PIN_TYPE_IDENTIFIERS[];
    extern const char *DIGITAL_INPUT_IDENTIFIER;
    extern const char *DIGITAL_OUTPUT_IDENTIFIER;
    extern const char *ANALOG_INPUT_IDENTIFIER;
    extern const char *ANALOG_OUTPUT_IDENTIFIER;
    extern const char *DIGITAL_INPUT_PULLUP_IDENTIFIER;
    extern const char *DIGITAL_STATE_HIGH_STRING;
    extern const char *DIGITAL_STATE_LOW_STRING;
    extern const char *INVALID_HEADER;
    extern const char *DIGITAL_READ_HEADER;
    extern const char *ANALOG_READ_HEADER;
    extern const char *DIGITAL_WRITE_HEADER;
    extern const char *ANALOG_WRITE_HEADER;
    extern const char *SOFT_DIGITAL_READ_HEADER;
    extern const char *SOFT_ANALOG_READ_HEADER;
    extern const char *PIN_TYPE_HEADER;
    extern const char *PIN_TYPE_CHANGE_HEADER;
    extern const char *ARDUINO_TYPE_HEADER;
    extern const char *CHANGE_A_TO_D_THRESHOLD_HEADER;
    extern const char *CURRENT_A_TO_D_THRESHOLD_HEADER;

    extern const char *HEARTBEAT_HEADER;
    extern const char *IO_REPORT_HEADER;
    extern const char *ANALOG_IO_REPORT_HEADER;
    extern const char *DIGITAL_IO_REPORT_HEADER;
    extern const char *INPUT_REPORT_HEADER;
    extern const char *OUTPUT_REPORT_HEADER;
    extern const char *DIGITAL_INPUT_REPORT_HEADER;
    extern const char *DIGITAL_OUTPUT_REPORT_HEADER;
    extern const char *ANALOG_INPUT_REPORT_HEADER;
    extern const char *ANALOG_OUTPUT_REPORT_HEADER;
    extern const char *IO_REPORT_END_HEADER;

    extern const char *CAN_BUS_ENABLED_HEADER;
    extern const char *FIRMWARE_VERSION_HEADER;
    extern const char *INVALID_LENGTH_EXCEEDED_HEADER;
    extern const char *UNSPECIFIED_IO_TYPE_IDENTIFIER;
    extern const char *CAN_BUS_ENABLED_STRING;
    extern const char *CAN_BUS_DISABLED_STRING;
    extern const char *FIRMWARE_VERSION;
    extern const char *EMPTY_STRING;
    extern const char *OPERATION_FAILURE_STRING;
    extern const char *OPERATION_SUCCESS_STRING;
    extern const char *HEARTBEAT_HEADER;

    extern const char *A0_STRING;
    extern const char *A1_STRING;
    extern const char *A2_STRING;
    extern const char *A3_STRING;
    extern const char *A4_STRING;
    extern const char *A5_STRING;
    
    extern const char *A0_EQUIVALENT_STRING;
    extern const char *A1_EQUIVALENT_STRING;
    extern const char *A2_EQUIVALENT_STRING;
    extern const char *A3_EQUIVALENT_STRING;
    extern const char *A4_EQUIVALENT_STRING;
    extern const char *A5_EQUIVALENT_STRING;

    #if defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560)
        extern const char *A8_EQUIVALENT_STRING;
        extern const char *A9_EQUIVALENT_STRING;
        extern const char *A10_EQUIVALENT_STRING;
        extern const char *A11_EQUIVALENT_STRING;
        extern const char *A12_EQUIVALENT_STRING;
        extern const char *A13_EQUIVALENT_STRING;
        extern const char *A14_EQUIVALENT_STRING;
        extern const char *A15_EQUIVALENT_STRING;
    #endif
    #if (defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560))
        extern const char *A6_STRING;
        extern const char *A7_STRING;
        extern const char *A6_EQUIVALENT_STRING;
        extern const char *A7_EQUIVALENT_STRING;
    #endif
    #if defined(ARDUINO_AVR_MEGA1280) || defined(ARDUINO_AVR_MEGA2560)
        extern const char *A8_STRING;
        extern const char *A9_STRING;
        extern const char *A10_STRING;
        extern const char *A11_STRING;
        extern const char *A12_STRING;
        extern const char *A13_STRING;
        extern const char *A14_STRING;
        extern const char *A15_STRING;
    #endif
}

#endif //ARDUINOPC_ARDUINOPCSTRINGS_H