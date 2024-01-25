#include <iostream>
using std::cout;
using std::endl;
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include "scout.h"
#include "map.h"
#include "vehicle.h"


#define DANGEROUS_POSITION 0.6


//initialize static variables
int scout::all_num_flags = 0;
//static methods
int scout::get_all_num_flags(){
       return all_num_flags;
}
void scout::reset_scout_class(){
     all_num_flags = 0;
}
//define static constants
const int scout::dir_x[8]={1, 1, 0, -1, -1, -1,  0,  1};
const int scout::dir_y[8]={0, 1, 1,  1,  0, -1, -1, -1};


//constructor
scout::scout(void)
      : vehicle()
      {
                    //set vehicle type scout:1
                    vehicle::type = 1; 
                    
                    //increase scouts
                    vehicle::num_alive_scouts++;
                    vehicle::num_scouts++;
                    
                    //set number of flags planted 0
                    num_flags = 0;
                    
                    //set access coefficient
                    vehicle::set_access(-3);
                    
                    //set scout speed
                    vehicle::set_speed(-1);
                  
}



//action
bool scout::action(){
     if(!vehicle::get_status() || vehicle::get_damaged()) return false; //vehicle is dead
     
     int block_type = sim_map->get_block_type(vehicle::pos_x, vehicle::pos_y);
     bool block_flag = sim_map->has_flag(vehicle::pos_x, vehicle::pos_y);
     float block_danger = sim_map->get_danger(vehicle::pos_x, vehicle::pos_y);
     
     if(block_type == 2 && !block_flag && block_danger >= DANGEROUS_POSITION){
                   if(sim_map->set_flag(vehicle::pos_x, vehicle::pos_y)){
                           num_flags++;
                           all_num_flags++;
                           return true;
                           }
                   else 
                        return false;
                   }
     else {
                   return false;
                   }
}






//move
bool scout::move(){
     if(!vehicle::in_map) return false; //check if in map
     get_status(); //check for rounds damaged
     int moved = 0;
     bool mine_check = false; //boolean True:found mine
     bool road_check = false; //boolean True:last move was on road
     //will stop if it finds an unflaged mine to examine it
     //there's a possibility the vehicle gets surrounded

     //movement,  check if vehivle is damaged or/and alive, repeat for vehicle_speed times ecxept if mine was found first
     while(!vehicle::get_damaged() && vehicle::get_status() && moved<vehicle::get_speed()  && (!mine_check)){

               //get vehicle position
               int x = vehicle::pos_x; 
               int y = vehicle::pos_y;
               
               
         // ******** check for mines  ********
               int i = 0;
               //check 8 possible positions while no mine is found
               while(i < 8 && !mine_check){
                       int temp_x = x + dir_x[i];
                       int temp_y = y + dir_y[i];
                       
                       //check for mine, flag, empty position and avoid previous positon
                       if( (sim_map->get_block_type(temp_x,temp_y) == 2)  &&  !(sim_map->has_flag(temp_x,temp_y))  &&  (sim_map->get_id(temp_x,temp_y) == 0) && !(vehicle::prev_pos_x==temp_x) && !(vehicle::prev_pos_y==temp_y) )
                           //try to move to the suitable position
                           {if( vehicle::move_to_pos(temp_x,temp_y) ) mine_check = true;}
                       //else try another surrounding block
                       i++;
               }
         // ******** end of check for mines ********


         // ******** check for road (mine_check==false) ********
               int guard_value = 0; //max 8 loops
               road_check = false; //reinitialize road_check
               i = rand() % 8;
               //check 8 possible positions while no mine or road is found
               while( !road_check && guard_value < 8 && !mine_check ){
                      
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
     
     
     
     
     // ******** examine mine ********
     if(mine_check) action();
 
     // ******** return ********
     if(road_check || mine_check) return true;
     else return false;
     
}
     





int scout::get_num_flags(){
    return num_flags;
}




