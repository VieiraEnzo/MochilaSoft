#include<iostream>
using namespace std; 

// https://www.programiz.com/cpp-programming/access-modifiers
// The private members can only be accessed from within the class.

class Sample{
    private:
        int age; 

    public:
        void displayAge(int a){
            age = a; 
            cout << "Age = " << age << endl; 
        }
}; 

int main(){

    Sample obj1; 
    int ageInput; 

    cout << "Enter your age: "; 

    cin >> ageInput; 
    obj1.displayAge(ageInput); 

    return 0; 
}