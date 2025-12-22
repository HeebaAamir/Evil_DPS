#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include <future>
#include<chrono>
#include<thread>
#include <iomanip>
#include <windows.h>
//using vector for a dynamic array to store inventory items as it is modifiable. 
//Items can be both added and removed 
#include<vector>
#include<algorithm>//for find fucntion
#include<fstream>
#include<conio.h>
#include<limits>


//Colour coding function
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

using namespace std;

//Struct for the user details
struct User{
    string name;
    int health;
    vector <string>inventory;
    int energy;
    bool isAlive;
};

//Struct for item management
struct Item {
    string name;
    int uses;          
};

                                //Function Definitions
void delay(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}



int GetTimedInput(int seconds) {
    std::string buf; // collect typed digits
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);

    // Clean any leftover characters (e.g., a newline from previous cin >>)
    if (std::cin.rdbuf()->in_avail() > 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Draw once at start
    int lastShown = -1;
    bool userStartedTyping = false;

    while (true) {
        // Show countdown only if user hasn't started typing
        if (!userStartedTyping) {
            auto now = std::chrono::steady_clock::now();
            int remaining = (int)std::chrono::duration_cast<std::chrono::seconds>(deadline - now).count();
            if (remaining < 0) remaining = 0;

            if (remaining != lastShown) {
                 setColor(1);
                std::cout << "\rTime remaining: " << std::setw(2) << std::setfill('0')
                          << remaining << "s " << std::flush;
                lastShown = remaining;
                 setColor(7);
            }
        }

        // Key pressed?
        if (_kbhit()) {
            int ch = _getch();
            userStartedTyping = true; // stop overwriting countdown once typing begins

            // Enter ends input
            if (ch == '\r' || ch == '\n') {
                std::cout << "\n";
                if (buf.empty()) return -1;
                try {
                    return std::stoi(buf);
                } catch (...) {
                    return -1;
                }
            }

            // Backspace
            if (ch == 8) {
                if (!buf.empty()) {
                    buf.pop_back();
                    // erase last char visually
                    std::cout << "\b \b" << std::flush;
                }
                continue;
            }

            // Only accept digits
            if (ch >= '0' && ch <= '9') {
                if (buf.size() < 3) {         // basic guard
                    buf.push_back((char)ch);
                    std::cout << (char)ch << std::flush;
                }
            }
            // Ignore other keys
        }

        // Timed out?
        if (std::chrono::steady_clock::now() >= deadline) {
             setColor(4);
            std::cout << "\nTIME'S UP! You hesitated!\n";
             setColor(7);
            // Clean any pending stdin
            if (std::cin.rdbuf()->in_avail() > 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            return -1;
        }

        // Sleep small, but redraw is gated to once per second via lastShown
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
//Function for items that the user can carry
void Inventory(User& player, string item){
    if (player.inventory.size()<5){
         setColor(6);
        cout<<"Adding to inventory.....\n";
        player.inventory.push_back(item);
         setColor(2);
        cout<<item<<" was added to your inventory.\n";
         setColor(7);
    }
    else{
         setColor(4);
        cout<<"Item wasn't added. You can carry maximum 5 items at a time!\n";
         setColor(7);
    }
}

//Function to see if user has smth in their inventory
//passing the 2 structs into it
bool HasItem(const User& player, const std::string& item) {

    //auto will automatically choose the right data type for variable "checker"
    for (const auto& checker : player.inventory) {
        if (checker == item) {
            return true;   
        }
    }
    return false;
}


//Function to update stats
void Update_Stats(User& player, int health_change, int energy_change){
    //I will pass neg values for health and energy points loss so I am simply adding them
    player.health +=health_change;
    player.energy+=energy_change;

    //Making sure that the values stay in range
    if (player.energy > 100 ) {
            player.energy = 100;
        } 
    else if (player.energy < 0 ) {
            player.energy = 0;
        }

    if (player.energy==0){
        //being at 0 energy damages health so everytime this function gets called it will dec health additionally
        player.health-=3;
    }
    //fixing ranges for health as well
    if (player.health > 100) {
            player.health = 100;
        } else if (player.health < 0) {
            player.health = 0;
        }

    //For any neg health just consider it to be 0 to simplify things
    //At 0 health, game ends cause the player dies.
    if(player.health==0){
        player.isAlive = false;
    }
}
void Player_Condition(int&health, int&energy){
     setColor(1);
    cout<<"Current Energy And Health Level is: \n";
     setColor(6);
    cout<<"Health xp: "<<health<<" % "<<" || "<<"Energy xp: "<<energy<<" % "<<endl;
     setColor(7);
}



//Fucntion to remove item from inventory to be able to use it
bool RemoveItem(User& player, const string& item) {
    //This is a standard find function with its syntax.
    //due to auto, the variable takes the data type that it is supposed to be
    //Rest is the syntax of the find function
    auto var = find(player.inventory.begin(), player.inventory.end(), item);
    if (var != player.inventory.end()) {
        player.inventory.erase(var);
        return true;
    }
    return false;
}


//function to print out an inventory for user
void ShowInventory(const User& player) {
     setColor(5);
cout << "Your inventory: ";
//this is a variable counter about how many actual items there are
//Bcs right now I have initialized the inventory as "EMPTY"
    int Real_Item_Count = 0;
    for (const auto& itemName : player.inventory) {
        
        //if that item isnt empty and exsists print it out
            if (itemName != "Empty") {
                cout << "[" << itemName << "] ";
                ++Real_Item_Count;
            }
        }

    if (Real_Item_Count == 0) {
        cout << "(empty)";
    }
    cout << "\n";
     setColor(7);
}


//Offering user chances to pick up stuff
//USED AI REFRENCE FOR THIS FUNCTION
void OfferPickup(User& player, const vector<string>& itemsNearPlayer, const string& descriptionPrompt) {
    // If there is nothing to offer, do nothing and return.
    if (itemsNearPlayer.empty()) return;

    cout << "\n=== Nearby Items ===\n";
    cout << descriptionPrompt << "\n";

    // List all nearby items with a 1-based index for user selection.
    for (size_t index = 0; index < itemsNearPlayer.size(); ++index) {
        cout << (index + 1) << ". " << itemsNearPlayer[index] << "\n";
    }
    cout << "0. Ignore and continue\n";

    // Read the player's choice.
    int chosenIndex;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Add this line first
    cin >> chosenIndex;

    // Validate input: 0 means "skip", out-of-range means "invalid".
    if (chosenIndex <= 0 || chosenIndex > static_cast<int>(itemsNearPlayer.size())) {
         setColor(6);
        cout << "You decide not to pick up anything.\n";
        return;
    }

    // Convert the 1-based menu choice to a 0-based vector index and add the item.
    // Your Inventory(...) function already handles the "max 5 items" rule and prints feedback.
    const string& selectedItem = itemsNearPlayer[chosenIndex - 1];
    Inventory(player, selectedItem);
}


//AI HELP FOR THE USING FUNCTION
void UseInventoryItem(User& player) {
// Filter out the "Empty" placeholder
    vector<string> usable;
    usable.reserve(player.inventory.size());
    for (const auto& s : player.inventory) {
        if (s != "Empty") usable.push_back(s);
    }

    if (usable.empty()) {
         setColor(4);
        cout << "You have nothing useful to use right now.\n";
        return;
         setColor(7);
    }

    cout << "\n=== Use an Item ===\n";
    ShowInventory(player);
     setColor(5);
    cout << "Choose an item to use:\n";
    for (size_t i = 0; i < usable.size(); ++i) {
        cout << (i + 1) << ". " << usable[i] << "\n";
    }
    cout << "0. Cancel\n";

    int pick;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin >> pick;
   
    if (pick == 0) {
         setColor(6);
        cout << "You decided not to use anything.\n";
        return;
    }
    if (pick < 1 || pick > static_cast<int>(usable.size())) {
         setColor(14);
        cout << "Invalid choice.\n";
        return;
    }

    const string& item = usable[pick - 1];

    if (item == "First Aid Kit") {
         setColor(2);
        cout << "You open the First Aid Kit and treat minor wounds.\n";
         setColor(7);
        Update_Stats(player, +15, -5);
        RemoveItem(player, item);
    } else if (item == "Water Bottle") {
         setColor(2);
        cout << "You drink water and feel more alert.\n";
         setColor(7);
        Update_Stats(player, 0, +15);
   
    } else if (item == "Gaming Laptop") {
         setColor(4);
        cout << "You lug the gaming laptop around... it's not very helpful.\n";
         setColor(7);
        Update_Stats(player, -2, -8);
        RemoveItem(player, item);

    } else if (item == "Flashlight") {
         setColor(6);
        cout << "You switch on the flashlight. The path is clearer.\n";
         setColor(7);
        Update_Stats(player, 0, +8);
   
    } else if (item == "Battery-Powered Radio") {
         setColor(6);
        cout << "You tune into emergency broadcasts. The guidance calms you.\n";
        Update_Stats(player, 0, +6);
        RemoveItem(player, item);
         setColor(7);
        
    } else if (item == "Power Bank") {
         setColor(6);
        cout << "You top up your phone briefly and feel reassured.\n";
        Update_Stats(player, 0, +6);
        RemoveItem(player, item);
         setColor(7);
    } else {
         setColor(4);
        cout << "That item doesn't help here.\n";
        cout << "Can't use it right now.\n";
         setColor(7);
    }
    //displaying user's health now after using some items to heal himself
    Player_Condition(player.health, player.energy);
}




//Scenario = Earthquake
string EarthQuake(User& player){
    setColor(5);
    cout << "\n========================================" << endl;
    cout << "       CIRCUMSTANCE : MAGNITUDE 7.0          " << endl;
    cout << "========================================" << endl;

    //Some context
    delay(1);
    setColor(4);
    cout << "\n[WARNING]: Seismic activity detected nearby!" << endl;
    cout << "You have 15 seconds to grab ONE item nearby:" << endl;
     setColor(1);
    cout << "1. First Aid Kit" << endl;
    cout << "2. 5L Water Bottle" << endl;
    cout << "3. Gaming Laptop" << endl;
     setColor(6);
    cout << "What will you choose?: ";
    
    int choice;
    choice = GetTimedInput(15);

    if (choice == 1){
        Inventory(player, "First Aid Kit");
    } 
    else if (choice == 2) {
        Inventory(player, "Water Bottle");
    }
    else if (choice == 3) {
        Inventory(player, "Gaming Laptop");
    }else{
         setColor(4);
        cout << "Time's over! You didn't pick anything.\n";
    }

    //Question #1 
    delay(2);
     setColor(1);
    cout << "You are on the 4th floor. The room starts shaking violently." << endl;
    cout << "Bookshelf crashes down." << endl;
    cout << "You can hear the walls cracking and the ceiling fan creaking.\n"<<endl;
    cout << "What is your immediate action?" << endl;
     setColor(7);
    cout << "1. Run for the elevator." << endl;
    cout << "2. Drop, Cover, and Hold on to wait under a table till the shocks settle down." << endl;
    cout << "3. Run to the balcony." << endl;
    cout << "4. Use an item from your inventory.\n";
    setColor(6);
    cout<<"Enter your choice (1-4): \n";
    choice = GetTimedInput(20);
    
    switch(choice){
        case 1:
        setColor(4);
        cout<<"You run to the elevator.....";
        cout<<"ERROR:: Power failed. You are stuck inside\n";
        cout<<"It's extremely dark in the elevator\n";
        //He is stuck so he suffers a major energy loss
        Update_Stats(player, -5, -40 );
        break;

        case 2:
        setColor(2);
        cout<<"You scan the room to find a place you can take shelter in....\n";
        cout<<"You spot a wooden table nearby.\n";
        cout<<"You run towards it.\n";
        cout<<"You take cover and wait for the shocks to settle.\n";
        //Only energy less for finding and running, no effect on health
        Update_Stats(player, 0, -20);
        break;

        case 3:
        setColor(4);
        cout<<"You run towards the balcony.....\n";
        cout<<"You are unable to walk properly to the spot.\n";
        cout<<"The shaking ground and building overpowers you!\n";
        cout<<"You fall to the ground!!\n";
        cout<<"Your head hurts and your vision getes blurred.\n";
        //Major injury and loss to health
        Update_Stats(player, -70, -65);
        break;
        
        case 4: 
        UseInventoryItem(player);
        break;

        default:
        setColor(15);
        cout << "You hesitate and do nothing...\n";
        //Panic affects both physical and mental health and drains energy while wasting time
        Update_Stats(player, -5, -5); 
        break;
    }

    //Question #2 
    delay(2);
    setColor(1);
    cout << "\nThe shaking slows down... but the building isn't still yet.\n";
    cout << "Dust is in the air and small pieces of plaster are falling.\n";
    cout << "You fear an aftershock might hit at any moment.\n\n";
    cout << "What do you do now?\n";
     setColor(7);
    cout << "1. Stay under the table and wait for 1 more minute.\n";
    cout << "2. Move towards the emergency stairs carefully.\n";
    cout << "3. Call out for help to see if anyone else is nearby.\n";
    cout << "4. Use an item from your inventory.\n";
     setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(2);
        cout << "You stay under the cover protecting your head from any injury or bruise...\n";
        cout << "You remain safe as debris falls around you.\n";
        cout<<"You made a safe and good choice.\n";
        //Safe healthwise but can tire up a little
        Update_Stats(player, 0, -10); 
        break;

    case 2:
        setColor(14);
        cout << "You plan to reach for the emergency exit....\n";
        cout<<"You are moving very carefully; holding the railing and walls for support.\n";
        cout << "You stumble a bit due to some debris lying around, but you keep moving.\n";
        cout<<"You hurt your knee a little but the injury isn't worth worrying about.\n";
        //energy used
        Update_Stats(player, -5, -20); 
        break;

    case 3:
        setColor(4);
        cout << "You shout for help....\n";
        cout<<"You can hear a faint noise coming from the corridor./n";
        cout<<"You try to locate were they are.\n";
        Update_Stats(player, 0, -5); 
        break;

    case 4: 
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "You hesitate and do nothing...\n";
        Update_Stats(player, -5, -5); 
        break;
    }

    //Question 3
    delay(2);
    setColor(1);
    cout << "\nYou finally reach the staircase. The main path is blocked by fallen debris.\n";
    cout << "There is a narrow gap you might squeeze through, or you could try finding another route.\n";
    cout << "What is your call?\n";
     setColor(7);
    cout << "1. Try to clear some debris to make space to move.\n";
    cout << "2. Look for an alternate staircase or fire escape.\n";
    cout << "3. Crawl through the narrow gap carefully.\n";
    cout << "4. Use an item from your inventory.\n";
     setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

switch (choice) {
    case 1:
        setColor(14);
        cout << "You push through the piles of concrete and wood....\n";
        cout << "Your hands get scraped and your breathing gets heavy.\n";
        cout<<"It is tiring.\n";
        Update_Stats(player, -10, -27); 
        break;

    case 2:
        setColor(2);
        cout << "You scan the corrdior for another escape....\n";
        cout << "You find a marked fire door slightly to your left.\n";
        cout <<"You are hopeful.\n";
        Update_Stats(player, -3, -15); 
        break;

    case 3:
        setColor(4);
        cout << "You flatten yourself and try to slide through the gap.\n";
        cout << "A jagged edge cuts your arm as you squeeze past!\n";
        Update_Stats(player, -15, -10); // more health loss, less energy
        break;

    case 4:                           
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout<<"YOu do nothing.\n";
        cout << "You stand still overwhelmed by the situation...\n";
        Update_Stats(player, -5, -5);
        break;
    }
    
// Question #4 
    delay(2);
    setColor(1);
    cout << "\nYou smell smoke. A thin haze fills the corridor near the ceiling....\n";
    cout << "You hear cracking sounds from a room down the hall.\n";
    delay(1);
    //Giving a chance midgame to pick items
    OfferPickup(player, {"Water Bottle", "First Aid Kit"},
            "You spot a half-full water bottle and a small first-aid pouch on a cart.");
    delay(2);
     setColor(1);
    cout << "How do you proceed?\n";
     setColor(7);
    cout << "1. Cover your mouth with cloth and crawl low to the exit.\n";
    cout << "2. Use nearby water and try setting off small flames near the door (if any).\n";
    cout << "3. Run past the smoke quickly to save time.\n";
    cout << "4. Use your inventoy.\n";
    setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(14);
        cout << "You get down on your hands and knees...\n";
        cout <<"You are trying to keep yourself below the rising smoke.\n";
        cout << "Your eyes sting, but you keep moving to reach the exit.\n";
        Update_Stats(player, -8, -15); 
        break;

    case 2:
        setColor(2);
        cout << "You see some fire flames rising from the bottom of a door in the corridor.\n";
         cout<<"Your eyes hurt due to rising smoke.\n";
        cout <<"You find an emergency water hydrant at the edge of the corridor.\n";
        cout << "The flames subside, making the corridor safer to pass.\n";
        Update_Stats(player, -5, -15); 
        break;

    case 3:
        setColor(4);
        cout << "You sprint through the smoke!\n";
        cout << "You cough violently and feel dizzy.\n";
        //risky choice
        Update_Stats(player, -25, -15); 
        break;

    case 4: 
        UseInventoryItem(player);
        break;

       default:
        setColor(15);
        cout << "You hesitate, breathing more smoke as time passes.\n";
        Update_Stats(player, -15, -5);
        break;

    }
    //Question #5
    delay(2);
    setColor(1);
    cout << "\nYou spot someone on the floor holding their leg, calling for help.\n";
    cout << "They are conscious but in pain.\n\n";
    cout << "What do you do?\n";
     setColor(7);
    cout << "1. Help them stand and guide them to a safer area.\n";
    cout << "2. Provide quick first aid and move on.\n";
    cout << "3. Promise to call rescuers and continue towards the exit.\n";
     setColor(6);
    cout << "Enter your choice (1-3): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(14);
        cout <<"You move closer to the injured person.\n";
        cout << "You support their weight and move slowly.\n";
        cout << "It takes time and drains your strength.\n";
        Update_Stats(player, -3, -22); 
        break;

    case 2:
        cout <<"You move closer to the injured person.\n";
        cout << "You apply pressure and stabilize the injury as best as you can.\n";
        cout << "They thank you and you both feel a bit safer.\n";
        cout <<"You keep moving to find an exit and help.\n";
        Update_Stats(player, -3, -12); 
        break;

    case 3:
        cout <<"You move closer to the injured person.\n";
        cout << "You reassure them and prioritize reaching an exit.\n";
        cout << "You conserve your energy and keep moving forward but feel guilty.\n";
        Update_Stats(player, 0, -5); // minimal cost
        break;

    default:
        setColor(15);
        cout << "You freeze; unable to decide what to choose and lose precious seconds.\n";
        Update_Stats(player, -5, -5);
        break;
    }
     setColor(6);
    return "EarthQuake Simulation Ccmplete!\n";
}



