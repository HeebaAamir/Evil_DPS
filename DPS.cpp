#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // For rand()
#include <ctime>   // For time()

using namespace std;

// --- DATA STRUCTURES (STRUCTS ONLY) ---
// We use a struct to group data, but NO functions inside it.
struct Survivor {
    string name;
    int health;
    int energy;
    vector<string> inventory; // Dynamic array for items
    bool isAlive;
};

// --- FUNCTION PROTOTYPES ---
// We pass the Survivor struct by REFERENCE (&) so we can modify it.
void updateStats(Survivor& s, int healthChange, int energyChange);
void addToInventory(Survivor& s, string item);
void showStatus(const Survivor& s);
string scenarioEarthquake(Survivor& s);

int main() {
    // Seed random number generator
    srand(time(0));

    cout << "--- DISASTER PREPAREDNESS SIMULATOR (C++ Prototype) ---" << endl;
    
    // Initialize the Survivor Struct
    Survivor player;
    cout << "Enter Survivor Name: ";
    getline(cin, player.name);
    player.health = 100;
    player.energy = 100;
    player.isAlive = true;

    // --- PRE-GAME SETUP ---
    cout << "\n[WARNING]: Seismic activity detected nearby!" << endl;
    cout << "You have seconds to grab ONE item nearby:" << endl;
    cout << "1. First Aid Kit" << endl;
    cout << "2. 5L Water Bottle" << endl;
    cout << "3. Gaming Laptop" << endl;
    cout << "Choice: ";
    
    int choice;
    cin >> choice;

    if (choice == 1) addToInventory(player, "First Aid Kit");
    else if (choice == 2) addToInventory(player, "Water Bottle");
    else if (choice == 3) addToInventory(player, "Gaming Laptop");

    // --- START SCENARIO ---
    string outcome = scenarioEarthquake(player);

    // --- END GAME REPORT ---
    if (player.isAlive) {
        cout << "\n--- SCENARIO COMPLETE ---" << endl;
        cout << "Outcome: " << outcome << endl;

        // Calculate Score (Simple Algorithm)
        double score = (player.health * 0.4) + (player.energy * 0.3);
        
        // Bonus points for correct items
        bool hasMedKit = false;
        for (const string& item : player.inventory) {
            if (item == "First Aid Kit") {
                score += 20;
                hasMedKit = true;
            }
        }

        cout << "Final Score: " << score << "/100" << endl;

        if (score > 80) cout << "Rating: SURVIVAL EXPERT" << endl;
        else if (score > 50) cout << "Rating: SURVIVOR" << endl;
        else cout << "Rating: NOVICE - Needs Training" << endl;
    } else {
        cout << "\n--- GAME OVER ---" << endl;
        cout << "You did not survive the event." << endl;
    }

    return 0;
}

// --- FUNCTION DEFINITIONS ---

void updateStats(Survivor& s, int healthChange, int energyChange) {
    s.health += healthChange;
    s.energy += energyChange;

    // Cap stats between 0 and 100
    if (s.health > 100) s.health = 100;
    if (s.energy > 100) s.energy = 100;
    if (s.health <= 0) {
        s.health = 0;
        s.isAlive = false;
    }

    showStatus(s);
}

void addToInventory(Survivor& s, string item) {
    if (s.inventory.size() < 5) {
        s.inventory.push_back(item);
        cout << "[ITEM ADDED]: " << item << endl;
    } else {
        cout << "[INVENTORY FULL]: Cannot carry " << item << endl;
    }
}

void showStatus(const Survivor& s) {
    cout << "\n[STATUS] HP: " << s.health << " | Energy: " << s.energy << "%" << endl;
}

string scenarioEarthquake(Survivor& s) {
    cout << "\n========================================" << endl;
    cout << "       SCENARIO: MAGNITUDE 7.0          " << endl;
    cout << "========================================" << endl;
    cout << "You are on the 4th floor. The room starts shaking violently." << endl;
    cout << "Bookshelf crashes down. Debris is falling." << endl;
    cout << "What is your immediate action?" << endl;
    cout << "1. Run for the elevator." << endl;
    cout << "2. Drop, Cover, and Hold On (under table)." << endl;
    cout << "3. Run to the balcony." << endl;
    
    int choice;
    cout << "Enter Choice (1-3): ";
    cin >> choice;

    if (choice == 1) {
        cout << "\n>> You run to the elevator." << endl;
        cout << "ERROR: Power fails. You are trapped in the dark." << endl;
        updateStats(s, -40, -20); // Big health penalty
        return "Trapped in Elevator";
    }
    else if (choice == 2) {
        cout << "\n>> You dive under the sturdy oak table." << endl;
        cout << "Ceiling plaster smashes on the table, but you are safe." << endl;
        updateStats(s, 0, -5); // Only energy loss
        return "Survived Unharmed";
    }
    else if (choice == 3) {
        cout << "\n>> You run to the balcony." << endl;
        cout << "The shaking throws you against the railing." << endl;
        updateStats(s, -60, -10); // Major injury
        return "Injured on Balcony";
    }
    else {
        cout << "\n>> You freeze in panic." << endl;
        updateStats(s, -20, 0);
        return "Panicked";
    }
}