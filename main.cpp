#include <iostream>
#include <queue>
#include <stack>
#include <cstdlib>

using namespace std;

struct Employee;
struct Package;
struct User;
struct Admin;
int employeeCount();
int cityIndex(string CITY);

bool Admin_data = false;
bool TourConfirmation = false;

stack<Package*> previous_records;

const int NO_OF_CITIES = 10, infinity = INT_MAX;
int CST[NO_OF_CITIES];
int TIM[NO_OF_CITIES];
int visited[NO_OF_CITIES];

string CITIES[NO_OF_CITIES] = { "New York","Colorado","California","Texas","Alabama","Georgia",
                               "Arkansas","Florida","Alaska","Arizona" };


/////   GRAPHS    /////
/////   GRAPH WRT COST    /////
int COST[NO_OF_CITIES][NO_OF_CITIES] = { {    0, 1000, 2100, 3300,    0,    0,    0, 7500,    0,    0},
                                      { 1000,    0, 1000, 2000,    0,    0,    0,    0,    0,    0},
                                      { 2100, 1000,    0, 1200,    0,    0,    0, 5000,    0, 7000},
                                      { 3300, 2000, 1200,    0, 1200,    0, 4000, 4000,    0,    0},
                                      {    0,    0,    0, 1200,    0, 1000,    0,    0,    0,    0},
                                      {    0,    0,    0,    0, 1000,    0, 1000, 1200,    0,    0},
                                      {    0,    0,    0, 4000,    0, 1000,    0, 1200, 1200, 3500},
                                      { 7500,    0, 5000, 4000,    0, 1200, 1200,    0, 1000,    0},
                                      {    0,    0,    0,    0,    0,    0, 1200, 1000,    0, 3000},
                                      {    0,    0, 7000,    0,    0,    0, 3500,    0, 3000,    0} };

/////   GRAPH WRT TIME(Min)    /////

int TIME[NO_OF_CITIES][NO_OF_CITIES] = { {   0, 120, 350, 385,   0,   0,   0, 700,   0,   0},
                                      { 120,   0, 120, 125,   0,   0,   0,   0,   0,   0},
                                      { 350, 120,   0, 130,   0,   0,   0, 360,   0, 570},
                                      { 385, 125, 130,   0, 135,   0, 360, 360,   0,   0},
                                      {   0,   0,   0, 135,   0, 100,   0,   0,   0,   0},
                                      {   0,   0,   0,   0, 100,   0, 150, 130,   0,   0},
                                      {   0,   0,   0, 360,   0, 150,   0, 120, 190, 410},
                                      { 700,   0, 360, 360,   0, 130, 120,   0, 200,   0},
                                      {   0,   0,   0,   0,   0,   0, 190, 200,   0, 420},
                                      {   0,   0, 570,   0,   0,   0, 410,   0, 410,   0} };


struct Admin {
    string CompanyName;
    string PhoneNo;
    queue<Package*> To_Assign_Organizer;
    stack<Package*> Previous_Tour_Records;

};


Admin* Company = NULL;

struct User {
    string username;
    string password;
    string firstName;
    string lastName;
    string phoneNo;
    string email;
    User* next = NULL;
    User* prev = NULL;
    Package* package = NULL;
};

User* firstUser = NULL;
User* lastUser = NULL;

struct Employee {
    string username;
    string password;
    string firstName;
    string lastName;
    string phoneNo;
    string email;
    Employee* next = NULL;
    Employee* prev = NULL;
};

Employee* firstEmployee = NULL;
Employee* lastEmployee = NULL;




struct Package {
    string From;
    string To;
    int capacity;
    bool return_trip;
    bool confirmation;
    string preferance;
    Employee* organizer = NULL;
};

void EnterUpdateAdmin() {
    Admin* admin = new Admin();
    cout << "Enter Company Name" << endl;
    cin >> admin->CompanyName;
    cout << "Enter Company Phone Number" << endl;
    cin >> admin->PhoneNo;
    Company = admin;
    Admin_data = true;
}

