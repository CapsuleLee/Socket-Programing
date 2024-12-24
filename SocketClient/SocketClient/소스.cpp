#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>


#pragma comment(lib, "ws2_32.lib") // Winsock 라이브러리 링크

#define PORT 8080
#define BUFFER_SIZE 1024
#define PYTHON_FILE "BusTimeTable.py"

void error_handling(const char* message) {
    fprintf(stderr, "%s: %d\n", message, WSAGetLastError());
    exit(1);
}
void execute_python_file() {
    wchar_t wCommand[BUFFER_SIZE];
    wchar_t wPythonCommand[BUFFER_SIZE];

    // Python 실행 명령 생성: python 파일 실행 후 창 유지
    char command[BUFFER_SIZE];
    snprintf(command, sizeof(command), "cmd.exe /k python \"%s\"", PYTHON_FILE);

    // 멀티바이트 문자열을 유니코드로 변환
    MultiByteToWideChar(CP_ACP, 0, "cmd.exe", -1, wCommand, BUFFER_SIZE);
    MultiByteToWideChar(CP_ACP, 0, command, -1, wPythonCommand, BUFFER_SIZE);

    // ShellExecute를 통해 Python 실행
    if (ShellExecute(NULL, L"open", wCommand, wPythonCommand, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32) {
        printf("실행 실패: %s\n", PYTHON_FILE);
    }
    
}


int main() {
    WSADATA wsa;
    SOCKET client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        error_handling("WSAStartup failed");

    // 소켓 생성
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == INVALID_SOCKET)
        error_handling("Socket creation failed");

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) != 1) {
        printf("IP 주소 변환에 실패했습니다.\n");
        return 1; // 오류 처리
    }
    server_addr.sin_port = htons(PORT);

    // 서버에 연결
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        error_handling("Connect failed");

    printf("Connected to server!\n");

    while (1) {
        // 사용자 입력
        printf("보낼 메시지: ");
        
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // 개행 문자 제거

        // 메시지 전송
        send(client_fd, buffer, strlen(buffer), 0);

        // 종료 조건
        if (strcmp(buffer, "exit") == 0) {
            printf("연결을 종료합니다.\n");
            break;
        }

        // 서버 응답 수신
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("서버: %s\n", buffer);

            if (strcmp(buffer, "EXECUTE_PYTHON") == 0) {
                printf("이스터 에그 발동!\n");
                execute_python_file();
            }
        }
        else {
            printf("서버 응답 수신 오류.\n");
            break;
        }
    }
   
    closesocket(client_fd);
    WSACleanup(); // Winsock 종료
    return 0;
}