//Scenario: Power Outage
string Power_Outage(User& player){
        setColor(5);
        cout<<endl;
        cout << "========================================" << endl;
        cout << "     CIRCUMSTANCE : CITY-WIDE BLACKOUT         " << endl;
        cout << "========================================" << endl;

    // Some context
    delay(1);
     setColor(1);
    cout << "\n[ALERT]: The power has gone out across the city!" << endl;
    cout << "It's night, and the building emergency lights are flickering." << endl;
    cout << "You have 15 seconds to grab ONE item nearby:" << endl;
     setColor(7);
    cout << "1. Flashlight" << endl;
    cout << "2. Battery-Powered Radio" << endl;
    cout << "3. Power Bank (Phone at 20%)" << endl;
     setColor(6);
    cout << "What will you choose?: ";

    int choice;
    choice = GetTimedInput(15);

    if (choice == 1) {
        Inventory(player, "Flashlight");
       
    }  else if (choice == 2) {
        Inventory(player, "Battery-Powered Radio");
        
    } else if (choice == 3) {
        Inventory(player, "Power Bank");
    }else{
         setColor(4);
        cout << "Time's over! You didn't pick anything\n";
    }

    delay(2);
     setColor(1);
    cout<<".............................................................................\n";
    cout << "\nYou are on the 4th floor. The emergency lights flicker and then turn off" << endl;
    cout << "The hallway is pitch dark and you hear distant sirens outside." << endl;
    cout << "What is your immediate action?" << endl;
     setColor(7);
    cout << "1. Head for the elevator to reach the lobby." << endl;
    cout << "2. Stay put, use your light, and secure hazards (You unplug all electronics)." << endl;
    cout << "3. Step onto the balcony to assess what's happening outside." << endl;
    cout << "4. Use an item from your inventory. \n";
     setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {

        case 1:
            setColor(4);
            cout << "You walk toward the elevator....\n";
            cout << "ERROR: No power. The elevator is dead.\n";
            cout << "You fumble in the dark and feel anxious.\n";
            Update_Stats(player, -3, -25);
            break;

        case 2:
            setColor(2); 
            cout << "You turn on a  small light source and do a quick safety sweep...\n";
            cout << "You unplug sensitive devices and clear trip hazards.\n";
            cout << "Good call! You reduce risk while conserving energy.\n";
           // best-practice behavior
            Update_Stats(player, 0, -10);
            break;
        
        case 3: 
            setColor(14);
            cout << "You step onto the balcony to look around...\n";
            cout << "The wind picks up; it's cold and visibility is poor.\n";
            cout << "You get a mild chill but spot that the whole block is dark.\n";
            Update_Stats(player, -5, -12);
            break;

        case 4:
        UseInventoryItem(player);
        break;

        default:
            setColor(15);
            cout << "You hesitate and do nothing...\n";
            Update_Stats(player, -5, -5);
            break;
    }

    //Question #2
    delay(2);
     setColor(1);
    cout << "\nYou hear a faint buzzing from your apartment's breaker panel.\n";
    cout << "A neighbor shouts that the outage might be on the grid.\n";
    cout<<".........................\n";
    delay(2);
     setColor(7);
    OfferPickup(player, {"Flashlight"},
            "Near the breaker panel, you notice a spare flashlight on a shelf.");
         setColor(1);
         delay(1);
    cout << "What do you do now?\n";
     setColor(7);
    cout << "1. Inspect your unit's breaker panel carefully with a flashlight.\n";
    cout << "2. Head to the basement to check a building generator (if any).\n";
    cout << "3. Use your radio/phone to get utility updates and stay put.\n";
    cout << "4. Use an item from your inventory.\n";
    setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch(choice){
    case 1:
        setColor(14);
        cout << "You open the panel gently and check for tripped breakers...\n";
        cout << "No burning smell, no visible damage. It looks normal.\n";
        cout << "You close it and decide to wait for updates.\n";
        Update_Stats(player,0, -15);
        break;

        case 2:
        setColor(4);
        cout << "You make your way to the basement...\n";
        cout << "It's dark, stuffy, and a bit disorienting.\n";
        cout << "You don't find a safe way to start anything; better to avoid risks.\n";
        Update_Stats(player,-5 ,-10);
        break;

        case 3:
        setColor(2);
        cout << "You tune into emergency updates...\n";
        cout << "Authorities confirm a grid failure and advise staying indoors.\n";
        cout << "You conserve energy and plan for several hours without power.\n";
        cout<<"You made a good choice!\n";
        Update_Stats(player, 0, -5);
        break;

        case 4:
        UseInventoryItem(player);
        break;

        default:
        setColor(15);
        cout << "You do nothing and hope power returns...\n";
        Update_Stats(player, -5, -5);
    }  

    //Question #3
    delay(2);
    setColor(1);
    cout << "\nYour refrigerator and freezer are now off.\n";
    cout << "Perishables could spoil if the outage lasts long.\n";
    cout << "How do you manage food safety?\n";
     setColor(7);
    cout << "1. Keep doors closed; move essentials to a cooler with ice if available.\n";
    cout << "2. Open the fridge repeatedly to check and snack now.\n";
    cout << "3. Cook perishables immediately on a gas stove for light and warmth.\n";
    cout << "4. Use something from the inventory.\n";
     setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch(choice){
        case 1:
        setColor(2);
        cout << "You limit opening and use a cooler efficiently...\n";
        cout << "You remember that you have stored perishable items in your refrigrator which you can use in an emergency.\n";
        cout << "You find an ice cooler to store milk and leftover food from tomorrow.\n";
        cout << "You extend the safe time for your food.\n";
        Update_Stats(player,0,-10);
        break;

        case 2:
        setColor(4);
        cout << "You open the fridge often and grab snacks...\n";
        cout << "Cold escapes quickly; food warms faster.\n";
        cout<<"You realize that your choice wasn't that well-thought upon.\n";
        Update_Stats(player,-17,-5);
        break;

        case 3:
        setColor(14); 
        cout << "You cook some perishables using the gas stove...\n";
        cout << "You ventilate, but there's minor exposure to fumes.\n";
        cout << "You secure some meals but feel a bit lightheaded.\n";
        cout <<"You won some, lost some. Overall not that bad of a choice.\n";
        Update_Stats(player, -9,-16);
        break;

        case 4:
        UseInventoryItem(player);
        break;

        default:
        setColor(15);
        cout << "You do nothing and hope power returns...\n";
        Update_Stats(player, -5, -5);
    }

    //Question 4
    
    OfferPickup(player, {"Power Bank"},
            "In a drawer, you find a spare power bank with some charge left.");
    delay(2);
     setColor(1);
    cout<<" 45 minutes have passed......\n";
    cout << "\nNight deepens. You need light and a way to charge your phone.\n";
    cout << "What is your approach?\n";
     setColor(7);
    cout << "1. Use battery lanterns/flashlight and ration batteries.\n";
    cout << "2. Light multiple candles around the apartment for brightness.\n";
    cout << "3. Sit in your idling car inside the garage to charge devices.\n";
    cout << "4. Use something from your inventory.\n";
    setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
        case 1:
            setColor(2);
            cout << "You rely on safe battery lights and plan usage...\n";
            cout << "Visibility is decent and fire risk is minimal.\n";
            Update_Stats(player, 0, -10);
            break;
        
        case 2: 
            setColor(14);
            cout << "You light several candles for illumination...\n";
            cout << "A small flame tips near papers—close call!\n";
            cout << "You quickly clear the area but remain wary of fire hazards.\n";
            Update_Stats(player, -8, -12);
            break;
        
        case 3: 
            setColor(4);
            cout << "You sit in your idling car in the closed garage to charge...\n";
            cout << "BAD IDEA!! Carbon monoxide risk rises fast!\n";
            cout << "You feel dizzy and rush back out; that was dangerous.\n";
            Update_Stats(player, -25, -15);
            break;
        
        case 4:
        UseInventoryItem(player);
        break;
        
        default:
            setColor(15);
            cout << "You delay, straining your eyes in the dark and wasting time.\n";
            Update_Stats(player, -5, -5);
            break;
    }

    //Question 5
    delay(2);
    setColor(1);
    cout << "\nYou encounter a neighbor whose medical device (CPAP/oxygen concentrator) needs power.\n";
    cout << "They are conscious but anxious and mildly short of breath.\n";
    cout << "What do you do?\n";
     setColor(7);
    cout << "1. Help them relocate to a nearby clinic/shelter with backup power.\n";
    cout << "2. Offer your power bank for phone calls and stay with them briefly.\n";
    cout << "3. Promise to inform responders and continue managing your own situation.\n";
    cout << "4. Use an item from your inventory.\n";
     setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
        case 1: 
            setColor(14);
            cout << "You support the neighbor and move carefully to a safer location...\n";
            cout << "It takes effort, but their condition stabilizes.\n";
            cout<<"You lost energy and feel drained.\n";
            Update_Stats(player, -5, -22);
            break;
        
        case 2: 
            setColor(2);
            cout << "You share your power bank and help them make necessary calls...\n";
            cout << "You reassure them and note the nearest facility.\n";
            cout<<"You power bank is a bit drained now but you feel happy to help.\n";
            Update_Stats(player, -2, -12);
            break;
        
        case 3:
            setColor(14); 
            cout << "You promise to notify responders and proceed to secure your area...\n";
            cout << "You conserve energy but feel uneasy about leaving.\n";
            cout<<"You know that you can be of help to someone else only if you stay alive and safe yourself.\n";
            Update_Stats(player, 0, -6);
            break;
        
        case 4:
        UseInventoryItem(player);
        break;

        default:
            cout << "You freeze, losing precious minutes...\n";
            Update_Stats(player, -5, -5);
            break;
    }
     setColor(6);
    return "PowerOutage Completed";
}

