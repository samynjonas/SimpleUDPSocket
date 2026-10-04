#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>

namespace UDP
{
	class UDPReceiver
	{
	public:
		enum INIT_RESULTS : int
		{
			INIT_SUCCES = 0,
			WSA_STARTUP_FAIL,
			SOCKET_CREATION_FAIL,
			SOCKET_BIND_FAIL
		};

		enum RECEIVER_RESULTS : int
		{
			RECEIVER_SUCCES = 0,
			SOCKET_RECEIVE_ERROR,
			SETUP_ERROR
		};

	public:
		UDPReceiver();
		~UDPReceiver() = default;

		INIT_RESULTS Initialise(int port);
		void Uninitialise();

		RECEIVER_RESULTS Receive(void* buffer, int bufferSize, int& bytesReceived);
	private:
		SOCKET m_SocketHandle;
		sockaddr_in m_SocketAddress;
		bool m_ReadyToReceive;
	};
}


