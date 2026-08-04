//
// Created by jude2k6 on 8/2/26.
//
#include <iostream>
#include <print>
#include <stdio.h>

class ScopedFile {

    FILE* file;

public:
    ScopedFile(std::string s) {
        std::print("Scoped file constructor\n");
        const char* path = s.c_str();
        file = fopen(path, "w+");
    }

    ~ScopedFile() {
        std::print("Scoped file destructor\n");
        fclose(file);
    }


};


int main(int argc, char* argv[]) {
    std::string s("/home/jude2k6/learn-cpp/moduels/2/scoped_file/test.txt");
    try{ScopedFile f(s);
        throw std::exception();
        std::print("test");}
    catch (std::exception e ){}
}