string Fire(User& player) {
    setColor(5);
    cout << "\n========================================" << endl;
    cout << "       CIRCUMSTANCE : BUILDING ON FIRE         " << endl;
    cout << "========================================" << endl;

    //Some context
    delay(2);
    setColor(4);
    cout << "\n[ALERT]: You smell smoke coming from outside your apartment.The fire alarm has not been triggered yet. !" << endl;
    cout << "You hear the panicked screams of people" << endl;
    cout << "You have 15 seconds to grab ONE item nearby:" << endl;
     setColor(7);
    cout << "1. First Aid Kit" << endl;
    cout << "2. Fire Blanket/ Protective Cloth" << endl;
    cout << "3. Water Bottle" << endl;
     setColor(6);
    cout << "What will you choose?: ";

    int choice;
    choice = GetTimedInput(15);

    if (choice == 1) {
        Inventory(player, "First Aid Kit");
    }
    else if (choice == 2) {
        Inventory(player, "Fire Blanket");
    }
    else if (choice == 3) {
        Inventory(player, "Water Bottle");
    }else{
         setColor(4);
        cout << "Time's over! You didn't pick an item within time.\n";
    }

    //Question #1 
    delay(2);
    setColor(1);
    cout << "Smoke smell increases, but the fire alarm is not ringing" << endl;
    cout << "Some neighbors claim it's “a false alarm”." << endl;
    cout << "Others are evacuating in panic" << endl;
    cout << "What is your immediate action?" << endl;
     setColor(7);
    cout << "1. Trust your gut and move to evacuate the building." << endl;
    cout << "2. Wait for the confirmation." << endl;
    cout << "3. Investigate the source." << endl;
    cout << "4. Use an item from your inventory.\n";
     setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(2);
        cout << "You act early, avoiding panic and congestion.\n ";
        Update_Stats(player, -5, 0);
        break;

    case 2:
        setColor(14);
        cout << "You decide to wait and observe.\n";
        cout << "The smell of smoke becomes stronger.\n";
        Update_Stats(player, 0, -15);
        break;

    case 3:
        setColor(14);
        cout << "You cautiously step into the hallway.\n";
        cout << "Smoke is visible near the ceiling.\n";
        cout << "You confirm a fire below your floor!\n";
        if (HasItem(player, "Fire Blanket")) {
            cout << "You cover your mouth with the blanket\n";
            Update_Stats(player, 0, -10);
        }
        else {
            Update_Stats(player, -70, -65);
        }
        break;

    case 4:
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "You hesitate, unsure what to do.\n";
        cout << "Valuable time is lost as the situation worsens.\n";
        Update_Stats(player, -5, -5);
        break;
    }

    //Question #2 
    delay(1);
    OfferPickup(player, { "Fire Extnguisher" },
        "You found a fire extinguisher outside your apartment.");
    delay(2);
    setColor(15);
     setColor(1);
    cout << "\nSmoke thickens rapidly..\n";
    cout << "Heat rises and visibility drops.\n";
    cout << "What do you do now?\n";
     setColor(7);
    cout << "1. Use the staircase.\n";
    cout << "2. Try the elevator.\n";
    cout << "3. Seal your apartment temporarily.\n";
    cout << "4. Use an item from your inventory.\n";
     setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(2);
        cout << "You head for the emergency staircase.\n";
        cout << "Visibility is poor but manageable..\n";
        cout << "Your fire blanket helps you in not inhaling the smoke.\n";
        //Safe healthwise but can tire up a little
        Update_Stats(player, -5, -10);
        break;

    case 2:
        setColor(4);
        cout << "You press the elevator button.\n";
        cout << "ERROR: Power failure..\n";
        cout << "The elevator doors remain shut.\n";
        cout << "You panic as smoke fills the corridor.\n";
        //Safe healthwise but can tire up a little
        Update_Stats(player, -15, -20);
        break;
    case 3:
        setColor(4);
        cout << "You block vents and seal the door with cloth.\n";
        cout << "Smoke enters slowly...\n";
        Update_Stats(player, 0, -15);
        break;

    case 4:
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "You hesitate and do nothing...\n";
        Update_Stats(player, -5, -5);
        break;
    }

    //Question 3
    delay(2);
    setColor(1);
    cout << "\nThe ceiling creaks.\n";
    cout << "Smoke forms layers — breathable air near the floor..\n";
    cout << "What is your call?\n";
     setColor(7);
    cout << "1. Crawl low.\n";
    cout << "2. Run for your life.\n";
    cout << "3. Retreat.\n";
    cout << "4. Use an item from your inventory.\n";
     setColor(6);
    cout << "Enter your choice (1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(2);
        cout << "You duck down and start crawling...\n";
        cout << "You stay close to the floor where air is cleaner..\n";
        cout << "It is tiring to crawl.\n";
        Update_Stats(player, -5, -18);
        break;

    case 2:
        setColor(4);
        cout << "You attempt to run through the smoke....\n";
        cout << "You inhale thick fumes and start coughing violently.\n";
        Update_Stats(player, -30, -15);
        break;
    case 3:
        setColor(14);
        cout << "You retreat briefly, trying to find a safer route.\n";
        cout << "Smoke continues spreading.\n";
        Update_Stats(player, -10, -5);
        break;

    case 4:
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "You freeze as smoke thickens around you.\n";
        Update_Stats(player, -10, -5);
        break;
    }

    // RANDOM EVENT: FLASHOVER
    if (rand() % 100 < 25) {
         setColor(4);
        cout << "\n[CRITICAL EVENT] Flashover occurs!\n";
        cout << "Extreme heat fills the corridor suddenly.\n";
        Update_Stats(player, -20, -10);
    }

    // Question #4
    delay(2);
    setColor(1);
    cout << "\nPanicked residents rush past you.\n";
    cout << "Someone screams that the staircase is blocked!\n";
    cout << "What do you do?\n";
     setColor(7);
    cout << "1. Follow the crowd.\n";
    cout << "2. Follow emergency exit signs.\n";
    cout << "3. Stop and reassess the situation.\n";
    cout << "4. Use an item from inventory.\n";
     setColor(6);
    cout << "Enter your choice (1-4)\n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(14);
        cout << "You follow the panicked crowd.\n";
        Update_Stats(player, -5, -15);
        break;

    case 2:
        setColor(2);
        cout << "You follow the emergency signs calmly.\n";
        Update_Stats(player, 0, -10);
        break;

    case 3:
        setColor(14);
        cout << "You pause to observe smoke movement and noise.\n";
        Update_Stats(player, -5, -5);
        break;

    case 4:
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "You lose time in confusion.\n";
        Update_Stats(player, -8, -8);
        break;
    }

    // RANDOM EVENT: STAIRCASE COLLAPSE
    if (rand() % 100 < 30) {
         setColor(4);
        cout << "\n[EVENT] A section of the staircase collapses!\n";
        cout << "You are forced to take a longer route.\n";
        Update_Stats(player, -10, -15);
    }

    // Question #5
    delay(2);
    setColor(1);
    cout << "\nSirens are audible outside.\n";
    cout << "Smoke is thick but an exit is visible.\n";
    cout << "Final decision:\n";
     setColor(7);
    cout << "1. Push through the smoke.\n";
    cout << "2. Wait for rescue.\n";
    cout << "3. Use an alternate emergency exit.\n";
    cout << "4. Use an item from inventory.\n";
     setColor(6);
    cout << "Enter your choice(1-4);\n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(14);
        cout << "You push through the smoke, coughing violently.\n";
        Update_Stats(player, -25, -15);
        break;

    case 2:
        setColor(4);
        cout << "You wait, hoping firefighters arrive in time.\n";
        Update_Stats(player, -30, -10);
        break;

    case 3:
        setColor(2);
        cout << "You find a secondary exit and escape outside.\n";
        Update_Stats(player, -5, -10);
        break;

    case 4:
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "You hesitate at the final moment.\n";
        Update_Stats(player, -15, -10);
        break;
    }
     setColor(6);
    return "Fire Scenario Complete";
}

