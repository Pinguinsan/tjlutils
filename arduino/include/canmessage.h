#ifndef TJLUTILS_CANMESSAGE_H
#define TJLUTILS_CANMESSAGE_H

#include "candatapacket.h"
#include "generalutilities.h"
#include <string>

class CanMessage
{
public:
    CanMessage(uint32_t id, uint8_t frame, uint8_t length, const CanDataPacket &data);
    CanMessage();
    uint32_t id() const;
    uint8_t frame() const;
    uint8_t length() const;
    CanDataPacket dataPacket() const;


    static CanMessage parseCanMessage(const std::string &str);
    bool setDataPacketNthByte(int index, unsigned char nth);
    void setID(uint32_t id);
    void setFrame(uint8_t frame);
    void setLength(uint8_t length);
    void setDataPacket(const CanDataPacket &dataPacket);
    std::string toString() const;
    static uint32_t parseCanID(const std::string &str);
    static uint8_t parseCanByte(const std::string &str);  
    static const int CAN_BYTE_WIDTH;
    static const int CAN_ID_WIDTH;  
    static const unsigned int CAN_MESSAGE_SIZE;
    static const unsigned char CAN_FRAME;
    static const unsigned char CAN_MESSAGE_LENGTH;


    unsigned char nthDataPacketByte(int index);
    std::string toPrettyString() const;
private:
    uint32_t m_id;
    uint8_t m_frame;
    uint8_t m_length;
    CanDataPacket m_dataPacket;
};

#endif //TJLUTILS_CANMESSAGE_H