/////This function Assigns Organizers (Employees) to pending tourism requests of users/////
void assignOrganizer() {
    while (true) {
        if (!Company->To_Assign_Organizer.empty()) {
            Package* package = Company->To_Assign_Organizer.front();
            int size = employeeCount();
            int employee_Number = 1 + (rand() % size);
            Employee* currEmp = firstEmployee;

            for (int count = 1; currEmp != NULL; count++) {
                if (count == employee_Number) {
                    package->organizer = currEmp;
                    package->confirmation = true;
                    break;
                }
                currEmp = currEmp->next;
            }
            Company->To_Assign_Organizer.pop();
            Company->Previous_Tour_Records.push(package);
        }
        else {
            break;
        }
    }
}

void DisplayPrevTrips() {
    if (!Company->Previous_Tour_Records.empty()) {
        printf("%-14s%-12s%-15s%-15s", "TRIP FROM", "TRIP TO", "TRIP STATUS", "CAPACITY");
        cout << endl;
        while (true) {
            if (!Company->Previous_Tour_Records.empty()) {
                Package* p = Company->Previous_Tour_Records.top();

                string from = p->From;
                string to = p->To;
                string status;
                if (p->return_trip) {
                    status = "return trip";
                }
                else {
                    status = "1 way";
                }
                printf("%-11s-> %-12s%-15s%-15i", from.c_str(), to.c_str(), status.c_str(), p->capacity);
                cout << endl;

                previous_records.push(p);
                Company->Previous_Tour_Records.pop();
            }
            else {
                while (!previous_records.empty()) {
                    Company->Previous_Tour_Records.push(previous_records.top());
                    previous_records.pop();
                }
                break;
            }
        }
    }
    else {
        cout << "No trip available" << endl;
    }
}



void addUser() {
    User* curr = new User;

    cout << endl << "Enter Username : ";
    cin >> curr->username;
    cout << endl << "Enter Password : ";
    cin >> curr->password;
    cout << endl << "Enter First Name : ";
    cin >> curr->firstName;
    cout << endl << "Enter Last Name : ";
    cin >> curr->lastName;
    cout << endl << "Enter Phone No. : ";
    cin >> curr->phoneNo;
    cout << endl << "Enter Email Address : ";
    cin >> curr->email;
    if (firstUser == NULL)
    {
        firstUser = lastUser = curr;
    }
    else {
        lastUser->next = curr;
        curr->prev = lastUser;
        lastUser = curr;
    }
}

void displayUser() {
    if (firstUser == NULL)
    {
        cout << "No Record Entered";
    }
    else {
        User* curr = firstUser;
        printf("%-15s%-15s%-15s%-15s%-20s%-30s", "Username", "Password", "First Name", "Last Name", "Phone No", "E-mail");
        cout << endl;
        while (curr != NULL)
        {

            printf("%-15s%-15s%-15s%-15s%-20s%-30s", curr->username.c_str(),
                curr->password.c_str(), curr->firstName.c_str(), curr->lastName.c_str(),
                curr->phoneNo.c_str(), curr->email.c_str());
            cout << endl;
            curr = curr->next;
        }
    }
}

void searchUser() {
    string search;
    cout << "Enter User's Username you want to Search : ";
    cin >> search;
    User* curr = firstUser;
    while (curr != NULL)
    {
        if (curr->username == search)
        {
            cout << endl << "User Record Found" << endl;
            printf("%-15s%-15s%-15s%-15s%-20s%-30s", "Username", "Password", "First Name", "Last Name", "Phone No", "E-mail");
            cout << endl;
            printf("%-15s%-15s%-15s%-15s%-20s%-30s", curr->username.c_str(),
                curr->password.c_str(), curr->firstName.c_str(), curr->lastName.c_str(),
                curr->phoneNo.c_str(), curr->email.c_str());
            cout << endl;

            break;
        }
        curr = curr->next;
    }
    if (curr == NULL)
    {
        cout << "Record not Found";
    }
}

