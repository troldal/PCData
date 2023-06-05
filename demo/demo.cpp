//
// Created by Kenneth Balslev on 03/06/2023.
//

#include <iostream>
#include <algorithm>

#include <PCData.hpp>

int main()
{
    PCData db("./pcd.db");

    std::cout << db.find("DiButyl Carbonate") << std::endl;
//    db.erase("DiButyl Carbonate");
//        std::cout << db.find("DiButyl Carbonate") << std::endl;

//    auto names = db.names();
//
//    for (const auto& name : names) {
//        std::cout << name << std::endl;
//    }

    return 0;
}