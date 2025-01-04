
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

using namespace std;


class DatabaseConnection {
public:
	pqxx::connection conn;
	
	DatabaseConnection(const std::string& connection_string) {
		try {
			conn = pqxx::connection(connection_string);
			if (conn.is_open()) {
				std::cout << "Connected to database!" << std::endl;
			} else {
				std::cerr << "Failed to connect to database!" << std::endl;
			}
		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
	pqxx::work create_transaction() {
		
	return pqxx::work(conn);
	}
};
	

class Authorization {
public:
	static bool validate_token(const std::string& token) {
		// Примерная валидация JWT токена (реальная валидация должна быть сложнее)
		if (token.empty()) return false;
			
		// Здесь должна быть логика для декодирования токена, проверки подписи и получения разрешений
		return true;
	}
	
	static std::vector<std::string> get_permissions(const std::string& token) {
		// Декодируем токен и получаем список разрешений (например, из payload)
		// Примерно:
		std::vector<std::string> permissions = {"user:list:read", "course:test:write"};
		return permissions;
	}
};


class User {
private:
	std::string token;
	std::vector<std::string> permissions;
	
public:
	User(const std::string& token) : token(token) {
		if (Authorization::validate_token(token)) {
			permissions = Authorization::get_permissions(token);
		} else {
			std::cerr << "Invalid token!" << std::endl;
		}
	}
	
	bool has_permission(const std::string& permission) {
		return std::find(permissions.begin(), permissions.end(), permission) != permissions.end();
	}
	
	const std::vector<std::string>& get_permissions() const {
		return permissions;
	}
};
	
class UserResource {
private:
	DatabaseConnection& db;
public:
	UserResource(DatabaseConnection& db) : db(db) {}
	
	void list_users(User& user) {
		if (!user.has_permission("user:list:read")) {
			std::cout << "403 Forbidden: Insufficient permissions" << std::endl;
			return;
		}
	
		// Выполнение SQL запроса для получения списка пользователей
		pqxx::work txn(db.create_transaction());
		pqxx::result res = txn.exec("SELECT id, full_name FROM users");
	
		for (const auto& row : res) {
			std::cout << "ID: " << row["id"].as<int>() << ", Name: " << row["full_name"].as<std::string>() << std::endl;
		}
	}
	
	void get_user_info(User& user, int user_id) {
		// Пример проверки разрешений
		if (!(user.has_permission("user:data:read") || user_id == 1)) {  // Только если это свой пользователь
			std::cout << "403 Forbidden: Insufficient permissions" << std::endl;
			return;
		}
	
		// Выполнение запроса
		pqxx::work txn(db.create_transaction());
		pqxx::result res = txn.exec_params("SELECT full_name, courses FROM users WHERE id = $1", user_id);
	
		if (res.empty()) {
			std::cout << "User not found" << std::endl;
			return;
		}
	
		// Пример вывода информации о пользователе
		std::cout << "Full Name: " << res[0]["full_name"].as<std::string>() << std::endl;
	}
};
	
	

int main(void)
{
	int main() {
		// Создание подключения к базе данных PostgreSQL
		DatabaseConnection db("dbname=postgres user=postgres password=1111");
	
		// Пример токена
		std::string token = "your-jwt-token-here";
		User user(token);
	
		// Создание ресурсов
		UserResource user_resource(db);
	
		// Обработка запроса на просмотр списка пользователей
		user_resource.list_users(user);
	
		// Получение информации о пользователе (например, с ID = 1)
		user_resource.get_user_info(user, 1);
	
		return 0;
	}
	
}