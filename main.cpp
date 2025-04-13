//This program is able to read existing party and passenger information from a file, and allow agents to add, edit, or remove passengers from a party, and save the information to a new file.
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>

using namespace std;

const int MAX_PARTIES = 100;
const int MAX_PASSENGERS_PER_PARTY = 12;

struct Passenger {
    string fullName;
    int age;
    string size; // T-shirt size
};

struct Party {
    string partyName;
    int partySize = 0;
    int numberOfRooms = 0;
    Passenger passengers[MAX_PASSENGERS_PER_PARTY];
};

//List of functions necessary in instructions
void readInitialPassengerData(Party parties[], int& partyCount);
int menuHandler();
void addPassenger(Party parties[], int& partyCount);
void editPassenger(Party parties[], int partyCount);
void removePassenger(Party parties[], int& partyCount);
void printPassengerList(const Party parties[], int partyCount);
void printPartyList(const Party parties[], int partyCount);
void saveToFile(const string& fileName, const Party parties[], int partyCount);

int main() {
    Party parties[MAX_PARTIES];
    int partyCount = 0; //Keeps count of parties
    readInitialPassengerData(parties, partyCount); //This reads the passenger data from file

    //Option 1-6 user input loop
    while (true) {
        int choice = menuHandler(); //Provide the menu
        switch (choice) {
            case 1:
                addPassenger(parties, partyCount); // Add a passenger
                break;
            case 2:
                editPassenger(parties, partyCount); // Edit a passenger
                break;
            case 3:
                removePassenger(parties, partyCount); // Remove a passenger
                break;
            case 4:
                printPassengerList(parties, partyCount); //Show passenger list
                break;
            case 5:
                printPartyList(parties, partyCount); //Show party list
                break;
            case 6:
            {
                string fileName;
                cout << "Please enter the filename to save: ";
                cin >> fileName;
                saveToFile(fileName, parties, partyCount);
            }
                break;
            case 7:
                return 0; //Exit the program
            default:
                cout << "Invalid choice, please try again."; //Incorrect choice
                cout << endl;
        }
    }

    return 0;
}

//Create the functions

//Read passenger data from the file
void readInitialPassengerData(Party parties[], int& partyCount) {
    string fileName;
    cout << "Enter the filename: ";
    cin >> fileName;

    //Error message incase file name is not found
    ifstream inFile(fileName);
    if (!inFile) {
        cerr << "Error Opening File, please enter valid file name: ";
        return;
    }
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        Passenger passenger;
        string partyName;

        //Get the full nname of passenger
        string firstName, lastName;
        ss >> firstName >> lastName;
        passenger.fullName = firstName + " " + lastName;

        ss >> passenger.age >> passenger.size >> partyName;

        //Make sure passenger age is valid
        if (passenger.age < 1 || passenger.age > 105) {
            cerr << "Invalid age for passenger: " << passenger.fullName;
            cout << endl;
            continue;
        }
        //Validate passenger t shirt size
        if (passenger.size != "xxs" && passenger.size != "xs" && passenger.size != "s" &&
            passenger.size != "m" && passenger.size != "l" && passenger.size != "xl" &&
            passenger.size != "xxl" && passenger.size != "xxxl") {
            cerr << "Invalid t-shirt size for passenger: " << passenger.fullName;
            cout << endl;
            continue;
        }


        //Make sure party does not already exist
        bool partyFound = false;
        for (int i = 0; i < partyCount; i++) {
            if (parties[i].partyName == partyName) {
                parties[i].passengers[parties[i].partySize++] = passenger;
                partyFound = true;
                break;
            }
        }

        //If party is not existing, create a new party
        if (!partyFound) {
            parties[partyCount].partyName = partyName;
            parties[partyCount].passengers[0] = passenger;
            parties[partyCount].partySize = 1;
            partyCount++;
        }
    }

    //How many rooms for each party - calculation
    for (int i = 0; i < partyCount; i++) {
        parties[i].numberOfRooms = (parties[i].partySize + 3) / 4;
    }
}


