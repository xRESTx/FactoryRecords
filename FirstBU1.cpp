#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct MaterialRecord {
	int id;
	int factoryNumber;
	int branchNumber;
	string LastName;
	double startValue;
	double receivedValue;
	double disposedValue;
	MaterialRecord* next; // указатель на следующую запись
};

MaterialRecord* createRecord( int id, int factoryNumber, int branchNumber, const string& lastName, double startValue, double receivedValue, double disposedValue) {
	MaterialRecord* newRecord = new MaterialRecord;
	newRecord->id = id;
	newRecord->factoryNumber = factoryNumber;
	newRecord->branchNumber = branchNumber;
	newRecord->LastName = lastName;
	newRecord->startValue = startValue;
	newRecord->receivedValue = receivedValue;
	newRecord->disposedValue = disposedValue;
	newRecord->next = nullptr;
	return newRecord;
}

void idCheck(MaterialRecord*& head, int& id) {
    MaterialRecord* current = head;
    while (current != nullptr) {
        if (current->id == id) {
            cout << "Record with ID " << id << " already exists! Cannot add duplicate ID." << endl;
            cout << "Please enter a new ID: ";
            cin >> id;
            current = head; // Начнем проверку с начала списка
        } else {
            current = current->next;
        }
    }
    cout << "The ID does not repeat, please continue entering data.\n";
}

void addRecord(MaterialRecord*& head, MaterialRecord* newRecord) {
	if (head == nullptr) {
		head = newRecord;
	} else {
		MaterialRecord* temp = head;
		while (temp->next != nullptr) {
			temp = temp->next;
		}
		temp->next = newRecord;
	}
	cout << "Record added successfully!" << endl;
}

void displayRecords(MaterialRecord* head) {
	cout << "--------------------------------------------------------------------------------------------------------------" << endl;
    cout << "|   ID   | Factory | Branch |   Last Name   | Start Value  | Received Value | Disposed Value |" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------" << endl;
    MaterialRecord* current = head;
    while (current != nullptr) {
        printf("| %6d | %7d | %6d | %13s | %12.2f | %14.2f | %14.2f |\n", current->id, current->factoryNumber,
               current->branchNumber, current->LastName.c_str(), current->startValue, current->receivedValue,
               current->disposedValue);
        current = current->next;
    }
    cout << "--------------------------------------------------------------------------------------------------------------" << endl;
}

void deleteRecord(MaterialRecord*& head, int idToDelete) {
	if (head == nullptr) {
		cout << "List is empty!" << endl;
		return;
	}

	if (head->id == idToDelete) {
		MaterialRecord* temp = head;
		head = head->next;
		delete temp;
		cout << "Record with ID " << idToDelete << " has been deleted." << endl;
		return;
	}

	MaterialRecord* current = head;
	while (current->next != nullptr) {
		if (current->next->id == idToDelete) {
			MaterialRecord* temp = current->next;
			current->next = current->next->next;
			delete temp;
			cout << "Record with ID " << idToDelete << " has been deleted." << endl;
			return;
		}
		current = current->next;
	}

	cout << "Record with ID " << idToDelete << " not found!" << endl;
}

void saveToTextFile(MaterialRecord* head, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Unable to open file!" << endl;
        return;
    }

    MaterialRecord* current = head;
    while (current != nullptr) {
        file << current->id << ' ' << current->factoryNumber << ' ' << current->branchNumber << ' '
             << current->LastName << ' ' << current->startValue << ' ' << current->receivedValue
             << ' ' << current->disposedValue << endl;
        current = current->next;
    }

    file.close();
    cout << "Data saved to text file successfully!" << endl;
}

void saveToBinaryFile(MaterialRecord* head, const string& filename) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Unable to open file!" << endl;
        return;
    }

    MaterialRecord* current = head;
    while (current != nullptr) {
        file.write(reinterpret_cast<char*>(current), sizeof(MaterialRecord));
        current = current->next;
    }

    file.close();
    cout << "Data saved to binary file successfully!" << endl;
}

void editRecord(MaterialRecord* head, int idToEdit) {
    MaterialRecord* current = head;
    while (current != nullptr) {
        if (current->id == idToEdit) {
            cout << "Editing Record with ID: " << idToEdit << endl;
            cout << "Enter new Factory Number: ";
            cin >> current->factoryNumber;

            cout << "Enter new Branch Number: ";
            cin >> current->branchNumber;

            cout << "Enter new Last Name: ";
            cin >> current->LastName;

            cout << "Enter new Start Value: ";
            cin >> current->startValue;

            cout << "Enter new Received Value: ";
            cin >> current->receivedValue;

            cout << "Enter new Disposed Value: ";
            cin >> current->disposedValue;

            cout << "Record updated successfully!" << endl;
            return;
        }
        current = current->next;
    }

    cout << "Record with ID " << idToEdit << " not found!" << endl;
}

int main() {
	MaterialRecord* records = nullptr; // голова списка записей

	// defoltnoe(стартовое) заполнение таблицы
	addRecord(records, createRecord(1, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
    addRecord(records, createRecord(2, 1, 102, "Johnson", 7000.0, 1500.0, 800.0));
    addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));

	int choice;
	while (true) {
		cout << "\nMenu:" << endl;
		cout << "1. View Records" << endl;
		cout << "2. Add Record" << endl;
		cout << "3. Delete Record" << endl;
		cout << "4. Edit Record" << endl;
		cout << "5. Save table to file.txt" << endl;
		cout << "6. Save table to file.bin" << endl;
		
		
		cout << "99. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
			case 1: {
				cout << "\nMaterial Records:" << endl;
				displayRecords(records);
				break;
			}
			case 2: {
				int id, factory, branch;
				string lastName;
				double start, received, disposed;
				cout << "Enter ID, Factory Number, Branch Number, Last Name, Start Value, Received Value, Disposed Value: ";
				cin >> id;
				idCheck(records, id);
				cin >> factory >> branch >> lastName >> start >> received >> disposed;
				addRecord(records, createRecord(id, factory, branch, lastName, start, received, disposed));
				break;
			}
			case 3:{
				int idToDelete;
				cout << "Enter ID of the record to delete: ";
				cin >> idToDelete;
				deleteRecord(records, idToDelete);
				break;
			}
			case 4: {
				int idToEdit;
				cout << "Enter ID of the record to edit: ";
				cin >> idToEdit;
				editRecord(records, idToEdit);
				break;
			}
			case 5: {
				saveToTextFile(records, "material_records.txt");
				break;
			}
			case 6: {
				saveToBinaryFile(records, "material_records.bin");
				break;
			}
			
			case 99:{
				cout << "Exiting program..." << endl;
				return 0;
			}
			default:
				cout << "Invalid choice! Please enter a valid option." << endl;
		}
	}

	return 0;
}
