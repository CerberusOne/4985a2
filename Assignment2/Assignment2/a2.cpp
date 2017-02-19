/*-----------------------------------------------------------------------
--  SOURCE FILE:   a2.cpp   
--
--  PROGRAM:       COMP 4981 Assignment 2
--
--  FUNCTIONS:
--                  int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
--							  		  LPSTR lspszCmdParam, int nCmdShow);
--                  LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
--
--
--  DATE:          Feb 3, 2017
--
--  DESIGNER:      Aing Ragunathan
--
--
--  NOTES:
--		
----------------------------------------------------------------------------*/

#define STRICT
#define _CRT_SECURE_NO_WARNINGS
#define _UNICODE
#define SERVER_TCP_PORT			7000	// Default port
#define BUFSIZE					255		// Buffer length

#include <windows.h>
#include <string>
#include <iostream>
#include <streambuf>
#include <vector>
#include "MenuSettings.h"

using namespace std;

//Function prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			//Receives input directed 
bool openFile(static HWND FilenameWnd);							//Opens a file browser
OPENFILENAME createOfnStruct(char filenameBuffer[128]);			//creates an OPENFILENAME object
void sendFile();												//send files to server

//Global variables
static TCHAR Name[] = TEXT("Assignment 2");			//Name of main window
HWND hwnd;											//Handle to main window
HANDLE fileHandle;									//Handle to the requested file to send
HINSTANCE mainInst;									//Instance of main window//at this application

