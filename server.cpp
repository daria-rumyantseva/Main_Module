#include <iostream>
#include <string>
#include <vector>
#include <jwt-cpp/jwt.h> // Библиотека для jwt токенов
#include <libpq-fe.h> // Библиотека для работы с postgreSQL
//#include <pqxx/pqxx> // Подключаем библиотеку pqxx
#include <sqlpp11/sqlpp11.h> // Библиотека для работы с postgreSQL



#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h> // Библиотека для работы с сокетами



// Функция для проверки JWT токена
bool validate_jwt(const std::string& token, const std::string& secret, const std::string& required_permission) {
    try {
        // Расшифровка JWT токена
        auto dec_obj = jwt::decode(token);

        // Проверка подписи
        dec_obj.verify(jwt::params::algorithms::hs256{secret});

        // Проверка срока действия токена
        auto exp = dec_obj.payload().get_claim_value_by_name("exp").as_int();
        if (exp < time(nullptr)) {
            std::cerr << "Token has expired." << std::endl;
            return false;
        }

        // Проверка разрешений
        auto permissions = dec_obj.payload().get_claim_value_by_name("permissions").as_array();
        for (const auto& perm : permissions) {
            if (perm.as_string() == required_permission) {
                return true; // Разрешение найдено
            }
        }

        std::cerr << "Permission '" << required_permission << "' is missing." << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cerr << "JWT validation error: " << e.what() << std::endl;
        return false;
    }
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
    sockaddr_in serverAddr;
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
        std::vector<char> clientBuff(1024);
        int bytesReceived = recv(ClientConn, clientBuff.data(), clientBuff.size(), 0);

        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Receive failed. Error #" << WSAGetLastError() << std::endl;
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

        // Секрет для проверки подписи
        std::string secret = "secret"; 

        // Проверка JWT токена
        if (!validate_jwt(enc_str, secret, required_permission)) {
            // Если токен не прошел проверку, отправим ошибку
            const std::string error_message = "Unauthorized (401)";
            send(ClientConn, error_message.c_str(), error_message.size(), 0);
            continue; // Переход к следующему циклу
        }

        // Если токен валидный и разрешение на действие есть
        // Выполнение запроса (например, получение/создание/изменение данных)
        const std::string success_message = "Action performed successfully.";
        send(ClientConn, success_message.c_str(), success_message.size(), 0);

        // Подключение базы данных
        try {
            pqxx::connection C("dbname=postgres user=postgres password=1111 hostaddr=localhost port=5432");
            if (C.is_open()) {
                std::cout << "Opened database successfully: " << C.dbname() << std::endl;
            } else {
                std::cout << "Can't open database" << std::endl;
                return 1;
            }
 
            // Здесь можно выполнять SQL-запросы

            






















 
            C.disconnect();
        } catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    // Закрытие сокетов и очистка
    closesocket(ClientConn);
    WSACleanup();
    return 0;
}


