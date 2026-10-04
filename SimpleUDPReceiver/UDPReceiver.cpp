#include "UDPReceiver.h"

#include <iostream>
#pragma comment(lib, "ws2_32.lib")

namespace UDP
{
    UDPReceiver::UDPReceiver()
        : m_ReadyToReceive(false)
    {

    }

    UDPReceiver::INIT_RESULTS UDPReceiver::Initialise(int port)
    {
        m_ReadyToReceive = false;

        // Start WinSock
        WSADATA wsaData;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
#ifdef _DEBUG
            std::cerr << "WSAStartup failed" << std::endl;
#endif // _DEBUG
            return INIT_RESULTS::WSA_STARTUP_FAIL;
        }

        // Create Socket
        m_SocketHandle = socket(
            AF_INET,
            SOCK_DGRAM,
            IPPROTO_UDP
        );

        if (m_SocketHandle == INVALID_SOCKET)
        {
#ifdef _DEBUG
            std::cerr << "Failed to create socket" << std::endl;
#endif // _DEBUG
            return INIT_RESULTS::SOCKET_CREATION_FAIL;
        }

        m_SocketAddress = sockaddr_in{};
        m_SocketAddress.sin_family = AF_INET;
        m_SocketAddress.sin_port = htons(port);
        m_SocketAddress.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(
            m_SocketHandle,
            reinterpret_cast<sockaddr const*>(&m_SocketAddress),
            sizeof(m_SocketAddress)) == SOCKET_ERROR)
        {
#ifdef _DEBUG
            std::cerr << "Bind failed" << std::endl;
#endif // _DEBUG

            Uninitialise();
            
            return INIT_RESULTS::SOCKET_BIND_FAIL;
        }

        m_ReadyToReceive = true;
        return INIT_RESULTS::INIT_SUCCES;
    }

    void UDPReceiver::Uninitialise()
    {
        m_ReadyToReceive = false;
        closesocket(m_SocketHandle);
        WSACleanup();
    }

    UDPReceiver::RECEIVER_RESULTS UDPReceiver::Receive(void* buffer, int bufferSize, int& bytesReceived)
    {
        if (!m_ReadyToReceive)
        {
            return RECEIVER_RESULTS::SETUP_ERROR;
        }

        sockaddr_in senderAddress{};
        int senderAddressSize = sizeof(senderAddress);

        bytesReceived = recvfrom(
            m_SocketHandle,
            reinterpret_cast<char*>(buffer),
            bufferSize,
            0,
            reinterpret_cast<sockaddr*>(&senderAddress),
            &senderAddressSize
        );

        if (bytesReceived == SOCKET_ERROR)
        {
            return RECEIVER_RESULTS::SOCKET_RECEIVE_ERROR;
        }

#ifdef _DEBUG
        std::cout << "Packet Received [" << m_ReadyToReceive << "]" << std::endl;
#endif // _DEBUG

        return RECEIVER_RESULTS::RECEIVER_SUCCES;
    }
}