string Flood(User& player) {
    setColor(5);
    cout << "\n========================================" << endl;
    cout << "        DISASTER: FLASH FLOOD            " << endl;
    cout << "========================================" << endl;

    delay(2);
    setColor(4);
    cout << "\n[ALERT]: Heavy rain for hours! Streets flooding rapidly!\n";
     setColor(4);
    cout << "Emergency sirens echo in the distance.\n";
    cout << "You have seconds to grab ONE item:\n";
     setColor(7);
    cout << "1. Life Jacket\n";
    cout << "2. Laptop\n";
    cout << "3. Hat\n";
    int choice;
    choice = GetTimedInput(15);

    if (choice == 1){
        Inventory(player, "Life Jacket");
    }else if (choice == 2){
    Inventory(player, "Rope");
    }else if (choice == 3) {
        Inventory(player, "Hat");
    }else{
         setColor(4);
        cout << "Time's Over! You didn't choose anything.\n";
    }

    // Question 1
    delay(3);
     setColor(1);
    cout << "Water reaches your ankles and is rising FAST!" << endl;
    cout << "You look outside, cars are half submerged." << endl;
    cout << "The current is getting stronger.\n" << endl;
    cout << "What do you do?\n";
     setColor(7);
    cout << "1. Stay indoors on the ground floor and wait\n";
    cout << "2. Move to the roof immediately\n";
    cout << "3. Go outside to assess how bad it is\n";
    cout << "4. Use your inventory.\n";
     setColor(6);
    cout << "Choice (1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(4);
        cout << " The water keeps rising! You're trapped!\n";
        cout << "Water is now at your knees!\n";
        Update_Stats(player, -15, -20);
        break;

    case 2:
        setColor(2);
        cout << "Smart! You gain elevation and safety!\n";
        cout << "You can see the extent of flooding from above.\n";
        Update_Stats(player, 0, -10);
        break;

    case 3:
        setColor(4);
        cout << "You step into cold, fast-moving water!\n";
        cout << "Debris slams into your legs!\n";
        Update_Stats(player, -20, -25);
        break;
    case 4:
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "You hesitate - water rises to your knees!\n";
        Update_Stats(player, -10, -15);
        break;
    }

    // RANDOM EVENT 1
    if (rand() % 100 < 35) {
         setColor(4);
        cout << "\n[SUDDEN SURGE!]\n";
        cout << "A wave of water rushes through the area!\n";
        cout << "You struggle to keep your balance!\n";
         setColor(7);
        Update_Stats(player, -10, -15);
    }

    // Question 2
    OfferPickup(player, { "Rope" },
        "On your rooftop, you find a rope tied with a wheel hanging as a decor.");
    delay(2);
    setColor(1);
    cout << "\nThe water level is now at your waist!" << endl;
    cout << "The current is strong and pulling at you." << endl;
    cout << "You can barely stand upright.\n" << endl;
    cout << "What's your next move?\n";
     setColor(7);
    cout << "1. Try to wade through the water to higher ground\n";
    cout << "2. Grab onto something stable and wait\n";
    cout << "3. Attempt to swim with the current\n";
    cout << "4. Use Inventory\n";
     setColor(6);
    cout << "Enter Your Choice(1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(14);
        cout << "You push forward against the powerful current...\n";
        cout << "Every step is exhausting!\n";
        cout << "Your muscles burn with effort.\n";
        Update_Stats(player, -15, -30);
        break;

    case 2:
        setColor(2);
        cout << "You hold onto a pole/railing!\n";
        cout << "The current flows past you safely.\n";
        cout << "Smart thinking - you conserve energy.\n";
        Update_Stats(player, 0, -10);
        break;

    case 3:
        setColor(14);
        if (HasItem(player, "Life Jacket")) {
            cout << "Your Life Jacket keeps you afloat!\n";
            cout << "You swim with the current carefully.\n";
            Update_Stats(player, -5, -15);
        }
        else {
            setColor(4);
            cout << "The current is fast and powerfull, it pulls you with it!\n";
            cout << "You swallow water and panic!\n";
            Update_Stats(player, -30, -35);
        }
        break;

    case 4:
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "Indecision causes you to slip!\n";
        Update_Stats(player, -15, -20);
        break;
    }

    // Question 3
    delay(2);
    setColor(1);
    cout << "You hear desperate cries for help nearby!" << endl;
    cout << "Someone is clinging to a car roof, an elderly person!" << endl;
    cout << "The water continues to rise steadily.\n" << endl;
    cout << "What do you do?\n";
     setColor(7);
    cout << "1. Try to reach them and help\n";
    cout << "2. Throw them something to hold onto\n";
    cout << "3. Shout that you'll get rescue and keep moving\n";
    cout << "4. Use your inventory.\n";
     setColor(6);
    cout << "Choice(1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(14);
        cout << "You fight through the current to reach them!\n";
        if (HasItem(player, "Life Jacket")) {
            cout << "Your Life Jacket helps you both!\n";
            cout << "You guide them to safety!\n";
            Update_Stats(player, -10, -25);
        }
        else {
            cout << "You help them but it drains you completely!\n";
            cout << "Both of you barely make it!\n";
            Update_Stats(player, -25, -40);
        }
        cout << "You saved someone!\n";
        // Track people helped
        break;

    case 2:
        if (HasItem(player, "Rope")) {
            setColor(2);
            cout << "You throw them your rope!\n";
            cout << "They grab it and you pull them closer!\n";
            cout << "Smart use of resources!\n";
            Update_Stats(player, -5, -15);
        }
        else {
            setColor(14);
            cout << "You have nothing to throw!\n";
            cout << "You feel guilty and helpless.\n";
            Update_Stats(player, 0, -10);
        }
        break;

    case 3:
        setColor(2);
        cout << "You prioritize your own survival...\n";
        cout << "You keep moving to higher ground.\n";
        Update_Stats(player, 0, -5);
        break;

    case 4:
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "You freeze up while they struggle!\n";
        Update_Stats(player, -5, -15);
        break;
    }

    // RANDOM EVENT 2 - Debris
    if (rand() % 100 < 40) {
         setColor(4);
        cout << "\n[FLOATING DEBRIS!]\n";
        cout << "A large object crashes toward you!\n";
        if (HasItem(player, "Waterproof Backpack")) {
            cout << "You use your backpack to shield yourself!\n";
            Update_Stats(player, -5, -5);
        }
        else {
            cout << "It strikes your side hard!\n";
            Update_Stats(player, -20, -10);
        }
         setColor(7);
    }

    // Question 4
    delay(2);
    setColor(1);
    cout << "\nYou spot a rescue boat in the distance!" << endl;
    cout << "It's about 50 meters away, moving slowly." << endl;
    cout << "The water is chest-deep now and very cold.\n" << endl;
    cout << "How do you signal them?\n";
     setColor(7);
    cout << "1. Swim toward the boat as fast as you can\n";
    cout << "2. Wave your arms and shout loudly\n";
    cout << "3. Stay put and use a bright object to signal\n";
    cout << "4. Use your inventory.\n";
    setColor(6);
    cout << "Choice(1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        if (HasItem(player, "Life Jacket")) {
            setColor(2);
            cout << "Life Jacket helps you swim efficiently!\n";
            cout << "You reach the boat safely!\n";
            Update_Stats(player, -10, -20);
        }
        else {
            setColor(2);
            cout << "Swimming against current exhausts you!\n";
            cout << "You barely reach the boat, gasping for air!\n";
            Update_Stats(player, -30, -35);
        }
        break;

    case 2:
        setColor(14);
        cout << "You wave frantically and yell!\n";
        cout << "They spot you after a tense minute!\n";
        cout << "The boat turns toward you!\n";
        Update_Stats(player, 0, -15);
        break;

    case 3:
        setColor(2);
        cout << "You use reflective material to signal!\n";
        cout << "Clear, visible signal works perfectly!\n";
        cout << "Rescue boat heads directly to you!\n";
        Update_Stats(player, 0, -10);
        break;

    case 4:
        UseInventoryItem(player);
        break;

    default:
        setColor(15);
        cout << "Confused signaling delays rescue!\n";
        Update_Stats(player, -10, -20);
        break;
    }

    // Question 5 - FINAL/ENDING QUESTION
    delay(2);
    setColor(15);
    setColor(1);
    cout << "RESCUE BOAT ARRIVES!" << endl;
    cout << "The rescue boat pulls up beside you!" << endl;
    cout << "Rescuers extend their hands to pull you aboard." << endl;
    cout << "But you notice 3 other people still in the water nearby!\n" << endl;
    cout << "The boat has limited space. What do you do?\n";
    setColor(7);
    cout << "1. Board immediately - you're exhausted and need help\n";
    cout << "2. Help pull others aboard first, then board\n";
    cout << "3. Give up your spot for someone more vulnerable\n";
    cout << "4. Use your inventory.\n";
    setColor(6);
    cout << "Choice(1-4): \n";
    choice = GetTimedInput(20);

    switch (choice) {
    case 1:
        setColor(2);
        cout << "You climb aboard the rescue boat.\n";
        cout << "The rescuers wrap you in a thermal blanket.\n";
        cout << "You made it to safety!\n";
        Update_Stats(player, 5, -10);
        break;

    case 2:
        setColor(14);
        cout << " Despite your exhaustion, you help others!\n";
        cout << "You pull 2 people aboard before climbing in.\n";
        cout << "The rescuers commend your bravery!\n";
        cout << "Everyone makes it to safety!\n";
        Update_Stats(player, -10, -25);
        break;

    case 3:
    setColor(4);
        cout << "You give your spot to an elderly person!\n";
        cout << "The boat takes them first.\n";
        cout << "You struggle to stay afloat while waiting...\n";
        cout << "It feels like forever but the boat returns!\n";
        cout << "You're pulled aboard, freezing but alive!\n";
        Update_Stats(player, -25, -35);
        break;

    case 4:
        UseInventoryItem(player);
        break;

    default:
        cout << "Hesitation causes confusion!\n";
        cout << "You eventually board but lost precious time.\n";
        Update_Stats(player, -10, -15);
        break;
    }

    // Final flood scenario text
    delay(2);
    setColor(6);
    cout << "The boat takes you to higher ground." << endl;
    cout << "Emergency services are coordinating evacuations." << endl;
    setColor(7);
    return "Flood";
}


