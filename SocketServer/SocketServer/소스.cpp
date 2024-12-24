#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <conio.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib") // Winsock 라이브러리 링크

#define PORT 8080
#define BUFFER_SIZE 1024
#define _CRT_SECURE_NO_WARNINGS

void error_handling(const char* message) {
    fprintf(stderr, "%s: %d\n", message, WSAGetLastError());
    exit(1);
}


int main() {
    WSADATA wsa;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    const char* python_command = "EXECUTE_PYTHON";

    srand((unsigned int)time(NULL));

    // Winsock 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        error_handling("WSAStartup failed");

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET)
        error_handling("Socket creation failed");

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        error_handling("Bind failed");

    // 연결 대기
    if (listen(server_fd, 5) == SOCKET_ERROR)
        error_handling("Listen failed");

    printf("Server is listening on port %d...\n", PORT);

    // 클라이언트 연결
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_size);
    if (client_fd == INVALID_SOCKET)
        error_handling("Accept failed");

    printf("Client connected!\n");

    // 메시지 수신
   
    while (1) {
        memset(buffer, 0, BUFFER_SIZE); // 버퍼 초기화
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // 문자열 끝 지정
            printf("클라이언트: %s\n", buffer);

            if (strcmp(buffer, "버스") == 0) {
                printf("이스터에그 발동\n");
                send(client_fd, python_command, strlen(python_command), 0);
                
            }
                
            
            // 종료 조건: "exit" 메시지를 받으면 서버 종료
            if (strcmp(buffer, "exit") == 0) {
                printf("클라이언트 종료 요청. 서버를 종료합니다.\n");
                break;
            }

            // 응답 메시지 보내기
            printf("보낼 메시지: ");

            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = '\0'; // 개행 문자 제거

            // 메시지 전송
            send(client_fd, buffer, strlen(buffer), 0);
        }
        else if (bytes_received == 0) {
            printf("클라이언트와의 연결이 종료되었습니다.\n");
            break;
        }
        else {
            printf("데이터 수신 오류 발생: %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup(); // Winsock 종료
    return 0;
}
