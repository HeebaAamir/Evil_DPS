#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
//using vector for a dynamic array to store inventory items as it is modifiable. 
//Items can be both added and removed 
#include<vector>

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

//Function for items that the user can carry
void Inventory(User& player, string item){
    if (player.inventory.size()<5){
        cout<<"Adding to inventory.....\n";
        player.inventory.push_back(item);
        cout<<item<<" was added to your inventory.\n";
    }
    else{
        cout<<"Item wasn't added. You can carry maximum 5 items at a time!\n";
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
    cout<<"Current Energy And Health Level is: \n";
    cout<<"Health xp: "<<health<<" % "<<" || "<<"Energy xp: "<<energy<<" % "<<endl;
}
//Scenario = Earthquake
string EarthQuake(User& player){
    cout << "\n========================================" << endl;
    cout << "       CIRCUMSTANCE : MAGNITUDE 7.0          " << endl;
    cout << "========================================" << endl;

    //Some context
    cout << "\n[WARNING]: Seismic activity detected nearby!" << endl;
    cout << "You have 15 seconds to grab ONE item nearby:" << endl;
    cout << "1. First Aid Kit" << endl;
    cout << "2. 5L Water Bottle" << endl;
    cout << "3. Gaming Laptop" << endl;
    cout << "What will you choose?: ";
    
    int choice;
    cin >> choice;

    if (choice == 1){
        Inventory(player, "First Aid Kit");
    } 
    else if (choice == 2) {
        Inventory(player, "Water Bottle");
    }
    else if (choice == 3) {
        Inventory(player, "Gaming Laptop");
    }

    //Question #1 
    cout << "You are on the 4th floor. The room starts shaking violently." << endl;
    cout << "Bookshelf crashes down." << endl;
    cout << "You can hear the walls cracking and the ceiling fan creaking.\n"<<endl;
    cout << "What is your immediate action?" << endl;
    cout << "1. Run for the elevator." << endl;
    cout << "2. Drop, Cover, and Hold on to wait under a table till the shocks settle down." << endl;
    cout << "3. Run to the balcony." << endl;

    cout<<"Enter your choice (1-3): \n";
    cin>>choice;
    
    switch(choice){
        case 1:
        cout<<"You run to the elevator.....";
        cout<<"ERROR:: Power failed. You are stuck inside\n";
        cout<<"It's extremely dark in the elevator\n";
        //He is stuck so he suffers a major energy loss
        Update_Stats(player, -5, -40 );
        break;

        case 2:
        cout<<"You scan the room to find a place you can take shelter in....\n";
        cout<<"You spot a wooden table nearby.\n";
        cout<<"You run towards it.\n";
        cout<<"You take cover and wait for the shocks to settle.\n";
        //Only energy less for finding and running, no effect on health
        Update_Stats(player, 0, -20);
        break;

        case 3:
        cout<<"You run towards the balcony.....\n";
        cout<<"You are unable to walk properly to the spot.\n";
        cout<<"The shaking ground and building overpowers you!\n";
        cout<<"You fall to the ground!!\n";
        cout<<"Your head hurts and your vision getes blurred.\n";
        //Major injury and loss to health
        Update_Stats(player, -70, -65);
        break;

        default:
        cout << "You hesitate and do nothing...\n";
        //Panic affects both physical and mental health and drains energy while wasting time
        Update_Stats(player, -5, -5); 
        break;
    }

    //Question #2 
    cout << "\nThe shaking slows down... but the building isn't still yet.\n";
    cout << "Dust is in the air and small pieces of plaster are falling.\n";
    cout << "You fear an aftershock might hit at any moment.\n\n";
    cout << "What do you do now?\n";
    cout << "1. Stay under the table and wait for 1 more minute.\n";
    cout << "2. Move towards the emergency stairs carefully.\n";
    cout << "3. Call out for help to see if anyone else is nearby.\n";

    cout << "Enter your choice (1-3): \n";
    cin >> choice;

    switch (choice) {
    case 1:
        cout << "You stay under the cover protecting your head from any injury or bruise...\n";
        cout << "You remain safe as debris falls around you.\n";
        cout<<"You made a safe and good choice.\n";
        //Safe healthwise but can tire up a little
        Update_Stats(player, 0, -10); 
        break;

    case 2:
        cout << "You plan to reach for the emergency exit....\n";
        cout<<"You are moving very carefully; holding the railing and walls for support.\n";
        cout << "You stumble a bit due to some debris lying around, but you keep moving.\n";
        cout<<"You hurt your knee a little but the injury isn't worth worrying about.\n";
        //energy used
        Update_Stats(player, -5, -20); 
        break;

    case 3:
        cout << "You shout for help....\n";
        cout<<"You can hear a faint noise coming from the corridor./n";
        cout<<"You try to locate were they are.\n";
        Update_Stats(player, 0, -5); 
        break;

    default:
        cout << "You hesitate and do nothing...\n";
        Update_Stats(player, -5, -5); 
        break;
    }

    //Question 3
    cout << "\nYou finally reach the staircase. The main path is blocked by fallen debris.\n";
    cout << "There is a narrow gap you might squeeze through, or you could try finding another route.\n";
    cout << "What is your call?\n";
    cout << "1. Try to clear some debris to make space to move.\n";
    cout << "2. Look for an alternate staircase or fire escape.\n";
    cout << "3. Crawl through the narrow gap carefully.\n";

    cout << "Enter your choice (1-3): \n";
    cin >> choice;

switch (choice) {
    case 1:
        cout << "You push through the piles of concrete and wood....\n";
        cout << "Your hands get scraped and your breathing gets heavy.\n";
        cout<<"It is tiring.\n";
        Update_Stats(player, -10, -27); 
        break;

    case 2:
        cout << "You scan the corrdior for another escape....\n";
        cout << "You find a marked fire door slightly to your left.\n";
        cout <<"You are hopeful.\n";
        Update_Stats(player, -3, -15); 
        break;

    case 3:
        cout << "You flatten yourself and try to slide through the gap.\n";
        cout << "A jagged edge cuts your arm as you squeeze past!\n";
        Update_Stats(player, -15, -10); // more health loss, less energy
        break;

    default:
        cout<<"YOu do nothing.\n";
        cout << "You stand still overwhelmed by the situation...\n";
        Update_Stats(player, -5, -5);
        break;
    }
    
// Question #4 
    cout << "\nYou smell smoke. A thin haze fills the corridor near the ceiling....\n";
    cout << "You hear cracking sounds from a room down the hall.\n";
    cout << "How do you proceed?\n";
    cout << "1. Cover your mouth with cloth and crawl low to the exit.\n";
    cout << "2. Use nearby water and try setting off small flames near the door (if any).\n";
    cout << "3. Run past the smoke quickly to save time.\n";

    cout << "Enter your choice (1-3): \n";
    cin >> choice;

    switch (choice) {
    case 1:
        cout << "You get down on your hands and knees...\n";
        cout <<"You are trying to keep yourself below the rising smoke.\n";
        cout << "Your eyes sting, but you keep moving to reach the exit.\n";
        Update_Stats(player, -8, -15); 
        break;

    case 2:
        cout << "You see some fire flames rising from the bottom of a door in the corridor.\n";
         cout<<"Your eyes hurt due to rising smoke.\n";
        cout <<"You find an emergency water hydrant at the edge of the corridor.\n";
        cout << "The flames subside, making the corridor safer to pass.\n";
        Update_Stats(player, -5, -15); 
        break;

    case 3:
        cout << "You sprint through the smoke!\n";
        cout << "You cough violently and feel dizzy.\n";
        //risky choice
        Update_Stats(player, -25, -15); 
        break;

       default:
        cout << "You hesitate, breathing more smoke as time passes.\n";
        Update_Stats(player, -15, -5);
        break;

    }
    //Question #5
    cout << "\nYou spot someone on the floor holding their leg, calling for help.\n";
    cout << "They are conscious but in pain.\n\n";
    cout << "What do you do?\n";
    cout << "1. Help them stand and guide them to a safer area.\n";
    cout << "2. Provide quick first aid and move on.\n";
    cout << "3. Promise to call rescuers and continue towards the exit.\n";

    cout << "Enter your choice (1-3): \n";
    cin >> choice;

    switch (choice) {
    case 1:
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
        cout << "You freeze; unable to decide what to choose and lose precious seconds.\n";
        Update_Stats(player, -5, -5);
        break;
    }
    return "EarthQuake Simulation Ccmplete!\n";
}



//Scenario: Power Outage
string Power_Outage(User& player){
        cout << "\n========================================" << endl;
        cout << "         CIRCUMSTANCE : CITY-WIDE BLACKOUT         " << endl;
        cout << "========================================" << endl;

    // Some context
    cout << "\n[ALERT]: The power has gone out across the city!" << endl;
    cout << "It's night, and the building emergency lights are flickering." << endl;
    cout << "You have 15 seconds to grab ONE item nearby:" << endl;
    cout << "1. Flashlight" << endl;
    cout << "2. Battery-Powered Radio" << endl;
    cout << "3. Power Bank (Phone at 20%)" << endl;
    cout << "What will you choose?: ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        Inventory(player, "Flashlight");
       
    }  else if (choice == 2) {
        Inventory(player, "Battery-Powered Radio");
        
    } else if (choice == 3) {
        Inventory(player, "Power Bank");
    }

    cout << "\nYou are on the 4th floor. The emergency lights flicker and then turn off" << endl;
    cout << "The hallway is pitch dark and you hear distant sirens outside." << endl;
    cout << "What is your immediate action?" << endl;
    cout << "1. Head for the elevator to reach the lobby." << endl;
    cout << "2. Stay put, use your light, and secure hazards (You unplug all electronics)." << endl;
    cout << "3. Step onto the balcony to assess what's happening outside." << endl;

    cout << "Enter your choice (1-3): \n";
    cin >> choice;

    switch (choice) {

        case 1:
            cout << "You walk toward the elevator....\n";
            cout << "ERROR: No power. The elevator is dead.\n";
            cout << "You fumble in the dark and feel anxious.\n";
            Update_Stats(player, -3, -25);
            break;

        case 2: 
            cout << "You turn on a  small light source and do a quick safety sweep...\n";
            cout << "You unplug sensitive devices and clear trip hazards.\n";
            cout << "Good call! You reduce risk while conserving energy.\n";
           // best-practice behavior
            Update_Stats(player, 0, -10);
            break;
        
        case 3: {
            cout << "You step onto the balcony to look around...\n";
            cout << "The wind picks up; it’s cold and visibility is poor.\n";
            cout << "You get a mild chill but spot that the whole block is dark.\n";
            Update_Stats(player, -5, -12);
            break;
        }
        default:
            cout << "You hesitate and do nothing...\n";
            Update_Stats(player, -5, -5);
            break;
    }

    //Question #2
    cout << "\nYou hear a faint buzzing from your apartment's breaker panel.\n";
    cout << "A neighbor shouts that the outage might be on the grid.\n";
    cout << "What do you do now?\n";
    cout << "1. Inspect your unit’s breaker panel carefully with a flashlight.\n";
    cout << "2. Head to the basement to check a building generator (if any).\n";
    cout << "3. Use your radio/phone to get utility updates and stay put.\n";

    cout << "Enter your choice (1-3): \n";
    cin >> choice;

    switch(choice){
    case 1:
        cout << "You open the panel gently and check for tripped breakers...\n";
        cout << "No burning smell, no visible damage. It looks normal.\n";
        cout << "You close it and decide to wait for updates.\n";
        Update_Stats(player,0, -15);
        break;

        case 2:
        cout << "You make your way to the basement...\n";
        cout << "It’s dark, stuffy, and a bit disorienting.\n";
        cout << "You don’t find a safe way to start anything; better to avoid risks.\n";
        Update_Stats(player,-5 ,-10);
        break;

        case 3:
        cout << "You tune into emergency updates...\n";
        cout << "Authorities confirm a grid failure and advise staying indoors.\n";
        cout << "You conserve energy and plan for several hours without power.\n";
        cout<<"You made a good choice!\n";
        Update_Stats(player, 0, -5);
        break;

        default:
        cout << "You do nothing and hope power returns...\n";
        Update_Stats(player, -5, -5);
    }  

    //Question #3
    cout << "\nYour refrigerator and freezer are now off.\n";
    cout << "Perishables could spoil if the outage lasts long.\n";
    cout << "How do you manage food safety?\n";
    cout << "1. Keep doors closed; move essentials to a cooler with ice if available.\n";
    cout << "2. Open the fridge repeatedly to check and snack now.\n";
    cout << "3. Cook perishables immediately on a gas stove for light and warmth.\n";

    cout << "Enter your choice (1-3): \n";
    cin >> choice;

    switch(choice){
        case 1:
        cout << "You limit opening and use a cooler efficiently...\n";
        cout << "You remember that you have stored perishable items in your refrigrator which you can use in an emergency.\n";
        cout << "You find an ice cooler to store milk and leftover food from tomorrow.\n";
        cout << "You extend the safe time for your food.\n";
        Update_Stats(player,0,-10);
        break;

        case 2:
        cout << "You open the fridge often and grab snacks...\n";
        cout << "Cold escapes quickly; food warms faster.\n";
        cout<<"You realize that your choice wasn't that well-thought upon.\n";
        Update_Stats(player,-17,-5);
        break;

        case 3: 
        cout << "You cook some perishables using the gas stove...\n";
        cout << "You ventilate, but there’s minor exposure to fumes.\n";
        cout << "You secure some meals but feel a bit lightheaded.\n";
        cout <<"You won some, lost some. Overall not that bad of a choice.\n";
        Update_Stats(player, -9,-16);
        break;

        default:
        cout << "You do nothing and hope power returns...\n";
        Update_Stats(player, -5, -5);
    }

    //Question 4

    cout << "\nNight deepens. You need light and a way to charge your phone.\n";
    cout << "What is your approach?\n";
    cout << "1. Use battery lanterns/flashlight and ration batteries.\n";
    cout << "2. Light multiple candles around the apartment for brightness.\n";
    cout << "3. Sit in your idling car inside the garage to charge devices.\n";

    cout << "Enter your choice (1-3): \n";
    cin >> choice;

    switch (choice) {
        case 1: 
            cout << "You rely on safe battery lights and plan usage...\n";
            cout << "Visibility is decent and fire risk is minimal.\n";
            Update_Stats(player, 0, -10);
            break;
        
        case 2: 
            cout << "You light several candles for illumination...\n";
            cout << "A small flame tips near papers—close call!\n";
            cout << "You quickly clear the area but remain wary of fire hazards.\n";
            Update_Stats(player, -8, -12);
            break;
        
        case 3: 
            cout << "You sit in your idling car in the closed garage to charge...\n";
            cout << "Bad idea—carbon monoxide risk rises fast!\n";
            cout << "You feel dizzy and rush back out; that was dangerous.\n";
            Update_Stats(player, -25, -15);
            break;
        
        default:
            cout << "You delay, straining your eyes in the dark and wasting time.\n";
            Update_Stats(player, -5, -5);
            break;
    }

    //Question 5

    cout << "\nYou encounter a neighbor whose medical device (CPAP/oxygen concentrator) needs power.\n";
    cout << "They are conscious but anxious and mildly short of breath.\n";
    cout << "What do you do?\n";
    cout << "1. Help them relocate to a nearby clinic/shelter with backup power.\n";
    cout << "2. Offer your power bank for phone calls and stay with them briefly.\n";
    cout << "3. Promise to inform responders and continue managing your own situation.\n";

    cout << "Enter your choice (1-3): \n";
    cin >> choice;

    switch (choice) {
        case 1: {
            cout << "You support the neighbor and move carefully to a safer location...\n";
            cout << "It takes effort, but their condition stabilizes.\n";
            cout<<"You lost energy and feel drained.\n";
            Update_Stats(player, -5, -22);
            break;
        }
        case 2: {
            cout << "You share your power bank and help them make necessary calls...\n";
            cout << "You reassure them and note the nearest facility.\n";
            cout<<"You power bank is a bit drained now but you feel happy to help.\n";
            Update_Stats(player, -2, -12);
            break;
        }
        case 3: {
            cout << "You promise to notify responders and proceed to secure your area...\n";
            cout << "You conserve energy but feel uneasy about leaving.\n";
            cout<<"You know that you can be of help to someone else only if you stay alive and safe yourself.\n";
            Update_Stats(player, 0, -6);
            break;
        }
        default:
            cout << "You freeze, losing precious minutes...\n";
            Update_Stats(player, -5, -5);
            break;
    }

    return "PowerOutage Completed";

}