//Display the menu to get user input
int menuHandler() {
    cout << "Please select one of the following Spudwegian choices:";
    cout << endl;
    cout << "1. Add a passenger";
    cout << endl;
    cout << "2. Edit a passenger";
    cout << endl;
    cout << "3. Remove a passenger";
    cout << endl;
    cout << "4. Print Passenger List";
    cout << endl;
    cout << "5. Print Party List";
    cout << endl;
    cout << "6. Save to file";
    cout << endl;
    cout << "7. Exit";
    cout << endl;

    int choice;
    cin >> choice;
    return choice;
}


//Add a passenger to the party
void addPassenger(Party parties[], int& partyCount) {
    Passenger newPassenger;
    string partyName;

    //Passenger details
    cout << "Enter party name: ";
    cin >> partyName;
    cout << "Enter full name: ";
    cin >> newPassenger.fullName;
    cout << "Enter age: ";
    cin >> newPassenger.age;
    cout << "Enter t-shirt size (xxs, xs, s, m, l, xl, xxl, xxxl): ";
    cin >> newPassenger.size;

    //Validate passenger age
    if (newPassenger.age < 1 || newPassenger.age > 105) {
        cerr << "Invalid age";
        cout << endl;
        return;
    }

    //Validate tshirt size
    if (newPassenger.size != "xxs" && newPassenger.size != "xs" && newPassenger.size != "s" &&
        newPassenger.size != "m" && newPassenger.size != "l" && newPassenger.size != "xl" &&
        newPassenger.size != "xxl" && newPassenger.size != "xxxl") {
        cerr << "Invalid t-shirt size.";
        cout << endl;
        return;
    }


    //Check fi party already exists
    for (int i = 0; i < partyCount; i++) {
        if (parties[i].partyName == partyName) {
            if (parties[i].partySize < MAX_PASSENGERS_PER_PARTY) {
                parties[i].passengers[parties[i].partySize++] = newPassenger;
                parties[i].numberOfRooms = (parties[i].partySize + 3) / 4;
                cout << "The passenger was added to the existing party";
                cout << endl;
            } else {
                cerr << "The party is full";
                cout << endl;
            }
            return;
        }
    }
// Create a new party if it does not exist
    if (partyCount < MAX_PARTIES) {
        parties[partyCount].partyName = partyName;
        parties[partyCount].passengers[0] = newPassenger;
        parties[partyCount].partySize = 1;
        parties[partyCount].numberOfRooms = 1;
        partyCount++;
        cout << "Party not found, creating party and adding passenger.";
        cout << endl;
    } else {
        cerr << "Maximum number of parties reached.";
        cout << endl;
    }
}


//Exit the passenger's information
void editPassenger(Party parties[], int partyCount) {
    string partyName, passengerName;

    cout << "Enter party name: ";
    cin >> partyName;

    //find party
    int partyIndex = -1;
    for (int i = 0; i < partyCount; i++) {
        if (parties[i].partyName == partyName) {
            partyIndex = i;
            break;
        }
    }

    if (partyIndex == -1) {
        cerr << "Party not found.";
        cout << endl;
        return;
    }

    cout << "Enter passenger name: ";
    cin >> passengerName;

    // Locate passenger by name
    int passengerCase = -1;
    for (int i = 0; i < parties[partyIndex].partySize; i++) {
        if (parties[partyIndex].passengers[i].fullName == passengerName) {
            passengerCase = i;
            break;
        }
    }

    if (passengerCase == -1) {
        cerr << "Passenger not found.";
        cout << endl;
        return;
    }

    //Display menu for updating passenger information
    while (true) {
        cout << "What would you like to update?";
        cout << endl;
        cout << "1. Update name";
        cout << endl;
        cout << "2. Update age";
        cout << endl;
        cout << "3. Update t-shirt size";
        cout << endl;
        cout << "4. Exit";
        cout << endl;

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter new name: ";
                cin >> parties[partyIndex].passengers[passengerCase].fullName;
                cout << "The name was updated successfully";
                cout << endl;
                break;
            }
            case 2: {
                int newAge;
                cout << "Enter new age: ";
                cin >> newAge;
                if (newAge < 1 || newAge > 105) {
                    cerr << "That is an invalid age.";
                    cout << endl;
                } else {
                    parties[partyIndex].passengers[passengerCase].age = newAge;
                    cout << "Age was updated successfully";
                    cout << endl;
                }
                break;
            }
            case 3: {
                string newSize;
                cout << "Enter new t-shirt size: ";
                cin >> newSize;
                if (newSize != "xxs" && newSize != "xs" && newSize != "s" &&
                    newSize != "m" && newSize != "l" && newSize != "xl" &&
                    newSize != "xxl" && newSize != "xxxl") {
                    cerr << "Invalid t-shirt size.";
                    cout << endl;
                } else {
                    parties[partyIndex].passengers[passengerCase].size = newSize;
                    cout << "T-shirt size updated successfully.";
                    cout << endl;
                }
                break;
            }
            case 4:
                return;
            default:
                cerr << "Invalid choice.";
                cout << endl;
        }
    }
}


