#include <iostream>
#include <thread>
#include <chrono>

#include "UDPSocket.h"

namespace TEST_1
{
    struct Packet
    {
        int IntegerValue{ 0 };
        bool BooleanValue{ false };
        float FloatValue{ 0.f };
    };

    void SimplePacketSendingTest(int port, char const* ipAddress, int secInbetweenSends)
    {
        UDP::UDPSocket udpSocket{};
        udpSocket.Initialise(port, ipAddress);

        Packet packet{};
        while (true)
        {
            udpSocket.Send(packet);
            packet.IntegerValue += 1;
            packet.BooleanValue = !packet.BooleanValue;
            packet.FloatValue += 0.1f;

            std::this_thread::sleep_for(std::chrono::seconds(secInbetweenSends));
        }
        udpSocket.Uninitialise();
    }
}

namespace TEST_2
{
    struct Header
    {
        int Type{ -1 };
        int HeaderData{ 0 };
    };

    struct PacketType_1
    {
        int PacketData_1{};
        int PacketData_2{};
    };

    struct PacketType_2
    {
        float PacketData_1{};
        bool PacketData_2{};
    };

    struct HeaderPacketCombo_1
    {
        Header Header{ 1 };
        PacketType_1 Packet{};
    };

    struct HeaderPacketCombo_2
    {
        Header Header{ 2 };
        PacketType_2 Packet{};
    };

    void HeaderPacketSendingTest(int port, char const* ipAddress, int minSecInbetweenSend)
    {
        UDP::UDPSocket udpSocket{};
        udpSocket.Initialise(port, ipAddress);

        HeaderPacketCombo_1 comboType_1{};
        HeaderPacketCombo_2 comboType_2{};

        while (true)
        {
            // SETUP AND SEND FIRST PACKET
            udpSocket.Send(comboType_1);
            comboType_1.Packet.PacketData_1 += 1;
            comboType_1.Packet.PacketData_2 -= 1;

            int randomSendDelay{ rand() % 3 + minSecInbetweenSend };
            std::this_thread::sleep_for(std::chrono::seconds(randomSendDelay));

            // SETUP AND SEND SECOND PACKET
            udpSocket.Send(comboType_2);
            comboType_2.Packet.PacketData_1 += 0.1f;
            comboType_2.Packet.PacketData_2 = !comboType_2.Packet.PacketData_2;

            randomSendDelay = rand() % 3 + minSecInbetweenSend;
            std::this_thread::sleep_for(std::chrono::seconds(randomSendDelay));
        }
        udpSocket.Uninitialise();
    }
}

int main()
{
    int constexpr c_Port{ 8080 };
    char const* c_IpAddress{ "127.0.0.1" };

    int constexpr c_SendDelayInSec{ 2 };
    //TEST_1::SimplePacketSendingTest(c_Port, c_IpAddress, c_SendDelayInSec);

    TEST_2::HeaderPacketSendingTest(c_Port, c_IpAddress, c_SendDelayInSec);
}