void Scoring_System_For_Ending_Display(User&player){
     double score = (player.health * 0.4) + (player.energy * 0.3);
     setColor(4);
        cout << "Final Score: " << score << "/100" << endl;
        setColor(2);
        if (score > 80) {
            cout << "Rating: SURVIVAL EXPERT" << endl;
        }
        else if(score > 50){
             setColor(2);
             cout << "Rating: SURVIVOR" << endl;}
        else {
             setColor(4);
            cout << "Rating: NOTICE - Needs Training" << endl;}
}

//Regarding game saving i.e including the data handling
void SaveGameResult(const User& player, const string& disaster) {
    ofstream file("game_history.txt", ios::app);
    setColor(6);
    file << "====================\n";
    setColor(7);
    file << "Name: " << player.name << endl;
    file << "Disaster: " << disaster << endl;
    file << "Survived: " << (player.isAlive ? "YES" : "NO") << endl;
    file << "Final Health: " << player.health << endl;
    file << "Final Energy: " << player.energy << endl;
    setColor(6);
    file << "====================\n";
    setColor(7);
    file.close();
}



//We want to allow user to be able to play multiple times
string PlayOneRound(User& player, string& disaster) {
    // Reset player stats
    player.health = 100;
    player.energy = 100;
    player.isAlive = true;
    player.inventory = {"Empty"};

    // Pick random scenario
    srand((unsigned)time(NULL));
    int random_number = rand() % 4 + 1;

    switch (random_number) {
        case 1: disaster = "EarthQuake";     
        return EarthQuake(player);
        break;

        case 2: disaster = "Flood";           
        return Flood(player);
        break;

        case 3: disaster = "Building on Fire"; 
        return Fire(player);
        break; 

        case 4: disaster = "Power Outage";    
        return Power_Outage(player);
        break;
    }
    setColor(6);
    return "Scenario completed.";
    setColor(7);
}



