//
// Created by jude on 5/31/26.
//

#include <iostream>
#include <string>
#include <string_view>

std::string_view test()
{
    std::string s  = "hello";
    std::string_view sv = s.substr(1,3);
    return sv;
}


int main(int argc, char* argv[])
{
    std::string_view sv = test();
    std::cout<<sv;

}

