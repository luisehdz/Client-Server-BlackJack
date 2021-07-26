#include<stdio.h>
#include<winsock2.h>
#include<Ws2tcpip.h>
#include<string.h>
#include<thread>
#include<iostream>
#include"blackjack.h";

#pragma comment(lib,"ws2_32.lib") //Winsock Library

void reverseChar(char* str)
{
	std::reverse(str, str + strlen(str));
}

void writeToClient(SOCKET skt, const char* message)
{
	send(skt, message, strlen(message), 0);
}

bool result(char response[1])
{
	if (response[0] == 'Y' || response[0] == 'y')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main(int argc, char* argv[])
{
	BlackJack game;
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	const char* message = " ";
	char response[1];
	int readValue = 1;
	int c;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	puts("Bind done");

	//Listen to incoming connections
	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);

	while ((new_socket = accept(s, (struct sockaddr*)&client, &c)) != INVALID_SOCKET)
	{
		puts("Connection accepted");

		//Reply to the client
		message = "Welcome to the House of Card! Today we will be playing a simple game of BlackJack\n";
		writeToClient(new_socket, message);

		//initial buffer
		readValue = shutdown(s, SD_SEND);

		do
		{
			std::string initialMessage = game.play();
			writeToClient(new_socket, initialMessage.c_str());
			std::string gameMessage = game.logic();
			writeToClient(new_socket, gameMessage.c_str());

			while (game.hasLost != true || game.hasWon != true || readValue == 0)
			{
				recv(new_socket, response, strlen(response), 0);
				if (result(response) && game.hasWon || result(response) && game.hasLost)
				{
					//puts("game won or lost replay");
					initialMessage = game.play();
					writeToClient(new_socket, initialMessage.c_str());
					gameMessage = game.logic();
					writeToClient(new_socket, gameMessage.c_str());
				}
				else if (!result(response) && game.hasWon || !result(response) && game.hasLost)
				{
					//puts("game won or lost and quit");
					puts("User Disconnected");
					std::string finalMessage = "Connection Closed";
					writeToClient(new_socket, finalMessage.c_str());
					readValue = 0;
					break;
				}
				else if (result(response) && !game.hasWon)
				{
					//puts("game not won draw another");
					std::string dealtCard = game.deal();
					writeToClient(new_socket, dealtCard.c_str());
					gameMessage = game.logic();
					writeToClient(new_socket, gameMessage.c_str());
				}
				else if (!result(response) && !game.hasWon)
				{
					//puts("game not won no draw");
					std::string retryMessage = "Would you like to play again? (Y/N) ";
					writeToClient(new_socket, retryMessage.c_str());
					recv(new_socket, response, strlen(response), 0);
					if (result(response))
					{
						initialMessage = game.play();
						writeToClient(new_socket, initialMessage.c_str());
						gameMessage = game.logic();
						writeToClient(new_socket, gameMessage.c_str());
					}
					else
					{
						//puts("game finished quit");
						puts("User Disconnected");
						std::string finalMessage = "Connection Closed";
						writeToClient(new_socket, finalMessage.c_str());
						readValue = 0;
						break;
					}
				}
			}

		} while (readValue > 0);
	}

	if (new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
		return 1;
	}

	closesocket(s);
	WSACleanup();

	return 0;
}