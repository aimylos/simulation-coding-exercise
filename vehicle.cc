#include <iostream>
using std::cout;
using std::endl;
#include <list>
using std::list;
#include <algorithm>
using std::replace;
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;


#include "map.h"
#include "vehicle.h"

//simulation limits
#define MAX_SPEED 5
#define MIN_SPEED 1
#define MAX_DAMAGED_ROUNDS 20


//initialize static variables
int vehicle::num_vehicles = 0;
int vehicle::num_alive_vehicles = 0;
int vehicle::num_damaged_vehicles = 0;
int vehicle::num_scouts = 0;
int vehicle::num_miners = 0;
int vehicle::num_rescuers = 0;
int vehicle::num_alive_scouts = 0;
int vehicle::num_alive_miners = 0;
int vehicle::num_alive_rescuers = 0;
list<vehicle*> vehicle::vehicle_address_list;

//static members
vehicle* vehicle::get_vehicle_address(int v_id){
         if(v_id<=0 || v_id>num_vehicles) return (vehicle*)0;

         list<vehicle*>::iterator itr = vehicle_address_list.begin();
         for(int i=1; i<v_id && itr!=vehicle_address_list.end(); i++, itr++);
         return *itr;
}

bool vehicle::remove_vehicle_address(int v_id){
         if(v_id<=0 || v_id>num_vehicles) return false;
         
         replace (vehicle_address_list.begin(), vehicle_address_list.end(), get_vehicle_address(v_id), (vehicle*)0);
         return true;
}

void vehicle::reset_vehicle_class(){
     num_vehicles = 0;
     num_alive_vehicles = 0;
     num_damaged_vehicles = 0;
     num_scouts = 0;
     num_miners = 0;
     num_rescuers = 0;
     num_alive_scouts = 0;
     num_alive_miners = 0;
     num_alive_rescuers = 0;
     vehicle_address_list.clear();
}

vehicle::vehicle(){
           //if(num_vehicles == 0) srand(time(NULL)); //stop seeding after the first vehicle has been created
           
           //set status alive
           status = true;
           //not in map yet
           in_map = false;
           //increase num vehicle and set id
           num_vehicles++;
           num_alive_vehicles++;
           id = num_vehicles;
           //add vehicle address to address list
           vehicle_address_list.push_back(this);
           //set no damage
           damaged = false;
           //type 0 for now
           type = 0;
           //set time damaged to 0
           times_damaged = 0;
           //set moved positions to 0
           moved_pos = 0;
           //set map null
           sim_map = NULL;
           //set current round NULL
           current_round = NULL;
           //set previous position to (0,0) as guard value
           prev_pos_x = 0;
           prev_pos_y = 0;
           //initialize positon
           pos_x = 0;
           pos_y = 0;
           
}


//general members
int vehicle::get_id(){
    return id;
}
int vehicle::get_type(){
    return type;
}

//statics
int* vehicle::get_round(){
    return current_round;
}
int vehicle::get_num_vehicles(){
    return num_vehicles;
}
int vehicle::get_num_alive_vehicles(){
    return num_alive_vehicles;
}
int vehicle::get_num_damaged_vehicles(){
    return num_damaged_vehicles;
}
int vehicle::get_num_alive_scouts(){
    return num_alive_scouts;
}
int vehicle::get_num_alive_miners(){
    return num_alive_miners;
}
int vehicle::get_num_alive_rescuers(){
    return num_alive_rescuers;
}
int vehicle::get_num_scouts(){
    return num_scouts;
}
int vehicle::get_num_miners(){
    return num_miners;
}
int vehicle::get_num_rescuers(){
    return num_rescuers;
}
//end of general members

//status
bool vehicle::set_status(bool x){
     //if death remove from alive vehicles
     if(status && !x){
               num_alive_vehicles--; 
               if(type==1) num_alive_scouts--;
               else if(type==2) num_alive_miners--;
               else if(type==3) num_alive_rescuers--;
     }
     //if revive
     if(!status && x) {
               num_alive_vehicles++;
               if(type==1) num_alive_scouts++;
               else if(type==2) num_alive_miners++;
               else if(type==3) num_alive_rescuers++;
               if(!in_map && sim_map!=NULL && current_round!=NULL){
                          status = true;
                          enter_map(current_round,-1,-1);
               }
     }
     status = x;
     if(!status){
               if(in_map){
                        sim_map->remove_vehicle(pos_x, pos_y); //remove vehicle from map if it's dead
                        in_map = false;
               }
     }                 
     return status;
}

