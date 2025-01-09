#pragma once

#include <regex>
#include <iostream>
#include <string>


// Функция для проверки на совпадение с запросом
inline bool checkRequest(const std::string& request, const std::string& pattern) {
	std::regex re(pattern);
	return std::regex_match(request, re);
}

// Функция для выполнения SQL запроса
inline void executeSQLQuery(const std::string& query) {

}

// Функция для выполнения запроса
bool executionRequest(const const std::string& query, const std::string& role, const int& id) {
    // Роль студент
    if (role == "student") {
        if (checkRequest(query, R"(users:list:read)")) {
            return false;
        }
        if (checkRequest(query, R"(users:fullname:read:me)")) {
            std::string sql = "SELECT fullname FROM users WHERE id = " + id; // Пример для текущего пользователя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:fullname:read:other)")) {
            return false;
        }
        if (checkRequest(query, R"(users:fullname:write:me)")) {
            std::string sql = "UPDATE users SET fullname = 'New Name' WHERE id = " + id; // Пример для себя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:fullname:write:other)")) {
            return false;
        }
        if (checkRequest(query, R"(users:data:read:me)")) {
            std::string sql = "SELECT courses, grades, tests FROM users WHERE id = " + id; // Пример данных
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:data:read:other id_user=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(users:roles:read:me)")) {
            std::string sql = "SELECT * FROM roles WHERE user_id = " + id; // Пример запроса для ролей
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:roles:read:other id_user=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(users:roles:write:me)")) {
            return false;
        }
        if (checkRequest(query, R"(users:roles:write:other id_user=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(users:block:read:me)")) {
            std::string sql = "SELECT blocked FROM users WHERE id = " + id; // Пример для получения статуса блокировки
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:block:read:other id_user=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(users:block:write:me)")) {
            return false;
        }
        if (checkRequest(query, R"(users:block:write:other id_user=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(users:unblock:write:me)")) {
            return false;
        }
        if (checkRequest(query, R"(users:unblock:write:other id_user=\d+)")) {
            return false;
        }
    
        // Проверка запросов для курсов
        if (checkRequest(query, R"(course:list:read)")) {
            std::string sql = "SELECT * FROM courses"; // Пример запроса для получения списка курсов
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(course:data:read id_course=\d+)")) {
            std::regex re(R"(course:data:read id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "SELECT * FROM courses WHERE id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:name:write id_course=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(course:info:write id_course=\d+)")) {
            return false;
        }
    
        if (checkRequest(query, R"(course:testList:read:me id_course=\d+)")) {
            return false;
        }
    
        if (checkRequest(query, R"(course:testList:read:other id_course=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(course:test:read:me id_test=\d+)")) {
            std::regex re(R"(course:test:read:me id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM tests WHERE id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:read:other id_test=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(course:test:activate id_test=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(course:test:deactivate id_test=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(course:test:add id_course=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(course:test:del id_course=\d+)")) {
            return false;
        }
    
        // Проверка запросов для пользователей в курсе (course:user)
        if (checkRequest(query, R"(course:userList:read)")) {
            return false;
        }
        if (checkRequest(query, R"(course:user:add id_user=\d+ id_course=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(course:user:delete id=\d+ id_course=\d+)")) {
            return false;
        }
    
        // Добавление курса (course:add)
        if (checkRequest(query, R"(course:add)")) {
            return false;
        }
    
        // Удаление курса (course:del)
        if (checkRequest(query, R"(course:del id_course=\d+)")) {
            return false;
        }
    
        // Проверка запросов для вопросов в тестах (test:quest)
        if (checkRequest(query, R"(test:quest:del id_test=\d+ id_quest=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(test:quest:add id_test=\d+ id_quest=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(test:quest:update id_test=\d+)")) {
            return false;
        }
    
        // Ответы на тесты (test:answer)
        if (checkRequest(query, R"(test:answer:read id_test=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(test:answer:read:me id_test=\d+)")) {
            std::regex re(R"(test:answer:read:me id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id + " AND user_id = 1";
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:answer:read:other id_user=\d+ id_test=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(test:answer:create id_test=\d+)")) {
            return false;
        }
        
        if (checkRequest(query, R"(test:answer:update id_test=\d+)")) {
            return false;
        }
        
        if (checkRequest(query, R"(test:answer:del id_test=\d+)")) {
            return false;
        }
        
        if (checkRequest(query, R"(test:answer:read id_test=\d+)")) {
            std::regex re(R"(test:answer:read id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Читаем все ответы для данного теста
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:list:read:me id_test=\d+)")) {
            std::regex re(R"(quest:list:read:me id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Читаем все вопросы для теста
                std::string sql = "SELECT * FROM questions WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:list:read:other id_user=\d+ id_test=\d+)")) {
            return false;
        }
        
        if (checkRequest(query, R"(quest:read id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(quest:read id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Читаем конкретный вопрос по тесту и ответу
                std::string sql = "SELECT * FROM questions WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:update id_test=\d+ id_answer=\d+)")) {
            return false;
        }
        
        if (checkRequest(query, R"(quest:create id_test=\d+)")) {
            std::regex re(R"(quest:create id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Создаем новый вопрос для теста
                std::string sql = "INSERT INTO questions (test_id, question_text) VALUES (" + test_id + ", 'New Question')";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:del id_test=\d+)")) {
            std::regex re(R"(quest:del id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Удаляем все вопросы для данного теста
                std::string sql = "DELETE FROM questions WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:create id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:create id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Создаем новый ответ для теста
                std::string sql = "INSERT INTO answers (test_id, answer_id, answer_text) VALUES (" + test_id + ", " + answer_id + ", 'New Answer')";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:read id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:read id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Читаем ответ по test_id и answer_id
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:update id_test=\d+ id_answer=\d+)")) {
            return false;
        }
        
        if (checkRequest(query, R"(answer:del id_test=\d+ id_answer=\d+)")) {
            return false;
        }
    }




    // Роль преподаватель
    if (role == "teacher") {
        if (checkRequest(query, R"(users:list:read)")) {
            std::string sql = "SELECT * FROM users"; // Пример запроса для получения списка пользователей
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:fullname:read:me)")) {
            std::string sql = "SELECT fullname FROM users WHERE id = " + id; // Пример для текущего пользователя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:fullname:read:other)")) {
            std::regex re(R"(users:fullname:read:other id_user=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string sql = "SELECT fullname FROM users WHERE id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(users:fullname:write:me)")) {
            std::string sql = "UPDATE users SET fullname = 'New Name' WHERE id = " + id; // Пример для себя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:fullname:write:other)")) {
            return false;
        }
        if (checkRequest(query, R"(users:data:read:me)")) {
            std::string sql = "SELECT courses, grades, tests FROM users WHERE id = " + id; // Пример данных
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:data:read:other id_user=\d+)")) {
             // Извлечение ID пользователя из строки запроса
             std::regex re(R"(users:data:read:other id_user=(\d+))");
             std::smatch match;
             if (std::regex_search(query, match, re)) {
                 std::string user_id = match[1];
                 std::string sql = "SELECT courses, grades, tests FROM users WHERE id = " + user_id;
                 executeSQLQuery(sql);
             }
             return true;
        }
        if (checkRequest(query, R"(users:roles:read:me)")) {
            std::string sql = "SELECT * FROM roles WHERE user_id = " + id; // Пример запроса для ролей
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:roles:read:other id_user=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(users:roles:write:me)")) {
            std::string sql = "UPDATE roles SET role = 'Admin' WHERE user_id = " + id; // Пример для текущего пользователя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:roles:write:other id_user=\d+)")) {
            std::regex re(R"(users:roles:write:other id_user=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string sql = "UPDATE roles SET role = 'Admin' WHERE user_id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(users:block:read:me)")) {
            std::string sql = "SELECT blocked FROM users WHERE id = " + id; // Пример для получения статуса блокировки
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:block:read:other id_user=\d+)")) {
            std::regex re(R"(users:block:read:other id_user=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string sql = "SELECT blocked FROM users WHERE id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(users:block:write:me)")) {
            return false;
        }
        if (checkRequest(query, R"(users:block:write:other id_user=\d+)")) {
            return false;
        }
        if (checkRequest(query, R"(users:unblock:write:me)")) {
            return false;
        }
        if (checkRequest(query, R"(users:unblock:write:other id_user=\d+)")) {
            return false;
        }
    
        // Проверка запросов для курсов
        if (checkRequest(query, R"(course:list:read)")) {
            std::string sql = "SELECT * FROM courses"; // Пример запроса для получения списка курсов
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(course:data:read id_course=\d+)")) {
            std::regex re(R"(course:data:read id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "SELECT * FROM courses WHERE id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:name:write id_course=\d+)")) {
            std::regex re(R"(course:name:write id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "UPDATE courses SET name = 'New Course Name' WHERE id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:info:write id_course=\d+)")) {
            std::regex re(R"(course:info:write id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "UPDATE courses SET description = 'New Description' WHERE id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        if (checkRequest(query, R"(course:testList:read:me id_course=\d+)")) {
            std::regex re(R"(course:testList:read:me id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "SELECT * FROM tests WHERE course_id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        if (checkRequest(query, R"(course:testList:read:other id_course=\d+)")) {
            std::regex re(R"(course:testList:read:other id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "SELECT * FROM tests WHERE course_id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:read:me id_test=\d+)")) {
            std::regex re(R"(course:test:read:me id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM tests WHERE id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:read:other id_test=\d+)")) {
            std::regex re(R"(course:test:read:other id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM tests WHERE id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:activate id_test=\d+)")) {
            std::regex re(R"(course:test:activate id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "UPDATE tests SET status = 'active' WHERE id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:deactivate id_test=\d+)")) {
            std::regex re(R"(course:test:deactivate id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "UPDATE tests SET status = 'inactive' WHERE id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:add id_course=\d+)")) {
            std::regex re(R"(course:test:add id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "INSERT INTO tests (course_id, name, description) VALUES (" + course_id + ", 'New Test', 'Test description')";
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:del id_course=\d+)")) {
            std::regex re(R"(course:test:del id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "DELETE FROM tests WHERE course_id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        // Проверка запросов для пользователей в курсе (course:user)
        if (checkRequest(query, R"(course:userList:read)")) {
            std::string sql = "SELECT u.id, u.fullname FROM users u JOIN course_user cu ON u.id = cu.user_id";
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(course:user:add id_user=\d+ id_course=\d+)")) {
            std::regex re(R"(course:user:add id_user=(\d+) id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string course_id = match[2];
                std::string sql = "INSERT INTO course_user (user_id, course_id) VALUES (" + user_id + ", " + course_id + ")";
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:user:delete id=\d+ id_course=\d+)")) {
            std::regex re(R"(course:user:delete id=(\d+) id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string course_id = match[2];
                std::string sql = "DELETE FROM course_user WHERE user_id = " + user_id + " AND course_id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        // Добавление курса (course:add)
        if (checkRequest(query, R"(course:add)")) {
            std::string sql = "INSERT INTO courses (name, description) VALUES ('New Course', 'Course description')";
            executeSQLQuery(sql);
            return true;
        }
    
        // Удаление курса (course:del)
        if (checkRequest(query, R"(course:del id_course=\d+)")) {
            std::regex re(R"(course:del id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "DELETE FROM courses WHERE id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        // Проверка запросов для вопросов в тестах (test:quest)
        if (checkRequest(query, R"(test:quest:del id_test=\d+ id_quest=\d+)")) {
            std::regex re(R"(test:quest:del id_test=(\d+) id_quest=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string quest_id = match[2];
                std::string sql = "DELETE FROM questions WHERE id = " + quest_id + " AND test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:quest:add id_test=\d+ id_quest=\d+)")) {
            std::regex re(R"(test:quest:add id_test=(\d+) id_quest=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string quest_id = match[2];
                std::string sql = "INSERT INTO questions (test_id, question_text) VALUES (" + test_id + ", 'New Question')";
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:quest:update id_test=\d+)")) {
            std::regex re(R"(test:quest:update id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "UPDATE questions SET question_text = 'Updated Question Text' WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        // Ответы на тесты (test:answer)
        if (checkRequest(query, R"(test:answer:read id_test=\d+)")) {
            std::regex re(R"(test:answer:read id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:answer:read:me id_test=\d+)")) {
            std::regex re(R"(test:answer:read:me id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id + " AND user_id = 1";
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:answer:read:other id_user=\d+ id_test=\d+)")) {
            std::regex re(R"(test:answer:read:other id_user=(\d+) id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string test_id = match[2];
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id + " AND user_id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:answer:create id_test=\d+)")) {
            std::regex re(R"(test:answer:create id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Здесь, скорее всего, потребуются параметры для нового ответа (например, user_id, текст ответа)
                std::string sql = "INSERT INTO answers (test_id, user_id, answer_text) VALUES (" + test_id + ", 1, 'new answer')";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(test:answer:update id_test=\d+)")) {
            std::regex re(R"(test:answer:update id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Пример обновления: обновляем ответ по тесту и user_id
                std::string sql = "UPDATE answers SET answer_text = 'updated answer' WHERE test_id = " + test_id + " AND user_id = 1";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(test:answer:del id_test=\d+)")) {
            std::regex re(R"(test:answer:del id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Удаляем все ответы для данного теста
                std::string sql = "DELETE FROM answers WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(test:answer:read id_test=\d+)")) {
            std::regex re(R"(test:answer:read id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Читаем все ответы для данного теста
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:list:read:me id_test=\d+)")) {
            std::regex re(R"(quest:list:read:me id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Читаем все вопросы для теста
                std::string sql = "SELECT * FROM questions WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:list:read:other id_user=\d+ id_test=\d+)")) {
            std::regex re(R"(quest:list:read:other id_user=(\d+) id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string test_id = match[2];
                // Читаем все вопросы для теста для другого пользователя
                std::string sql = "SELECT * FROM questions WHERE test_id = " + test_id + " AND user_id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:read id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(quest:read id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Читаем конкретный вопрос по тесту и ответу
                std::string sql = "SELECT * FROM questions WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:update id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(quest:update id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Обновляем вопрос с определенным id_answer
                std::string sql = "UPDATE questions SET question_text = 'Updated Question' WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:create id_test=\d+)")) {
            std::regex re(R"(quest:create id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Создаем новый вопрос для теста
                std::string sql = "INSERT INTO questions (test_id, question_text) VALUES (" + test_id + ", 'New Question')";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:del id_test=\d+)")) {
            std::regex re(R"(quest:del id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Удаляем все вопросы для данного теста
                std::string sql = "DELETE FROM questions WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:create id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:create id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Создаем новый ответ для теста
                std::string sql = "INSERT INTO answers (test_id, answer_id, answer_text) VALUES (" + test_id + ", " + answer_id + ", 'New Answer')";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:read id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:read id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Читаем ответ по test_id и answer_id
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:update id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:update id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Обновляем ответ по test_id и answer_id
                std::string sql = "UPDATE answers SET answer_text = 'Updated Answer' WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:del id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:del id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Удаляем ответ по test_id и answer_id
                std::string sql = "DELETE FROM answers WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    }








    // Роль администратор
    if (role == "admin") {
        if (checkRequest(query, R"(users:list:read)")) {
            std::string sql = "SELECT * FROM users"; // Пример запроса для получения списка пользователей
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:fullname:read:me)")) {
            std::string sql = "SELECT fullname FROM users WHERE id = " + id; // Пример для текущего пользователя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:fullname:read:other)")) {
            std::regex re(R"(users:fullname:read:other id_user=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string sql = "SELECT fullname FROM users WHERE id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(users:fullname:write:me)")) {
            std::string sql = "UPDATE users SET fullname = 'New Name' WHERE id = " + id; // Пример для себя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:fullname:write:other)")) {
             // Извлечение ID пользователя из строки запроса
             std::regex re(R"(users:fullname:write:other id_user=(\d+))");
             std::smatch match;
             if (std::regex_search(query, match, re)) {
                 std::string user_id = match[1];
                 std::string sql = "UPDATE users SET fullname = 'New Name' WHERE id = " + user_id;
                 executeSQLQuery(sql);
             }
             return true;
        }
        if (checkRequest(query, R"(users:data:read:me)")) {
            std::string sql = "SELECT courses, grades, tests FROM users WHERE id = " + id; // Пример данных
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:data:read:other id_user=\d+)")) {
             // Извлечение ID пользователя из строки запроса
             std::regex re(R"(users:data:read:other id_user=(\d+))");
             std::smatch match;
             if (std::regex_search(query, match, re)) {
                 std::string user_id = match[1];
                 std::string sql = "SELECT courses, grades, tests FROM users WHERE id = " + user_id;
                 executeSQLQuery(sql);
             }
             return true;
        }
        if (checkRequest(query, R"(users:roles:read:me)")) {
            std::string sql = "SELECT * FROM roles WHERE user_id = " + id; // Пример запроса для ролей
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:roles:read:other id_user=\d+)")) {
            std::regex re(R"(users:roles:read:other id_user=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string sql = "SELECT * FROM roles WHERE user_id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(users:roles:write:me)")) {
            std::string sql = "UPDATE roles SET role = 'Admin' WHERE user_id = " + id; // Пример для текущего пользователя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:roles:write:other id_user=\d+)")) {
            std::regex re(R"(users:roles:write:other id_user=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string sql = "UPDATE roles SET role = 'Admin' WHERE user_id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(users:block:read:me)")) {
            std::string sql = "SELECT blocked FROM users WHERE id = " + id; // Пример для получения статуса блокировки
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:block:read:other id_user=\d+)")) {
            std::regex re(R"(users:block:read:other id_user=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string sql = "SELECT blocked FROM users WHERE id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(users:block:write:me)")) {
            std::string sql = "UPDATE users SET blocked = true WHERE id = " + id; // Пример для блокировки пользователя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:block:write:other id_user=\d+)")) {
            std::regex re(R"(users:block:write:other id_user=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string sql = "UPDATE users SET blocked = true WHERE id = " + user_id;
                executeSQLQuery(sql);
                return true;
            }
        }
        if (checkRequest(query, R"(users:unblock:write:me)")) {
            std::string sql = "UPDATE users SET blocked = false WHERE id = " + id; // Пример для разблокировки пользователя
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(users:unblock:write:other id_user=\d+)")) {
            std::regex re(R"(users:unblock:write:other id_user=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string sql = "UPDATE users SET blocked = false WHERE id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        // Проверка запросов для курсов
        if (checkRequest(query, R"(course:list:read)")) {
            std::string sql = "SELECT * FROM courses"; // Пример запроса для получения списка курсов
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(course:data:read id_course=\d+)")) {
            std::regex re(R"(course:data:read id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "SELECT * FROM courses WHERE id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:name:write id_course=\d+)")) {
            std::regex re(R"(course:name:write id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "UPDATE courses SET name = 'New Course Name' WHERE id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:info:write id_course=\d+)")) {
            std::regex re(R"(course:info:write id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "UPDATE courses SET description = 'New Description' WHERE id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        if (checkRequest(query, R"(course:testList:read:me id_course=\d+)")) {
            std::regex re(R"(course:testList:read:me id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "SELECT * FROM tests WHERE course_id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        if (checkRequest(query, R"(course:testList:read:other id_course=\d+)")) {
            std::regex re(R"(course:testList:read:other id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "SELECT * FROM tests WHERE course_id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:read:me id_test=\d+)")) {
            std::regex re(R"(course:test:read:me id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM tests WHERE id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:read:other id_test=\d+)")) {
            std::regex re(R"(course:test:read:other id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM tests WHERE id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:activate id_test=\d+)")) {
            std::regex re(R"(course:test:activate id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "UPDATE tests SET status = 'active' WHERE id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:deactivate id_test=\d+)")) {
            std::regex re(R"(course:test:deactivate id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "UPDATE tests SET status = 'inactive' WHERE id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:add id_course=\d+)")) {
            std::regex re(R"(course:test:add id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "INSERT INTO tests (course_id, name, description) VALUES (" + course_id + ", 'New Test', 'Test description')";
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:test:del id_course=\d+)")) {
            std::regex re(R"(course:test:del id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "DELETE FROM tests WHERE course_id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        // Проверка запросов для пользователей в курсе (course:user)
        if (checkRequest(query, R"(course:userList:read)")) {
            std::string sql = "SELECT u.id, u.fullname FROM users u JOIN course_user cu ON u.id = cu.user_id";
            executeSQLQuery(sql);
            return true;
        }
        if (checkRequest(query, R"(course:user:add id_user=\d+ id_course=\d+)")) {
            std::regex re(R"(course:user:add id_user=(\d+) id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string course_id = match[2];
                std::string sql = "INSERT INTO course_user (user_id, course_id) VALUES (" + user_id + ", " + course_id + ")";
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(course:user:delete id=\d+ id_course=\d+)")) {
            std::regex re(R"(course:user:delete id=(\d+) id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string course_id = match[2];
                std::string sql = "DELETE FROM course_user WHERE user_id = " + user_id + " AND course_id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        // Добавление курса (course:add)
        if (checkRequest(query, R"(course:add)")) {
            std::string sql = "INSERT INTO courses (name, description) VALUES ('New Course', 'Course description')";
            executeSQLQuery(sql);
            return true;
        }
    
        // Удаление курса (course:del)
        if (checkRequest(query, R"(course:del id_course=\d+)")) {
            std::regex re(R"(course:del id_course=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string course_id = match[1];
                std::string sql = "DELETE FROM courses WHERE id = " + course_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        // Проверка запросов для вопросов в тестах (test:quest)
        if (checkRequest(query, R"(test:quest:del id_test=\d+ id_quest=\d+)")) {
            std::regex re(R"(test:quest:del id_test=(\d+) id_quest=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string quest_id = match[2];
                std::string sql = "DELETE FROM questions WHERE id = " + quest_id + " AND test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:quest:add id_test=\d+ id_quest=\d+)")) {
            std::regex re(R"(test:quest:add id_test=(\d+) id_quest=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string quest_id = match[2];
                std::string sql = "INSERT INTO questions (test_id, question_text) VALUES (" + test_id + ", 'New Question')";
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:quest:update id_test=\d+)")) {
            std::regex re(R"(test:quest:update id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "UPDATE questions SET question_text = 'Updated Question Text' WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    
        // Ответы на тесты (test:answer)
        if (checkRequest(query, R"(test:answer:read id_test=\d+)")) {
            std::regex re(R"(test:answer:read id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:answer:read:me id_test=\d+)")) {
            std::regex re(R"(test:answer:read:me id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id + " AND user_id = 1";
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:answer:read:other id_user=\d+ id_test=\d+)")) {
            std::regex re(R"(test:answer:read:other id_user=(\d+) id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string test_id = match[2];
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id + " AND user_id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        if (checkRequest(query, R"(test:answer:create id_test=\d+)")) {
            std::regex re(R"(test:answer:create id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Здесь, скорее всего, потребуются параметры для нового ответа (например, user_id, текст ответа)
                std::string sql = "INSERT INTO answers (test_id, user_id, answer_text) VALUES (" + test_id + ", 1, 'new answer')";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(test:answer:update id_test=\d+)")) {
            std::regex re(R"(test:answer:update id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Пример обновления: обновляем ответ по тесту и user_id
                std::string sql = "UPDATE answers SET answer_text = 'updated answer' WHERE test_id = " + test_id + " AND user_id = 1";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(test:answer:del id_test=\d+)")) {
            std::regex re(R"(test:answer:del id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Удаляем все ответы для данного теста
                std::string sql = "DELETE FROM answers WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(test:answer:read id_test=\d+)")) {
            std::regex re(R"(test:answer:read id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Читаем все ответы для данного теста
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:list:read:me id_test=\d+)")) {
            std::regex re(R"(quest:list:read:me id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Читаем все вопросы для теста
                std::string sql = "SELECT * FROM questions WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:list:read:other id_user=\d+ id_test=\d+)")) {
            std::regex re(R"(quest:list:read:other id_user=(\d+) id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string user_id = match[1];
                std::string test_id = match[2];
                // Читаем все вопросы для теста для другого пользователя
                std::string sql = "SELECT * FROM questions WHERE test_id = " + test_id + " AND user_id = " + user_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:read id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(quest:read id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Читаем конкретный вопрос по тесту и ответу
                std::string sql = "SELECT * FROM questions WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:update id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(quest:update id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Обновляем вопрос с определенным id_answer
                std::string sql = "UPDATE questions SET question_text = 'Updated Question' WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:create id_test=\d+)")) {
            std::regex re(R"(quest:create id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Создаем новый вопрос для теста
                std::string sql = "INSERT INTO questions (test_id, question_text) VALUES (" + test_id + ", 'New Question')";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(quest:del id_test=\d+)")) {
            std::regex re(R"(quest:del id_test=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                // Удаляем все вопросы для данного теста
                std::string sql = "DELETE FROM questions WHERE test_id = " + test_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:create id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:create id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Создаем новый ответ для теста
                std::string sql = "INSERT INTO answers (test_id, answer_id, answer_text) VALUES (" + test_id + ", " + answer_id + ", 'New Answer')";
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:read id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:read id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Читаем ответ по test_id и answer_id
                std::string sql = "SELECT * FROM answers WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:update id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:update id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Обновляем ответ по test_id и answer_id
                std::string sql = "UPDATE answers SET answer_text = 'Updated Answer' WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
        
        if (checkRequest(query, R"(answer:del id_test=\d+ id_answer=\d+)")) {
            std::regex re(R"(answer:del id_test=(\d+) id_answer=(\d+))");
            std::smatch match;
            if (std::regex_search(query, match, re)) {
                std::string test_id = match[1];
                std::string answer_id = match[2];
                // Удаляем ответ по test_id и answer_id
                std::string sql = "DELETE FROM answers WHERE test_id = " + test_id + " AND answer_id = " + answer_id;
                executeSQLQuery(sql);
            }
            return true;
        }
    }
}