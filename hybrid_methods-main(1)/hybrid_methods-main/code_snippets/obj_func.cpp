// #include<iostream>
// using namespace std; 


// class Student{

//     public:
//         double marks; 

//         Student(double m){
//             marks = m; 
//         }
// };

// void calculateAverage(Student s1, Student s2){
//     double average = (s1.marks + s2.marks)/2; 
//     cout << "Average marks = " << average << endl; 
// }


// int main(){
//     Student s1(88.0), s2(56.0); 
    
//     calculateAverage(s1, s2); 
//     return 0; 
// }


#include<iostream>
using namespace std; 


class Student{

    public:
        double marks1, marks2; 
};

Student createStudent(){
    Student student; 

    student.marks1 = 96.5; 
    student.marks2 = 75.0;

    // print member variables of Student
    cout << "Marks 1 = " << student.marks1 << endl;
    cout << "Marks 2 = " << student.marks2 << endl; 


}

int main(){
    Student s1; 

    s1 = createStudent(); 
    return 0; 
}
