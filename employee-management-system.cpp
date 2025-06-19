/*
  Project: Employee Management System
  Created by: silentSoul-cloud
  Description: Simple Admin/HR/Employee system with Add ,view, update, delete, export,payroll handling etc
  Date: 17/06/2025
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <map>
#include <iomanip>
#include <algorithm>
using namespace std;

class Employee{
private:
    int id;
    string name;
    string title;
    string role; // part-time/Intern /Full-time
    double salary;
    string joinDate;
    string hrComments;

public:
    Employee() {}

    int getId() const { return id; }
    string getName() const { return name; }
    double getSalary() const { return salary; }
    string getTitle() const { return title; }
    string getRole() const { return role; }
    string getComments() const { return hrComments; }
    void setComments(const string &comments) { hrComments = comments; }

    void addEmploye(int ID, string Name, string Title, string Role, double Salary){
        id = ID;
        name = Name;
        title = Title;
        role = Role;
        salary = Salary;

        time_t ti = time(0);
        tm *date = localtime(&ti);
        joinDate = to_string(1900 + date->tm_year) + "-" + to_string(1 + date->tm_mon) + "-" + to_string(date->tm_mday);
    }

    void viewEmployee() const{
        cout << "\n--- Employee info ---\n";
        cout << "ID: " << id << "\nName: " << name << "\nDepartment: " << title << "\nRole: " << role << "\nSalary: $" << salary << "\nJoin Date: " << joinDate << endl;
    }

    void updateEmployee(){
        int opt;
        string c;
        cout << "\n--- Update Employee ---\n";
        cout << "1. Update Department\n"
             << "2. Update Role(part-time/Intern /Full-time)" << endl
             << "3. Update Salary" << endl;
        cin >> opt;
        cin.ignore();

        if (opt == 1){
            cout << "New Title: ";
            getline(cin, title);
        }
        else if (opt == 2){
            cout << "New Role: ";
            getline(cin, role);
        }
        else if (opt == 3){
            cout << "New Salary: ";
            cin >> salary;
        }
        else{
            cout << "Invalid choice.\n";
        }
        cout << "Do you want to add a comment for this Employee (y/n):";
        cin >> c;
        cin.ignore();
        if (c == "Y" || c == "y"){
            cout << "Enter comment: ";
            getline(cin, hrComments);
        }else cout<<"No comments";
        logAction("Updated Employee ID " + to_string(id) + " (" + name + "): " + hrComments);
    }

    void payslip(){
        double tax = salary * 0.10; // 10%
        double net = salary - tax;
        cout << "\n--- Payslip ---\n";
        cout << "Name: " << name << "\nTitle:" << title << "\nRole: " << role << "\nGross: $" << salary << "\nNet gross(Deducted 10%): $" << net << endl;
    }

    void report(){
        ofstream out("payslip_" + to_string(id) + ".txt");
        double tax = salary * 0.10;
        double net = salary - tax;

        out << "Name: " << name << "\nTitle: " << title << "\nRole: " << role << "\nSalary: $" << salary
            << "\nJoin Date: " << joinDate << "\nTax: $" << tax << "\nNet Salary: $" << net << "\n";
        out.close();
        cout << "Payslip exported.\n";
    }

    int months() const{
        int y, m, d;
        sscanf(joinDate.c_str(), "%d-%d-%d", &y, &m, &d);
        time_t ti = time(0);
        tm *date = localtime(&ti);
        int total = (date->tm_year + 1900 - y) * 12 + ((date->tm_mon + 1) - m);
        return (total < 0 ? 0 : total);
    }

    string writeOut() const{
        return to_string(id) + "|" + name + "|" + title + "|" + role + "|" + to_string(salary) + "|" + joinDate + "|" + hrComments;
    }

    void readOut(const string &line){
        stringstream ss(line);
        string salaryStr, idStr;
        getline(ss, idStr, '|');
        id = stoi(idStr);
        getline(ss, name, '|');
        getline(ss, title, '|');
        getline(ss, role, '|');
        getline(ss, salaryStr, '|');
        salary = stod(salaryStr);
        getline(ss, joinDate, '|');
        getline(ss, hrComments);
    }
};
//I was going to use vector but map gives better lookup for ID
map<int, Employee> employees;

void deleteEmp(int id){
    string c;
    cout << "Are you sure you want to Delete the info? (Y/N): ";
    cin >> c;
    cin.ignore();
    if(c == "Y" || c == "y"){
        if (employees.count(id)) {
            string empName = employees[id].getName();
            cout << "Do you want to add a comment for this Employee (y/n): ";
            string opt;
            cin >> opt;
            cin.ignore();
                    
            string comment = "";
            if (opt == "Y" || opt == "y"){
                cout << "Enter comment: ";
                getline(cin, comment);
            }
            logAction("Deleted Employee ID " + to_string(id) + " (" + empName + "): " + comment);
            employees.erase(id);
            save();
            cout << "Employee data has been Deleted.\n";
        } else cout << "Employee not found.\n";
    }
    else cout << "Delete cancelled.\n";
}


//Save and load data
void save(){
    ofstream out("Employees.txt");
    for (auto &empPair : employees){ //Don't know what to do to replace this empPair
        out << empPair.second.writeOut() <<endl;
    }
    out.close();
}

void load(){
    ifstream in("Employees.txt");
    if (!in){
        cout << "Warning: 'Employees.txt' not found.\nStarting fresh....\n"
             << endl;
        return;
    }
    string line;
    while (getline(in, line)){
        Employee emp;
        emp.readOut(line);
        employees[emp.getId()] = emp;
    }
    in.close();
}
//for admin only
void showAuditLog() {
    ifstream file("audit_log.txt");
    if (!file.is_open()) {
        cout << "\nNo audit log found yet.\n";
        return;
    }

    string line;
    cout << "\n=== Audit Log ===\n";
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void showAllHRComments() {
    cout << "\n--- HR Comments for All Employees ---\n";
    for (auto& pair : employees) {
        cout << "ID: " << pair.first << " | Name: " << pair.second.getName() << "\n";
        cout << "Comment: " << pair.second.getComments() << "\n\n";
    }
}


void logAction(const string &msg) {
    time_t date = time(0);
    tm *ltm = localtime(&date);
    char clock[32];
    strftime(clock, sizeof(clock), "%Y-%m-%d %H:%M:%S", ltm);

    string logLine = "[" + string(clock) + "] " + msg;
    ofstream logFile("audit_log.txt", ios::app); //save
    if (logFile.is_open()) {
        logFile<<logLine <<endl;
        logFile.close();
    }
    cout<<logLine<<endl;
}


void clearAllEmployeeData() {
    cout << "\nAre you 100% sure you want to delete all employee records? This cannot be undone.\n";
    cout << "Type 'YES' to proceed: ";
    string confirm;
    getline(cin, confirm);

    if (confirm == "YES" || confirm =="Yes") {
        employees.clear();
        ofstream wipe("Employees.txt", ios::trunc); //Clear all files
        wipe.close();
        cout << "✔ All employee records have been wiped clean.\n";
    } else {
        cout << "Reset cancelled.\n";
    }
}

void HR_accounts() {
    int option;
    cout << "\n--- HR Management ---\n";
    cout << "1. Add HR\n2. Remove HR\nChoose : ";
    cin >> option;
    cin.ignore();

    string hrName;
    cout << "Enter HR name: ";
    getline(cin, hrName);

    if (option == 1) {
        ofstream out("hrs.txt", ios::app);
        out << hrName << '\n';
        out.close();
        cout << "HR \""<< hrName <<"\" has been added.\n";
    } 
    else if (option == 2) {
        ifstream in("hrs.txt");
        ofstream temp("temp_hrs.txt");
        string name;
        int removed = 0;
        while (getline(in, name)) {
            if (name != hrName) {
                temp<<name << '\n';
            } else removed = 1;
        }
        in.close();
        temp.close();
        remove("hrs.txt");
        rename("temp_hrs.txt", "hrs.txt");

        if (removed){
            cout << "HR \"" << hrName << "\" removed successfully.\n";
        } else cout << "HR not found.\n";
    } 
    else cout << "Invalid option.\n";
}


//HR menu panel
void HR(){
    cout << "\nWelcome to Human Resource Menu\n";
    int opt;
    do{
        cout << "\n--- HR Panel ---\n";
        cout << "1. Add Employee\n";
        cout << "2. View All Employees\n";
        cout << "3. Update the employee Data\n";
        cout << "4. Total PayRoll\n";
        cout << "5. Export Expense\n";
        cout << "6. Delete Employee\n";
        cout << "7. Exit\n";
        cout << "Your Choice : ";
        cin >> opt;
        cin.ignore();

        if (opt == 1){
            Employee emp;
            int id;
            string name, title, role;
            double salary;
            cout << "Enter employee ID : ";
            while (!(cin >> id) || id < 0) {
                cout << "Invalid ID. Enter a positive number: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            cin.ignore();
            if (employees.count(id)){
                cout << "ID already exists . Type again.\n";
                continue;
            }
            cout << "Enter Name : ";
            getline(cin, name);
            while (name.empty()) {
                cout << "The Employee Name cannot be empty. Enter again : ";
                getline(cin, name);
            }

            cout << "Enter Department: ";
            getline(cin, title);

            cout << "Enter Role (Part-time, Intern, Full-time): ";
            getline(cin, role);
            transform(role.begin(), role.end(), role.begin(), ::tolower);
            while (role != "intern" && role != "part-time" && role != "full-time") {
                cout << "Invalid role. Choose from: Part-time, Intern, Full-time: ";
                getline(cin, role);
                transform(role.begin(), role.end(), role.begin(), ::tolower);
            }

            cout << "Enter Salary: ";
            while (!(cin >> salary) || salary <= 0) {
                cout << "Invalid salary. Enter a reasonable number: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }cin.ignore();

            emp.addEmploye(id, name, title, role, salary);
            employees[id] = emp;
            save();
            cout << "Employee added.\n";
        }
        else if (opt == 2){
            for (auto &empPair : employees){
                empPair.second.viewEmployee();
            }
        }
        else if (opt == 3){
            int id;
            cout << "Enter ID to update: ";
            cin >> id;
            if (employees.count(id)){
                employees[id].updateEmployee();
                save();
            }
            else
                cout << "Not found.\n";
        }
        else if (opt == 4){
            double total = 0;
            for (auto &empPair : employees){
                total += empPair.second.getSalary();
            }
            cout << "Total Payroll: $" << fixed << setprecision(2) << total << endl;
        }
        else if (opt == 5){
            double total = 0;
            for (auto &empPair : employees){
                total += empPair.second.getSalary();
            }
            ofstream out("Personnel expenses.txt");
            out << "Total Payroll: $" <<fixed<< setprecision(2) << total << endl;
            out.close();
            cout << "Payroll exported to Personnel expenses.txt\n";
        }
        else if (opt == 6){
            int id;
            string c;
            cout << "Enter ID to delete: ";
            cin >> id;
            deleteEmp(id);
       }

        else if (opt == 7){
            save();
            cout << "Data saved. Exiting HR panel.\n";
            break;
        }
        else
            cout << "Invalid choice.\n";

    } while (1);
}

// Employee Menu panel
void empPanel(){
    cout << "Welcome to Employee Menu\n";
    string name;
    cout << "Enter your name: ";
    getline(cin, name);

    Employee emp;
    int found = 0;
    for (auto &empPair : employees){
        if (empPair.second.getName() == name){
            emp = empPair.second;
            found = 1;
            break;
        }
    }
    if (!found){
        cout << "Employee not found.\n";
        return;
    }

    int opt;
    do{
        cout << "\n--- Employee Panel ---\n";
        cout << "1. View Employee\n2. Payslip\n3. Export Payslip\n4. Total month Pay\n5. Months Worked\n6. Logout\nChoice: ";
        cin >> opt;

        switch (opt){
        case 1:
            emp.viewEmployee();
            break;
        case 2:
            emp.payslip();
            break;
        case 3:
            emp.report();
            break;
        case 4:
        {
            string r = emp.getRole();
            transform(r.begin(), r.end(), r.begin(), ::tolower);
            double mon = 0; //month
            if (r == "part-time")
                mon = 4;

            else if (r == "intern")
                mon = 6;

            else if (r == "full-time")
                mon = 12;

            else{
                cout << "Unknown role. Cannot calculate pay for that.\n";
                break;
            }
            double totalPay = emp.getSalary() * mon;
            cout << "Monthly salary: $"<<fixed << setprecision(2) << emp.getSalary() << "\n";
            cout << "Role-based yearly total: $"<<fixed << setprecision(2) << totalPay << "\n";
            break;
        }
        case 5:
            cout << "You worked for " << emp.months() << " month(s).\n";
            break;
        case 6:
            cout << "Logging out...\n";
            return;
        default:
            cout << "Invalid choice.\n";
        }
    } while (1);
}

//Admin panel d
void adminPanel() {
    int choice;

    do {
        cout << "\n=== Admin Control Panel ===\n";
        cout << "1. View Audit Log\n";
        cout << "2. View All HR Comments\n";
        cout << "3. Reset All Employee Records\n";
        cout << "4. Add / Remove HRs\n";
        cout << "5. Return to Main Menu\n";
        cout << "Your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: showAuditLog(); break;
            case 2: showAllHRComments(); break;
            case 3: clearAllEmployeeData(); break;
            case 4: HR_accounts(); break;
            case 5: return;
            default: cout << "Invalid choice. Try again plz.\n";
        }
    } while (1);
}


// TODO: Not storing passwords need to understand the way to do it.
// TODO: Might add employee login auth later .Needs to understand hashing
// need to add a CSV

// Main menu panel
int main(){
    load();
    cout << "Welcome to Cloud system\n";
    int opt;
    do
    {
        cout << "Select Role:\n1. Admin\n2. HR Department\n3. Employee Account\n4. Exit\nChoice: ";
        cin >> opt;
        cin.ignore();
        if (opt == 1) {
           adminPanel();
        }
        else if (opt == 2){
            HR();
            break;
        }
        else if (opt == 3){
            empPanel();
            break;
        }
        else if (opt == 4){
            cout << " Thank you for using this system. :-> \n";
            cout << "Good bye";
            return 0;
        }
        else cout << "Invalid role. NOT FOUND\n";
    } while (1);
}
