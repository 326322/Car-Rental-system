#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm> // Added for std::find()

using namespace std;

// Car class
class Car {
public:
    string carID, model, brand;
    bool isRented;

    Car(string id, string m, string b) {
        carID = id;
        model = m;
        brand = b;
        isRented = false;
    }
};

// Customer class
class Customer {
public:
    string name, licenseID;
    vector<string> rentedCars;

    Customer(string n, string l) {
        name = n;
        licenseID = l;
    }
};

// Car Rental System
class CarRentalSystem {
private:
    unordered_map<string, Car*> cars; // Hash map for quick lookup
    unordered_map<string, Customer*> customers; // Customer database

public:
    // Load cars from file
    void loadCars() {
        ifstream file("cars.txt");
        string id, model, brand;
        while (file >> id >> model >> brand) {
            cars[id] = new Car(id, model, brand);
        }
        file.close();
    }

    // Add a car
    void addCar(string id, string model, string brand) {
        cars[id] = new Car(id, model, brand);
        cout << "Car added successfully.\n";
    }

    // Display available cars
    void showAvailableCars() {
        cout << "\nAvailable Cars:\n";
        for (auto& pair : cars) {
            if (!pair.second->isRented) {
                cout << "ID: " << pair.second->carID << " | Model: " << pair.second->model << " | Brand: " << pair.second->brand << endl;
            }
        }
    }

    // Rent a car
    void rentCar(string customerName, string licenseID, string carID) {
        if (cars.find(carID) == cars.end()) {
            cout << "Car not found!\n";
            return;
        }
        if (cars[carID]->isRented) {
            cout << "Car is already rented!\n";
            return;
        }

        if (customers.find(licenseID) == customers.end()) {
            customers[licenseID] = new Customer(customerName, licenseID);
        }

        customers[licenseID]->rentedCars.push_back(carID);
        cars[carID]->isRented = true;

        cout << "Car rented successfully to " << customerName << ".\n";
    }

    // Return a car
    void returnCar(string licenseID, string carID) {
        if (customers.find(licenseID) == customers.end()) {
            cout << "Customer not found!\n";
            return;
        }

        vector<string>& rentedCars = customers[licenseID]->rentedCars;
        auto it = std::find(rentedCars.begin(), rentedCars.end(), carID);
        if (it == rentedCars.end()) {
            cout << "Car not rented by this customer!\n";
            return;
        }

        rentedCars.erase(it);
        cars[carID]->isRented = false;

        cout << "Car returned successfully.\n";
    }

    // Show rental history
    void showCustomerHistory(string licenseID) {
        if (customers.find(licenseID) == customers.end()) {
            cout << "Customer not found!\n";
            return;
        }

        cout << "\nRental History for " << customers[licenseID]->name << ":\n";
        for (string carID : customers[licenseID]->rentedCars) {
            cout << "Car ID: " << carID << endl;
        }
    }
};

// Main function
int main() {
    CarRentalSystem rental;
    rental.loadCars();
    
    int choice;
    do {
        cout << "\nCar Rental System Menu:\n";
        cout << "1. Show Available Cars\n2. Rent a Car\n3. Return a Car\n4. Show Rental History\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            rental.showAvailableCars();
        } else if (choice == 2) {
            string name, license, carID;
            cout << "Enter Name: ";
            cin >> name;
            cout << "Enter License ID: ";
            cin >> license;
            cout << "Enter Car ID to Rent: ";
            cin >> carID;
            rental.rentCar(name, license, carID);
        } else if (choice == 3) {
            string license, carID;
            cout << "Enter License ID: ";
            cin >> license;
            cout << "Enter Car ID to Return: ";
            cin >> carID;
            rental.returnCar(license, carID);
        } else if (choice == 4) {
            string license;
            cout << "Enter License ID: ";
            cin >> license;
            rental.showCustomerHistory(license);
        }
    } while (choice != 5);

    return 0;
}