User* userLogin() {

    string U_Name;
    string U_Password;
    cout << "Enter Username" << endl;
    cin >> U_Name;
    cout << "Enter Password" << endl;
    cin >> U_Password;
    User* curr = firstUser;
    while (curr != NULL)
    {
        if (curr->username == U_Name)
        {
            if (curr->password == U_Password) {
                return curr;

            }
        }
        curr = curr->next;
    }
    if (curr == NULL)
    {
        return NULL;
    }
}

void updateUser() {
    string update_username, update_password;
    cout << endl << "Enter Username whose Record you want to Update : ";
    cin >> update_username;
    cout << endl << "Enter Password : ";
    cin >> update_password;
    User* curr = firstUser;
    while (curr != NULL)
    {
        if (curr->username == update_username && curr->password == update_password)
        {
            cout << endl << "Enter new Username : ";
            cin >> curr->username;
            cout << endl << "Enter new Password : ";
            cin >> curr->password;
            cout << endl << "Enter new First Name : ";
            cin >> curr->firstName;
            cout << endl << "Enter new Last Name : ";
            cin >> curr->lastName;
            cout << endl << "Enter new Phone No. : ";
            cin >> curr->phoneNo;
            cout << endl << "Enter new Email Address : ";
            cin >> curr->email;
            break;
        }
        curr = curr->next;
    }
    if (curr == NULL)
    {
        cout << "User not Found whose Record you want to Update";
    }
}

void deleteUser() {
    string delete_username, delete_password;
    cout << endl << "Enter Username whose Record you want to Delete : ";
    cin >> delete_username;
    cout << endl << "Enter Password : ";
    cin >> delete_password;
    User* curr = firstUser;
    while (curr != NULL)
    {
        if (curr->username == delete_username && curr->password == delete_password)
        {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            delete curr;
            cout << "User Record Deleted";
            break;
        }
        curr = curr->next;
    }
    if (curr == NULL)
    {
        cout << "User not Found whose Record you want to Update";
    }
}


void addEmployee() {
    Employee* curr = new Employee;
    curr->next = NULL;
    cout << endl << "Enter Username : ";
    cin >> curr->username;
    cout << endl << "Enter Password : ";
    cin >> curr->password;
    cout << endl << "Enter First Name : ";
    cin >> curr->firstName;
    cout << endl << "Enter Last Name : ";
    cin >> curr->lastName;
    cout << endl << "Enter Phone No. : ";
    cin >> curr->phoneNo;
    cout << endl << "Enter Email Address : ";
    cin >> curr->email;
    if (firstEmployee == NULL)
    {
        firstEmployee = lastEmployee = curr;
    }
    else {
        lastEmployee->next = curr;
        curr->prev = lastEmployee;
        lastEmployee = curr;
    }
}

void DisplayEmployee() {
    if (firstEmployee == NULL)
    {
        cout << "No Record Entered";
    }
    else {
        Employee* curr = firstEmployee;
        printf("%-15s%-15s%-15s%-15s%-20s%-30s", "Username", "Password", "First Name", "Last Name", "Phone No", "E-mail");
        cout << endl;
        while (curr != NULL)
        {

            printf("%-15s%-15s%-15s%-15s%-20s%-30s", curr->username.c_str(),
                curr->password.c_str(), curr->firstName.c_str(), curr->lastName.c_str(),
                curr->phoneNo.c_str(), curr->email.c_str());
            cout << endl;
            curr = curr->next;
        }
    }
}

