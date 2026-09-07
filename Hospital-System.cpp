#include <iostream>
using namespace std;

struct Patient {
    int id;
    string name;
    int age;
    int department_id;
};
Patient patients [100];
int numberofPatients = 0;

void addPatients (){
    cout << "Enter number of patients: ";
    cin >> numberofPatients;
    if (numberofPatients > 100) {
    cout << "Maximum number of patients is 100.\n";
    return;
}
    for(int i =0;i<numberofPatients;i++){
        cout << "\n--- Patient " << (i + 1) << " ---\n";

        cout << "ID: ";
        cin >> patients[i].id;

        cout << "Name: ";
        cin >> patients[i].name;

        cout << "Age: ";
        cin >> patients[i].age;

        cout << "Department ID (1-8): ";
        cin >> patients[i].department_id;
    }

}

struct doctor {
    int doctorID;
    string name;
    int age;
    int departmentID;
};
doctor doctors[33] = {


    {120, "Ahmed", 34, 1},
    {121, "Mohamed", 50, 1},
    {122, "Khaled", 28, 1},
    {123, "Sara", 33, 1},
    {124, "Hana", 30, 1},
    {125, "Omar", 46, 1},
    {126, "Samah", 74, 1},
    {127, "Amal", 60, 1} ,

    {200, "Mahmoud", 30, 2},
    {201, "Hamdy", 33, 2},
    {202, "Ahmed", 50, 2},
    {203, "Hany", 50, 2},
    {204, "Sameh", 36, 2},

    {310, "Ahmed", 34, 3},
    {311, "Mohamed", 35, 3},
    {312, "Mahmoud", 36, 3},
    {313, "Hasan", 50, 3},
    {314, "Hany", 68, 3},
    {315, "Sara", 34, 3},

    {317, "Abdullah", 30, 4},
    {318, "Adel", 37, 4},
    {319, "Saly", 44, 4},
    {320, "Seham", 65, 4},

    {450, "Amal", 34, 5},
    {451, "Hazem", 40, 5},
    {452, "Mostafa", 35, 5},

    {453, "Naglaa", 40, 6},
    {454, "Sahar", 32, 6},
    {455, "Kamal", 37, 6},

    {456, "Esam", 30, 7},
    {457, "Belal", 32, 7},

    {128, "Hasan", 38, 8},
    {205, "Sara", 30, 8}
};
void doctors_display(int x){
    if(x < 1 || x > 8){
        cout << "Invalid department ID\n";
        return;
    }

    for(int i = 0; i < 33; i++){
        if(doctors[i].departmentID == x){
            cout << "Doctor name: " << doctors[i].name << "\n";
            cout << "Doctor ID: " << doctors[i].doctorID << "\n";
            cout << "Doctor age: " << doctors[i].age << "\n";
            cout << "----------------------\n";
        }
    }
}
struct Department {
    int id;
    string name;
};
Department departments[8] = {
    {1, "Emergency"},
    {2, "Cardiology"},
    {3, "Internal Medicine"},
    {4, "Pediatrics"},
    {5, "Orthopedics"},
    {6, "Neurology"},
    {7, "Ophthalmology"},
    {8, "Dermatology"}
};

void display_all_departments(){
    cout << "\n===== Hospital Departments =====\n";

    for(int i = 0; i < 8; i++){
        cout << "ID: " << departments[i].id
             << " | Name: " << departments[i].name << "\n";
    }

    cout << "-------------------------------\n";
}

void displayDepartmentdetails(int dept_id, doctor docs[], int docCount,
                              Patient pts[], int ptCount){

    if(dept_id < 1 || dept_id > 8){
        cout << "Invalid Department ID!\n";
        return;
    }

    cout << "====================================\n";
    cout << "DEPARTMENT : " << departments[dept_id - 1].name << "\n";
    cout << "====================================\n";

    cout << "---- Doctors in this Department ----\n";

    for(int i = 0; i < docCount; i++){
        if(docs[i].departmentID == dept_id){
            cout << "ID : " << docs[i].doctorID
                 << " | Dr. " << docs[i].name
                 << " | Age: " << docs[i].age << "\n";
        }
    }

    cout << "---- Patients in this Department ----\n";

    for(int i = 0; i < ptCount; i++){
        if(pts[i].department_id == dept_id){
            cout << "ID : " << pts[i].id
                 << " | Name: " << pts[i].name
                 << " | Age: " << pts[i].age << "\n";
        }
    }

    cout << "---------------------------------------\n";
}
void editPatient() {
    int edit_id;

    cout << "\nEnter Patient ID to EDIT: ";
    cin >> edit_id;

    bool found_edit = false;

    for (int i = 0; i < numberofPatients; i++) {
        if (patients[i].id == edit_id) {
            found_edit = true;

            cout << "Patient Found! Enter New Details:\n";

            cout << "New Name: ";
            cin >> patients[i].name;

            cout << "New Age: ";
            cin >> patients[i].age;

            cout << "New Department ID (1-8): ";
            cin >> patients[i].department_id;

            cout << "Patient data updated successfully!\n";
            break;
        }
    }

    if (!found_edit) {
        cout << "Patient ID not found for editing.\n";
    }
}
void deletePatient() {
    int delete_id;

    cout << "\nEnter Patient ID to DELETE: ";
    cin >> delete_id;

    bool found_delete = false;

    for (int i = 0; i < numberofPatients; i++) {
        if (patients[i].id == delete_id) {
            found_delete = true;

            for (int j = i; j < numberofPatients - 1; j++) {
                patients[j] = patients[j + 1];
            }

            numberofPatients--;

            cout << "Patient deleted successfully!\n";
            break;
        }
    }

    if (!found_delete) {
        cout << "Patient ID not found for deletion.\n";
    }
}
void displayPatients() {
    cout << "\n=== All Patients ===\n";

    for (int i = 0; i < numberofPatients; i++) {
        cout << "ID: " << patients[i].id
             << " | Name: " << patients[i].name
             << " | Age: " << patients[i].age
             << " | Department ID: " << patients[i].department_id
             << "\n";
    }

    cout << "-------------------------------\n";
}
void totalPatients() {
    cout << "Total number of patients: "
         << numberofPatients << endl;
}

