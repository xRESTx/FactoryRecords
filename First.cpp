#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <map>

using namespace std;

//##############################################BASEment###################################//

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

//#############################################Prototype functions####################################//

void saveToTextFile(MaterialRecord* head, const string& filename);
void saveToBinaryFile(MaterialRecord* head, const string& filename);
void calculateEndPeriodValueByBranch(MaterialRecord* head);
void calculateEndPeriodValueByFactory(MaterialRecord* head);
void calculateTotals(MaterialRecord* head);

//##############################################Record handing###################################//

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

int countRecords(MaterialRecord* head) {
    int count = 0;
    MaterialRecord* current = head;

    while (current != nullptr) {
        count++;
        current = current->next;
    }

    return count;
}

void displayRecords(MaterialRecord* head) {
    int currentPage = 1;
    int itemsPerPage = 20;
    char c = 0;
    int maxPage = countRecords(head);
    MaterialRecord* current = head;

    do {
        system("cls");
        printf("Current Page: %d\n", currentPage);
		cout << "----------------------------------------------------------------------------------------------" << endl;
		cout << "|   ID   | Factory | Branch |   Last Name   | Start Value  | Received Value | Disposed Value |" << endl;
		cout << "----------------------------------------------------------------------------------------------" << endl;
        int count = 0;
        while (current != nullptr && count < itemsPerPage) {
            printf("| %6d | %7d | %6d | %13s | %12.2f | %14.2f | %14.2f |\n", current->id, current->factoryNumber,
		       current->branchNumber, current->LastName.c_str(), current->startValue, current->receivedValue,
		       current->disposedValue);
            current = current->next;
            count++;
        }

        printf("\nPress 'w' for previous page, 's' for next page, or any other key to exit: ");

        c = getch();

        if ((c==115 || c==80) && current != nullptr) {
            currentPage++;
        } else if ((c==119 || c==72) && currentPage > 1) {
            currentPage--;
            MaterialRecord* temp = head;
            for (int i = 1; i < (currentPage - 1) * itemsPerPage; ++i) {
                temp = temp->next;
            }
            current = temp;
        } else {
        	break;
		}
    }while (c!=13);
}


//void displayRecords(MaterialRecord* head) {
//	cout << "----------------------------------------------------------------------------------------------" << endl;
//	cout << "|   ID   | Factory | Branch |   Last Name   | Start Value  | Received Value | Disposed Value |" << endl;
//	cout << "----------------------------------------------------------------------------------------------" << endl;
//	MaterialRecord* current = head;
//	while (current != nullptr) {
////		cout << "| " << setw(6) << current->id << " | " << setw(7) << current->factoryNumber << " | " << setw(6) << current->branchNumber << " | " << setw(13) << current->LastName << " | " << setw(14) << current->startValue << " | " << setw(16) << current->receivedValue << " | " << setw(16) << current->disposedValue << " |"<< endl;
//		printf("| %6d | %7d | %6d | %13s | %12.2f | %14.2f | %14.2f |\n", current->id, current->factoryNumber,
//		       current->branchNumber, current->LastName.c_str(), current->startValue, current->receivedValue,
//		       current->disposedValue);
//		current = current->next;
//	}
//	cout << "----------------------------------------------------------------------------------------------" << endl;
//}

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

//##############################################rabota s failami###################################//

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

void saveToBinaryFile(MaterialRecord* head, const char* filename) {
    FILE* file = fopen(filename, "wb"); // Открытие файла для записи в бинарном режиме 

    if (file == nullptr) {
        cerr << "Unable to open file for writing!" << endl;
        return;
    }

    MaterialRecord* current = head;
    while (current != nullptr) {
        fwrite(current, sizeof(MaterialRecord), 1, file); // Запись одной записи в файл
        current = current->next;
    }

    fclose(file); // Закрытие файла
    cout << "Data saved to binary file successfully!" << endl;
}
//void saveToBinaryFile(MaterialRecord* head, const string& filename) {
//	ofstream file(filename, ios::binary);
//	if (!file.is_open()) {
//		cout << "Unable to open file!" << endl;
//		return;
//	}
//
//	MaterialRecord* current = head;
//	while (current != nullptr) {
//		file.write(reinterpret_cast<char*>(current), sizeof(MaterialRecord));
//		current = current->next;
//	}
//
//	file.close();
//	cout << "Data saved to binary file successfully!" << endl;
//}