void SearchEmployee() {
    string search;
    cout << "Enter Employee's Username you want to Search : ";
    cin >> search;
    Employee* curr = firstEmployee;
    while (curr != NULL)
    {
        if (curr->username == search)
        {
            cout << "Employee Record Found" << endl;
            printf("%-15s%-15s%-15s%-15s%-20s%-30s", "Username", "Password", "First Name", "Last Name", "Phone No", "E-mail");
            cout << endl;
            printf("%-15s%-15s%-15s%-15s%-20s%-30s", curr->username.c_str(),
                curr->password.c_str(), curr->firstName.c_str(), curr->lastName.c_str(),
                curr->phoneNo.c_str(), curr->email.c_str());
            cout << endl;

            break;
        }
        curr = curr->next;
    }
    if (curr == NULL)
    {
        cout << "Record not Found";
    }
}


void UpdateEmployee() {
    string update_username, update_password;
    cout << endl << "Enter Username whose Record you want to Update : ";
    cin >> update_username;
    cout << endl << "Enter Password : ";
    cin >> update_password;
    Employee* curr = firstEmployee;
    while (curr != NULL)
    {
        if (curr->username == update_username && curr->password == update_password)
        {
            cout << endl << "Enter new Username : ";
            cin >> curr->username;
            cout << endl << "Enter new Password : ";
            cin >> curr->password;
            cout << endl << "Enter new First Name : ";
            cin >> curr->firstName;
            cout << endl << "Enter new Last Name : ";
            cin >> curr->lastName;
            cout << endl << "Enter new Phone No. : ";
            cin >> curr->phoneNo;
            cout << endl << "Enter new Email Address : ";
            cin >> curr->email;
            break;
        }
        curr = curr->next;
    }
    if (curr == NULL)
    {
        cout << "Employee not Found whose Record you want to Update";
    }
}

void DeleteEmployee() {
    string delete_username, delete_password;
    cout << endl << "Enter Username whose Record you want to Delete : ";
    cin >> delete_username;
    cout << endl << "Enter Password : ";
    cin >> delete_password;
    Employee* curr = firstEmployee;
    while (curr != NULL)
    {
        if (curr->username == delete_username && curr->password == delete_password)
        {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
            delete curr;
            cout << "User Record Deleted";
            break;
        }
        curr = curr->next;
    }
    if (curr == NULL)
    {
        cout << "Employee not Found";
    }
}

bool employeeLOGIN() {
    if (firstEmployee == NULL) {
        return true;
    }
    string U_Name;
    string U_Password;
    cout << "Enter Employee Username" << endl;
    cin >> U_Name;
    cout << "Enter Employee Password" << endl;
    cin >> U_Password;
    Employee* curr_E = firstEmployee;
    while (curr_E != NULL) {
        if (curr_E->username == U_Name) {
            if (curr_E->password == U_Password) {
                return true;
            }
        }
        curr_E = curr_E->next;
    }
    if (curr_E == NULL)
    {
        return false;
    }
}

int employeeCount() {
    Employee* curr = firstEmployee;
    int count = 0;
    while (curr != NULL) {
        curr = curr->next;
        count++;
    }
    return count;
}