/*---------------------------------------------------------------------------------
--  FUNCTION:      WinMain
--
--  DATE:          Feb 3, 2017
--
--  DESIGNER:      Aing Ragunathan
--
--
--  INTERFACE:     int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
--                                  	LPSTR lspszCmdParam, int nCmdShow)
--                        
--
--  RETURNS:       WM_QUITS message value or 0 if no messages sent
--
--  NOTES:
--		This function creates the main window, registers it, displays it 
--		and sends messages. 
-----------------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance,
	LPSTR lspszCmdParam, int nCmdShow)
{
	MSG Msg;
	WNDCLASSEX Wcl;
	mainInst = hInst;

	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = CS_HREDRAW | CS_VREDRAW;
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc;
	Wcl.hInstance = hInst;
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = Name;

	Wcl.lpszMenuName = TEXT("MYMENU"); // The menu class
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	// Register the class
	if (!RegisterClassEx(&Wcl))
		return 0;

	hwnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW, 10, 10,
		475, 700, NULL, NULL, hInst, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, "Main window failed to open, program shutting down", "ERROR", MB_OK);
		return 0;
	}

	// Open up a Winsock v2.2 session
	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	WSAStartup(wVersionRequested, &wsaData);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}

/*---------------------------------------------------------------------------------
--  FUNCTION:     WndProc
--
--  DATE:         Feb 3, 2017
--
--  DESIGNER:     Aing Ragunathan
--
--  INTERFACE:    LRESULT CALLBACK WndProc(HWND hwnd, UINT Message,
--	                                        WPARAM wParam, LPARAM lParam)
--                           HWND hwnd: A handle to the window
--                           UINT Message: Recieved message
--                           WPARAM wParam: Additional message information
--                           LPARAM lParam: Additional message information
--
--  RETURNS:      Result of message processing
--
--  NOTES:
--		This function is used to parse the menu selections and handle all system messages.
--		It also calls appropriate functions to resolve input from the user.
-----------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	//Handles for statistics
	static HWND IPWndTitle;
	static HWND IPWnd;

	static HWND PortWndTitle;
	static HWND PortWnd;

	static HWND ProtocolWndTitle;
	static HWND ProtocolWnd;

	static HWND PacketSizeWndTitle;
	static HWND PacketSizeWnd;

	static HWND PacketQuantityWndTitle;
	static HWND PacketQuantityWnd;

	static HWND FilenameWndTitle;
	static HWND FilenameWnd;

	static HWND ReceivedMessageWndTitle;
	static HWND ReceivedMessageWnd;


	switch (Message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))  //Parsing the menu selections
		{
		case IDM_OPEN_FILE:
			//openFile();
			//OutputDebugStringA("hello");
			if (openFile(FilenameWnd)) {
				OutputDebugString("File open: Successful");
			}
			else {
				OutputDebugString("File open: Failed!");
			}
			break;
		case IDM_SEND:
			sendFile();
			break;
		case WM_DESTROY:	// Terminate program
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;
	case WM_CREATE:
		IPWndTitle = CreateWindowEx(NULL, "STATIC", "IP:", WS_VISIBLE | WS_CHILD,
			10, 10, 200, 20, hwnd, NULL, mainInst, NULL);
		IPWnd = CreateWindowEx(NULL, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
			10, 30, 200, 50, hwnd, NULL, mainInst, NULL);

		PortWndTitle = CreateWindowEx(NULL, "STATIC", "Port:", WS_VISIBLE | WS_CHILD,
			250, 10, 200, 20, hwnd, NULL, mainInst, NULL);
		PortWnd = CreateWindowEx(NULL, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
			250, 30, 200, 50, hwnd, NULL, mainInst, NULL);

		ProtocolWndTitle = CreateWindowEx(NULL, "STATIC", "Protocol:", WS_VISIBLE | WS_CHILD,
			10, 100, 200, 20, hwnd, NULL, mainInst, NULL);
		ProtocolWnd = CreateWindowEx(NULL, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
			10, 120, 200, 50, hwnd, NULL, mainInst, NULL);

		PacketQuantityWndTitle = CreateWindowEx(NULL, "STATIC", "Packet Quantity:", WS_VISIBLE | WS_CHILD,
			10, 190, 200, 20, hwnd, NULL, mainInst, NULL);
		PacketQuantityWnd = CreateWindowEx(NULL, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
			10, 210, 200, 50, hwnd, NULL, mainInst, NULL);

		PacketSizeWndTitle = CreateWindowEx(NULL, "STATIC", "Packet Size:", WS_VISIBLE | WS_CHILD,
			250, 190, 200, 20, hwnd, NULL, mainInst, NULL);
		PacketSizeWnd = CreateWindowEx(NULL, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
			250, 210, 200, 50, hwnd, NULL, mainInst, NULL);

		FilenameWndTitle = CreateWindowEx(NULL, "STATIC", "Filename:", WS_VISIBLE | WS_CHILD,
			10, 280, 440, 20, hwnd, NULL, mainInst, NULL);
		FilenameWnd = CreateWindowEx(NULL, "STATIC", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
			10, 310, 440, 50, hwnd, NULL, mainInst, NULL);

		ReceivedMessageWndTitle = CreateWindowEx(NULL, "STATIC", "Output:", WS_VISIBLE | WS_CHILD,
			10, 370, 445, 20, hwnd, NULL, mainInst, NULL);
		ReceivedMessageWnd = CreateWindowEx(NULL, "STATIC", "", WS_VISIBLE | WS_CHILD,
			10, 400, 445, 320, hwnd, NULL, mainInst, NULL);
		break;

	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}


//openFile(){}
bool openFile(static HWND FilenameWnd) {
	OPENFILENAME openFilenameStruct;	//dialog box structure for file browser
	char filenameBuffer[128];			//buffer for filename
	TCHAR filename[MAX_PATH];
	DWORD dwRet;

	//fileHandle = NULL;					//reset the handle
	openFilenameStruct = createOfnStruct(filenameBuffer); //initialize the dialog box structure

	//display a file browser
	if (GetOpenFileName(&openFilenameStruct) == TRUE) {
		fileHandle = CreateFile(openFilenameStruct.lpstrFile,
			GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);
	}

	if (fileHandle == INVALID_HANDLE_VALUE) {
		printf("Count not open file (error %d\n)", GetLastError());
		return false;
	}
	
//set filename sub windows to new filename

	//LPSTR output = openFilenameStruct.lpstrFile;
	dwRet = GetFinalPathNameByHandle(fileHandle, filename, MAX_PATH, VOLUME_NAME_NT);
	if (dwRet < MAX_PATH) {
		//printf(TEXT("\nThe final path is: %s\n"), filename);
		//SetWindowText(FilenameWnd, filename);
	}

	
	cout << openFilenameStruct.lpstrFile;

	//loadFile(fileHandle); //save file 

	CloseHandle(fileHandle);

	return true;
}

OPENFILENAME createOfnStruct(char filenameBuffer[128]) {
	OPENFILENAME openFilenameStruct;	//dialog box structure for file browser
	
	//initialize the dialog box structure
	ZeroMemory(&openFilenameStruct, sizeof(openFilenameStruct));
	openFilenameStruct.lStructSize = sizeof(openFilenameStruct);
	openFilenameStruct.hwndOwner = hwnd;
	openFilenameStruct.lpstrFile = filenameBuffer;
	openFilenameStruct.lpstrFile[0] = '\0';	//set to null so that struct doesn't use garbage
	openFilenameStruct.nMaxFile = sizeof(filenameBuffer);
	openFilenameStruct.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	openFilenameStruct.nFilterIndex = 1;
	openFilenameStruct.lpstrFileTitle = NULL;
	openFilenameStruct.nMaxFileTitle = 0;
	openFilenameStruct.lpstrInitialDir = NULL;
	openFilenameStruct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	return openFilenameStruct;
}

/*
void loadFile(HANDLE filename) {
	char //create a buffer
	//save file content to a buffer
	//packetizeFile(file buffer)
}
*/