//Remove a passenger from the party
void removePassenger(Party parties[], int& partyCount) {
    string partyName, passengerName;

    cout << "Enter party name: ";
    cin >> partyName;

    //Find the party
    int partyIndex = -1;
    for (int i = 0; i < partyCount; i++) {
        if (parties[i].partyName == partyName) {
            partyIndex = i;
            break;
        }
    }

    //Error message if party isnt real
    if (partyIndex == -1) {
        cerr << "Party not found.";
        cout << endl;
        return;
    }


    //Prompt user for passenger name
    cout << "Enter passenger name: ";
    cin >> passengerName;

    //Find the passenger name in the party
    int passengerCase = -1;
    for (int i = 0; i < parties[partyIndex].partySize; i++) {
        if (parties[partyIndex].passengers[i].fullName == passengerName) {
            passengerCase = i;
            break;
        }
    }

    if (passengerCase == -1) {
        cerr << "Passenger not found.";
        cout << endl;
        return;
    }

    if (parties[partyIndex].partySize == 1) {
        cerr << "You must add a passenger before removing. Parties must have 1 passenger at all times.";
        cout << endl;
        return;
    }

}


//Print the list of passengers for every party --

void printPassengerList(const Party parties[], int partyCount) {
    cout << setw(50 ) << "Passenger Name" << " |"
         << setw(5 ) << "Age" << " |"
         << setw(12 ) << "Tshirt Size" << " |"
         << setw(30 ) << "Party Name" << endl;
    cout << setfill('-') << setw(104) << " " << endl << setfill(' ');
    for (int i = 0; i < partyCount; i++) {
        for (int j = 0; j < parties[i].partySize; j++) {
            cout << setw(50 ) << parties[i].passengers[j].fullName << " |"
                 << setw(5 ) << parties[i].passengers[j].age << " |"
                 << setw(12 ) << parties[i].passengers[j].size << " |"
                 << setw(30 ) << parties[i].partyName << endl;
        }
    }
}


// Print the list of all the parties --

void printPartyList(const Party parties[], int partyCount) {
    cout << setw(15) << "Party Name" << " |"
         << setw(20) << "# of Passengers" << " |"
         << setw(15) << "# of Rooms" << endl;
    cout << setfill('-') << setw(55) << " " << endl << setfill(' ');
    for (int i = 0; i < partyCount; i++) {
        cout << setw(15) << parties[i].partyName << " |"
             << setw(20) << parties[i].partySize << " |"
             << setw(15) << parties[i].numberOfRooms << endl;
    }
}


//Save the data to the filename
void saveToFile(const string& fileName, const Party parties[], int partyCount) {
    ofstream outFile(fileName);
    if (!outFile) {
        cerr << "Error opening file for writing: " << fileName << endl;
        return;
    }

    for (int i = 0; i < partyCount; i++) {
        outFile << parties[i].partyName << " "
                << parties[i].partySize << " "
                << parties[i].numberOfRooms << endl;
    }

    outFile << setfill('-') << setw(55) << " " << endl << setfill(' ');

    for (int i = 0; i < partyCount; i++) {
        for (int j = 0; j < parties[i].partySize; j++) {
            outFile << setw(50) << parties[i].passengers[j].fullName << " |"
                    << setw(5) << parties[i].passengers[j].age << " |"
                    << setw(12) << parties[i].passengers[j].size << " |"
                    << setw(30) << parties[i].partyName << endl;
        }
    }
//Confirmation message
    cout << "Data saved to " << fileName << " successfully." << endl;
}