void AddPackage(User* curr) {
    cout << curr->firstName + " " + curr->lastName + " please enter your Package details" << endl;
    Package* curr_package = new Package();
    curr->package = curr_package;
    cout << "ADD PACKAGE DETAILS" << endl;
    int option_FROM;
    while (true) {

        cout << "Select trip FROM City:\n"
            "1.  New York\n"
            "2.  Colorado\n"
            "3.  California\n"
            "4.  Texas\n"
            "5.  Alabama\n"
            "6.  Georgia\n"
            "7.  Arkansas\n"
            "8.  Florida\n"
            "9.  Alaska\n"
            "10. Arizona" << endl;
        cin >> option_FROM;
        if (option_FROM > 0 && option_FROM < 11) {
            curr_package->From = CITIES[option_FROM - 1];
            break;
        }
        else {
            cout << "Enter Valid City" << endl;
        }
    }

    while (true) {
        int option_TO;
        cout << "Select trip FROM City:\n"
            "1.  New York\n"
            "2.  Colorado\n"
            "3.  California\n"
            "4.  Texas\n"
            "5.  Alabama\n"
            "6.  Georgia\n"
            "7.  Arkansas\n"
            "8.  Florida\n"
            "9.  Alaska\n"
            "10. Arizona" << endl;
        cin >> option_TO;
        if (option_TO > 0 && option_TO < 11 && option_FROM != option_TO) {
            curr_package->To = CITIES[option_TO - 1];
            break;
        }
        else if (option_FROM == option_TO) {
            cout << "Your trip FROM and TO City cannot be same " << endl;
        }
        else {
            cout << "Enter Valid City" << endl;
        }
    }

    while (true) {
        cout << "Enter \n"
            "1. For 1 way Trip\n"
            "2. For Return trip" << endl;
        int trip_status;
        cin >> trip_status;
        if (trip_status == 1) {
            curr_package->return_trip = false;
            break;
        }
        else if (trip_status == 2) {
            curr_package->return_trip = true;
            break;
        }
        else {
            cout << "Enter Valid input" << endl;
        }
    }

    cout << "Enter Number of seats :" << endl;
    cin >> curr_package->capacity;

}

void DeletePackage(User* curr) {
    Package* curr_package = curr->package;
    delete curr_package;
    curr->package = NULL;
    cout << "Package Deleted successfully" << endl;
}

void ViewPackage(User* curr) {
    Package* curr_package = curr->package;

    printf("%-14s%-12s%-15s%-15s", "TRIP FROM", "TRIP TO", "TRIP STATUS", "CAPACITY");
    cout << endl;
    string from = curr_package->From;
    string to = curr_package->To;
    string status;
    if (curr_package->return_trip) {
        status = "return trip";
    }
    else {
        status = "1 way";
    }
    printf("%-11s-> %-12s%-15s%-15i", from.c_str(), to.c_str(), status.c_str(), curr_package->capacity);
    cout << endl;

}

void  UpdatePackage(User* curr) {
    Package* curr_package = curr->package;
    cout << " " << endl;
    int FROMopt;
    while (true) {

        cout << "Select take off City(FROM) for your trip: \n"
            "1.  New York\n"
            "2.  Colorado\n"
            "3.  California\n"
            "4.  Texas\n"
            "5.  Alabama\n"
            "6.  Georgia\n"
            "7.  Arkansas\n"
            "8.  Florida\n"
            "9.  Alaska\n"
            "10. Arizona" << endl;
        cin >> FROMopt;
        if (FROMopt > 0 && FROMopt < 11) {
            curr_package->From = CITIES[FROMopt - 1];
            break;
        }
        else {
            cout << "Enter Valid City" << endl;
        }

    }

    while (true) {
        int TOopt;
        cout << "Select destination City(TO) for your trip: \n"
            "1.  New York\n"
            "2.  Colorado\n"
            "3.  California\n"
            "4.  Texas\n"
            "5.  Alabama\n"
            "6.  Georgia\n"
            "7.  Arkansas\n"
            "8.  Florida\n"
            "9.  Alaska\n"
            "10. Arizona" << endl;
        cin >> TOopt;
        if (TOopt > 0 && TOopt < 11 && FROMopt != TOopt) {
            curr_package->To = CITIES[TOopt - 1];
            break;
        }
        else if (FROMopt == TOopt) {
            cout << "Your trip FROM and TO City cannot be same " << endl;
        }
        else {
            cout << "Enter Valid City" << endl;
        }

    }
    while (true) {
        cout << "Enter \n"
            "1. For 1 way Trip\n"
            "2. For Return trip" << endl;
        int trip_status;
        cin >> trip_status;
        if (trip_status == 1) {
            curr_package->return_trip = false;
            break;
        }
        else if (trip_status == 2) {
            curr_package->return_trip = true;
            break;
        }
        else {
            cout << "Enter Valid input" << endl;
        }
    }
    cout << "Enter Number of seats :" << endl;
    cin >> curr_package->capacity;

}