void loadFromTextFile(MaterialRecord*& head, const string& filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Unable to open file!" << endl;
		return;
	}

	int id, factory, branch;
	string lastName;
	double start, received, disposed;

	while (file >> id >> factory >> branch >> lastName >> start >> received >> disposed) {
		MaterialRecord* newRecord = new MaterialRecord;
		newRecord->id = id;
		newRecord->factoryNumber = factory;
		newRecord->branchNumber = branch;
		newRecord->LastName = lastName;
		newRecord->startValue = start;
		newRecord->receivedValue = received;
		newRecord->disposedValue = disposed;
		newRecord->next = nullptr;

		if (head == nullptr) {
			head = newRecord;
		} else {
			MaterialRecord* temp = head;
			while (temp->next != nullptr) {
				temp = temp->next;
			}
			temp->next = newRecord;
		}
	}

	file.close();
	cout << "Data loaded from text file successfully!" << endl;
}

void loadFromBinaryFile(MaterialRecord*& head, const char* filename) {
    FILE* file = fopen(filename, "rb"); // Открытие файла для чтения в бинарном режиме (rb - read binary)

    if (file == nullptr) {
        cerr << "Unable to open file for reading!" << endl;
        return;
    }

    while (!feof(file)) {
        MaterialRecord* newRecord = new MaterialRecord;
        size_t bytesRead = fread(newRecord, sizeof(MaterialRecord), 1, file); // Чтение одной записи из файла

        if (bytesRead != 1) {
            delete newRecord;
            break;
        }

        newRecord->next = nullptr;

        if (head == nullptr) {
            head = newRecord;
        } else {
            MaterialRecord* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newRecord;
        }
    }

    fclose(file); // Закрытие файла
    cout << "Data loaded from binary file successfully!" << endl;
}

//void loadFromBinaryFile(MaterialRecord*& head, const string& filename) {
//	ifstream file(filename, ios::binary);
//	if (!file.is_open()) {
//		cout << "Unable to open file!" << endl;
//		return;
//	}
//
//	while (!file.eof()) {
//		MaterialRecord* newRecord = new MaterialRecord;
//		file.read(reinterpret_cast<char*>(newRecord), sizeof(MaterialRecord));
//
//		if (file.gcount() != sizeof(MaterialRecord)) {
//			delete newRecord;
//			break;
//		}
//
//		newRecord->next = nullptr;
//
//		if (head == nullptr) {
//			head = newRecord;
//		} else {
//			MaterialRecord* temp = head;
//			while (temp->next != nullptr) {
//				temp = temp->next;
//			}
//			temp->next = newRecord;
//		}
//	}
//
//	file.close();
//	cout << "Data loaded from binary file successfully!" << endl;
//}

//##############################################sortirovka###################################//

bool compareByColumn(const MaterialRecord& a, const MaterialRecord& b, int column) {
	switch (column) {
		case 1:
			return a.id > b.id;
		case 2:
			return a.factoryNumber > b.factoryNumber;
		case 3:
			return a.branchNumber > b.branchNumber;
		case 4:
			return a.LastName > b.LastName;
		case 5:
			return a.startValue > b.startValue;
		case 6:
			return a.receivedValue > b.receivedValue;
		case 7:
			return a.disposedValue > b.disposedValue;
	}
}