void totalDoctors() {
    cout << "Total number of doctors: 33" << endl;
}

void patientsPerDepartment() {
    int count[9] = {0};

    for (int i = 0; i < numberofPatients; i++) {
        int dept = patients[i].department_id;

        if (dept >= 1 && dept <= 8) {
            count[dept]++;
        }
    }

    cout << "\n=== Patients Per Department ===\n";

    for (int i = 1; i <= 8; i++) {
        cout << "Department: " << departments[i - 1].name
             << ": " << count[i] << " patients\n";
    }
}
void mostCrowdedDepartment() {
    int count[9] = {0};

    for (int i = 0; i < numberofPatients; i++) {
        int dept = patients[i].department_id;

        if (dept >= 1 && dept <= 8) {
            count[dept]++;
        }
    }

    int maxPatients = count[1];
    int maxDepartment = 1;

    for (int i = 2; i <= 8; i++) {
        if (count[i] > maxPatients) {
            maxPatients = count[i];
            maxDepartment = i;
        }
    }

    cout << "\n=== Most Crowded Department ===\n";
    cout << "Department: " << departments[maxDepartment - 1].name << "\n";
    cout << "Number of patients: " << maxPatients << endl;
}


void averagePatientAge() {
    if (numberofPatients == 0) {
        cout << "No patients available.\n";
        return;
    }

    int sum = 0;

    for (int i = 0; i < numberofPatients; i++) {
        sum += patients[i].age;
    }

    double average = (double)sum / numberofPatients;

    cout << "\nAverage patient age: "
         << average << endl;
}


void patientsPerDoctor() {
    double ratio = (double)numberofPatients / 33;

    cout << "Patients per doctor: "
         << ratio << endl;
}
int main(){

     int choice =0;
     while (choice !=5){
    cout << "=====================================\n "
         << "          HOSPITAL SYSTEM   \n"
         << "=====================================\n";

    cout << "1. Patients\n"
     << "2. Departments\n"
     << "3. Doctors\n"
     << "4. Statistics & Reports\n"
     << "5. Exit\n";


     cout << "Please choose number: "<< endl;
     cin >> choice;
     if (cin.fail()) {
    cin.clear();
    cin.ignore(1000, '\n');
    cout << "Invalid choice. Please enter a number.\n";
    continue;
}

     switch (choice) {
    case 1:
        int patientChoice;

    cout << "\n===== Patients =====\n";
    cout << "1. Add Patient\n";
    cout << "2. Edit Patient\n";
    cout << "3. Delete Patient\n";
    cout << "4. Display All Patients\n";
    cout << "5. Back to Main Menu\n";

    cout << "Please choose number: ";
    cin >> patientChoice;

    switch (patientChoice) {
        case 1:
            addPatients();
            break;

        case 2:
            editPatient();
            break;

        case 3:
            deletePatient();
            break;

        case 4:
            displayPatients();
            break;

        case 5:
            break;

        default:
            cout << "Invalid choice\n";
    }

    break;
        break;

   case 2:
        display_all_departments();
        int dept_id;
        cout << "Enter Department Id:";
        cin >> dept_id;
        displayDepartmentdetails(
        dept_id, doctors, 33, patients, numberofPatients);
        break;


    case 3:
        int departmentID;
        cout << "Please enter department ID: ";
        cin >> departmentID;

        doctors_display(departmentID);
        break;


    case 4:
        totalPatients();
        totalDoctors();
        patientsPerDepartment();
        mostCrowdedDepartment();
        averagePatientAge();
        patientsPerDoctor();
        break;

    case 5:
        cout << "Exit"<< endl ;
        break;

    default:
        cout << "Invalid choice" << endl;
}

}
    return 0;
 }
