#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<algorithm>
//for a dynamic array to store inventory items as it is modifiable. 
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

                                //Function Definitions
//Fucntion to update stats
void Update_Stats(User&player, int health_change, int energy_change){
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
    //Once the player loses all energy, his health will 
    //For any neg health just consider it to be 0 to simplify things
    //At 0 health, game ends cause the player dies.
    if(player.health <0){
        player.health=0;
        player.isAlive = false;
    }

    

}
//Scenario = Earthquake
string EarthQuake(User&player){
    cout << "\n========================================" << endl;
    cout << "       CIRCUMSTANCE : MAGNITUDE 7.0          " << endl;
    cout << "========================================" << endl;
    cout << "You are on the 4th floor. The room starts shaking violently." << endl;
    cout << "Bookshelf crashes down." << endl;
    cout<<"You can hear the walls cracking and the ceiling fan creaking.\n"<<endl;
    cout << "What is your immediate action?" << endl;
    cout << "1. Run for the elevator." << endl;
    cout << "2. Drop, Cover, and Hold on to wait under a table till the shocks settle down." << endl;
    cout << "3. Run to the balcony." << endl;

    int choice;
    cin>>choice;

    if(choice ==1){
        
    }
    
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
switch(random_number){
    case 1:
    disaster = "EarthQuake";
    break;
    case 2:
    disaster = "Flood";
    break;
    case 3:
    disaster = "Building on Fire";
    break;
    case 4:
    disaster = "Power Outage";
    break;

    }
    
    //Initializing stats for the player using tthe struct
    User player;
    cout<<"Enter your name to proceed: ";
    getline(cin, player.name);
    player.health=100;
    player.energy=100;
    player.isAlive=true;
    player.inventory={"Empty"};


}