void swap(MaterialRecord* a, MaterialRecord* b) {
	int tempId = a->id;
	int tempFactory = a->factoryNumber;
	int tempBranch = a->branchNumber;
	string tempLastName = a->LastName;
	double tempStart = a->startValue;
	double tempReceived = a->receivedValue;
	double tempDisposed = a->disposedValue;

	a->id = b->id;
	a->factoryNumber = b->factoryNumber;
	a->branchNumber = b->branchNumber;
	a->LastName = b->LastName;
	a->startValue = b->startValue;
	a->receivedValue = b->receivedValue;
	a->disposedValue = b->disposedValue;

	b->id = tempId;
	b->factoryNumber = tempFactory;
	b->branchNumber = tempBranch;
	b->LastName = tempLastName;
	b->startValue = tempStart;
	b->receivedValue = tempReceived;
	b->disposedValue = tempDisposed;
}

void sortRecords(MaterialRecord*& head, int column) {
	if (head == nullptr || head->next == nullptr) {
		cout << "List has zero or one element. No need for sorting." << endl;
		return;
	}

	bool swapped;
	MaterialRecord* current;
	MaterialRecord* last = nullptr;

	do {
		swapped = false;
		current = head;

		while (current->next != last) {
			if (compareByColumn(*current, *(current->next), column)) {
				swap(current, current->next);
				swapped = true;
			}
			current = current->next;
		}
		last = current;
	} while (swapped);
	string column_print;
	switch (column) {
		case 1: {
			column_print = "id";
			break;
		}
		case 2: {
			column_print = "Factory Number";
			break;
		}
		case 3: {
			column_print = "Brunch Number";
			break;
		}
		case 4: {
			column_print = "LastName";
			break;
		}
		case 5: {
			column_print = "Start Value";
			break;
		}
		case 6: {
			column_print = "Received Value";
			break;
		}
		case 7: {
			column_print = "Disposed Value";	
			break;
		}
	}
	cout << "Records sorted by column " << column_print << " within the list successfully!" << endl;
	cout << "-->Press any key to go back<--";
	getch();
	system("cls");
}

//##############################################clear memory###################################//

void releaseMemory(MaterialRecord*& head) {
	while (head != nullptr) {
		MaterialRecord* temp = head;
		head = head->next;
		delete temp;
	}
}

void exitProgram(MaterialRecord*& head) {
	cout << "Maybe save Records?(Y=1/N=any symbol + Enter): ";
	int saveChoice;
	cin >> saveChoice;
	if (saveChoice == 1) {
		cout << "Format?(Bin=1/txt=any key + Enter): ";
		int formatChoice;
		cin >> formatChoice;
		if (formatChoice == 1) {
			saveToBinaryFile(head, "material_records.bin");
		} else {
			saveToTextFile(head, "material_records.txt");
		}
	}
	releaseMemory(head);
	cout << "Exiting the program. Memory has been freed." << endl;
	exit(0);
}

//##############################################poisk###################################//

void searchByLastName(MaterialRecord* head, const string& lastName) {
	MaterialRecord* current = head;
	bool found = false;

	while (current != nullptr) {
		if (current->LastName == lastName) {
			if (!found) {
				found = true;
				cout << "Search results for '" << lastName << "':" << endl;
				cout << "--------------------------------------------------------------------------------------------------------------" << endl;
				cout << "|   ID   | Factory | Branch |   Last Name   | Start Value | Received Value | Disposed Value |" << endl;
				cout << "--------------------------------------------------------------------------------------------------------------" << endl;
			}
//			cout << "| " << setw(6) << current->id << " | " << setw(7) << current->factoryNumber << " | " << setw(6) << current->branchNumber << " | " << setw(13) << current->LastName << " | " << setw(14) << current->startValue << " | " << setw(16) << current->receivedValue << " | " << setw(16) << current->disposedValue << " |"<< endl;
			printf("| %6d | %7d | %6d | %13s | %12.2f | %14.2f | %14.2f |\n", current->id, current->factoryNumber,
			       current->branchNumber, current->LastName.c_str(), current->startValue,
			       current->receivedValue, current->disposedValue);
		}
		current = current->next;
	}

	if (!found) {
		cout << "No records found for '" << lastName << "'." << endl;
	} else {
		cout << "--------------------------------------------------------------------------------------------------------------" << endl;
	}
	cout << "-->Press any key to go back<--";
	getch();
	system("cls");
}

