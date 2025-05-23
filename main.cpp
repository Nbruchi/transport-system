#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

// Structure to represent a city
struct City {
    int index;
    string name;
};

// Structure to represent a road
struct Road {
    string city1;
    string city2;
    double budget;
};

// Global variables
vector<City> cities;
vector<Road> roads;
const int MAX_CITIES = 30; // Maximum number of cities allowed
double roadMatrix[MAX_CITIES][MAX_CITIES] = {0}; // Adjacency matrix for roads
double budgetMatrix[MAX_CITIES][MAX_CITIES] = {0}; // Adjacency matrix for budgets

// Function declarations
void displayMenu();
void addCities();
void addRoads();
void addBudget();
void editCity();
void searchCity();
void displayCities();
void displayRoads();
void displayAllData();
void saveToFiles();
void loadFromFiles();
bool cityExists(const string& cityName);
int getCityIndex(const string& cityName);
void initializeRoadMatrices();

int main() {
    loadFromFiles(); // Load existing data if any
    
    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                addCities();
                break;
            case 2:
                addRoads();
                break;
            case 3:
                addBudget();
                break;
            case 4:
                editCity();
                break;
            case 5:
                searchCity();
                break;
            case 6:
                displayCities();
                break;
            case 7:
                displayRoads();
                break;
            case 8:
                displayAllData();
                break;
            case 9:
                saveToFiles();
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while(choice != 9);
    
    return 0;
}

void displayMenu() {
    cout << "\nMenu" << endl;
    cout << "1. Add new city(ies)" << endl;
    cout << "2. Add roads between cities" << endl;
    cout << "3. Add the budget for roads" << endl;
    cout << "4. Edit city" << endl;
    cout << "5. Search for a city using its index" << endl;
    cout << "6. Display cities" << endl;
    cout << "7. Display roads" << endl;
    cout << "8. Display recorded data on console" << endl;
    cout << "9. Exit" << endl;
}

void addCities() {
    int numCities;
    cout << "Enter the number of cities to add: ";
    cin >> numCities;
    
    if (numCities <= 0) {
        cout << "Invalid number of cities." << endl;
        return;
    }
    
    for (int i = 0; i < numCities; i++) {
        City newCity;
        newCity.index = cities.size() + 1;
        cout << "Enter name for city " << newCity.index << ": ";
        cin >> newCity.name;
        
        if (cityExists(newCity.name)) {
            cout << "City already exists!" << endl;
            i--;
            continue;
        }
        
        cities.push_back(newCity);
    }
    saveToFiles();
}

bool cityExists(const string& cityName) {
    for (const auto& city : cities) {
        if (city.name == cityName) {
            return true;
        }
    }
    return false;
}

int getCityIndex(const string& cityName) {
    for (const auto& city : cities) {
        if (city.name == cityName) {
            return city.index;
        }
    }
    return -1;
}

void editCity() {
    int index;
    cout << "Enter the index of the city to edit: ";
    cin >> index;
    
    if (index < 1 || index > cities.size()) {
        cout << "Invalid city index!" << endl;
        return;
    }
    
    string newName;
    cout << "Enter new name for city: ";
    cin >> newName;
    
    if (cityExists(newName)) {
        cout << "City name already exists!" << endl;
        return;
    }
    
    cities[index - 1].name = newName;
    cout << "City updated successfully." << endl;
    saveToFiles();
}

void searchCity() {
    int index;
    cout << "Enter the index of the city to search: ";
    cin >> index;
    
    if (index < 1 || index > cities.size()) {
        cout << "City not found!" << endl;
        return;
    }
    
    cout << "City found: " << cities[index - 1].name << endl;
}

void displayCities() {
    cout << "\nCities:" << endl;
    for (const auto& city : cities) {
        cout << city.index << ": " << city.name << endl;
    }
}

void addRoads() {
    string city1, city2;
    cout << "Enter the name of the first city: ";
    cin >> city1;
    cout << "Enter the name of the second city: ";
    cin >> city2;
    
    if (!cityExists(city1) || !cityExists(city2)) {
        cout << "One or both cities do not exist!" << endl;
        return;
    }
    
    int idx1 = getCityIndex(city1);
    int idx2 = getCityIndex(city2);
    
    if (roadMatrix[idx1-1][idx2-1] == 1) {
        cout << "Road already exists between these cities!" << endl;
        return;
    }
    
    roadMatrix[idx1-1][idx2-1] = 1;
    roadMatrix[idx2-1][idx1-1] = 1;
    
    Road newRoad = {city1, city2, 0.0};
    roads.push_back(newRoad);
    
    cout << "Road added between " << city1 << " and " << city2 << endl;
    saveToFiles();
}