void ConfirmTour(User* curr) {
    Package* curr_package = curr->package;
    if (!TourConfirmation) {
        Company->To_Assign_Organizer.push(curr_package);
        TourConfirmation = true;
    }
    else if (TourConfirmation && curr_package->organizer == NULL) {
        cout << "Your Trip request is already forwarded to admin\n" << endl;
    }
    else if (curr_package->organizer != NULL) {
        cout << "Your trip request is confirmed please check your organizer details " << endl;
    }
}

void calculateCost(User* curr) {
    Package* curr_package = curr->package;
    int option;
    while (true) {
        cout << "Enter Preference :\n"
            "1. COST EFFECTIVE TRIP\n"
            "2. TIME EFFECTIVE TRIP" << endl;
        cin >> option;
        if (option == 1) {
            curr_package->preferance = "COST";
            break;
        }
        else if (option == 2) {
            curr_package->preferance = "TIME";
            break;
        }
        else {
            cout << "Enter Valid Option" << endl;
        }
    }

    string Status;
    int Cost_Status;
    if (curr_package->return_trip == true) {
        Status = "Return";
        Cost_Status = 2;
    }
    else {
        Status = "1 way";
        Cost_Status = 1;
    }
    if (curr_package->preferance == "COST") {

        for (int i = 0; i < NO_OF_CITIES; i++) {
            CST[i] = infinity;
            visited[i] = 0;
            TIM[i] = infinity;

        }
        CST[cityIndex(curr_package->From)] = 0;
        TIM[cityIndex(curr_package->From)] = 0;

        for (int x = 0; x < NO_OF_CITIES; x++) {
            int min_dist = infinity, index;

            for (int j = 0; j < NO_OF_CITIES; j++) {
                if (visited[j] == 0) {

                    if (CST[j] <= min_dist) {
                        min_dist = CST[j];
                        index = j;
                    }
                }
            }
          cout << "index: " << index << endl;
            visited[index] = 1;
            for (int k = 0; k < NO_OF_CITIES; k++) {
                if (visited[k] == 0 && COST[index][k] > 0 && CST[index] != infinity) {
                    if (CST[index] + COST[index][k] < CST[k]) {
                        CST[k] = CST[index] + COST[index][k];
                        cout << "k: " << k << "  index:  " << index << "cost " << CST[k];
                        TIM[k] = TIM[index] + TIME[index][k];
                    }
                }
            }
        }

        printf("%-14s%-12s%-20s%-15s%-15s%-15s%-15s", "TRIP FROM", "TRIP TO", "COST(1 way/1 seat)", "TIME(1 way)", "YOUR STATUS", "CAPACITY", "TOTAL COST");
        cout << endl;
        printf("%-11s-> %-12s%-20i%-15i%-15s%-15i%-15i", curr_package->From.c_str(), curr_package->To.c_str(),
            CST[cityIndex(curr_package->To)], TIM[cityIndex(curr_package->To)], Status.c_str(), curr_package->capacity,
            Cost_Status * curr_package->capacity * CST[cityIndex(curr_package->To)]);
        cout << endl;
    }
    else {
        for (int i = 0; i < NO_OF_CITIES; i++) {
            TIM[i] = infinity;
            visited[i] = 0;
            CST[i] = infinity;

        }
        CST[cityIndex(curr_package->From)] = 0;
        TIM[cityIndex(curr_package->From)] = 0;

        for (int x = 0; x < NO_OF_CITIES; x++) {
            int min_dist = infinity, index;

            for (int j = 0; j < NO_OF_CITIES; j++) {
                if (visited[j] == 0) {
                    if (TIM[j] <= min_dist) {
                        min_dist = CST[j];
                        index = j;
                    }
                }
            }

            visited[index] = 1;
            for (int k = 0; k < NO_OF_CITIES; k++) {
                if (visited[k] == 0 && TIME[index][k] > 0 && TIM[index] != infinity) {
                    if (TIM[index] + TIME[index][k] < TIM[k]) {
                        TIM[k] = TIM[index] + TIME[index][k];
                        CST[k] = CST[index] + COST[index][k];
                    }
                }
            }
        }

        printf("%-14s%-12s%-20s%-15s%-15s%-15s%-15s", "TRIP FROM", "TRIP TO", "COST(1 way/1 seat)", "TIME(1 way)", "YOUR STATUS", "CAPACITY", "TOTAL COST");
        cout << endl;
        printf("%-11s-> %-12s%-20i%-15i%-15s%-15i%-15i", curr_package->From.c_str(), curr_package->To.c_str(),
            CST[cityIndex(curr_package->To)], TIM[cityIndex(curr_package->To)], Status.c_str(), curr_package->capacity,
            Cost_Status * curr_package->capacity * CST[cityIndex(curr_package->To)]);
        cout << endl;

    }
}
void viewOrganizer(User* curr) {
    Package* curr_package = curr->package;
    if (curr_package->organizer == NULL) {
        cout << "Organizer Not Assigned Yet" << endl;
    }
    else {
        Employee* curr_Emp = curr_package->organizer;
        cout << "Organizer Info" << endl;

        printf("%-15s%-15s%-20s%-30s", "First Name", "Last Name", "Phone No", "E-mail");
        cout << endl;

        printf("%-15s%-15s%-20s%-30s", curr->package->organizer->firstName.c_str(), curr->package->organizer->lastName.c_str(), curr->package->organizer->phoneNo.c_str(), curr->package->organizer->email.c_str());
        cout << endl;

    }
}


