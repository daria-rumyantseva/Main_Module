-- Пользователи
CREATE TABLE users (
    user_id SERIAL PRIMARY KEY,             -- уникальный идентификатор пользователя
    username VARCHAR(255) UNIQUE NOT NULL,  -- имя пользователя
    first_name VARCHAR(255),               -- имя пользователя
    last_name VARCHAR(255),                -- фамилия пользователя
    email VARCHAR(255) UNIQUE,             -- электронная почта
    password_hash VARCHAR(255),            -- хэш пароля
    roles VARCHAR(255)[],                  -- роли пользователя (можно хранить как массив строк)
    is_blocked BOOLEAN DEFAULT FALSE,      -- заблокирован ли пользователь
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата создания
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP -- дата обновления
);

-- Дисциплины
CREATE TABLE courses (
    course_id SERIAL PRIMARY KEY,           -- уникальный идентификатор дисциплины
    course_name VARCHAR(255) NOT NULL,      -- название дисциплины
    description TEXT,                       -- описание дисциплины
    teacher_id INT,                         -- внешний ключ на преподавателя (user_id)
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата создания
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата обновления
    FOREIGN KEY (teacher_id) REFERENCES users(user_id) ON DELETE SET NULL -- связь с пользователем (преподавателем)
);

-- Запись пользователей на курсы
CREATE TABLE course_users (
    course_user_id SERIAL PRIMARY KEY,      -- уникальный идентификатор записи
    course_id INT NOT NULL,                  -- внешний ключ на дисциплину
    user_id INT NOT NULL,                    -- внешний ключ на пользователя
    enrollment_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата записи на курс
    FOREIGN KEY (course_id) REFERENCES courses(course_id) ON DELETE CASCADE, -- связь с таблицей дисциплин
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE -- связь с таблицей пользователей
);

-- Тесты
CREATE TABLE tests (
    test_id SERIAL PRIMARY KEY,              -- уникальный идентификатор теста
    course_id INT NOT NULL,                  -- внешний ключ на дисциплину
    test_name VARCHAR(255) NOT NULL,         -- название теста
    is_active BOOLEAN DEFAULT TRUE,          -- активен ли тест
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата создания
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата обновления
    FOREIGN KEY (course_id) REFERENCES courses(course_id) ON DELETE CASCADE -- связь с таблицей курсов
);

-- Вопросы
CREATE TABLE questions (
    question_id SERIAL PRIMARY KEY,           -- уникальный идентификатор вопроса
    test_id INT NOT NULL,                     -- внешний ключ на тест
    question_text TEXT NOT NULL,              -- текст вопроса
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата создания
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата обновления
    FOREIGN KEY (test_id) REFERENCES tests(test_id) ON DELETE CASCADE -- связь с таблицей тестов
);

-- Связь тестов и вопросов
CREATE TABLE test_questions (
    test_question_id SERIAL PRIMARY KEY,     -- уникальный идентификатор связи
    test_id INT NOT NULL,                     -- внешний ключ на тест
    question_id INT NOT NULL,                 -- внешний ключ на вопрос
    question_order INT,                       -- порядок следования вопросов в тесте
    FOREIGN KEY (test_id) REFERENCES tests(test_id) ON DELETE CASCADE, -- связь с тестами
    FOREIGN KEY (question_id) REFERENCES questions(question_id) ON DELETE CASCADE -- связь с вопросами
);

-- Ответы
CREATE TABLE answers (
    answer_id SERIAL PRIMARY KEY,            -- уникальный идентификатор ответа
    test_id INT NOT NULL,                    -- внешний ключ на тест
    user_id INT NOT NULL,                    -- внешний ключ на пользователя
    answer_text TEXT,                        -- текст ответа
    is_correct BOOLEAN,                      -- правильный ли ответ
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата создания
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата обновления
    FOREIGN KEY (test_id) REFERENCES tests(test_id) ON DELETE CASCADE, -- связь с тестами
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE -- связь с пользователями
);

-- Попытки
CREATE TABLE attempts (
    attempt_id SERIAL PRIMARY KEY,            -- уникальный идентификатор попытки
    test_id INT NOT NULL,                      -- внешний ключ на тест
    user_id INT NOT NULL,                      -- внешний ключ на пользователя
    attempt_status VARCHAR(50),                -- статус попытки (например, 'in_progress', 'completed')
    start_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- время начала попытки
    end_time TIMESTAMP,                        -- время завершения попытки
    FOREIGN KEY (test_id) REFERENCES tests(test_id) ON DELETE CASCADE, -- связь с тестами
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE -- связь с пользователями
);

-- Роли пользователей
CREATE TABLE roles (
    role_id SERIAL PRIMARY KEY,               -- уникальный идентификатор роли
    role_name VARCHAR(255) NOT NULL,           -- название роли (например, 'student', 'teacher')
    description TEXT                          -- описание роли
);

-- Связь пользователей и ролей
CREATE TABLE user_roles (
    user_role_id SERIAL PRIMARY KEY,           -- уникальный идентификатор
    user_id INT NOT NULL,                      -- внешний ключ на пользователя
    role_id INT NOT NULL,                      -- внешний ключ на роль
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE, -- связь с пользователями
    FOREIGN KEY (role_id) REFERENCES roles(role_id) ON DELETE CASCADE -- связь с ролями
);

-- Блокировки пользователей
CREATE TABLE user_blocks (
    block_id SERIAL PRIMARY KEY,              -- уникальный идентификатор блокировки
    user_id INT NOT NULL,                     -- внешний ключ на пользователя
    blocked_by INT NOT NULL,                  -- внешний ключ на администратора, который заблокировал
    block_reason TEXT,                        -- причина блокировки
    block_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата блокировки
    unblock_date TIMESTAMP,                   -- дата разблокировки (если применимо)
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE, -- связь с пользователями
    FOREIGN KEY (blocked_by) REFERENCES users(user_id) ON DELETE CASCADE -- связь с пользователем-администратором
);

-- Запись пользователей на тесты
CREATE TABLE user_tests (
    user_test_id SERIAL PRIMARY KEY,          -- уникальный идентификатор записи
    user_id INT NOT NULL,                     -- внешний ключ на пользователя
    test_id INT NOT NULL,                     -- внешний ключ на тест
    start_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- дата начала
    end_time TIMESTAMP,                       -- дата завершения
    score INT,                                -- оценка за тест
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE, -- связь с пользователями
    FOREIGN KEY (test_id) REFERENCES tests(test_id) ON DELETE CASCADE -- связь с тестами
);
