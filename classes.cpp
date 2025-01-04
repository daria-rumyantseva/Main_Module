#include <iostream>
#include <string>

class Users {
public:
    int iser_id;
    std::string full_name;
    bool is_blocked = false;
};

class Courses {
public:
    int course_id;
    std::string name;
    std::string description;
    int teacher_id;
};

class Tests {
    int test_id;
    int course_id;
    std::string title;
    bool is_active = true;
};

class Grades {
    int grade_id;
    int user_id;
    int course_id;
    float grade;
};

