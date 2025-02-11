#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <variant>

namespace cpp_db{

    using c_types = std::variant<int, float, std::string, bool>;
    std::ostream& operator<<(std::ostream& os, const c_types& v){
        std::visit([&os](const auto& v){
            os << v;
        }, v);
        return os;
    }


    enum class data_type{
        INT,
        FLOAT,
        STRING,
        BOOL,
    };

    static std::string to_string(data_type dt){
        switch(dt){
            case data_type::INT:
                return "INT";
            case data_type::FLOAT:
                return "FLOAT";
            case data_type::STRING:
                return "STRING";
            case data_type::BOOL:
                return "BOOL";
        }
    }

    static data_type from_string(std::string s){
        if(s == "INT"){
            return data_type::INT;
        }else if(s == "FLOAT"){
            return data_type::FLOAT;
        }else if(s == "STRING"){
            return data_type::STRING;
        }else if(s == "BOOL"){
            return data_type::BOOL;
        }
    }

    struct row{
        std::vector<c_types> data;

        void add(std::vector<c_types> values){
            data = values;
        }

        friend std::ostream& operator<<(std::ostream& os, const row& r){
            for(auto d : r.data){
                os << d << ", ";
            }
            return os;
        }
    };

    struct table{
        std::string name;
        std::vector<std::pair<std::string, data_type>> schema;
        std::unordered_map<std::string, size_t> schema_map; // for ~O(1) lookup from column name to index

        // rows
        std::vector<row> rows;

        void add_row(row r){
            rows.push_back(r);
        }

        void add_row(std::vector<c_types> values){
            row r;
            int i = 0;
            for(auto c : schema){
                r.data.push_back(values[i]);
                i++;
            }
            rows.push_back(r);
        }

        void remove_row(int index){
            rows.erase(rows.begin() + index);
        }

        // override <<
        friend std::ostream& operator<<(std::ostream& os, const table& t){
            os << t.name << std::endl;
            for(auto c : t.schema){
                os << c.first << " : " << to_string(c.second) << ", ";
            }
            os << std::endl;

            for(auto r : t.rows){
                os << r << std::endl;
            }
            return os;
        }

        table select(std::string column, c_types value){
            table t2;
            t2.name = this->name;
            t2.schema = this->schema;
            t2.schema_map = this->schema_map;

            for(auto r : this->rows){
                for(int i = 0; i < r.data.size(); i++){
                    if(this->schema[i].first == column && r.data[i] == value){
                        t2.add_row(r);
                    }
                }
            }

            return t2;
        }

        table project(std::vector<std::string> columns){
            table t2;
            t2.name = this->name;
            t2.schema_map = this->schema_map;

            for(auto c : columns){
                for(int i = 0; i < this->schema.size(); i++){
                    if(this->schema[i].first == c){
                        t2.schema.push_back(this->schema[i]);
                    }
                }
            }

            for(auto r : this->rows){
                row r2;
                for(int i = 0; i < r.data.size(); i++){
                    for(auto c : columns){
                        if(this->schema[i].first == c){
                            r2.data.push_back(r.data[i]);
                        }
                    }
                }
                t2.add_row(r2);
            }

            return t2;
        }

        table cross(table t){
            table t_out;
            t_out.name = this->name + " x " + t.name;

            for(auto c : this->schema){
                t_out.schema.push_back(c);
            }

            for(auto c : t.schema){
                t_out.schema.push_back(c);
            }

            for(auto r1 : this->rows){
                for(auto r2 : t.rows){
                    row r3;
                    for(auto d : r1.data){
                        r3.data.push_back(d);
                    }
                    for(auto d : r2.data){
                        r3.data.push_back(d);
                    }
                    t_out.add_row(r3);
                }
            }

            return t_out;
        }
    };

    struct database{
        std::string name;
        std::vector<table> tables;

        void add_table(table t){
            tables.push_back(t);
        }

        void remove_table(std::string name){
            for(auto it = tables.begin(); it != tables.end(); it++){
                if(it->name == name){
                    tables.erase(it);
                    break;
                }
            }
        }

        table get_table(std::string name){
            for(auto t : tables){
                if(t.name == name){
                    return t;
                }
            }
        }

        void print_tables(){
            for(auto t : tables){
                std::cout << t << std::endl;
            }
        }
    };

    table rename(table t, std::string old_name, std::string new_name){
        table t2;
        t2.name = t.name;
        t2.schema = t.schema;
        t2.schema_map = t.schema_map;

        for(int i = 0; i < t.schema.size(); i++){
            if(t.schema[i].first == old_name){
                t2.schema[i].first = new_name;
            }
        }

        for(auto r : t.rows){
            t2.add_row(r);
        }

        return t2;
    }
}