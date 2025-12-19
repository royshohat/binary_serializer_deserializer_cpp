#include "../include/Serializer.hpp"


struct player{
    char id;
    int number;

    void serialize(Serializer& s){
        s.write(id);
        s.write(number);
    }
};

int main(){
    player play{'a', 123};    

    Serializer seri;

    seri.write(play);
    
}

