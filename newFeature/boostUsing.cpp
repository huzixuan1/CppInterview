https://www.boost.org/doc/libs/1_86_0/

#include <iostream>
#include <string>

#include "boost/pfr.hpp"

// struct some_person {
//     std::string name;
//     unsigned birth_year;
// };

// int main() {
//     some_person val{"Edgar Allan Poe", 1809};

//     std::cout << boost::pfr::get<0>(val)                // No macro!
//         << " was born in " << boost::pfr::get<1>(val);  // Works with any aggregate initializables!

//     std::cout << boost::pfr::io(val);                   // Outputs: {"Edgar Allan Poe", 1809}
//     std::cout<<std::endl;
// }



struct dataBase{
    std::string name;
    double age;
};

int main()
{
    dataBase db{"TaoTao",30};

    std::cout<<boost::pfr::get<0>(db)<<":"<<boost::pfr::get<1>(db)<<std::endl;

    std::cout<<boost::pfr::io(db)<<std::endl;


    // c++ 20获取到结构体成员变量
    constexpr std::string_view n1 = boost::pfr::get_name<0, dataBase>(); // returns "some_integer"
    constexpr std::string_view n2 = boost::pfr::get_name<1, dataBase>(); // returns "c"

    std::cout<<n1<<std::endl;
    std::cout<<n2<<std::endl;
    

    return 0;
}
