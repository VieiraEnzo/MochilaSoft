#include <iostream>
using namespace std;

class MyClass {
public:
    int data;

    // Default Constructor
    MyClass(int val = 0) : data(val) {}

    void display() const {
        cout << "Data: " << data << endl;
    }
};

int main() {
    MyClass obj1(10);

        MyClass obj3;
    for(int i = 0; i < 10; i++){
        obj3 = obj1; // Assignment operator is called
        obj3.display();
    }
    

    return 0;
}
