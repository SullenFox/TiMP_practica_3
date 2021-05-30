#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

void errHandler(const char * why, const int exitCode = 1)
{
    cerr << why << endl;
    exit(exitCode);
}
int main(int argc, char **argv)
{
    //создание адресной струтуры c адресом клиента
    sockaddr_in * myAddr =  new(sockaddr_in);
    myAddr->sin_family = AF_INET;
    myAddr->sin_port = 0;
    myAddr->sin_addr.s_addr = 0;

    //создание адресной структуры c адресом сервера
    sockaddr_in * servAddr = new(sockaddr_in);
    servAddr->sin_family = AF_INET;
    servAddr->sin_port = htons(13);
    servAddr->sin_addr.s_addr = inet_addr("82.179.90.12");

    //буфер для данных
    char * buf = new char[256];
    strcpy(buf, "Скока времени?\n");
    int dlinaMsg = strlen(buf);
    
    //Создание сокета
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket == -1) {
        errHandler("Ошибка при создании сокета", 11);
    }

    //связать сокет с адресом
    int rc = bind(mySocket, (const sockaddr*) myAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Ошибка сокета и адреса", 12);
    }

    //Установка соединения
    rc = connect(mySocket, (const sockaddr*) servAddr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(mySocket);
        errHandler("Ошибка установки соединения", 13);
    }

    //Передать ланные
    rc = send(mySocket, buf, dlinaMsg, 0);
    if (rc == -1){
        close(mySocket);
        errHandler("Ошибка при отправке сообщения", 14);
        }
    cout << "Было отправлено: " << buf << endl;
    
    //Получние ответа
    rc = recv(mySocket, buf, 256, 0);
    if (rc == -1){
        close(mySocket);
        errHandler("Ошибка при получении сообщения",15);        
        }
    
    buf[rc] = '\0';
    cout << "Было получено: " << buf << endl;
    close(mySocket);
    delete myAddr;
    delete servAddr;
    delete[] buf;
         return 0;
}
