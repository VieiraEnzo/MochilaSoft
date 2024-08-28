// https://cplusplus.com/doc/tutorial/pointers/
// https://cplusplus.com/doc/tutorial/dynamic/


#include <iostream>
#include <cstring>
using namespace std; 

int main(){
    // int x = 5; 
    // cout << x << endl;
    // cout << &x << endl; 
    // cout << *(&x) << endl; 

    // int myNumbers[4] = {25, 50, 75, 100};

    // // Get the memory address of the myNumbers array
    // printf("%p\n", myNumbers);

    // // Get the memory address of the first array element
    // printf("%p\n", &myNumbers[0]);


    // pointers just point to a memory address
    int var = 8; 
    int* ptr = &var; 
    double* ptr2 = (double*)&var; 

    cout << ptr << endl;
    cout << *ptr << endl;   

    *ptr = 10; 
    cout << *ptr << endl;   

    char* buffer = new char[8]; // pointer to eight bytes of memory (point to the begining)
    memset(buffer, 0, 8);

    char** ptr3 = &buffer;

    delete[] buffer; 

    // https://stackoverflow.com/questions/8617683/how-to-return-a-2d-array-from-a-function



    return 0;
}