//##############################################report###################################//
//void generateReport(MaterialRecord* head) {
//    calculateEndPeriodValueByBranch(head);
//    calculateEndPeriodValueByFactory(head);
//    calculateTotals(head);
//}

void generateReport(MaterialRecord* head) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int c,i=1;
    string print_report[6] = { "           Choose Report:",
							   "Calculate End Period Value by Branch",
                               "Calculate End Period Value by Factory",
                               "Calculate Totals",
                               "Back to Main Menu",
                               "Exit"
                              };
    int int_print_report = 6;
	while (true) {
	    do {
	        for (int j = 0; j < int_print_report; j++) {
	            if (j == i) {
	                SetConsoleTextAttribute(hConsole, 10);
	                cout << " -> " << print_report[j] << endl;
	            } else
	                cout << "    " << print_report[j] << endl;
	            SetConsoleTextAttribute(hConsole, 15);
	        }
	        c = getch();
	        if (c == 115 || c == 80) i++;
	        if (c == 119 || c == 72) i--;
	        if (i < 1) i = 1;
	        if (i > int_print_report - 1) i = int_print_report - 1;
	        system("cls");
	    } while (c != 13);
	
	    switch (i) {
	        case 1: {
	            calculateEndPeriodValueByBranch(head);
	            break;
	        }
	        case 2: {
	            calculateEndPeriodValueByFactory(head);
	            break;
	        }
	        case 3: {
	        	calculateTotals(head);
				break;
			}
	        case 4: {
	            return;
	        }
	        case 5: {
	            exitProgram(head);
	        }
	    }
    }
}

void calculateEndPeriodValueByBranch(MaterialRecord* head) {
    map<pair<int, int>, double> endPeriodValueByBranch;

    MaterialRecord* current = head;

    while (current != nullptr) {
        pair<int, int> branchFactoryPair = make_pair(current->branchNumber, current->factoryNumber);

        if (endPeriodValueByBranch.find(branchFactoryPair) != endPeriodValueByBranch.end()) {
            endPeriodValueByBranch[branchFactoryPair] += current->startValue + current->receivedValue - current->disposedValue;
        } else {
            endPeriodValueByBranch[branchFactoryPair] = current->startValue + current->receivedValue - current->disposedValue;
        }
        current = current->next;
    }

    cout << "\nEnd Period Value by Branch:" << endl;
    cout << "--------------------------------------" << endl;
    cout << "| Factory | Branch | End Period Value |" << endl;
    cout << "--------------------------------------" << endl;

    cout << fixed << setprecision(2);

    for (const auto& pairValue : endPeriodValueByBranch) {
        cout << "| " << setw(7) << pairValue.first.second << " | " << setw(6) << pairValue.first.first << " | " << setw(16) << pairValue.second << " |" << endl;
    }

    cout << "--------------------------------------" << endl;
    cout << "-->Press any key to go back<--";
    getch();
    system("cls");
    return;
}

void calculateEndPeriodValueByFactory(MaterialRecord* head) {
    map<int, double> endPeriodValueByFactory;

    MaterialRecord* current = head;
    
    while (current != nullptr) {
        // Если номер завода уже есть, добавляем стоимость к текущей сумме
        if (endPeriodValueByFactory.find(current->factoryNumber) != endPeriodValueByFactory.end()) {
            endPeriodValueByFactory[current->factoryNumber] += current->startValue + current->receivedValue - current->disposedValue;
        } else { // Иначе добавляем новую запись
            endPeriodValueByFactory[current->factoryNumber] = current->startValue + current->receivedValue - current->disposedValue;
        }
        current = current->next;
    }

    cout << "\nEnd Period Value by Branch:" << endl;
    cout << "-----------------------------" << endl;
    cout << "| Factory | End Period Value |" << endl;
    cout << "-----------------------------" << endl;

    cout << fixed << setprecision(2);

    for (const auto& pairValue : endPeriodValueByFactory) {
        cout << "| " << setw(7) << pairValue.first <<  " | " << setw(16) << pairValue.second << " |" << endl;
    }

    cout << "-----------------------------" << endl;
    cout << "-->Press any key to go back<--";
    getch();
    system("cls");
    return;
}

