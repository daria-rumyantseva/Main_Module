#undef min
#undef max

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <iomanip> // Библиотека для красивого вывода
#include <jwt-cpp/jwt.h> // Библиотека для jwt токенов
#include <libpq-fe.h> // Библиотека для работы с PostgreSQL
//#include <pqxx/pqxx> // Подключаем библиотеку pqxx
//#include <sqlpp11/sqlpp11.h> // Библиотека для работы с SQL в C++


//#include <date/date.h>
#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h> // Библиотека для работы с сокетами

// Функция для вывода красивого текста
void beautyPrint(SOCKET soc, std::string text)
{
    std::cout << '[' << std::setfill(' ') << std::setw(7) << soc << "] " << text << std::endl;
}

// Функция для отправки сообщений пользователю
void sendToUser(SOCKET clientSocket, int code, std::string message)
{
    std::ostringstream responseStream;
    responseStream << "HTTP/1.1 " << code << " Forbidden\r\n"
                   << "\r\n"
                   << message;

    // Преобразуем поток в строку
    std::string httpResponse = responseStream.str();

    // Отправляем ответ клиенту
    send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);
}

// Функция для получения токена из запроса клиента
std::string findToken(std::string message)
{
    std::string token = "";

    size_t bearerPos = message.find("Bearer ");
    if (bearerPos != std::string::npos) {
        token = message.substr(bearerPos + 7);
    }

    size_t cut_pos = token.size();
    if (token.find(' ') != std::string::npos) {
        if (cut_pos > token.find(' '))
            cut_pos = token.find(' ');
    }
    if (token.find(' ') != std::string::npos) {
        if (cut_pos > token.find('\n'))
            cut_pos = token.find('\n');
    }
    if (token.find(' ') != std::string::npos) {
        if (cut_pos > token.find('\r'))
            cut_pos = token.find('\r');
    }

    token = token.substr(0, cut_pos);

    return token;
}


// Функция для проверки JWT токена
std::unordered_map<jwt::traits::kazuho_picojson::string_type, jwt::claim> CheckToken(SOCKET clientSocket, char* message, int msgSize)
{
    std::unordered_map<jwt::traits::kazuho_picojson::string_type, jwt::claim> payload;
    std::string secret = "key";

    beautyPrint(clientSocket, "Find token...");
    std::string token = findToken(message);

    if (token == "")
    {
        beautyPrint(clientSocket, "Token not found.");
        return payload;
    }

    // Выполняем проверку подписи токена ключом SECRET
    jwt::decoded_jwt<jwt::traits::kazuho_picojson> decoded_token = jwt::decode(token);
    try {
        jwt::verifier<jwt::default_clock, jwt::traits::kazuho_picojson> verifier = jwt::verify().allow_algorithm(jwt::algorithm::hs256{ secret });
        verifier.verify(decoded_token);
        // Если токен валидный, проходим дальше иначе попадаем в catch
        payload = decoded_token.get_payload_claims();
    }catch (...) {
        beautyPrint(clientSocket, "Invalid token.");
    }

    return payload;
}

// Функция для обработки клиента
void handle_client(SOCKET client_socket) {
    std::cout << "Client connected." << std::endl;
    while (true) {
        std::vector<char> clientBuff(1024);
        const int msgSize = 2048;
        char* message = new char[msgSize];
        int bytesReceived = recv(client_socket, clientBuff.data(), clientBuff.size(), 0);

        if (bytesReceived == SOCKET_ERROR) {
             beautyPrint(client_socket, "Receive failed. Error #" + WSAGetLastError());
            break;
        }

        // Проверка на завершение общения
        if (bytesReceived > 0 && clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
            std::cout << "Client requested to close the connection." << std::endl;
            break; // Выход из цикла
        }

        // Обработка полученных данных
        std::string enc_str(clientBuff.begin(), clientBuff.begin() + bytesReceived);
        // Пример для теста — требуемое разрешение для выполнения действия
        std::string required_permission = "perform_action";

        auto permission = CheckToken(client_socket, message, msgSize);
        if (permission.empty())
        {
            beautyPrint(client_socket, "Error: 401");
            beautyPrint(client_socket, "Send code 401");
            sendToUser(client_socket, 401, "Access Denied");
            closesocket(client_socket);
            beautyPrint(client_socket, "Socket closed.");
            continue;
        }
        beautyPrint(client_socket, "Token correct: 200");
        beautyPrint(client_socket, "Send code 200");
        sendToUser(client_socket, 200, "TODO");
        closesocket(client_socket);
        beautyPrint(client_socket, "Socket closed.");













        

        // Здесь можно добавить логику обработки запроса после успешной валидации токена
    }

    closesocket(client_socket);
    std::cout << "Client disconnected." << std::endl;
}

int main() {
    // Инициализация Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed. Error #" << WSAGetLastError() << std::endl;
        return 1;
    }

    // Создание сокета
    SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ServSock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error #" << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Привязка сокета
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(1111); // Порт сокета используемый для подключения
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(ServSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error #" << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }

    // Прослушивание порта
    if (listen(ServSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed. Error #" << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }

    std::cout << "Waiting for client connection..." << std::endl;

    // Принятие соединения
    SOCKET ClientConn = accept(ServSock, nullptr, nullptr);
    if (ClientConn == INVALID_SOCKET) {
        std::cerr << "Accept failed. Error #" << WSAGetLastError() << std::endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected." << std::endl;

    while (true) {
        SOCKET ClientConn = accept(ServSock, nullptr, nullptr);
        if (ClientConn == INVALID_SOCKET) 
        { std::cerr << "Accept failed. Error #" << WSAGetLastError() << std::endl;
            continue; // Продолжаем ожидание следующего клиента
        }

        // Создаем новый поток для обработки клиента
        std::thread(handle_client, ClientConn).detach();

    }

    // Закрытие соединения и очистка Winsock
    closesocket(ClientConn);
    closesocket(ServSock);
    WSACleanup();

    return 0;
}
