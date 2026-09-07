#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

// ==================== DATA ====================

struct Patient {
    int id;
    string name;
    int age;
    int department_id;
};

Patient patients[100];
int numberofPatients = 0;

struct Doctor {
    int doctorID;
    string name;
    int age;
    int departmentID;
};

Doctor doctors[33] = {
    {120,"Ahmed",34,1},
    {121,"Mohamed",50,1},
    {122,"Khaled",28,1},
    {123,"Sara",33,1},
    {124,"Hana",30,1},
    {125,"Omar",46,1},
    {126,"Samah",74,1},
    {127,"Amal",60,1},

    {200,"Mahmoud",30,2},
    {201,"Hamdy",33,2},
    {202,"Ahmed",50,2},
    {203,"Hany",50,2},
    {204,"Sameh",36,2},

    {310,"Ahmed",34,3},
    {311,"Mohamed",35,3},
    {312,"Mahmoud",36,3},
    {313,"Hasan",50,3},
    {314,"Hany",68,3},
    {315,"Sara",34,3},

    {317,"Abdullah",30,4},
    {318,"Adel",37,4},
    {319,"Saly",44,4},
    {320,"Seham",65,4},

    {450,"Amal",34,5},
    {451,"Hazem",40,5},
    {452,"Mostafa",35,5},

    {453,"Naglaa",40,6},
    {454,"Sahar",32,6},
    {455,"Kamal",37,6},

    {456,"Esam",30,7},
    {457,"Belal",32,7},

    {128,"Hasan",38,8},
    {205,"Sara",30,8}
};

struct Department {
    int id;
    string name;
};

Department departments[8] = {
    {1,"Emergency"},
    {2,"Cardiology"},
    {3,"Internal Medicine"},
    {4,"Pediatrics"},
    {5,"Orthopedics"},
    {6,"Neurology"},
    {7,"Ophthalmology"},
    {8,"Dermatology"}
};

// ==================== GUI VARIABLES ====================

HWND mainWindow;
HWND outputBox;

HINSTANCE hInstanceGlobal;

// ==================== HELPER ====================

void showText(string text)
{
    SetWindowTextA(outputBox, text.c_str());
}

string getDepartmentName(int id)
{
    if(id >= 1 && id <= 8)
        return departments[id - 1].name;

    return "Unknown";
}

// ==================== STATISTICS ====================

string statistics()
{
    stringstream ss;

    int count[9] = {0};

    int sumAge = 0;

    for(int i = 0; i < numberofPatients; i++)
    {
        if(patients[i].department_id >= 1 &&
           patients[i].department_id <= 8)
        {
            count[patients[i].department_id]++;
        }

        sumAge += patients[i].age;
    }

    int maxPatients = count[1];
    int maxDepartment = 1;

    for(int i = 2; i <= 8; i++)
    {
        if(count[i] > maxPatients)
        {
            maxPatients = count[i];
            maxDepartment = i;
        }
    }

    double averageAge = 0;

    if(numberofPatients > 0)
        averageAge = (double)sumAge / numberofPatients;

    double patientsDoctor =
        (double)numberofPatients / 33;

    ss << "========================================\r\n";
    ss << "          HOSPITAL STATISTICS\r\n";
    ss << "========================================\r\n\r\n";

    ss << "Total Patients : "
       << numberofPatients << "\r\n";

    ss << "Total Doctors  : 33\r\n\r\n";

    ss << "----- Patients Per Department -----\r\n\r\n";

    for(int i = 1; i <= 8; i++)
    {
        ss << departments[i - 1].name
           << " : "
           << count[i]
           << " patients\r\n";
    }

    ss << "\r\n----------------------------------------\r\n";

    ss << "Most Crowded Department : "
       << getDepartmentName(maxDepartment)
       << "\r\n";

    ss << "Number of Patients      : "
       << maxPatients
       << "\r\n";

    ss << "Average Patient Age     : "
       << fixed << setprecision(2)
       << averageAge
       << "\r\n";

    ss << "Patients per Doctor     : "
       << fixed << setprecision(2)
       << patientsDoctor
       << "\r\n";

    ss << "========================================\r\n";

    return ss.str();
}

// ==================== PATIENTS ====================

string allPatients()
{
    stringstream ss;

    ss << "============ ALL PATIENTS ============\r\n\r\n";

    if(numberofPatients == 0)
    {
        ss << "No patients available.\r\n";
        return ss.str();
    }

    for(int i = 0; i < numberofPatients; i++)
    {
        ss << "Patient " << i + 1 << "\r\n";
        ss << "ID         : " << patients[i].id << "\r\n";
        ss << "Name       : " << patients[i].name << "\r\n";
        ss << "Age        : " << patients[i].age << "\r\n";
        ss << "Department : "
           << getDepartmentName(patients[i].department_id)
           << "\r\n";

        ss << "--------------------------------------\r\n";
    }

    return ss.str();
}

