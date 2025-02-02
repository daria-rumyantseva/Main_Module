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
#include <requests.h>

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

// Функция для получения самого запроса клиента
std::string findRequest(std::string message)
{
    std::string request = "";
    size_t requestPos = message.find("request:");
    if (requestPos != std::string::npos) {
        size_t start = requestPos + 8; // длина "request:" = 8
        size_t end = message.find('\n', start);
        request = message.substr(start, end - start);
    }

    return request;
}


// Функция для получения токена из запроса клиента
std::string findToken(std::string message)
{
    std::string token = "";
    size_t tokenPos = message.find("request:");
    if (tokenPos != std::string::npos) {
        size_t start = tokenPos + 6; // длина "token:" = 6
        token = message.substr(start);
    }

    return token;
}


// Функция для проверки JWT токена
std::unordered_map<jwt::traits::kazuho_picojson::string_type, jwt::claim> CheckToken(SOCKET clientSocket, char* message, int msgSize)
{
    std::unordered_map<jwt::traits::kazuho_picojson::string_type, jwt::claim> payload;
    std::string secret = "33c52f27d5ac48dfb3d2a6422f06b4a6";

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

// Функция для определения роли пользователя
std::string getUserRoleFromToken(const std::string& token) {
    try {
        // Декодируем токен
        auto decodedToken = jwt::decode(token);

        // Извлекаем роль пользователя из токена
        // Предполагаем, что роль хранится в поле "role"
        auto claims = decodedToken.get_payload_claims();
        
        if (claims.find("role") != claims.end()) {
            return claims["role"].as_string(); // Получаем значение роли
        } else {
            throw std::runtime_error("Role claim not found in token");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error decoding token: " << e.what() << std::endl;
        return "";
    }
}

// Функция для определения id пользователя
std::string getUserIdFromToken(const std::string& token) {
    try {
        // Декодируем токен
        auto decodedToken = jwt::decode(token);

        // Извлекаем роль пользователя из токена
        // Предполагаем, что роль хранится в поле "role"
        auto claims = decodedToken.get_payload_claims();
        
        if (claims.find("id") != claims.end()) {
            return claims["id"].as_string(); // Получаем id
        } else {
            throw std::runtime_error("Id claim not found in token");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error decoding token: " << e.what() << std::endl;
        return "";
    }
}

// Функция для подключения к базе данных
void executeQuery(const std::string& conninfo, const std::string& query, const std::string& role, const int& id) {
    // Подключение к базе данных
    PGconn* conn = PQconnectdb(conninfo.c_str());

    // Проверка на успешное подключение
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return;
    }



    // Выполнение запроса и проверка на наличие разрешения
    if (!executionRequest(query, role, id)){
        std::cerr << "You don't have the required permission: 403" << std::endl;
    }

    // Освобождение ресурсов
    PQfinish(conn);
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



        std::string conninfo = "dbname=postgres user=postgres password=1111 hostaddr=localhost port=5432";
        std::string query = findRequest(message); 
        std::string token = findToken(message);
        std::string user_role = getUserRoleFromToken(token);
        std::string user_id = getUserIdFromToken(token);


        executeQuery(conninfo, query, user_role, stoi(user_id));
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
    serverAddr.sin_port = htons(8080); // Порт сокета используемый для подключения
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