int cityIndex(string CITY) {
    for (int i = 0;i < NO_OF_CITIES;i++) {
        if (CITY == CITIES[i]) {
            return i;
        }
    }
    return 0;
}

int main() {
    int option;
    while (true) {
        cout << "MAIN MENU\n"
            "1. ADMIN\n"
            "2. USER\n"
            "3. TOURS \n"
            "4. EXIT" << endl;
        cin >> option;
        if (option == 1) {
            if (employeeLOGIN()) {
                int A_option;
                while (true) {
                    cout << "ADMIN MENU\n"
                        "1. ADD/UPDATE COMPANY\n"
                        "2. EMPLOYEE MENU\n"
                        "3. ASSIGN ORGANIZERS\n"
                        "4. DISPLAY PREVIOUS TRIPS\n"
                        "5. MAIN MENU" << endl;
                    cin >> A_option;
                    if (A_option == 1) {
                        cout << "ADMIN ENTER/UPDATE" << endl;
                        EnterUpdateAdmin();
                    }
                    else if (!Admin_data && A_option != 5) {
                        cout << "First enter Company data" << endl;
                    }
                    else if (A_option == 2) {
                        int E_option;
                        while (true) {
                            cout << "EMPLOYEE MENU\n"
                                "1. ADD EMPLOYEE\n"
                                "2. DISPLAY EMPLOYEE\n"
                                "3. SEARCH EMPLOYEE\n"
                                "4. UPDATE EMPLOYEE\n"
                                "5. DELETE EMPLOYEE\n"
                                "6. ADMIN MENU" << endl;
                            cin >> E_option;
                            if (E_option == 1) {
                                cout << "ADD EMPLOYEE" << endl;
                                addEmployee();
                            }
                            else if (E_option == 2) {
                                cout << "DISPLAY EMPLOYEE" << endl;
                                DisplayEmployee();
                            }
                            else if (E_option == 3) {
                                cout << "SEARCH EMPLOYEE" << endl;
                                SearchEmployee();
                            }
                            else if (E_option == 4) {
                                cout << "UPDATE EMPLOYEE" << endl;
                                UpdateEmployee();
                            }
                            else if (E_option == 5) {
                                cout << "DELETE EMPLOYEE" << endl;
                                DeleteEmployee();
                            }
                            else if (E_option == 6) {
                                break;
                            }
                            else {
                                cout << "Enter a valid Option" << endl;
                            }
                        }
                    }
                    else if (A_option == 3) {
                        cout << "ASSIGN ORGANIZERS TO PENDING REQUESTS" << endl;
                        assignOrganizer();
                    }
                    else if (A_option == 4) {
                        cout << "DISPLAY TRIP PACKAGES" << endl;
                        DisplayPrevTrips();
                    }
                    else if (A_option == 5) {
                        break;
                    }
                    else {
                        cout << "Enter Valid Option" << endl;
                    }
                }
            }
            else {
                cout << "Wrong Username or password" << endl;
            }
        }
        else if (!Admin_data && option != 4) {
            cout << "First Enter Admin Data " << endl;
        }
        else if (option == 2) {
            int U_option;
            while (true) {
                cout << "USER MENU\n"
                    "1. ADD USER\n"
                    "2. DISPLAY USER\n"
                    "3. SEARCH USER\n"
                    "4. UPDATE USER\n"
                    "5. DELETE USER\n"
                    "6. MAIN MENU" << endl;
                cin >> U_option;
                if (U_option == 1) {
                    cout << "ADD USER" << endl;
                    addUser();
                }
                else if (U_option == 2) {
                    cout << "DISPLAY USER" << endl;
                    displayUser();
                }
                else if (U_option == 3) {
                    cout << "SEARCH USER" << endl;
                    searchUser();
                }
                else if (U_option == 4) {
                    cout << "UPDATE USER" << endl;
                    updateUser();
                }
                else if (U_option == 5) {
                    cout << "DELETE USER" << endl;
                    deleteUser();
                }
                else if (U_option == 6) {
                    break;
                }
                else {
                    cout << "Enter a valid Option" << endl;
                }
            }
        }
        else if (option == 3) {
            cout << "USER LOGIN" << endl;
            User* Node = userLogin();
            if (Node != NULL) {
                int P_option;
                while (true) {
                    cout << "PACKAGE MENU\n"
                        "1. ADD PACKAGE\n"
                        "2. VIEW PACKAGE\n"
                        "3. UPDATE PACKAGE\n"
                        "4. DELETE PACKAGE\n"
                        "5. CALCULATE COST\n"
                        "6. CONFIRM TOUR  Note : Once Package is confirmed Package details can not be changed\n"
                        "7. VIEW TOUR ORGANIZER\n"
                        "8. MAIN MENU" << endl;
                    cin >> P_option;
                    if (P_option == 1) {
                        if (Node->package == NULL) {
                            AddPackage(Node);
                        }
                        else {
                            cout << "Complete or delete previous trip package to add a new one " << endl;
                        }
                    }
                    else if (Node->package == NULL) {
                        cout << "First Enter Package" << endl;
                    }
                    else if (P_option == 2) {
                        ViewPackage(Node);
                    }
                    else if (P_option == 3) {
                        UpdatePackage(Node);
                    }
                    else if (P_option == 4) {
                        if (Node->package != NULL) {
                            DeletePackage(Node);
                        }
                        else {
                            cout << "No trip package found" << endl;
                        }
                    }
                    else if (P_option == 5) {
                        calculateCost(Node);
                    }
                    else if (P_option == 6) {
                        ConfirmTour(Node);
                    }
                    else if (P_option == 7) {
                        viewOrganizer(Node);
                    }
                    else if (P_option == 8) {
                        break;
                    }
                    else {
                        cout << "Enter valid option" << endl;
                    }
                }
            }
            else {
                cout << "Wrong Username or password" << endl;
            }
        }
        else if (option == 4) {
            cout << " Program End Here" << endl;
            break;
        }
        else {
            cout << " Enter a Valid Option" << endl;
        }
    }
}