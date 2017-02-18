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

#include <windows.h>
#include <string>
#include <iostream>
#include <streambuf>
#include <vector>
#include "MenuSettings.h"

using namespace std;

//Function prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);			//Receives input directed 
				
//Global variables
static TCHAR Name[] = TEXT("Assignment 2");			//Name of main window
HWND hwnd;											//Handle to main window
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

	static HWND ReceivedMessageWndTitle;
	static HWND ReceivedMessageWnd;


	switch (Message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))  //Parsing the menu selections
		{
		case IDM_OPEN_FILE:
			//findIP(HostNameWnd, output);
			break;
		case IDM_SEND:
			
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

		PacketSizeWndTitle = CreateWindowEx(NULL, "STATIC", "Packet Size:", WS_VISIBLE | WS_CHILD,
			250, 100, 200, 20, hwnd, NULL, mainInst, NULL);
		PacketSizeWnd = CreateWindowEx(NULL, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
			250, 120, 200, 50, hwnd, NULL, mainInst, NULL);

		PacketQuantityWndTitle = CreateWindowEx(NULL, "STATIC", "Packet Quantity:", WS_VISIBLE | WS_CHILD,
			10, 190, 200, 20, hwnd, NULL, mainInst, NULL);
		PacketQuantityWnd = CreateWindowEx(NULL, "EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
			10, 210, 200, 50, hwnd, NULL, mainInst, NULL);

		ReceivedMessageWndTitle = CreateWindowEx(NULL, "STATIC", "Output:", WS_VISIBLE | WS_CHILD,
			10, 280, 445, 20, hwnd, NULL, mainInst, NULL);
		ReceivedMessageWnd = CreateWindowEx(NULL, "STATIC", "", WS_VISIBLE | WS_CHILD,
			10, 300, 445, 320, hwnd, NULL, mainInst, NULL);
		break;

	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