void addBudget() {
    string city1, city2;
    cout << "Enter the name of the first city: ";
    cin >> city1;
    cout << "Enter the name of the second city: ";
    cin >> city2;
    
    if (!cityExists(city1) || !cityExists(city2)) {
        cout << "One or both cities do not exist!" << endl;
        return;
    }
    
    int idx1 = getCityIndex(city1);
    int idx2 = getCityIndex(city2);
    
    if (roadMatrix[idx1-1][idx2-1] != 1) {
        cout << "No road exists between these cities!" << endl;
        return;
    }
    
    double budget;
    cout << "Enter the budget for the road: ";
    cin >> budget;
    
    if (budget < 0) {
        cout << "Invalid budget amount!" << endl;
        return;
    }
    
    budgetMatrix[idx1-1][idx2-1] = budget;
    budgetMatrix[idx2-1][idx1-1] = budget;
    
    // Update road budget in roads vector
    for (auto& road : roads) {
        if ((road.city1 == city1 && road.city2 == city2) ||
            (road.city1 == city2 && road.city2 == city1)) {
            road.budget = budget;
            break;
        }
    }
    
    cout << "Budget added for the road between " << city1 << " and " << city2 << endl;
    saveToFiles();
}

void displayRoads() {
    cout << "\nCities:" << endl;
    for (const auto& city : cities) {
        cout << city.index << ": " << city.name << endl;
    }
    
    cout << "\nRoads Adjacency Matrix:" << endl;
    for (int i = 0; i < cities.size(); i++) {
        for (int j = 0; j < cities.size(); j++) {
            cout << roadMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void displayAllData() {
    displayCities();
    
    cout << "\nRoads Adjacency Matrix:" << endl;
    for (int i = 0; i < cities.size(); i++) {
        for (int j = 0; j < cities.size(); j++) {
            cout << roadMatrix[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "\nBudgets Adjacency Matrix:" << endl;
    for (int i = 0; i < cities.size(); i++) {
        for (int j = 0; j < cities.size(); j++) {
            cout << fixed << setprecision(1) << budgetMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

void saveToFiles() {
    // Save cities to cities.txt
    ofstream cityFile("cities.txt");
    if (cityFile.is_open()) {
        cityFile << "╔════════════════════════════════════╗" << endl;
        cityFile << "║           CITIES DATABASE          ║" << endl;
        cityFile << "╠════════════════════════════════════╣" << endl;
        cityFile << "║ Index │ City Name                  ║" << endl;
        cityFile << "╟───────┼────────────────────────────╢" << endl;
        for (const auto& city : cities) {
            cityFile << "║ " << setw(5) << city.index << " │ " 
                    << left << setw(26) << city.name << "║" << endl;
        }
        cityFile << "╚════════════════════════════════════╝" << endl;
        cityFile.close();
    }
    
    // Save roads to roads.txt
    ofstream roadFile("roads.txt");
    if (roadFile.is_open()) {
        roadFile << "╔════════════════════════════════════════════════════════════╗" << endl;
        roadFile << "║                     ROADS DATABASE                         ║" << endl;
        roadFile << "╠════════════════════════════════════════════════════════════╣" << endl;
        roadFile << "║ Nbr. │ Road                      │ Budget                   ║" << endl;
        roadFile << "╟─────┼───────────────────────────────┼─────────────────────╢" << endl;
        int roadNum = 1;
        for (const auto& road : roads) {
            if (road.budget > 0) {
                roadFile << "║ " << setw(3) << roadNum << " │ " 
                        << left << setw(29) << (road.city1 + " - " + road.city2) << "│ "
                        << right << setw(17) << fixed << setprecision(2) << road.budget << " ║" << endl;
                roadNum++;
            }
        }
        roadFile << "╚════════════════════════════════════════════════════════════╝" << endl;
        roadFile.close();
    }
}

void loadFromFiles() {
    // Load cities from cities.txt
    ifstream cityFile("cities.txt");
    if (cityFile.is_open()) {
        string line;
        getline(cityFile, line); // Skip header
        while (getline(cityFile, line)) {
            stringstream ss(line);
            City city;
            ss >> city.index >> city.name;
            cities.push_back(city);
        }
        cityFile.close();
    }
    
    // Load roads from roads.txt
    ifstream roadFile("roads.txt");
    if (roadFile.is_open()) {
        string line;
        getline(roadFile, line); // Skip header
        while (getline(roadFile, line)) {
            stringstream ss(line);
            Road road;
            int nbr;
            string dash;
            ss >> nbr >> road.city1 >> dash >> road.city2 >> road.budget;
            
            int idx1 = getCityIndex(road.city1);
            int idx2 = getCityIndex(road.city2);
            
            if (idx1 != -1 && idx2 != -1) {
                roadMatrix[idx1-1][idx2-1] = 1;
                roadMatrix[idx2-1][idx1-1] = 1;
                budgetMatrix[idx1-1][idx2-1] = road.budget;
                budgetMatrix[idx2-1][idx1-1] = road.budget;
                roads.push_back(road);
            }
        }
        roadFile.close();
    }
}
