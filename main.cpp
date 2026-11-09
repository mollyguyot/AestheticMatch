
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

struct Product {
    string id;
    string name;
    string category;
    string color;
    string description;
    double price;
};

string makeLower(string text) {
    for (int i = 0; i < text.size(); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = text[i] + 32;
        }
    }
    return text;
}

vector<string> splitRow(string line) {
    vector<string> parts;
    string word = "";
    bool inQuotes = false;

    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                word += '"';
                i++;
            }
            else {
                inQuotes = !inQuotes;
            }
        }
        else if (line[i] == ',' && !inQuotes) {
            parts.push_back(word);
            word = "";
        }
        else {
            word += line[i];
        }
    }

    parts.push_back(word);
    return parts;
}

bool validPrice(string price) {
    if (price == "") {
        return false;
    }

    int decimals = 0;
    int digits = 0;

    for (int i = 0; i < price.size(); i++) {
        if (price[i] >= '0' && price[i] <= '9') {
            digits++;
        }
        else if (price[i] == '.') {
            decimals++;

            if (decimals > 1) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    return digits > 0;
}

double getPrice(string price) {
    double number = 0;
    double place = 0.1;
    bool decimal = false;

    for (int i = 0; i < price.size(); i++) {
        if (price[i] == '.') {
            decimal = true;
        }
        else {
            int digit = price[i] - '0';

            if (!decimal) {
                number = number * 10 + digit;
            }
            else {
                number += digit * place;
                place = place / 10;
            }
        }
    }

    return number;
}

vector<Product> loadProducts(string filename) {
    vector<Product> items;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Could not open the dataset.\n";
        return items;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        vector<string> data = splitRow(line);

        if (data.size() < 6) {
            continue;
        }

        if (!validPrice(data[4])) {
            continue;
        }

        Product item;

        item.id = data[0];
        item.name = data[1];
        item.category = data[2];
        item.color = data[3];
        item.price = getPrice(data[4]);
        item.description = data[5];

        if (item.price >= 0 && item.price < 1000000) {
            items.push_back(item);
        }
    }

    file.close();

    cout << "Loaded " << items.size() << " products!\n";
    return items;
}

bool comparePrice(const Product& first, const Product& second) {
    if (first.price == second.price) {
        return first.id < second.id;
    }

    return first.price < second.price;
}

int sortPart(vector<Product>& items, int low, int high) {
    Product pivot = items[high];
    int i = low;

    for (int j = low; j < high; j++) {
        if (comparePrice(items[j], pivot)) {
            swap(items[i], items[j]);
            i++;
        }
    }

    swap(items[i], items[high]);
    return i;
}

void quickSort(vector<Product>& items, int low, int high) {
    if (low < high) {
        int pivot = sortPart(items, low, high);

        quickSort(items, low, pivot - 1);
        quickSort(items, pivot + 1, high);
    }
}

void librarySort(vector<Product>& items) {
    stable_sort(items.begin(), items.end(), comparePrice);
}

bool checkOccasion(Product item, int occasion) {
    if (occasion == 0) {
        return true;
    }

    string category = makeLower(item.category);
    string name = makeLower(item.name);
    string text = category + " " + name;

    if (occasion == 1) {
        if (text.find("set") != string::npos ||
            text.find("jersey") != string::npos ||
            text.find("shirt") != string::npos ||
            text.find("shorts") != string::npos ||
            text.find("jeans") != string::npos ||
            text.find("jacket") != string::npos ||
            text.find("dress") != string::npos ||
            text.find("spirit") != string::npos) {
            return true;
        }
    }
    else if (occasion == 2) {
        if (text.find("leggings") != string::npos ||
            text.find("shirt") != string::npos ||
            text.find("shorts") != string::npos ||
            text.find("sneakers") != string::npos ||
            text.find("skirt") != string::npos ||
            text.find("athletic") != string::npos ||
            text.find("running") != string::npos) {
            return true;
        }
    }
    else if (occasion == 3) {
        if (text.find("dress") != string::npos ||
            text.find("skirt") != string::npos ||
            text.find("blouse") != string::npos ||
            text.find("formal") != string::npos ||
            text.find("fancy") != string::npos ||
            text.find("heels") != string::npos) {
            return true;
        }
    }

    return false;
}

vector<Product> searchProducts(vector<Product>& items,
                               int occasion,
                               string category,
                               string color,
                               double maxPrice) {
    vector<Product> results;

    category = makeLower(category);
    color = makeLower(color);

    for (int i = 0; i < items.size(); i++) {
        Product item = items[i];

        if (item.price > maxPrice) {
            continue;
        }

        if (!checkOccasion(item, occasion)) {
            continue;
        }

        string itemCategory = makeLower(item.category + " " + item.name);

        if (category != "") {
            if (itemCategory.find(category) == string::npos) {
                continue;
            }
        }

        string itemColor = makeLower(item.color);

        if (color != "") {
            if (itemColor.find(color) == string::npos) {
                continue;
            }
        }

        results.push_back(item);
    }

    return results;
}

void showProducts(vector<Product>& results) {
    cout << "\n----- FILTERED RESULTS -----\n";
    cout << "Found " << results.size() << " matching products!\n\n";

    int count = results.size();

    if (count > 20) {
        count = 20;
    }

    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << results[i].name
             << " - $" << results[i].price << "\n";

        cout << "   Category: " << results[i].category << "\n";
        cout << "   Color: " << results[i].color << "\n\n";
    }

    if (results.size() > 20) {
        cout << "Showing the first 20 products.\n";
    }
}

