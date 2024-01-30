#include <iostream>
using std::cout;
using std::endl;
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include "rescuer.h"
#include "map.h"
#include "vehicle.h"



//initialize static variables
int rescuer::all_num_rescued = 0;
//initialize static methods
int rescuer::get_all_num_rescued(){
    return all_num_rescued;
}
void rescuer::reset_rescuer_class(){
     all_num_rescued = 0;
}
//define static constants
const int rescuer::dir_x[8]={1, 1, 0, -1, -1, -1,  0,  1};
const int rescuer::dir_y[8]={0, 1, 1,  1,  0, -1, -1, -1};


rescuer::rescuer(void) : vehicle(){
                       
                       //set vehicle type rescuer:3
                       vehicle::type = 3; 
                       
                       //increase rescuers
                       vehicle::num_alive_rescuers++;
                       vehicle::num_rescuers++;
                    
                       //set number of rescued vehicles 0
                       num_rescued = 0;
                    
                       //set access coefficient
                       vehicle::set_access(-1);
                    
                       //set scout speed
                       vehicle::set_speed(-3);
                       
                       //initialize rescue positions to NULL
                       rescue_pos_x = -1;
                       rescue_pos_y = -1;
                       
}





//action
bool rescuer::action(){
     //check if vehicle is ok
     if(vehicle::get_damaged() || !vehicle::get_status()) return false;
     
     //check if vehicle is on position and get id
     int v_id = sim_map->get_id(rescue_pos_x, rescue_pos_y);
     if(v_id == 0) return false;
     
     //check if vehicle is damaged and alive
     if( !( (vehicle::get_vehicle_address(v_id))->get_damaged() && (vehicle::get_vehicle_address(v_id))->get_status() ) ) return false;
                                       
     //check if rescuer is next to damaged vehicle
     if( !surround_check(rescue_pos_x, rescue_pos_y) ) return false;
     
     //rescue vehicle
     if( !(vehicle::get_vehicle_address(v_id))->set_damaged(false) ){
         num_rescued++;
         all_num_rescued++;
         rescue_pos_x = -1;
         rescue_pos_y = -1;
         return true;
         }
     else
         return false;
}







//move
bool rescuer::move(){
     if(!vehicle::in_map) return false; //check if in map
     get_status(); //check for rounds damaged
     //check if rescue position isn't set
     if(rescue_pos_x==-1 && rescue_pos_y==-1){
                         
                         int moved = 0;
                         bool vehicle_check = false; //boolean True:damaged vehicle found
                         bool road_check = false; //boolean True:last move was on road
                         //will stop if it finds a damage but alive vehicle and repair it
                         //there's a possibility the vehicle gets surrounded

                         //movement,  check if vehivle is damaged or/and alive, repeat for vehicle_speed times ecxept if vehicle was found first
                         while(!vehicle::get_damaged() && vehicle::get_status() && moved<vehicle::get_speed()  && (!vehicle_check)){

                         //get vehicle position
                         int x = vehicle::pos_x; 
                         int y = vehicle::pos_y;
                         
               
               // ******** check for vehicles  ********
                         int i = 0;
                         //check 8 possible positions while no vehicle is found
                         while(i < 8 && !vehicle_check){
                                 int temp_x = x + dir_x[i];
                                 int temp_y = y + dir_y[i];
                                 
                                 //check for vehicle
                                 int v_id = sim_map->get_id(temp_x, temp_y);
                                 if(v_id!=0){
                                      //check for damaged but alive vehicle
                                      vehicle* v_address = vehicle::get_vehicle_address(v_id);
                                      if( v_address->get_damaged() && v_address->get_status() ){
                                          //set vehicle_check true
                                          vehicle_check = true;
                                          //set rescue position
                                          rescue_pos_x = temp_x;
                                          rescue_pos_y = temp_y;
                                      }
                                 }
                                 
                                 //else try another surrounding block
                                 i++;
                         }
               // ******** end of check for mines ********



               // ******** check for road (mine_check==false) ********
                         int guard_value = 0; //max 8 loops
                         road_check = false; //reinitialize road_check
                         i = rand() % 8;
                         //check 8 possible positions while no vehicle or road is found
                         while( !road_check && guard_value < 8 && !vehicle_check ){
                      
                                int temp_x = x + dir_x[i];
                                int temp_y = y + dir_y[i];         
                       
                                //check for road and empty position
                                if(sim_map->get_block_type(temp_x,temp_y) == 3  &&  sim_map->get_id(temp_x,temp_y) == 0)
                                      //try to move to the suitable position
                                      {if(vehicle::move_to_pos(temp_x,temp_y)) {road_check = true;} }
                        
                                //cycle i
                                i = ++i % 8; 
                       
                                //increase guard value
                                guard_value++;
                         }
               // ******** end of check for road ********
               
                         //icrease positions moved          
                         moved++;
                         }
                         //end of movement
                         
                         //rescue if needed
                         if(vehicle_check) action();
                         
                         if(vehicle_check || road_check) return true;
                         else return false; 
     }

     return false;
} 

//surround check
 bool rescuer::surround_check(int x, int y){
      if(!sim_map->check_coord(x,y)) return false;
      int v_x = vehicle::get_pos_x();
      int v_y = vehicle::get_pos_y();
      
      for(int i=0; i<8; i++)
              if( v_x+dir_x[i] == x  &&  v_y+dir_y[i] == y ) return true;
      
      return false;
}

//set rescue position
bool rescuer::set_rescue_pos(int x, int y){
     if(rescue_pos_x!=-1 && rescue_pos_y!=-1) return false;
     else if(!sim_map->check_coord(x,y)) return false;
     else {
          rescue_pos_x = x;
          rescue_pos_y = y;
          return true;
          }
}
bool rescuer::force_set_rescue_pos(int x, int y){
     if(!sim_map->check_coord(x,y)) return false;
     else {
          rescue_pos_x = x;
          rescue_pos_y = y;
          return true;
          }
}


//get members
int rescuer::get_num_rescued(){
    return num_rescued;
}
