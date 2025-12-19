
struct Item {
    string name;
    int uses;          // -1 for infinite, 0..n for limited
};

struct User {
    int health;        // 0..100
    int energy;        // 0..100
    int points;        // preparedness score
    vector<Item> bag;  // inventory
};

// Implementations
bool HasItem(const User& player, const string& item) {
    for (const auto& it : player.bag) if (it.name == item && it.uses != 0) return true;
    return false;
}

bool UseItem(User& player, const string& item) {
    for (auto& it : player.bag) {
        if (it.name == item) {
            if (it.uses == -) return true;       // infinite-use item
            if (it.uses > 0) { it.uses--; return true; }
            return false;                          // out of uses
        }
    }
    return false; // not found
}

void Inventory(User& player, const string& item) {
    // If item already exists, optionally increment uses; else push.
    for (auto& it : player.bag) {
        if (it.name == item) { if (it.uses >= 0) it.uses++; return; }
    }
    // Default uses per item (tune as needed)
    int defaultUses = (item == "Flashlight") ? -1 :
                      (item == "Battery-Powered Radio") ? -1 :
                      (item == "Power Bank") ? 3 : 1;
    player.bag.push_back({item, defaultUses});
}

void Update_Stats(User& player, int deltaHealth, int deltaEnergy) {
    player.health = max(0, min(100, player.health + deltaHealth));
    player.energy = max(0, min(100, player.energy + deltaEnergy));
}

void AddPoints(User& player, int amount) {
    player.points += amount;
}
