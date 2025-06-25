# Employee Management System

A simple command-line Employee Management System that allows Admins, HR, and Employees to manage organizational data efficiently.

## Features

- Add, view, update, and delete employee records
- Export employee data and payslips to CSV
- Payroll calculation and reporting
- Secure login for Admin, HR, and Employee roles
- Audit log for admin actions
- HR comment tracking for employees
- Data persistence using files
- Input validation for all user and salary inputs

## Upcoming Features

- **Password Change Functionality:** Employees and HR will be able to securely change their passwords after login, enhancing account security.
- **Multi-file Project Structure:** The codebase will be refactored from a single file to a modular, multi-file structure for easier maintenance and scalability.

## Recent Updates

- Enhanced input validation for names, roles, and salaries
- Improved user experience with clear prompts and error handling
- Added audit log functionality for tracking important actions
- HR and Employee menus refactored for better usability
- Bug fixes and code clean-up
- Added code comments for clarity and maintainability

## Usage

1. Compile the program using a C++ compiler:
    ```
    g++ -o ems main.cpp
    ```
    *(For multi-file version, use: `g++ -o ems *.cpp` or specify all `.cpp` files)*
2. Run the executable:
    ```
    ./ems
    ```
3. Follow the on-screen instructions to select your role and perform operations.

## File Structure

- `main.cpp` — Main source code *(will be split into multiple files in future updates)*
- `Employees.txt` — Persistent employee data
- `users.txt` — User login information
- `audit_log.txt` — Admin and HR actions log
- `Employee_Info.csv` — Exported employee summary
- `payslip_<id>.csv` — Individual employee payslips

## Default Admin Credentials

- Username: admin
- Password: admin123

> **Note:** For security, all users should change their default passwords after first login (feature coming soon).

## License

MIT License
