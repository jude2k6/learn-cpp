//
// Created by jude on 5/31/26.
#import <iostream>

struct Tracked
{
    int i {};

    ~Tracked(){
        std::cout<<i <<"Estrotyed\n";
    }
};

int main(int argc, char* argv[])
{

    Tracked a{1};
    Tracked b{2};

    {
        Tracked c{3};
    }

    Tracked d{4};

}
