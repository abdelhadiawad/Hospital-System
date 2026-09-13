#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>

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
    {120,"Ahmed",34,1}, {121,"Mohamed",50,1}, {122,"Khaled",28,1}, {123,"Sara",33,1},
    {124,"Hana",30,1}, {125,"Omar",46,1}, {126,"Samah",74,1}, {127,"Amal",60,1},
    {200,"Mahmoud",30,2}, {201,"Hamdy",33,2}, {202,"Ahmed",50,2}, {203,"Hany",50,2},
    {204,"Sameh",36,2}, {310,"Ahmed",34,3}, {311,"Mohamed",35,3}, {312,"Mahmoud",36,3},
    {313,"Hasan",50,3}, {314,"Hany",68,3}, {315,"Sara",34,3}, {317,"Abdullah",30,4},
    {318,"Adel",37,4}, {319,"Saly",44,4}, {320,"Seham",65,4}, {450,"Amal",34,5},
    {451,"Hazem",40,5}, {452,"Mostafa",35,5}, {453,"Naglaa",40,6}, {454,"Sahar",32,6},
    {455,"Kamal",37,6}, {456,"Esam",30,7}, {457,"Belal",32,7}, {128,"Hasan",38,8},
    {205,"Sara",30,8}
};

struct Department {
    int id;
    string name;
};

Department departments[8] = {
    {1,"Emergency"}, {2,"Cardiology"}, {3,"Internal Medicine"}, {4,"Pediatrics"},
    {5,"Orthopedics"}, {6,"Neurology"}, {7,"Ophthalmology"}, {8,"Dermatology"}
};

// ==================== GUI VARIABLES & COLORS ====================

HWND mainWindow;
HWND outputBox;
HWND hEditID, hEditName, hEditAge, hEditDept;
HWND hDeptLookupTitle, hDeptLookupLabel, hEditDeptLookup, hDeptLookupButton;
HWND departmentWindow, departmentDetailsOutput, hDetailsDeptEdit;
HINSTANCE hInstanceGlobal;

HFONT hTitleFont, hButtonFont, hTextFont, hLabelFont;
HBRUSH hBackgroundBrush;
HBRUSH hWhiteBrush;
HBRUSH hBlueBrush;
HBRUSH hDarkBlueBrush;
HBRUSH hCyanBrush;
HBRUSH hGreenBrush;
HBRUSH hRedBrush;
HBRUSH hInputBrush;

#define BTN_PATIENTS   1
#define BTN_DOCTORS    2
#define BTN_DEPTS      3
#define BTN_STATS      4
#define BTN_ADD_PAGE   5
#define BTN_SAVE_PAT   6
#define BTN_EXIT       7
#define BTN_DEPT_LOOKUP 8
#define BTN_DEPT_DETAILS_VIEW 9
#define BTN_DEPT_DETAILS_CLOSE 10

