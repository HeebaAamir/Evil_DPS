#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
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

int main(){
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
}