void Scoring_System_For_Ending_Display(User&player){
     double score = (player.health * 0.4) + (player.energy * 0.3);
        cout << "Final Score: " << score << "/100" << endl;

        if (score > 80) {
            cout << "Rating: SURVIVAL EXPERT" << endl;
        }
        else if(score > 50){
             cout << "Rating: SURVIVOR" << endl;}
        else {
            cout << "Rating: NOVICE - Needs Training" << endl;}
}

int main(){

//This code block randomly decides the disaster to be simulated
srand(time(NULL));
string disaster;

int random_number;
random_number=rand()%4+1;
if( random_number == 1){
    disaster = "EarthQuake";
}

 //Initializing stats for the player using the struct
    User player;
    cout<<"Enter your name to proceed: ";
    getline(cin, player.name);
    player.health=100;
    player.energy=100;
    player.isAlive=true;
    player.inventory={"Empty"};
    
    //I need to display smth that gives an end and closure to the game
    string outcome;
    //if user is alive then we can print smth like complete and stuff and see his score at that point and give a certification
    //for that we'll need to hardcode it for all cases ig


switch(random_number){
    case 1:
    disaster = "EarthQuake";
    outcome = EarthQuake(player);
    break;
    case 2:
    disaster = "Flood";
    break;
    case 3:
    disaster = "Building on Fire";
    break;
    case 4:
    disaster = "Power Outage";
    outcome = Power_Outage(player);
    break;
    }

    //This is smth we get out of each function, we will just display it at the ending of a function call
    cout<<outcome<<endl;

    //if the user is alive:
    if(player.isAlive==true){
       
        cout<<"Congragulations!! You survived !!\n";
        Scoring_System_For_Ending_Display(player);

    }

    if(player.isAlive==false) {
        cout << "\n--- GAME OVER ---" << endl;
        cout << "You failed to survive the disaster." << endl;
        Scoring_System_For_Ending_Display(player);
    }
    return 0;

}