void calculateTotals(MaterialRecord* head) {
    double startValueTotal = 0.0;
    double receivedValueTotal = 0.0;
    double disposedValueTotal = 0.0;
    double endPeriodValueTotal = 0.0;

    MaterialRecord* current = head;

    while (current != nullptr) {
        startValueTotal += current->startValue;
        receivedValueTotal += current->receivedValue;
        disposedValueTotal += current->disposedValue;
        endPeriodValueTotal += (current->startValue + current->receivedValue - current->disposedValue);

        current = current->next;
    }

    cout << "Totals:" << endl;
    cout << "-----------------------------" << endl;
    cout << "| Type        | Total Value |" << endl;
    cout << "-----------------------------" << endl;
    cout << "| Start Value | " << setw(11) << startValueTotal << " |" << endl;
    cout << "| Received    | " << setw(11) << receivedValueTotal << " |" << endl;
    cout << "| Disposed    | " << setw(11) << disposedValueTotal << " |" << endl;
    cout << "| End Period  | " << setw(11) << endPeriodValueTotal << " |" << endl;
    cout << "-----------------------------" << endl;
    cout << "-->Press any key to go back<--";
    getch();
    system("cls");
    return;
}

int main() {
	MaterialRecord* records = nullptr; // голова списка записей

	// defoltnoe(стартовое) заполнение таблицы
	addRecord(records, createRecord(1, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
	addRecord(records, createRecord(2, 1, 101, "Johnson", 7000.0, 1500.0, 800.0));
	addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));
	addRecord(records, createRecord(4, 2, 101, "Anderson", 400.0, 1500.0, 100.0));
	addRecord(records, createRecord(5, 1, 102, "Besson", 70000.0, 1100.0, 0.0));
		addRecord(records, createRecord(1, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
	addRecord(records, createRecord(2, 1, 101, "Johnson", 7000.0, 1500.0, 800.0));
	addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));
	addRecord(records, createRecord(4, 2, 101, "Anderson", 400.0, 1500.0, 100.0));
	addRecord(records, createRecord(5, 1, 102, "Besson", 70000.0, 1100.0, 0.0));
		addRecord(records, createRecord(1, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
	addRecord(records, createRecord(2, 1, 101, "Johnson", 7000.0, 1500.0, 800.0));
	addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));
	addRecord(records, createRecord(4, 2, 101, "Anderson", 400.0, 1500.0, 100.0));
	addRecord(records, createRecord(5, 1, 102, "Besson", 70000.0, 1100.0, 0.0));
		addRecord(records, createRecord(1, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
	addRecord(records, createRecord(2, 1, 101, "Johnson", 7000.0, 1500.0, 800.0));
	addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));
	addRecord(records, createRecord(4, 2, 101, "Anderson", 400.0, 1500.0, 100.0));
	addRecord(records, createRecord(5, 1, 102, "Besson", 70000.0, 1100.0, 0.0));
		addRecord(records, createRecord(111111, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
	addRecord(records, createRecord(2, 1, 101, "Johnson", 7000.0, 1500.0, 800.0));
	addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));
	addRecord(records, createRecord(4, 2, 101, "Anderson", 400.0, 1500.0, 100.0));
	addRecord(records, createRecord(5, 1, 102, "Besson", 70000.0, 1100.0, 0.0));
		addRecord(records, createRecord(1, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
	addRecord(records, createRecord(2, 1, 101, "Johnson", 7000.0, 1500.0, 800.0));
	addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));
	addRecord(records, createRecord(4, 2, 101, "Anderson", 400.0, 1500.0, 100.0));
	addRecord(records, createRecord(5, 1, 102, "Besson", 70000.0, 1100.0, 0.0));
		addRecord(records, createRecord(1, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
	addRecord(records, createRecord(222222, 1, 101, "Johnson", 7000.0, 1500.0, 800.0));
	addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));
	addRecord(records, createRecord(4, 2, 101, "Anderson", 400.0, 1500.0, 100.0));
	addRecord(records, createRecord(5, 1, 102, "Besson", 70000.0, 1100.0, 0.0));
		addRecord(records, createRecord(1, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
	addRecord(records, createRecord(2, 1, 101, "Johnson", 7000.0, 1500.0, 800.0));
	addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));
	addRecord(records, createRecord(4, 2, 101, "Anderson", 400.0, 1500.0, 100.0));
	addRecord(records, createRecord(5, 1, 102, "Besson", 70000.0, 1100.0, 0.0));
		addRecord(records, createRecord(1, 1, 101, "Smith", 5000.0, 1000.0, 500.0));
	addRecord(records, createRecord(2, 1, 101, "Johnson", 7000.0, 1500.0, 800.0));
	addRecord(records, createRecord(3, 2, 201, "Williams", 6000.0, 1200.0, 700.0));
	addRecord(records, createRecord(4, 2, 101, "Anderson", 400.0, 1500.0, 100.0));
	addRecord(records, createRecord(5, 1, 102, "Besson", 70000.0, 1100.0, 0.0));
	system("cls");

	int c,i=1;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string print_main[14]= {"            Menu:","View Records","Add Record","Delete Record","Edit Record",
	                        "Save Records to file.txt","Save Records to file.bin","Load Records from file.txt","Load Records from file.bin",
	                        "Sort Records","Search by LastName","Clear Records","Generate Report","Exit"
	                       };
	int int_print_main = 14;
	while(true) {
		do {
			for(int j=0; j<int_print_main; j++) {
				if(j==i) {
					SetConsoleTextAttribute(hConsole, 10);
					cout<<" -> "<<print_main[j]<<endl;
				} else
					cout<<"    "<<print_main[j]<<endl;
				SetConsoleTextAttribute(hConsole, 15);
			}
			c=getch();
			if(c==115 || c==80) i++;
			if(c==119 || c==72) i--;
			if(i<1) i=1;
			if(i>int_print_main-1) i=int_print_main-1;
			system("cls");
		} while(c!= 13);


		switch (i) {
			case 1: {
				cout << "\nMaterial Records:" << endl;
				displayRecords(records);
				break;
			}
			case 2: {
				int id, factory, branch;
				string lastName;
				double start, received, disposed;
				
				cout << "Enter ID: ";
				cin >> id;
				idCheck(records, id);
				cout << "Enter Factory Number: ";
				cin >> factory;
				cout << "Enter Branch Number: ";
				cin >> branch;
				cout << "Enter Last Name: ";
				cin >> lastName;
				cout << "Enter Start Value: ";
				cin >> start;
				cout << "Enter Received Value: ";
				cin >> received;
				cout << "Enter Disposed Value: ";
				cin >> disposed;
				
				addRecord(records, createRecord(id, factory, branch, lastName, start, received, disposed));
				break;
			}
			case 3: {
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
			case 7: {
				loadFromTextFile(records, "material_records.txt");
				break;
			}
			case 8: {
				loadFromBinaryFile(records, "material_records.bin");
				break;
			}
			case 9: {
				int columnSort;
				cout << "(1)ID (2)Factory (3)Branch (4)Last Name (5)Start Value (6)Received Value (7)Disposed Value \n";
				cout << "Enter number column for sort: ";
				cin >> columnSort;
				sortRecords(records, columnSort);
				break;
			}
			case 10: {
				string searchLastName;
				cout << "Enter Last Name to search: ";
				cin >> searchLastName;
				searchByLastName(records, searchLastName);
				break;
			}
			case 11: {
				releaseMemory(records);
				cout << "Records has been cleared." <<endl;
				cout << "-->Press any key to go back<--";
    			getch();
    			system("cls");
				break;
			}
			case 12: {
				generateReport(records);
				break;
			}
			case 13: {
				exitProgram(records);
			}
		}
	}
}
