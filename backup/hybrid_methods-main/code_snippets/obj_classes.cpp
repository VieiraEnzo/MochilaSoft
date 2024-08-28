#include<iostream>
using namespace std; 

// Note the use of the keyword public in the program. This means the members are public and can be accessed anywhere from the program.
// The private members of a class can only be accessed from within the class. 

class Room{

    private:
        double length; 
        double breadth; 
        double height; 

    public:

        void initData(double len, double brth, double hgt){
            length = len; 
            breadth = brth; 
            height = hgt; 
        }

        double calculateArea(){
            return length * breadth; 
        }

        double calculateVolume(){
            return length * breadth * height; 
        }
}; 


int main(){
    Room room1; 

    // room1.length = 5.5; 
    // room1.breadth = 2.3; 
    // room1.height = 1.5; 

    room1.initData(3.5, 2.8, 1.2); 

    cout << "Room Area: " << room1.calculateArea() << endl; 
    cout << "Room Volume: " << room1.calculateVolume() << endl; 

    return 0; 
}