#include "../include/serialization.hpp"


struct player{
    char id;
    int number;

    void serialize(Serialization::Serializer& s) const {
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
    int* ptr;
    size_t count;

    Serialization::Deserializer dese;
    dese.load_from_file("data.bin");
    dese.read(player1);
    dese.read_ptr(ptr, count);

    std::cout << "id: " << player1.id << ", number: " << player1.number << std::endl;

    player1.number = 123;
    player1.id = 'b';

    delete[] ptr; // dont forget to delete the readen data
    // this cause .read() actually mallocs (new T[]) for you  

    Serialization::Serializer seri;

    seri.write(player1);


    ptr = new int[5]{104,101,108,108,111};

    seri.write_Ptr(ptr, count);

    delete[] ptr;


    std::string str = "hello";
    seri.write_Ptr(str.data(), str.length());


    seri.save_to_file("data.bin");
    // if u save only player:
    // with a quick ls -l data.bin you can see that the size is 5 bytes
    // even tho if you know check the sizeof(player) it will say 8
    // this weird behavior is caused by padding between the char and the int
    // i am not storing this garbage data of padding using the serialize contracted method

    
}

