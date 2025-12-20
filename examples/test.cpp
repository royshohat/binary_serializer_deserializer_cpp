#include "../include/serialization.hpp"


struct player{
    char id;
    int number;

    void serialize(Serialization::Serializer& s){
        s.write(id);
        s.write(number);
    }
    void deserialize(Serialization::Deserializer& s){
        s.read(id);
        s.read(number);
    }
};

int main(){

    player player1;    

    Serialization::Deserializer dese;
    dese.load_from_file("data.bin");
    dese.read(player1);

    std::cout << "id: " << player1.id << ", number: " << player1.number << std::endl;
    player1.id = 'b';

    Serialization::Serializer seri;

    seri.write(player1);

    seri.save_to_file("data.bin");
    // with a quick ls -l data.bin you can see that the size is 5 bytes
    // even tho if you know check the sizeof(player) it will say 8
    // this weird behavior is caused by padding between the char and the int
    // i am not storing this garbage data of padding using the serialize contracted method

    
}

