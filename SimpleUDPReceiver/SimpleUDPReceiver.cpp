#include <iostream>
#include <array>

#include "UDPReceiver.h"

namespace TEST_1
{
    struct Packet
    {
        int integerValue;
        bool booleanValue;
        float floatValue;
    };

    void SimplePacketReceivingTest(int port)
    {
        UDP::UDPReceiver udpReceiver{};
        udpReceiver.Initialise(8080);

        std::array<std::byte, 1500> buffer;

        Packet packet{};
        while (true)
        {
            int bytesReceived{ 0 };
            udpReceiver.Receive(buffer.data(), static_cast<int>(buffer.size()), bytesReceived);

            if (bytesReceived < sizeof(Packet))
            {
                continue;
            }

            std::memcpy(&packet, buffer.data(), sizeof(Packet));

            std::cout << "Received Packet:\n"
                << " IntegerValue \t [" << packet.integerValue << "]\n"
                << " BooleanValue \t [" << packet.booleanValue << "]\n"
                << " FloatValue \t [" << packet.floatValue << "]\n"
                << std::endl;
        }
        udpReceiver.Uninitialise();
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

    void HeaderPacketReceivingTest(int port)
    {
        UDP::UDPReceiver udpReceiver{};
        udpReceiver.Initialise(8080);

        std::array<std::byte, 1500> buffer;

        while (true)
        {
            std::cout << "------------------------------------------------" << std::endl;

            int bytesReceived{ 0 };
            udpReceiver.Receive(buffer.data(), static_cast<int>(buffer.size()), bytesReceived);

            if (bytesReceived < sizeof(Header))
            {
                continue;
            }

            Header header{};

            std::memcpy(&header, buffer.data(), sizeof(Header));

            std::cout << "Received Header:\n"
                << " Type \t \t [" << header.Type << "]\n"
                << " HeaderData \t [" << header.HeaderData << "]\n"
                << std::endl;

            std::byte const* packetPayload{ buffer.data() + sizeof(Header) };
            switch (header.Type)
            {
                case 1:
                {
                    PacketType_1 packet{};
                    std::memcpy(&packet, packetPayload, sizeof(PacketType_1));

                    std::cout << "Received Packet:\n"
                        << " PacketData_1 \t [" << packet.PacketData_1 << "]\n"
                        << " PacketData_2 \t [" << packet.PacketData_2 << "]\n"
                        << std::endl;
                }
                break;
                case 2:
                {
                    PacketType_2 packet{};
                    std::memcpy(&packet, packetPayload, sizeof(PacketType_2));

                    std::cout << "Received Packet:\n"
                        << " PacketData_1 \t [" << packet.PacketData_1 << "]\n"
                        << " PacketData_2 \t [" << packet.PacketData_2 << "]\n"
                        << std::endl;
                }
                break;
                default:
                {
                    std::cout << "Received Invalid packet type" << std::endl;
                }
                break;
            }

        }
        udpReceiver.Uninitialise();
    }
}

int main()
{
    int constexpr c_Port{ 8080 };

    //TEST_1::SimplePacketReceivingTest(c_Port);
    TEST_2::HeaderPacketReceivingTest(c_Port);
}