int main() {
    string name,disaster, outcome;
    setColor(5);
    cout<<"____________________________________________________________________________________\n";
    cout<<"This is a Disaster Simulation meant to put you under pressure for a certain disaster.\n";
    cout<<"You have limited time and need to make life-saving decisions.\n";
    cout<<"You presence of mind and experience can save your life one day.\n";
    cout<<"Your ultimate goal is to survive while keeping the best of your health.\n";
    cout<<"_____________________________________Good Luck!______________________________________\n";
    delay(3);
    setColor(7);

    cout << "Enter your name: ";
    cin >> name;

    setColor(2);
    cout << "Welcome " << name << "!\n";
    setColor(7);

    User player;
    player.name = name;
    player.health = 100;
    player.energy = 100;
    player.isAlive = true;
    player.inventory = {"Empty"};

    bool playAgain = true;
    while (playAgain) {
    string disaster;
    PlayOneRound(player, disaster);
    
    Player_Condition(player.health, player.energy);
    Scoring_System_For_Ending_Display(player);
    SaveGameResult(player, disaster);
    
    // Show their history after game
    setColor(6);
    cout << "\n=== YOUR GAME HISTORY ===\n";
    setColor(7);
    ifstream historyFile("game_history.txt");
    string line;
    while (getline(historyFile, line)) {
        if (line.find(name) != string::npos) {
            cout << line << "\n";
            for (int i = 0; i < 5; i++) {
                getline(historyFile, line);
                cout << line << "\n";
            }
        }
    }
    historyFile.close();
    
    setColor(5);
    cout << "\nDo you want to play again? (y/n): ";
    setColor(7);
    char ans;
    cin >> ans;
    playAgain = (ans == 'y' || ans == 'Y');
    }
     
    setColor(6);
    cout << "_________Thanks for playing!_________\n";
    return 0;
}


