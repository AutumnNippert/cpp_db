#include <iostream>
#include "methods.hpp"

using namespace cpp_db;
using namespace db_ops;

int main(int argc, char const *argv[])
{
    table t;
    t.name = "USERS";
    t.schema = {{"name", data_type::STRING}, {"age", data_type::INT}, {"is_student", data_type::BOOL}};
    
    t.add_row({"John Doe", 25, true});
    t.add_row({"Jane Doe", 22, false});
    t.add_row({"Alice", 30, true});

    std::cout << t << std::endl << std::endl;

    table t2 = select(t, "name", "John Doe");
    t2 = project(t2, {"name", "age"});
    std::cout << t2 << std::endl;

    table t3 = cross_join(t, t2);
    std::cout << t3 << std::endl;
    
    return 0;
}
