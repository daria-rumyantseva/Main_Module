-- Таблица пользователей
CREATE TABLE Users (
    user_id SERIAL PRIMARY KEY,
    full_name VARCHAR(255) NOT NULL,
    is_blocked BOOLEAN DEFAULT FALSE
);

-- Таблица дисциплин
CREATE TABLE Courses (
    course_id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    description TEXT,
    teacher_id INT REFERENCES Users(user_id)
);

-- Таблица тестов
CREATE TABLE Tests (
    test_id SERIAL PRIMARY KEY,
    course_id INT REFERENCES Courses(course_id),
    title VARCHAR(255) NOT NULL,
    is_active BOOLEAN DEFAULT TRUE
);

-- Таблица оценок
CREATE TABLE Grades (
    grade_id SERIAL PRIMARY KEY,
    user_id INT REFERENCES Users(user_id),
    course_id INT REFERENCES Courses(course_id),
    grade DECIMAL(5, 2)
);

-- Таблица ролей
CREATE TABLE Roles (
    role_id SERIAL PRIMARY KEY,
    role_name VARCHAR(50) NOT NULL UNIQUE
);

-- Таблица разрешений
CREATE TABLE Permissions (
    permission_id SERIAL PRIMARY KEY,
    permission_name VARCHAR(255) NOT NULL UNIQUE
);

-- Таблица ролей и разрешений (связующая таблица)
CREATE TABLE Role_Permissions (
    role_id INT REFERENCES Roles(role_id),
    permission_id INT REFERENCES Permissions(permission_id),
    PRIMARY KEY (role_id, permission_id)
);

-- Таблица пользователей и ролей (связующая таблица)
CREATE TABLE User_Roles (
    user_id INT REFERENCES Users(user_id),
    role_id INT REFERENCES Roles(role_id),
    PRIMARY KEY (user_id, role_id)
);
