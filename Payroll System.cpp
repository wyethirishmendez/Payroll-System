#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

class Worker {
protected:
    string empID, empName;
public:
    Worker(string empID, string empName) : empID(empID), empName(empName) {}
    virtual string getPayroll() const = 0;
    virtual ~Worker() {}
    string getEmpID() const { return empID; }
};

class SalariedWorker : public Worker {
    double fixedSalary;
public:
    SalariedWorker(string empID, string empName, double fixedSalary) 
        : Worker(empID, empName), fixedSalary(fixedSalary) {}
    string getPayroll() const override {
        ostringstream oss;
        oss << "\033[1;35mEmployee:\033[0m " << empName << " (\033[1;33mID: " << empID << "\033[0m)\n";
        oss << "Fixed Monthly Salary: $" << fixedSalary << "\n\n";
        return oss.str();
    }
};

class HourlyWorker : public Worker {
    double wagePerHour;
    int totalHours;
public:
    HourlyWorker(string empID, string empName, double wagePerHour, int totalHours) 
        : Worker(empID, empName), wagePerHour(wagePerHour), totalHours(totalHours) {}
    string getPayroll() const override {
        ostringstream oss;
        double totalPay = wagePerHour * totalHours;
        oss << "\033[1;35mEmployee:\033[0m " << empName << " (\033[1;33mID: " << empID << "\033[0m)\n";
        oss << "Hourly Wage: $" << wagePerHour << "\n";
        oss << "Hours Worked: " << totalHours << "\n";
        oss << "\033[1;33mTotal Salary: $" << totalPay << "\033[0m\n\n";
        return oss.str();
    }
};

class ProjectWorker : public Worker {
    double payPerTask;
    int taskCount;
public:
    ProjectWorker(string empID, string empName, double payPerTask, int taskCount) 
        : Worker(empID, empName), payPerTask(payPerTask), taskCount(taskCount) {}
    string getPayroll() const override {
        ostringstream oss;
        double totalPay = payPerTask * taskCount;
        oss << "\033[1;35mEmployee:\033[0m " << empName << " (\033[1;33mID: " << empID << "\033[0m)\n";
        oss << "Contract Payment Per Project: $" << payPerTask << "\n";
        oss << "Projects Completed: " << taskCount << "\n";
        oss << "\033[1;33mTotal Salary: $" << totalPay << "\033[0m\n\n";
        return oss.str();
    }
};

double getValidDouble(const string& prompt) {
    double val;
    bool valid = false;
    do {
        cout << prompt;
        cin >> val;
        if (cin.fail()) {
            cout << "Invalid input! Enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            valid = true;
        }
    } while (!valid);
    return val;
}

int getValidInt(const string& prompt) {
    int val;
    bool valid = false;
    do {
        cout << prompt;
        cin >> val;
        if (cin.fail() || val <= 0 || cin.peek() != '\n') {
            cout << "Invalid input! Enter a positive whole number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            valid = true;
        }
    } while (!valid);
    return val;
}

int getValidChoice() {
    string input;
    int option;
    bool valid = false;
    do {
        cout << "\nMenu\n"
             << "1 - Full-time Employee\n"
             << "2 - Part-time Employee\n"
             << "3 - Contractual Employee\n"
             << "4 - Display Payroll Report\n"
             << "5 - Exit\n"
             << "Enter your choice: ";
        cin >> input;
        if (input.find_first_not_of("0123456789") == string::npos) {
            option = stoi(input);
            if (option >= 1 && option <= 5) {
                valid = true;
            }
        }
        if (!valid) {
            cout << "Invalid choice! Enter a number between 1 and 5.\n";
        }
    } while (!valid);
    return option;
}

int main() {
    vector<Worker*> staff;
    vector<string> empIDs;
    int option = 0;

    while (option != 5) {
        option = getValidChoice();

        if (option >= 1 && option <= 3) {
            string empID, empName;
            double numericInput;
            int additionalInput;
            bool exists;
            
            do {
                exists = false;
                cout << "Enter ID: ";
                cin >> empID;
                if (find(empIDs.begin(), empIDs.end(), empID) != empIDs.end()) {
                    cout << "Duplicate ID! Enter a different one.\n";
                    exists = true;
                }
            } while (exists);
            
            empIDs.push_back(empID);
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, empName);
            
            if (option == 1) {
                numericInput = getValidDouble("Enter Monthly Salary: ");
                staff.push_back(new SalariedWorker(empID, empName, numericInput));
            } else if (option == 2) {
                numericInput = getValidDouble("Enter Hourly Wage: ");
                additionalInput = getValidInt("Enter Hours Worked: ");
                staff.push_back(new HourlyWorker(empID, empName, numericInput, additionalInput));
            } else {
                numericInput = getValidDouble("Enter Salary per Project: ");
                additionalInput = getValidInt("Enter Number of Projects: ");
                staff.push_back(new ProjectWorker(empID, empName, numericInput, additionalInput));
            }
        } else if (option == 4) {
            cout << "\n------ Employee Payroll Report ------\n";
            if (staff.empty()) {
                cout << "No employees recorded.\n";
            } else {
                for (const auto& emp : staff) {
                    cout << emp->getPayroll();
                }
            }
        }
    }

    for (auto emp : staff) {
        delete emp;
    }
    return 0;
}
