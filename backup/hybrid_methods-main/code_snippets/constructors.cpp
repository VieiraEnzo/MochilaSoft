#include<iostream>
using namespace std; 

class Wall{
    private:
        double length; 
        double height; 

    public:
        Wall(double len, double hgt){
            length = len;
            height = hgt;  
            cout << "Creating a wall..." << endl;
        }

        // copy constructor with a Wall object as parameter
        // copies data of the obj parameter
        // Wall(Wall &obj) {
        //     length = obj.length;
        //     height = obj.height;
        // }


        double calculateArea(){
            return length * height; 
        }
}; 

int main(){
    Wall wall1(10.5, 8.6);
    // Wall wall2(8.5, 6.3);

    // copy contents of wall1 to wall2
    Wall wall2 = wall1;

    cout << "Area of Wall 1: " << wall1.calculateArea() << endl;
    cout << "Area of Wall 2: " << wall2.calculateArea() << endl; 

    return 0; 
}