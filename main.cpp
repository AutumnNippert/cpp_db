#include <iostream>
#include "cpp_db.hpp"

using namespace cpp_db;

int main(int argc, char const *argv[])
{
    table t;
    t.name = "USERS";
    t.schema = {{"name", data_type::STRING}, {"age", data_type::INT}, {"is_student", data_type::BOOL}};
    
    t.add_row({"John Doe", 25, true});
    t.add_row({"Jane Doe", 22, false});
    t.add_row({"Alice", 30, true});

    std::cout << t << std::endl << std::endl;

    table t2 = t.select("name", "John Doe").project({"name", "age"});
    std::cout << t2 << std::endl;

    table t3 = t.cross(t2);
    std::cout << t3 << std::endl;
    
    return 0;
}
