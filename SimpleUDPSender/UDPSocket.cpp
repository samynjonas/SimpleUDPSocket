#include "UDPSocket.h"

#pragma comment(lib, "ws2_32.lib")

namespace UDP
{
    UDPSocket::UDPSocket()
        : m_ReadyToSend(false)
    {

    }

    UDPSocket::INIT_RESULTS UDPSocket::Initialise(int port, char const* ipAddress)
	{
        m_ReadyToSend = false;

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

        inet_pton(
            AF_INET,
            ipAddress,
            &m_SocketAddress.sin_addr
        );

        m_ReadyToSend = true;
        return INIT_RESULTS::SUCCES;
	}

	void UDPSocket::Uninitialise()
	{
        m_ReadyToSend = false;
        closesocket(m_SocketHandle);
        WSACleanup();
	}
}