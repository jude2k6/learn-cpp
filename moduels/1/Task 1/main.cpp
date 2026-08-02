//
// Created by jude on 5/31/26.
//
#include <iostream>

struct BigStruct
{
    long l1{1};
    long l2{11};
    long l3 {111};
};
void byValue(BigStruct b )
{
    std::cout<< "by val"<<&b<<std::endl;
}
void byRef(BigStruct& b )
{
    std::cout<< "by ref"<<&b<<std::endl;

}
void byConstRef(const BigStruct& b)
{
    std::cout<< "by const ref"<<&b<<std::endl;

}



int main(int argc, char* argv[])
{
    BigStruct b;
    byValue(b);
    byRef(b);
    byConstRef(b);
}
