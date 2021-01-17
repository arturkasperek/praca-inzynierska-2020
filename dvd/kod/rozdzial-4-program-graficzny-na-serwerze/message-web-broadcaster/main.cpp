#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#include <winsock2.h>
#include <tchar.h>

SOCKET ConnectSocket;

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
#define bcopy(s1, s2, n) memmove((s2), (s1), (n))

int initWebsocket()
{
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %ld\n", iResult);
        return 1;
    }

    //----------------------
    // Create a SOCKET for connecting to server
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError() );
        WSACleanup();
        return 1;
    }

    struct hostent        *he;
    struct in_addr a;
    const char hostname[] = "web-listener";

    /* resolve hostname */
    if ( (he = gethostbyname(hostname) ) == NULL ) {
        printf("Hostname cant be resolved");
        exit(1);
    }

    if (he)
    {
        bcopy(*he->h_addr_list, (char *) &a, sizeof(a));
        printf("address: %s\n", inet_ntoa(a));
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    //addrServer.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    memcpy(&addrServer.sin_addr, he->h_addr_list[0], he->h_length);
    addrServer.sin_port = htons(8080);

	//----------------------
    // Connect to server.
    iResult = connect( ConnectSocket, (SOCKADDR*) &addrServer, sizeof(addrServer) );
    if ( iResult == SOCKET_ERROR) {
        closesocket (ConnectSocket);
        printf("Unable to connect to server: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    return 0;
}

int closeSocket() {
    closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}

int sendMessage(const char* message) {
    if( send(ConnectSocket , message , strlen(message) , 0) < 0)
    {
        printf("WebSocket Send failed");
        return 1;
    }
    return 0;
}


int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    if ( initWebsocket() != 0 ) {
        return 0;
    }

    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;

    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    if (!RegisterClassEx (&wincl))
        return 0;

    hwnd = CreateWindowEx (
           0,
           szClassName,
           _T("[Graphic App Spy]"),
           WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           544,
           375,
           HWND_DESKTOP,
           NULL,
           hThisInstance,
           NULL
           );

    printf("Message Web Broadcaster started\n");

    while (GetMessage (&messages, NULL, 0, 0))
    {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    closeSocket();

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_COPYDATA:
            {
                 COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
                 char * lpszString = (char *)(pcds->lpData);

                 printf("Web Broadcaster received COPYDATA message!!\n");
                 sendMessage((char*)lpszString);
                 break;
            }
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