LRESULT CALLBACK DepartmentWindowProcedure(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

void openDepartmentDetailsScreen();
void showDepartmentDetailsFromWindow();

// ==================== HELPER ====================

void showText(string text) {
    SetWindowTextA(outputBox, text.c_str());
    InvalidateRect(outputBox, NULL, TRUE);
    UpdateWindow(outputBox);
}

string getDepartmentName(int id) {
    if(id >= 1 && id <= 8)
        return departments[id - 1].name;
    return "Unknown";
}

string divider(int width = 56) {
    return string(width, '-') + "\r\n";
}

string fitText(const string& value, int width) {
    if((int)value.size() <= width)
        return value;

    if(width <= 3)
        return value.substr(0, width);

    return value.substr(0, width - 3) + "...";
}

void appendDoctorRecord(
    stringstream& ss,
    const Doctor& doctor,
    int recordNumber
) {
    ss << "Doctor " << recordNumber << "\r\n";
    ss << "  ID         : " << doctor.doctorID << "\r\n";
    ss << "  Name       : " << fitText(doctor.name, 30) << "\r\n";
    ss << "  Age        : " << doctor.age << "\r\n";
    ss << "  Department : " << getDepartmentName(doctor.departmentID)
       << "\r\n";
    ss << divider(48);
}

void setDepartmentLookupVisible(bool visible) {
    int command = visible ? SW_SHOW : SW_HIDE;

    if(hDeptLookupTitle != NULL)
        ShowWindow(hDeptLookupTitle, command);
    if(hDeptLookupLabel != NULL)
        ShowWindow(hDeptLookupLabel, command);
    if(hEditDeptLookup != NULL)
        ShowWindow(hEditDeptLookup, command);
    if(hDeptLookupButton != NULL)
        ShowWindow(hDeptLookupButton, command);
}

// ==================== STATISTICS ====================

string statistics() {
    stringstream ss;
    int count[9] = {0};
    int sumAge = 0;

    for(int i = 0; i < numberofPatients; i++) {
        if(patients[i].department_id >= 1 && patients[i].department_id <= 8)
            count[patients[i].department_id]++;

        sumAge += patients[i].age;
    }

    int maxPatients = count[1];
    int maxDepartment = 1;

    for(int i = 2; i <= 8; i++) {
        if(count[i] > maxPatients) {
            maxPatients = count[i];
            maxDepartment = i;
        }
    }

    double averageAge =
        (numberofPatients > 0) ?
        (double)sumAge / numberofPatients : 0;

    double patientsDoctor = (double)numberofPatients / 33;

    ss << "================ HOSPITAL STATISTICS ================\r\n\r\n";
    ss << left << setw(28) << "Total patients" << ": "
       << numberofPatients << "\r\n";
    ss << left << setw(28) << "Total doctors" << ": "
       << 33 << "\r\n\r\n";
    ss << "--------------- PATIENTS BY DEPARTMENT ---------------\r\n";
    ss << left << setw(24) << "Department" << right
       << setw(12) << "Patients" << "\r\n";
    ss << divider(50);

    for(int i = 1; i <= 8; i++) {
        ss << left << setw(24) << departments[i - 1].name
           << right << setw(12) << count[i] << "\r\n";
    }

    ss << "\r\n-------------------- SUMMARY --------------------\r\n";
    ss << left << setw(28) << "Most crowded department" << ": "
       << getDepartmentName(maxDepartment) << "\r\n";
    ss << left << setw(28) << "Number of patients" << ": "
       << maxPatients << "\r\n";
    ss << left << setw(28) << "Average patient age" << ": "
       << fixed << setprecision(2) << averageAge << "\r\n";
    ss << left << setw(28) << "Patients per doctor" << ": "
       << fixed << setprecision(2) << patientsDoctor << "\r\n";
    ss << "=======================================================\r\n";

    return ss.str();
}

// ==================== DISPLAY ====================

string allPatients() {
    stringstream ss;

    ss << "==================== ALL PATIENTS ====================\r\n\r\n";

    if(numberofPatients == 0) {
        ss << "No patients registered yet.\r\n";
        ss << "Use [ + Add Patient ] to register new patients.\r\n";
        return ss.str();
    }

    ss << left
       << setw(8) << "ID"
       << setw(20) << "Name"
       << setw(7) << "Age"
       << setw(21) << "Department" << "\r\n";
    ss << divider();

    for(int i = 0; i < numberofPatients; i++) {
        ss << left
           << setw(8) << patients[i].id
           << setw(20) << fitText(patients[i].name, 19)
           << setw(7) << patients[i].age
           << setw(21) << getDepartmentName(patients[i].department_id)
           << "\r\n";
    }

    ss << divider();
    return ss.str();
}

string allDoctors() {
    stringstream ss;

    ss << "================ ALL DOCTORS ================\r\n\r\n";

    for(int i = 0; i < 33; i++) {
        appendDoctorRecord(ss, doctors[i], i + 1);
    }

    return ss.str();
}

string allDepartments() {
    stringstream ss;

    ss << "================ HOSPITAL DEPARTMENTS ================\r\n\r\n";
    ss << left << setw(8) << "ID" << setw(42) << "Department" << "\r\n";
    ss << divider(50);

    for(int i = 0; i < 8; i++) {
        ss << left << setw(8) << departments[i].id
           << setw(42) << departments[i].name << "\r\n";
    }

    ss << "\r\nEnter a department ID on the right and click\r\n";
    ss << "[ View Department ] to open its details screen.\r\n";

    return ss.str();
}

string departmentDetails(int departmentId) {
    stringstream ss;
    int doctorCount = 0;
    int patientCount = 0;

    for(int i = 0; i < 33; i++) {
        if(doctors[i].departmentID == departmentId)
            doctorCount++;
    }

    for(int i = 0; i < numberofPatients; i++) {
        if(patients[i].department_id == departmentId)
            patientCount++;
    }

    ss << "================ DEPARTMENT DETAILS ================\r\n";
    ss << left << setw(16) << "Department ID" << ": "
       << departmentId << "\r\n";
    ss << left << setw(16) << "Department" << ": "
       << getDepartmentName(departmentId) << "\r\n";
    ss << left << setw(16) << "Doctors" << ": "
       << doctorCount << "\r\n";
    ss << left << setw(16) << "Patients" << ": "
       << patientCount << "\r\n\r\n";

    ss << "---------------------- DOCTORS ----------------------\r\n";

    if(doctorCount == 0) {
        ss << "No doctors are assigned to this department.\r\n";
    } else {
        int doctorNumber = 1;
        for(int i = 0; i < 33; i++) {
            if(doctors[i].departmentID == departmentId) {
                appendDoctorRecord(
                    ss,
                    doctors[i],
                    doctorNumber++
                );
            }
        }
    }

    ss << "\r\n---------------------- PATIENTS ---------------------\r\n";
    ss << left
       << setw(10) << "ID"
       << setw(24) << "Name"
       << setw(8) << "Age" << "\r\n";
    ss << divider(42);

    if(patientCount == 0) {
        ss << "No patients are registered in this department.\r\n";
    } else {
        for(int i = 0; i < numberofPatients; i++) {
            if(patients[i].department_id == departmentId) {
                ss << left
                   << setw(10) << patients[i].id
                   << setw(24) << fitText(patients[i].name, 23)
                   << setw(8) << patients[i].age << "\r\n";
            }
        }
    }

    return ss.str();
}

// ==================== ADD PATIENT ====================

void showAddPatientUI() {
    stringstream ss;

    ss << "============== ADD NEW PATIENT FORM ================\r\n\r\n";
    ss << "Please fill the input fields on the right panel\r\n";
    ss << "and click [ Save Patient Data ] to confirm.\r\n\r\n";
    ss << "---------------- DEPARTMENTS ----------------\r\n";
    ss << left << setw(8) << "ID" << setw(30) << "Department" << "\r\n";
    ss << divider(38);

    for(int i = 0; i < 8; i++) {
        ss << left << setw(8) << departments[i].id
           << setw(30) << departments[i].name << "\r\n";
    }

    setDepartmentLookupVisible(false);
    showText(ss.str());
}

void savePatientFromInput() {
    if(numberofPatients >= 100) {
        MessageBoxA(
            mainWindow,
            "Maximum capacity reached (100 Patients)!",
            "Error",
            MB_ICONERROR
        );
        return;
    }

    char bufID[20], bufName[50], bufAge[20], bufDept[20];

    GetWindowTextA(hEditID, bufID, 20);
    GetWindowTextA(hEditName, bufName, 50);
    GetWindowTextA(hEditAge, bufAge, 20);
    GetWindowTextA(hEditDept, bufDept, 20);

    if(strlen(bufID) == 0 ||
       strlen(bufName) == 0 ||
       strlen(bufAge) == 0 ||
       strlen(bufDept) == 0) {

        MessageBoxA(
            mainWindow,
            "Please fill all fields before saving!",
            "Input Error",
            MB_ICONWARNING
        );
        return;
    }

    int dept = atoi(bufDept);

    if(dept < 1 || dept > 8) {
        MessageBoxA(
            mainWindow,
            "Invalid Department ID! (Choose 1 to 8)",
            "Input Error",
            MB_ICONWARNING
        );
        return;
    }

    patients[numberofPatients].id = atoi(bufID);
    patients[numberofPatients].name = string(bufName);
    patients[numberofPatients].age = atoi(bufAge);
    patients[numberofPatients].department_id = dept;

    numberofPatients++;

    SetWindowTextA(hEditID, "");
    SetWindowTextA(hEditName, "");
    SetWindowTextA(hEditAge, "");
    SetWindowTextA(hEditDept, "");

    MessageBoxA(
        mainWindow,
        "Patient Added Successfully!",
        "Success",
        MB_ICONINFORMATION
    );

    setDepartmentLookupVisible(false);
    showText(allPatients());
}

void openDepartmentDetailsScreen() {
    char buffer[20];
    GetWindowTextA(hEditDeptLookup, buffer, sizeof(buffer));

    if(strlen(buffer) == 0) {
        MessageBoxA(
            mainWindow,
            "Enter a department ID from 1 to 8.",
            "Department Lookup",
            MB_ICONWARNING
        );
        return;
    }

    int departmentId = atoi(buffer);

    if(departmentId < 1 || departmentId > 8) {
        MessageBoxA(
            mainWindow,
            "Invalid Department ID! (Choose 1 to 8)",
            "Department Lookup",
            MB_ICONWARNING
        );
        return;
    }

    if(departmentWindow == NULL || !IsWindow(departmentWindow)) {
        departmentWindow = CreateWindowA(
            "DepartmentDetailsWindow",
            "Department Details",
            WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
            CW_USEDEFAULT, CW_USEDEFAULT, 720, 560,
            mainWindow, NULL,
            hInstanceGlobal, NULL
        );
    }

    if(departmentWindow == NULL)
        return;

    stringstream departmentIdStream;
    departmentIdStream << departmentId;
    SetWindowTextA(
        hDetailsDeptEdit,
        departmentIdStream.str().c_str()
    );
    SetWindowTextA(
        departmentDetailsOutput,
        departmentDetails(departmentId).c_str()
    );
    InvalidateRect(departmentDetailsOutput, NULL, TRUE);
    UpdateWindow(departmentDetailsOutput);

    ShowWindow(departmentWindow, SW_SHOW);
    UpdateWindow(departmentWindow);
    SetForegroundWindow(departmentWindow);
}

void showDepartmentDetailsFromWindow() {
    char buffer[20];
    GetWindowTextA(hDetailsDeptEdit, buffer, sizeof(buffer));

    int departmentId = atoi(buffer);

    if(strlen(buffer) == 0 || departmentId < 1 || departmentId > 8) {
        MessageBoxA(
            departmentWindow,
            "Enter a valid Department ID from 1 to 8.",
            "Department Lookup",
            MB_ICONWARNING
        );
        return;
    }

    SetWindowTextA(
        departmentDetailsOutput,
        departmentDetails(departmentId).c_str()
    );
    InvalidateRect(departmentDetailsOutput, NULL, TRUE);
    UpdateWindow(departmentDetailsOutput);
}

LRESULT CALLBACK DepartmentWindowProcedure(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
    switch(msg) {
        case WM_CREATE: {
            HWND title = CreateWindowA(
                "STATIC",
                "DEPARTMENT DETAILS",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                25, 18, 650, 30,
                hwnd, NULL, hInstanceGlobal, NULL
            );

            SendMessage(title, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

            HWND label = CreateWindowA(
                "STATIC",
                "Department ID (1-8):",
                WS_VISIBLE | WS_CHILD,
                25, 68, 160, 22,
                hwnd, NULL, hInstanceGlobal, NULL
            );

            SendMessage(label, WM_SETFONT, (WPARAM)hLabelFont, TRUE);

            hDetailsDeptEdit = CreateWindowA(
                "EDIT",
                "1",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                190, 64, 90, 30,
                hwnd, NULL, hInstanceGlobal, NULL
            );

            HWND viewButton = CreateWindowA(
                "BUTTON",
                "View Department",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                295, 64, 170, 30,
                hwnd, (HMENU)BTN_DEPT_DETAILS_VIEW,
                hInstanceGlobal, NULL
            );

            HWND closeButton = CreateWindowA(
                "BUTTON",
                "Close",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                480, 64, 100, 30,
                hwnd, (HMENU)BTN_DEPT_DETAILS_CLOSE,
                hInstanceGlobal, NULL
            );

            SendMessage(
                hDetailsDeptEdit,
                WM_SETFONT,
                (WPARAM)hButtonFont,
                TRUE
            );
            SendMessage(viewButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(closeButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);

            departmentDetailsOutput = CreateWindowA(
                "EDIT",
                "",
                WS_VISIBLE | WS_CHILD | WS_BORDER |
                ES_MULTILINE | ES_AUTOVSCROLL |
                ES_READONLY | ES_AUTOHSCROLL |
                WS_VSCROLL | WS_HSCROLL,
                25, 115, 650, 380,
                hwnd, NULL, hInstanceGlobal, NULL
            );

            SendMessage(
                departmentDetailsOutput,
                WM_SETFONT,
                (WPARAM)hTextFont,
                TRUE
            );

            break;
        }

        case WM_COMMAND: {
            switch(LOWORD(wParam)) {
                case BTN_DEPT_DETAILS_VIEW:
                    showDepartmentDetailsFromWindow();
                    break;

                case BTN_DEPT_DETAILS_CLOSE:
                    DestroyWindow(hwnd);
                    break;
            }

            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            HWND control = (HWND)lParam;

            if(control == departmentDetailsOutput) {
                SetBkMode(hdc, OPAQUE);
                SetTextColor(hdc, RGB(30, 50, 65));
                SetBkColor(hdc, RGB(255, 255, 255));
                return (LRESULT)hWhiteBrush;
            }

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(28, 78, 115));
            SetBkColor(hdc, RGB(235, 244, 250));
            return (LRESULT)hBackgroundBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            HWND control = (HWND)lParam;

            if(control == departmentDetailsOutput) {
                SetBkMode(hdc, OPAQUE);
                SetTextColor(hdc, RGB(30, 50, 65));
                SetBkColor(hdc, RGB(255, 255, 255));
                return (LRESULT)hWhiteBrush;
            }

            SetBkMode(hdc, OPAQUE);
            SetTextColor(hdc, RGB(30, 50, 65));
            SetBkColor(hdc, RGB(248, 252, 255));
            return (LRESULT)hInputBrush;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            departmentWindow = NULL;
            departmentDetailsOutput = NULL;
            hDetailsDeptEdit = NULL;
            break;

        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// ==================== WINDOW PROCEDURE ====================

LRESULT CALLBACK WindowProcedure(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
) {
    switch(msg) {

        case WM_CREATE: {

            // Fonts
            hTitleFont = CreateFontA(
                26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                "Segoe UI"
            );

            hButtonFont = CreateFontA(
                15, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                "Segoe UI"
            );

            // Fixed-width font keeps every table column aligned.
            hTextFont = CreateFontA(
                14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                "Courier New"
            );

            hLabelFont = CreateFontA(
                14, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                "Segoe UI"
            );

            // Colors
            hBackgroundBrush = CreateSolidBrush(RGB(235, 244, 250));
            hWhiteBrush      = CreateSolidBrush(RGB(255, 255, 255));
            hBlueBrush       = CreateSolidBrush(RGB(43, 120, 170));
            hDarkBlueBrush   = CreateSolidBrush(RGB(28, 78, 115));
            hCyanBrush       = CreateSolidBrush(RGB(70, 170, 200));
            hGreenBrush      = CreateSolidBrush(RGB(65, 160, 105));
            hRedBrush        = CreateSolidBrush(RGB(200, 75, 75));
            hInputBrush      = CreateSolidBrush(RGB(248, 252, 255));

            HWND title = CreateWindowA(
                "STATIC", "HOSPITAL MANAGEMENT SYSTEM",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                20, 18, 1040, 38,
                hwnd, NULL, hInstanceGlobal, NULL
            );
            SendMessage(title, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

            // Left navigation buttons
            HWND btnPatients = CreateWindowA(
                "BUTTON", "Patients",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                25, 75, 155, 38,
                hwnd, (HMENU)BTN_PATIENTS, hInstanceGlobal, NULL
            );
            HWND btnDoctors = CreateWindowA(
                "BUTTON", "Doctors",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                25, 122, 155, 38,
                hwnd, (HMENU)BTN_DOCTORS, hInstanceGlobal, NULL
            );
            HWND btnDepartments = CreateWindowA(
                "BUTTON", "Departments",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                25, 169, 155, 38,
                hwnd, (HMENU)BTN_DEPTS, hInstanceGlobal, NULL
            );
            HWND btnStats = CreateWindowA(
                "BUTTON", "Statistics",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                25, 216, 155, 38,
                hwnd, (HMENU)BTN_STATS, hInstanceGlobal, NULL
            );
            HWND btnAdd = CreateWindowA(
                "BUTTON", "+ Add Patient",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                25, 263, 155, 38,
                hwnd, (HMENU)BTN_ADD_PAGE, hInstanceGlobal, NULL
            );
            HWND btnExit = CreateWindowA(
                "BUTTON", "Exit",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                25, 310, 155, 38,
                hwnd, (HMENU)BTN_EXIT, hInstanceGlobal, NULL
            );

            SendMessage(btnPatients, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(btnDoctors, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(btnDepartments, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(btnStats, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(btnAdd, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(btnExit, WM_SETFONT, (WPARAM)hButtonFont, TRUE);

            // Main output area
            outputBox = CreateWindowA(
                "EDIT", "",
                WS_VISIBLE | WS_CHILD | WS_BORDER |
                ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY |
                ES_AUTOHSCROLL | WS_VSCROLL | WS_HSCROLL,
                205, 75, 590, 500,
                hwnd, NULL, hInstanceGlobal, NULL
            );
            SendMessage(outputBox, WM_SETFONT, (WPARAM)hTextFont, TRUE);

            // Right panel - Add patient inputs
            HWND formTitle = CreateWindowA(
                "STATIC", "PATIENT DATA",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                820, 75, 230, 28,
                hwnd, NULL, hInstanceGlobal, NULL
            );
            SendMessage(formTitle, WM_SETFONT, (WPARAM)hLabelFont, TRUE);

            HWND idLabel = CreateWindowA("STATIC", "Patient ID:", WS_VISIBLE | WS_CHILD,
                820, 118, 105, 22, hwnd, NULL, hInstanceGlobal, NULL);
            hEditID = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                820, 142, 230, 30, hwnd, NULL, hInstanceGlobal, NULL);

            HWND nameLabel = CreateWindowA("STATIC", "Name:", WS_VISIBLE | WS_CHILD,
                820, 182, 105, 22, hwnd, NULL, hInstanceGlobal, NULL);
            hEditName = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER,
                820, 206, 230, 30, hwnd, NULL, hInstanceGlobal, NULL);

            HWND ageLabel = CreateWindowA("STATIC", "Age:", WS_VISIBLE | WS_CHILD,
                820, 246, 105, 22, hwnd, NULL, hInstanceGlobal, NULL);
            hEditAge = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                820, 270, 230, 30, hwnd, NULL, hInstanceGlobal, NULL);

            HWND deptLabel = CreateWindowA("STATIC", "Department ID:", WS_VISIBLE | WS_CHILD,
                820, 310, 130, 22, hwnd, NULL, hInstanceGlobal, NULL);
            hEditDept = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                820, 334, 230, 30, hwnd, NULL, hInstanceGlobal, NULL);

            HWND saveButton = CreateWindowA(
                "BUTTON", "Save Patient Data",
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                820, 378, 230, 36,
                hwnd, (HMENU)BTN_SAVE_PAT, hInstanceGlobal, NULL
            );

            SendMessage(idLabel, WM_SETFONT, (WPARAM)hLabelFont, TRUE);
            SendMessage(nameLabel, WM_SETFONT, (WPARAM)hLabelFont, TRUE);
            SendMessage(ageLabel, WM_SETFONT, (WPARAM)hLabelFont, TRUE);
            SendMessage(deptLabel, WM_SETFONT, (WPARAM)hLabelFont, TRUE);
            SendMessage(hEditID, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(hEditName, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(hEditAge, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(hEditDept, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(saveButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);

            // Department lookup controls (shown only on Departments page)
            hDeptLookupTitle = CreateWindowA(
                "STATIC", "DEPARTMENT LOOKUP",
                WS_CHILD | SS_CENTER,
                820, 438, 230, 24,
                hwnd, NULL, hInstanceGlobal, NULL
            );
            hDeptLookupLabel = CreateWindowA(
                "STATIC", "Department ID (1-8):",
                WS_CHILD,
                820, 472, 160, 22,
                hwnd, NULL, hInstanceGlobal, NULL
            );
            hEditDeptLookup = CreateWindowA(
                "EDIT", "1",
                WS_CHILD | WS_BORDER | ES_NUMBER,
                820, 498, 85, 30,
                hwnd, NULL, hInstanceGlobal, NULL
            );
            hDeptLookupButton = CreateWindowA(
                "BUTTON", "View Department",
                WS_CHILD | BS_PUSHBUTTON,
                915, 498, 135, 30,
                hwnd, (HMENU)BTN_DEPT_LOOKUP, hInstanceGlobal, NULL
            );

            SendMessage(hDeptLookupTitle, WM_SETFONT, (WPARAM)hLabelFont, TRUE);
            SendMessage(hDeptLookupLabel, WM_SETFONT, (WPARAM)hLabelFont, TRUE);
            SendMessage(hEditDeptLookup, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            SendMessage(hDeptLookupButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            setDepartmentLookupVisible(false);

            showText(allPatients());
            break;
        }

        case WM_COMMAND: {
            switch(LOWORD(wParam)) {
                case BTN_PATIENTS:
                    setDepartmentLookupVisible(false);
                    showText(allPatients());
                    break;

                case BTN_DOCTORS:
                    setDepartmentLookupVisible(false);
                    showText(allDoctors());
                    break;

                case BTN_DEPTS:
                    setDepartmentLookupVisible(true);
                    showText(allDepartments());
                    break;

                case BTN_STATS:
                    setDepartmentLookupVisible(false);
                    showText(statistics());
                    break;

                case BTN_ADD_PAGE:
                    showAddPatientUI();
                    break;

                case BTN_SAVE_PAT:
                    savePatientFromInput();
                    break;

                case BTN_DEPT_LOOKUP:
                    openDepartmentDetailsScreen();
                    break;

                case BTN_EXIT:
                    DestroyWindow(hwnd);
                    break;
            }
            break;
        }

        case WM_CTLCOLORSTATIC: {
            HDC hdc = (HDC)wParam;
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(28, 78, 115));
            SetBkColor(hdc, RGB(235, 244, 250));
            return (LRESULT)hBackgroundBrush;
        }

        case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            HWND control = (HWND)lParam;

            if(control == outputBox) {
                SetBkMode(hdc, OPAQUE);
                SetTextColor(hdc, RGB(30, 50, 65));
                SetBkColor(hdc, RGB(255, 255, 255));
                return (LRESULT)hWhiteBrush;
            }

            SetBkMode(hdc, OPAQUE);
            SetTextColor(hdc, RGB(30, 50, 65));
            SetBkColor(hdc, RGB(248, 252, 255));
            return (LRESULT)hInputBrush;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            if(departmentWindow != NULL && IsWindow(departmentWindow))
                DestroyWindow(departmentWindow);

            DeleteObject(hTitleFont);
            DeleteObject(hButtonFont);
            DeleteObject(hTextFont);
            DeleteObject(hLabelFont);
            DeleteObject(hBackgroundBrush);
            DeleteObject(hWhiteBrush);
            DeleteObject(hBlueBrush);
            DeleteObject(hDarkBlueBrush);
            DeleteObject(hCyanBrush);
            DeleteObject(hGreenBrush);
            DeleteObject(hRedBrush);
            DeleteObject(hInputBrush);

            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// ==================== PROGRAM ENTRY ====================

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
) {
    (void)hPrevInstance;
    (void)lpCmdLine;

    hInstanceGlobal = hInstance;

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = "HospitalSystemWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if(!RegisterClassA(&wc)) {
        MessageBoxA(NULL, "Failed to register main window class.", "Error", MB_ICONERROR);
        return 0;
    }

    WNDCLASSA deptClass = {};
    deptClass.lpfnWndProc = DepartmentWindowProcedure;
    deptClass.hInstance = hInstance;
    deptClass.lpszClassName = "DepartmentDetailsWindow";
    deptClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    deptClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    deptClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if(!RegisterClassA(&deptClass)) {
        MessageBoxA(NULL, "Failed to register department window class.", "Error", MB_ICONERROR);
        return 0;
    }

    mainWindow = CreateWindowA(
        "HospitalSystemWindow",
        "Hospital Management System",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1100, 650,
        NULL, NULL,
        hInstance, NULL
    );

    if(mainWindow == NULL) {
        MessageBoxA(NULL, "Failed to create the main window.", "Error", MB_ICONERROR);
        return 0;
    }

    ShowWindow(mainWindow, nCmdShow);
    UpdateWindow(mainWindow);

    MSG msg = {};
    while(GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int)msg.wParam;
}