// ==================== DOCTORS ====================

string allDoctors()
{
    stringstream ss;

    ss << "============ ALL DOCTORS ============\r\n\r\n";

    for(int i = 0; i < 33; i++)
    {
        ss << "Doctor ID  : "
           << doctors[i].doctorID << "\r\n";

        ss << "Name       : "
           << doctors[i].name << "\r\n";

        ss << "Age        : "
           << doctors[i].age << "\r\n";

        ss << "Department : "
           << getDepartmentName(doctors[i].departmentID)
           << "\r\n";

        ss << "--------------------------------------\r\n";
    }

    return ss.str();
}

// ==================== DEPARTMENTS ====================

string allDepartments()
{
    stringstream ss;

    ss << "========= HOSPITAL DEPARTMENTS =========\r\n\r\n";

    for(int i = 0; i < 8; i++)
    {
        ss << departments[i].id
           << " - "
           << departments[i].name
           << "\r\n";
    }

    return ss.str();
}

// ==================== WINDOW PROCEDURE ====================

LRESULT CALLBACK WindowProcedure(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch(msg)
    {
        case WM_CREATE:
        {
            CreateWindowA(
                "STATIC",
                "HOSPITAL MANAGEMENT SYSTEM",
                WS_VISIBLE | WS_CHILD,
                230, 20, 400, 40,
                hwnd,
                NULL,
                hInstanceGlobal,
                NULL
            );

            CreateWindowA(
                "BUTTON",
                "Patients",
                WS_VISIBLE | WS_CHILD,
                50, 80, 150, 45,
                hwnd,
                (HMENU)1,
                hInstanceGlobal,
                NULL
            );

            CreateWindowA(
                "BUTTON",
                "Doctors",
                WS_VISIBLE | WS_CHILD,
                220, 80, 150, 45,
                hwnd,
                (HMENU)2,
                hInstanceGlobal,
                NULL
            );

            CreateWindowA(
                "BUTTON",
                "Departments",
                WS_VISIBLE | WS_CHILD,
                390, 80, 150, 45,
                hwnd,
                (HMENU)3,
                hInstanceGlobal,
                NULL
            );

            CreateWindowA(
                "BUTTON",
                "Statistics",
                WS_VISIBLE | WS_CHILD,
                560, 80, 150, 45,
                hwnd,
                (HMENU)4,
                hInstanceGlobal,
                NULL
            );

            CreateWindowA(
                "BUTTON",
                "Exit",
                WS_VISIBLE | WS_CHILD,
                730, 80, 100, 45,
                hwnd,
                (HMENU)5,
                hInstanceGlobal,
                NULL
            );

            outputBox = CreateWindowA(
                "EDIT",
                "",
                WS_VISIBLE |
                WS_CHILD |
                WS_BORDER |
                ES_MULTILINE |
                ES_AUTOVSCROLL |
                ES_READONLY |
                WS_VSCROLL,
                50, 150, 780, 420,
                hwnd,
                NULL,
                hInstanceGlobal,
                NULL
            );

            showText(
                "Welcome to Hospital Management System!\r\n\r\n"
                "Choose an option from the buttons above."
            );

            break;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case 1:
                {
                    string text = allPatients();
                    showText(text);
                    break;
                }

                case 2:
                {
                    string text = allDoctors();
                    showText(text);
                    break;
                }

                case 3:
                {
                    string text = allDepartments();
                    showText(text);
                    break;
                }

                case 4:
                {
                    string text = statistics();
                    showText(text);
                    break;
                }

                case 5:
                {
                    PostQuitMessage(0);
                    break;
                }
            }

            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }

        default:
            return DefWindowProc(
                hwnd,
                msg,
                wParam,
                lParam
            );
    }

    return 0;
}

// ==================== MAIN ====================

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    hInstanceGlobal = hInstance;

    WNDCLASSA wc = {};

    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProcedure;
    wc.lpszClassName = "HospitalWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClassA(&wc);

    mainWindow = CreateWindowA(
        "HospitalWindow",
        "Hospital Management System",
        WS_OVERLAPPEDWINDOW,
        100,
        50,
        900,
        650,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(mainWindow, nCmdShow);
    UpdateWindow(mainWindow);

    MSG msg;

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