int main() {
    vector<Product> items = loadProducts("products_hm.csv");

    if (items.empty()) {
        return 1;
    }

    int occasion = 0;
    string category = "";
    string color = "";
    double maxPrice = 1000000;
    int sortMethod = 1;
    int choice = 0;

    while (choice != 9) {
        cout << "\n========== AESTHETIC MATCH ==========\n";
        cout << "1. Select Occasion\n";
        cout << "2. Search by Category\n";
        cout << "3. Search by Color\n";
        cout << "4. Set Maximum Budget\n";
        cout << "5. Library Stable Sort\n";
        cout << "6. Quick Sort\n";
        cout << "7. View Filtered Results\n";
        cout << "8. Clear Filters\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (!cin) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            int option;

            cout << "\n----- SELECT AN OCCASION -----\n";
            cout << "1. Gameday\n";
            cout << "2. Athleisure Wear\n";
            cout << "3. Date Function\n";
            cout << "4. Back to Menu\n";
            cout << "Enter choice: ";
            cin >> option;

            if (cin && option >= 1 && option <= 3) {
                occasion = option;
                cout << "Occasion selected!\n";
            }
            else if (!cin) {
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }

        else if (choice == 2) {
            int option;

            cout << "\n----- SELECT A CATEGORY -----\n";
            cout << "1. Tops\n";
            cout << "2. Bottoms\n";
            cout << "3. Dresses\n";
            cout << "4. Shoes\n";
            cout << "5. Accessories\n";
            cout << "6. Any Category\n";
            cout << "7. Back to Menu\n";
            cout << "Enter choice: ";
            cin >> option;

            if (cin) {
                if (option == 1) {
                    category = "top";
                }
                else if (option == 2) {
                    category = "bottom";
                }
                else if (option == 3) {
                    category = "dress";
                }
                else if (option == 4) {
                    category = "shoe";
                }
                else if (option == 5) {
                    category = "accessor";
                }
                else if (option == 6) {
                    category = "";
                }

                if (option >= 1 && option <= 6) {
                    cout << "Category selected!\n";
                }
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }

        else if (choice == 3) {
            cout << "\n----- SEARCH BY COLOR -----\n";
            cout << "Enter a color (or press Enter for any): ";
            cin.ignore(10000, '\n');
            getline(cin, color);
            cout << "Color selected!\n";
        }

        else if (choice == 4) {
            double price;

            cout << "\n----- SET MAXIMUM BUDGET -----\n";
            cout << "Maximum price: $";
            cin >> price;

            if (cin && price >= 0) {
                maxPrice = price;
                cout << "Budget saved!\n";
            }
            else {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid budget.\n";
            }
        }

        else if (choice == 5) {
            sortMethod = 2;
            cout << "\nLibrary Stable Sort selected!\n";
            cout << "Choose option 7 to see your sorted results.\n";
        }

        else if (choice == 6) {
            sortMethod = 1;
            cout << "\nQuick Sort selected!\n";
            cout << "Choose option 7 to see your sorted results.\n";
        }

        else if (choice == 7) {
            vector<Product> results = searchProducts(
                items, occasion, category, color, maxPrice
            );

            if (results.empty()) {
                cout << "\nNo products matched your search.\n";
                cout << "Try changing your filters!\n";
            }
            else {
                auto start = chrono::high_resolution_clock::now();

                if (sortMethod == 1) {
                    quickSort(results, 0, (int)results.size() - 1);
                }
                else {
                    librarySort(results);
                }

                auto end = chrono::high_resolution_clock::now();

                double time = chrono::duration<double, milli>(end - start).count();

                showProducts(results);

                if (sortMethod == 1) {
                    cout << "Sorting algorithm: Quick Sort\n";
                }
                else {
                    cout << "Sorting algorithm: Library Stable Sort\n";
                }

                cout << "Sorting time: " << time << " ms\n";
            }
        }

        else if (choice == 8) {
            occasion = 0;
            category = "";
            color = "";
            maxPrice = 1000000;
            sortMethod = 1;

            cout << "\nFilters cleared!\n";
        }

        else if (choice == 9) {
            cout << "\nThanks for using AestheticMatch!\n";
        }

        else {
            cout << "Please choose an option from 1 to 9.\n";
        }
    }

    return 0;
}