bool vehicle::get_status() {
     if(damaged == true && (*current_round - damage_round) >= MAX_DAMAGED_ROUNDS) {
        set_status(false);
     }
     
     return status;
}
//end of status

//damage
bool vehicle::set_damaged(bool x){
     if(!damaged && x) {
                 times_damaged++; 
                 num_damaged_vehicles++;//increase times damaged and damaged vehicles
                 damage_round = *current_round; //doesn't matter if damaged true or false
                 }
     if(damaged && !x) num_damaged_vehicles--;//decrease damaged vehicles
     damaged = x;
     return damaged;
}
bool vehicle::get_damaged(){
     get_status(); //for rounds, because get_status might not be called
     return damaged;
}
//end of damagef

//access
float vehicle::set_access(float x){
      if((x > 1.0) || (x < 0.1 && x > -1.1) || x < -3.1){
           access = (double)rand() / RAND_MAX;
           if (access < 0.1) access = 0.1;   // 0.1 - 1
           }
      else if(x < 0 && x > -2.1){
           access = (double)rand() / RAND_MAX;
           if(access > 0.6) access -= 0.5;  // 0.1  - 0.6
           if(access < 0.1) access = 0.1;
           }
      else if(x < 0 && x > -3.1){
           access = (double)rand() / RAND_MAX;
           if(access <=0.6) access += 0.6;  // 0.6 - 1
           if(access > 1) access = 0.95;
           }
      else
          access =x;
      return access;
}
float vehicle::get_access(){
      return access;
}
//end of access

//speed
int vehicle::set_speed(int x){
    if(x == -1){
         speed = ( rand() % (MAX_SPEED - MIN_SPEED +1) ) + MIN_SPEED; //MIN - MAX
         }
    else if(x == -2) {
         speed = ( rand() % ((MAX_SPEED + MIN_SPEED)/2) ) + MIN_SPEED; //MIN - (MIN+MAX)/2
         }
    else if(x == -3) {
         speed = ( rand() % (MAX_SPEED - ((MAX_SPEED + MIN_SPEED)/2) +1) ) + (MAX_SPEED + MIN_SPEED)/2; //(MIN+MAX)/2 - MAX
         }     
    else if(x > MAX_SPEED || x < MIN_SPEED){
         speed = ( rand() % (MAX_SPEED - MIN_SPEED) ) + MIN_SPEED; //MIN - MAX
         }
    else {
         speed = x;
         }
    return speed;
}
int vehicle::get_speed(){
    return speed;
}
//end of speed

//map
bool vehicle::is_in_map(){
     return in_map;
}
void vehicle::set_map(map *m){
     sim_map = m;
}

bool vehicle::enter_map(int *round, int x, int y) {
     if(!get_status() || sim_map == NULL) return false;
     if(x==-1 && y==-1){
              int size=sim_map->get_size();
              x=0; y=0;
              while(y<size){//try all positions
                      if(sim_map->insert_vehicle(x, y, id, type)) {
                                current_round = round;
                                pos_x = x;
                                pos_y = y;
                                in_map = true;
                                return true;
                      }
                      else (x<size)?x++:(x=0, y++);
              }
              return false;
     }
     else if(sim_map->insert_vehicle(x, y, id, type)) {
           current_round = round;
           pos_x = x;
           pos_y = y;
           in_map = true;
           return true;
     }
     else {
           return false;
     }
}
//end of map

//position
bool vehicle::move_to_pos(int x, int y){
     if(!get_status() || get_damaged() || !in_map) return false; //vehicle can be moved on the map even if it's damaged but cannot succesfully do move_to_pos()
     if(sim_map->move_vehicle(x, y, pos_x, pos_y, id, type)) { //check if move succeeded
             prev_pos_x = pos_x;  //update previous vehicle position
             prev_pos_y = pos_y;
             pos_x = x; //update vehicle position
             pos_y = y;
             moved_pos++; //incease moved positions
             //damage check
             if(sim_map->get_block_type(pos_x, pos_y) == 2){
                     float damage_propability = sim_map->get_danger(x,y) * (1.0 - get_access()); 
                     float damage_occurred = (double)rand() / RAND_MAX; //the higher the safest
                     if(damage_occurred < damage_propability) set_damaged(true);
             }
             return true;
             }
     else
             return false;
}

int vehicle::get_pos_x(){
    return pos_x;
}

int vehicle::get_pos_y(){
    return pos_y;
}
//end of position



bool vehicle::move(){
     if(!in_map || !get_status() || get_damaged()) return false;
     else return true;
}



int vehicle::get_times_damaged(){
    return times_damaged;
}

int vehicle::get_moved_positions(){
    return moved_pos;
}
