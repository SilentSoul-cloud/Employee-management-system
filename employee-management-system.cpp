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
#include <random>
#include <map>
#include <functional>
#include <iomanip>
#include <algorithm>
using namespace std;

void logAction(const string &msg);
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
             << "2. Update Role(part-time/Intern /Full-time)\n"
             << "3. Update Salary\n"
             << "4. Back" <<endl;
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
        else if (opt == 4){
            cout << "Update cancelled.\n";
            return;
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
        }
        else {
            hrComments = "No comments";
            cout << "No comments";
        }
        logAction("Updated Employee ID " + to_string(id) + " (" + name + "): " + hrComments);
    }

    void payslip(){
        double tax = salary * 0.10; // 10% tax
        double net = salary - tax;
        cout << "\n--- Payslip ---\n";
        cout << "Name: " << name << "\nDepartment:" << title << "\nRole: " << role << "\nGross: $" << salary << "\nNet gross(Deducted 10%): $" << net << endl;
    }

    void report(){
        ofstream out("payslip_" + to_string(id) + ".csv");
        double tax = salary * 0.10;
        double net = salary - tax;

        out << "Name, Department, Role, Salary, Join Date, Tax, Net Salary\n";
        out << name << "," << title << "," << role << "," << salary << "," << joinDate << "," << tax << "," << net << "\n";
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
        cout << "\nWarning: 'Employees.txt' not found.\nStarting fresh....\n"
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


int lastID() {
    ifstream in("last_id.txt");
    if (!in) return 0;
    int id;
    in >> id;
    return id;
}

void saveLastID(int id) {
    ofstream out("last_id.txt");
    out << id;
}

int newID(const string& role) {
    int base = 0;
    if (role == "employee") base = 1000;
    else if (role == "hr") base = 500;
    else {
        cerr << "Error: Unknown role passed to newID().\n";
        exit(1);
    }
    int lastUsed = lastID();
    saveLastID(lastUsed + 1);
    return base + lastUsed + 1;
}

string encp(const string &pass) {
    hash<string> hasher;
    return to_string(hasher(pass));
}

void addUser(const string& loginName,int id,const string& defaultPass,const string& role){
    ofstream off("users.txt", ios::app);
    // loginName|id|hashedPass|role|changedFlag
    off << loginName << "|"<< id << "|"<< encp(defaultPass)<< "|"<< role<< "|0\n";
    cout << "User " << loginName << " (ID " << id << ") created. Default pass: " << defaultPass << "\n";
}


void user(const string &role) {
    int id = newID(role);
    string name, title, r;
    double salary;
    cout << "\nEnter Name: "; getline(cin, name);

    while (name.empty()) {
         cout << "Name cannot be empty. Try again: "; getline(cin, name); 
    }

    cout << "Enter Department: ";
    getline(cin, title);
    cout << "Enter Role category (Intern/Part-time/Full-time): ";
    getline(cin, r);
    transform(r.begin(), r.end(), r.begin(), ::tolower);

    while (r != "intern" && r != "part-time" && r != "full-time") {
        cout << "Invalid role. Try again: ";
        getline(cin, r);
        transform(r.begin(), r.end(), r.begin(), ::tolower);
    }
    cout << "Enter Salary: ";
    while (!(cin >> salary) || salary <= 0) {
        cout << " Unreasonable salary. Try again: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cin.ignore();

    Employee emp;
    emp.addEmploye(id, name, title, r, salary);
    employees[id] = emp;
    save();

    addUser(name, id, "Welcome123", role);
    logAction("Created new " + role + " ID " + to_string(id) + " (" + name + ")");
}

void deleteEmp(int id){
    auto it = employees.find(id);
    if (it == employees.end()) {
        cout << "Employee not found.\n";
        return;
    }

    string empName = it->second.getName();
    cout << "Name: " << empName << ", Role: " << it->second.getRole() << endl;

    string c;
    cout << "Are you sure you want to Delete this employee? (Y/N): ";
    cin >> c;

    if(c != "Y" && c != "y") {
        cout << "Delete cancelled.\n";
        return;
    }

    string opt, comment = "No comment provided";
    cout << "Do you want to add a comment for this Employee (y/n): ";
    cin >> opt;
    cin.ignore();
    
    if (opt == "Y" || opt == "y") {
        cout << "Enter comment: ";
        getline(cin, comment);
    }
    else comment = "No comment provided";

    logAction("Deleted Employee ID " + to_string(id) + " (" + empName + "). Comment: " + comment);
    employees.erase(it);
    save();
    cout << "Employee data has been Deleted.\n";
}

void export_payslip(){
    ofstream out("Employee_Info.csv");
    out << "ID,Name,Department,Role,Salary\n";
    for (auto &empPair : employees){
        out << empPair.second.getId() << ","
        << empPair.second.getName() << ","
        << empPair.second.getTitle() << ","
        << empPair.second.getRole() << ","
        << fixed << setprecision(2) << empPair.second.getSalary() << endl;
    }
    out.close();
    cout << "All employee data exported to 'Employee_Info.csv'.\n";
}

//for admin only
void AuditLog() {
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

void HRComments() {
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

    string logLine = "[" + string(clock) + "] Action: " + msg;
    ofstream logFile("audit_log.txt", ios::app); //save
    if (logFile.is_open()) {
        logFile<<logLine <<endl;
        logFile.close();
    }
}


void clearEmpData() {
    cout << "\nAre you 100% sure you want to delete all employee records? This cannot be undone.\n";
    cout << "Type 'YES' to proceed: ";
    string confirm;
    getline(cin, confirm);

    if (confirm == "YES" || confirm =="Yes") {
        employees.clear();
        ofstream wipe("Employees.txt", ios::trunc); //Clears all files
        wipe.close();
        cout << "✔ All employee records have been wiped clean.\n";
    } else {
        cout << "Reset cancelled.\n";
    }
    save();
}


//HR menu and handles employee data management operations
void HR(){
    cout << "\nWelcome to Human Resource Menu\n";
    int opt;
    do{
        cout << "\n--- HR Panel ---\n";
        cout << "1. Add Employee\n";
        cout << "2. View All Employees\n";
        cout << "3. Update the employee Data\n";
        cout << "4. Total PayRoll\n";
        cout << "5. Export Employee Data\n";
        cout << "6. Delete Employee\n";
        cout << "7. Exit\n";
        cout << "Your Choice : ";
        cin >> opt;
        cin.ignore();

        if (opt == 1){
            user("employee");
            save();
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
            export_payslip();
        }
        else if (opt == 6){
            int id;
            string c;
            cout << "Enter ID to delete: ";
            cin >> id;
            deleteEmp(id);
            save();
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
void empPanel(int id){
    cout << "Welcome to Employee Menu\n";
    string name;

    auto it = employees.find(id);
    if (it == employees.end()){
        cout << "Internal error: your employee record was not found.\n";
        return;
    }
    Employee &emp = it->second;

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

//Admin panel 
void adminPanel() {
    int choice;

    do {
        cout << "\n=== Admin Control Panel ===\n";
        cout << "1. View Audit Log\n";
        cout << "2. View All HR Comments\n";
        cout << "3. Reset All Employee Records\n";
        cout << "4. Add HR\n";
        cout << "4. Return to Main Menu\n";
        cout << "Your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: AuditLog(); break;
            case 2: HRComments(); break;
            case 3: clearEmpData(); break;
            case 4: user("hr"); break;
            case 5: return;
            default: cout << "Invalid choice. Try again plz.\n";
        }
    } while (1);
}

// Main menu panel
int main(){
    load();
    cout << "Welcome to Cloud system\n";
    int opt;
    string role;
    

    while (true) {
        cout << "\nSelect Role:\n" << "1. Admin\n"<< "2. HR Department\n"<< "3. Employee Account\n"<< "4. Exit\n" << "Choice: ";

    if (!(cin >> opt)) {
        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        continue;
    } 
    cin.ignore();

    if (opt == 1) role = "admin";
    else if (opt == 2) role = "hr";
    else if (opt == 3) role = "employee";
    else if (opt == 4) return 0;
    else {
         cout << "Invalid choice.\n"; continue; 
    }

    // --- ADMIN FLOW ---
    if (role == "admin") {
        string user, pass;
        cout << "Username: "; getline(cin, user);
        cout << "Password: "; getline(cin, pass);

        if (user == "admin" && pass == "admin123") {
            cout << "\nLogged in as Admin\n\n";
            adminPanel();
        } else {
            cout << "Wrong admin credentials.\n";
        }
        continue;
    }

    // --- HR / EMPLOYEE FLOW ---
    string user, pass;
    int id;
    cout << "Username: "; 
    getline(cin, user);
    cout << "User ID: ";
    if (!(cin >> id)) {
        cout << "Invalid ID. Please enter a number.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        continue;
    }
    cin.ignore();

    cout << "Password: ";
    getline(cin, pass);

    ifstream file("users.txt");
    if (!file.is_open()) {
        cout << "Cannot open users.txt\n";
        continue;
    }

    bool ok = false,changed = false;
    string line;
    while (getline(file, line)) {
        string fileUser, fileID, filehesh, fileRole, fileChanged;
        stringstream ss(line);
        getline(ss, fileUser,   '|');
        getline(ss, fileID,     '|');
        getline(ss, filehesh,   '|');
        getline(ss, fileRole,   '|');
        getline(ss, fileChanged,'|');

        if (fileUser == user && stoi(fileID) == id && fileRole == role && filehesh == encp(pass)){
            ok = true;
            changed = (fileChanged == "1");
            break;
        }
    }
    file.close();

    if (!ok) {
        cout << "Login failed. Check Id or Password and try again.\n";
        continue;
    }
    if (!changed) {
        cout << "You are still using the default password.\n"
             << "Please change it when you can for safety.\n";
    }

    if (role == "hr") HR();
    else if (role == "employee") empPanel(id);

    cout << "Logged in as " << (role=="hr"?"HR":"Employee") << "\n";
}

}
