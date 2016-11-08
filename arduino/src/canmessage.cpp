#include "canmessage.h"

const int CanMessage::CAN_BYTE_WIDTH{2};
const int CanMessage::CAN_ID_WIDTH{3};
const unsigned int CanMessage::CAN_MESSAGE_SIZE{9};
const unsigned char CanMessage::CAN_FRAME{0};
const unsigned char CanMessage::CAN_MESSAGE_LENGTH{8};
const char *CanMessage::NTH_DATA_PACKET_BYTE_INDEX_OUT_OF_RANGE_STRING{"ERROR: Invalid index passed to nthDataPacket(int index) :"};

CanMessage::CanMessage(uint32_t id, uint8_t frame, uint8_t length, const CanDataPacket &dataPacket) :
    m_id{id},
    m_frame{frame},
    m_length{length},
    m_dataPacket{dataPacket}
{

}

CanMessage::CanMessage() :
    m_id{0},
    m_frame{0},
    m_length{0},
    m_dataPacket{CanDataPacket{0, 0, 0, 0, 0, 0, 0, 0}}
{

}

void CanMessage::setID(uint32_t id)
{
    this->m_id = id;
}

void CanMessage::setFrame(uint8_t frame)
{
    this->m_frame = frame;
}

void CanMessage::setLength(uint8_t length)
{
    this->m_length = length;
}

void CanMessage::setDataPacket(const CanDataPacket &dataPacket)
{
    this->m_dataPacket = dataPacket;
}

uint32_t CanMessage::id() const
{
    return this->m_id;
}

uint8_t CanMessage::frame() const
{
    return this->m_frame;
}

uint8_t CanMessage::length() const
{
    return this->m_length;
}

unsigned char CanMessage::nthDataPacketByte(int index)
{
    if ((index > 8) || (index < 0)) {
        throw std::runtime_error(NTH_DATA_PACKET_BYTE_INDEX_OUT_OF_RANGE_STRING + std::to_string(index));
    }
    return this->m_dataPacket.nthByte(index);
}

bool CanMessage::setDataPacketNthByte(int index, unsigned char nth)
{
    return this->m_dataPacket.setNthByte(index, nth);
}

CanDataPacket CanMessage::dataPacket() const
{
    return this->m_dataPacket;
}

std::string CanMessage::toString() const
{
    using namespace GeneralUtilities;
    if ((this->m_id == 0) &&
        (this->m_frame == 0) &&
        (this->m_length == 0) &&
        (this->m_dataPacket == CanDataPacket{0, 0, 0, 0, 0, 0, 0, 0})) {
        return "";
    }
    std::string returnString{"0x" + toFixedWidth(toHexString(this->m_id), CAN_ID_WIDTH) + ":"};
    unsigned int i{0};
    for (auto &it : this->m_dataPacket.dataPacket()) {
        returnString += "0x" + toFixedWidth(toHexString(it), CAN_BYTE_WIDTH);
        if (i++ != (this->m_dataPacket.dataPacket().size()-1)) {
            returnString += ":";
        }
    }
    return returnString;
}

std::string CanMessage::toPrettyString() const
{
    using namespace GeneralUtilities;
    if ((this->m_id == 0) &&
        (this->m_frame == 0) &&
        (this->m_length == 0) &&
        (this->m_dataPacket == CanDataPacket{0, 0, 0, 0, 0, 0, 0, 0})) {
        return "";
    }
    std::string returnString{"0x" + toFixedWidth(toHexString(this->m_id), CAN_ID_WIDTH) + " : "};
    unsigned int i{0};
    for (auto &it : this->m_dataPacket.dataPacket()) {
        returnString += "0x" + toFixedWidth(toHexString(it), CAN_BYTE_WIDTH);
        if (i++ != (this->m_dataPacket.dataPacket().size()-1)) {
            returnString += " : ";
        }
    }
    return returnString;
}

uint32_t CanMessage::parseCanID(const std::string &str)
{
    using namespace GeneralUtilities;
    return hexStringToUInt(str);
}

uint8_t CanMessage::parseCanByte(const std::string &str)
{
    using namespace GeneralUtilities;
    return hexStringToUChar(str);
}

CanMessage CanMessage::parseCanMessage(const std::string &str)
{
    using namespace GeneralUtilities;
    std::vector<std::string> rawMsg{parseToVector(str, ':')};
    if (rawMsg.size() != CanMessage::CAN_MESSAGE_SIZE) {
        return CanMessage{};
    }
    CanMessage returnMessage;
    int i{0};
    returnMessage.setFrame(CAN_FRAME);
    returnMessage.setLength(CAN_MESSAGE_LENGTH);
    for (auto &it : rawMsg) {
        if (i++ == 0) {
            returnMessage.setID(CanMessage::parseCanID(it));
        } else {
            returnMessage.setDataPacketNthByte(i-2, CanMessage::parseCanByte(it));
        }
    }
    return returnMessage;
}

