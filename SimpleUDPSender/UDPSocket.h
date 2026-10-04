#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>

#include <iostream>

namespace UDP
{
	class UDPSocket final
	{
	public:
		enum INIT_RESULTS : int
		{
			SUCCES = 0,
			WSA_STARTUP_FAIL,
			SOCKET_CREATION_FAIL,
		};

	public:
		UDPSocket();
		~UDPSocket() = default;

		INIT_RESULTS Initialise(int port, char const* ipAddress);
		void Uninitialise();

		template<typename T>
		bool Send(T const& packetToSend)
		{
			if (m_ReadyToSend)
			{
				sendto(
					m_SocketHandle,
					reinterpret_cast<char const*>(&packetToSend),
					sizeof(packetToSend),
					0,
					reinterpret_cast<sockaddr const*>(&m_SocketAddress),
					sizeof(m_SocketAddress)
				);
			}

#ifdef _DEBUG
			std::cout << "Packet Send [" << m_ReadyToSend << "]" << std::endl;
#endif // _DEBUG
			return m_ReadyToSend;
		}

	private:
		SOCKET m_SocketHandle;
		sockaddr_in m_SocketAddress;
		bool m_ReadyToSend;
	};
}