//packetizeFile(file buffer)
	//create an array buffer of size "quantity of packets"
	//for every segment of data in file with the size of "quantity of packets"
	//**for(packetNumber = 0; packetNumber < "quantity of packets" && file isn't EOF; packetNumber++)
		//save in array buffer 


void sendFile() {
	int n, ns, bytes_to_read;
	int port, err;
	SOCKET sd;
	struct hostent	*hp;
	struct sockaddr_in server;
	char  *host, *bp, rbuf[BUFSIZE], sbuf[BUFSIZE], **pptr;
	WSADATA WSAData;
	WORD wVersionRequested;

	//send each packet in array buffer
	host = "192.168.0.12";
	port = SERVER_TCP_PORT;

	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &WSAData);
	if (err != 0) //No usable DLL
	{
		printf("DLL not found!\n");
		exit(1);
	}

	// Create the socket
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Cannot create socket");
		exit(1);
	}

	// Initialize and set up the address structure
	memset((char *)&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if ((hp = gethostbyname(host)) == NULL)
	{
		fprintf(stderr, "Unknown server address\n");
		exit(1);
	}

	// Copy the server address
	memcpy((char *)&server.sin_addr, hp->h_addr, hp->h_length);

	// Connecting to the server
	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		exit(1);
	}
	printf("Connected:    Server Name: %s\n", hp->h_name);
	pptr = hp->h_addr_list;
	printf("\t\tIP Address: %s\n", inet_ntoa(server.sin_addr));
	printf("Transmiting:\n");
	memset((char *)sbuf, 0, sizeof(sbuf));
	gets_s(sbuf); // get user's text

	// Transmit data through the socket
	ns = send(sd, sbuf, BUFSIZE, 0);
	printf("Receive:\n");
	bp = rbuf;
	bytes_to_read = BUFSIZE;

	// client makes repeated calls to recv until no more data is expected to arrive.
	while ((n = recv(sd, bp, bytes_to_read, 0)) < BUFSIZE)
	{
		bp += n;
		bytes_to_read -= n;
		if (n == 0)
			break;
	}
	printf("%s\n", rbuf);
	closesocket(sd);
	WSACleanup();


}