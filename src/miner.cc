#include <iostream>
using std::cout;
using std::endl;
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include "miner.h"
#include "map.h"
#include "vehicle.h"

#define MAX_MINER_LOAD 300
//starting stage
#define EXTRACT_RATE_USAGE 50.0 //% of extrace_rate that a mine resource must cover
//ending stage
#define ENDING_STAGE_PERC 80.0 //% of goal completed before miner goes on less resourceful mines
#define ENDING_STAGE_ROUNDS 100 //after 100 rounds miner goes on less resourceful mines


//initialize static variables
int miner::all_pa_extracted = 0;
int miner::all_ir_extracted = 0;
int miner::all_pl_extracted = 0;
int miner::all_dead_pa_ext = 0;
int miner::all_dead_ir_ext = 0;
int miner::all_dead_pl_ext = 0;

//static methods
int miner::get_all_pa_extracted(){
       return all_pa_extracted;
}
int miner::get_all_ir_extracted(){
       return all_ir_extracted;
}
int miner::get_all_pl_extracted(){
       return all_pl_extracted;
}
int miner::get_all_dead_pa_ext(){
    return all_dead_pa_ext;
}
int miner::get_all_dead_ir_ext(){
    return all_dead_ir_ext;
}
int miner::get_all_dead_pl_ext(){
    return all_dead_pl_ext;
}
void miner::reset_miner_class(){
     all_pa_extracted = 0;
     all_ir_extracted = 0;
     all_pl_extracted = 0;
}

//define static constants
const int miner::dir_x[8]={1, 1, 0, -1, -1, -1,  0,  1};
const int miner::dir_y[8]={0, 1, 1,  1,  0, -1, -1, -1};



//constructor
miner::miner(void)
      : vehicle()
      {
                    //set vehicle type miner:2
                    vehicle::type = 2; 
                    
                    //increase miners
                    vehicle::num_alive_miners++;
                    vehicle::num_miners++;
                    
                    //set max load
                    max_load = MAX_MINER_LOAD;
                    
                    //set load and extracted resources 0
                    pa_load = 0;
                    ir_load = 0;
                    pl_load = 0;
                    pa_extracted = 0;
                    ir_extracted = 0;
                    pl_extracted = 0;
                    
                    //set access coefficient
                    vehicle::set_access(-3);
                    
                    //set scout speed
                    vehicle::set_speed(-1);
                    
                    //set extract rate
                    extract_rate = 10*vehicle::get_speed();
                    
                    //set extract access coefficient, higher than access coefficient
                    extract_access = vehicle::get_access() * 1.3; //increased 30%
                    if(extract_access > 1.0) extract_access = 1;   
                    
                    //set resource priority
                    resource_priority = 1;
                    
                    //set miner life true
                    miner_life = true;                 
                    
}





//action, extract one resource at a time
bool miner::action(){
     //check if vehicle is ok
     if(vehicle::get_damaged() || !vehicle::get_status()) return false;
     
     int x = vehicle::pos_x; //get vehicle position
     int y = vehicle::pos_y;
     
     //check if miner is full and block is mine
     if( !is_full() && sim_map->get_block_type(x,y) == 2 ){
         
         //temporary extract rate to avoid extracting more than possible to carry
         int space_left = max_load - pa_load - ir_load - pl_load;
         int temp_extract_rate = (space_left >= extract_rate)? extract_rate : space_left;
         
         //extract the priority resource
         switch(resource_priority){
                 case 1:
                      if(sim_map->get_pa(x,y) > 0){
                          int ext = sim_map->extract_pa(x,y,temp_extract_rate);
                          pa_load += ext;
                          pa_extracted +=ext;
                          all_pa_extracted += ext;
                          break;
                      }
                 case 2:
                      if(sim_map->get_ir(x,y) > 0){
                          int ext = sim_map->extract_ir(x,y,temp_extract_rate);
                          ir_load += ext;
                          ir_extracted +=ext;
                          all_ir_extracted += ext;
                          break;
                      }
                 case 3:
                      if(sim_map->get_pl(x,y) > 0){
                          int ext = sim_map->extract_pl(x,y,temp_extract_rate);
                          pl_load += ext;
                          pl_extracted +=ext;
                          all_pl_extracted += ext;
                          break;
                      }
                 default:
                      if(sim_map->get_pa(x,y) > 0){
                          int ext = sim_map->extract_pa(x,y,temp_extract_rate);
                          pa_load += ext;
                          pa_extracted +=ext;
                          all_pa_extracted += ext;
                          }
                      else if(sim_map->get_ir(x,y) > 0){
                          int ext = sim_map->extract_ir(x,y,temp_extract_rate);
                          ir_load += ext;
                          ir_extracted +=ext;
                          all_ir_extracted += ext;
                          }
                      else{
                          int ext = sim_map->extract_pl(x,y,temp_extract_rate);
                          pl_load += ext;
                          pl_extracted +=ext;
                          all_pl_extracted += ext;
                          }
                 }
              
         //extraction damage check   
         float damage_propability = sim_map->get_danger(x,y) * (1.0 - extract_access); 
         float damage_occurred = (double)rand() / RAND_MAX; //the higher the safest
         if(damage_occurred < damage_propability){
                vehicle::set_damaged(true);
                return false;
                }
         else 
              return true;

     }
     else return false;
}





//unload
bool miner::unload(){
     //check if vehicle is ok
     if(vehicle::get_damaged() || !vehicle::get_status()) return false;
     
     //check if move to base was successful before unload
     if(vehicle::move_to_pos(sim_map->get_base_x(), sim_map->get_base_y()) ){
          //unload resources and keep what's left
          pa_load = sim_map->unload_pa(pa_load);
          ir_load = sim_map->unload_ir(ir_load);
          pl_load = sim_map->unload_pl(pl_load);
          
          //dump the resources left
          pa_load = 0;
          ir_load = 0;
          pl_load = 0;
          
          return is_full()?false:true;
     }
     else {
          return false;
     }

}




//move
bool miner::move(){
     if(!vehicle::in_map) return false; //check if in map
     //check for dead load
     check_dead_load();
     
     //check if vehicle is ok
     if(vehicle::get_damaged() || !vehicle::get_status()) return false;

     int x = vehicle::pos_x; //get vehicle position
     int y = vehicle::pos_y;
     
     // ******** check if vehicle is full or if vehicle load is enough to end the simulation ********
     if(is_full() || (enough_pa() && enough_ir() && enough_pl()) ){
                 return unload();
     }

     //  ******** if vehicle is on not-empty mine and if base isn't full, then extract what's needed ********
     if(sim_map->get_block_type(x,y) == 2  && !sim_map->is_full()){
              //if any of the resources are needed then extract
              if(!enough_pa()  &&  sim_map->get_pa(x,y) > 0){
                          resource_priority = 1; //set priority to palladium
                          //extract and if full unload
                          if(action()){
                               if(is_full()) return unload();
                               else return true;
                               }
                          else return false; 
                          }
              else if(!sim_map->is_full_ir()  &&  sim_map->get_ir(x,y) > 0){
                          resource_priority = 2; //set priority to iridium
                          //extract and if full unload
                          if(action()){
                               if(is_full()) return unload();
                               else return true;
                               }
                          else return false; 
                          }
              else if(!sim_map->is_full_pl()  &&  sim_map->get_pl(x,y) > 0){
                          resource_priority = 3; //set priority to platinum
                          //extract and if full unload
                          if(action()){
                               if(is_full()) return unload();
                               else return true;
                               }
                          else return false; 
                          }
     }//else continue to movement
              
           
              
     // **************** movement ****************        
     int moved = 0;
     bool mine_check = false; //boolean True:found mine
     bool road_check = false; //boolean True:last move was on road
     //will stop if it finds resourcefull mine
     //there's a possibility the vehicle gets surrounded
     
     //movement,  check if vehivle is damaged or/and alive, repeat for vehicle_speed times ecxept if mine was found first
     while(!vehicle::get_damaged() && vehicle::get_status() && moved<vehicle::get_speed()  && (!mine_check)){
               
               //get vehicle position
               x = vehicle::pos_x; 
               y = vehicle::pos_y;
               

          // ******** check for mines  ********
               int i = 0;
               //check 8 possible positions while no mine is found
               while(i < 8 && !mine_check){
                       int temp_x = x + dir_x[i];
                       int temp_y = y + dir_y[i];
                       
                       //check for check_mine, empty position and avoid previous positon
                       if( check_mine(temp_x,temp_y)  &&  (sim_map->get_id(temp_x,temp_y) == 0) && !(vehicle::prev_pos_x==temp_x) && !(vehicle::prev_pos_y==temp_y) )
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

     moved++;
     }
     //end of movement
     
     
     // ******** extract if mine was found and unlaod if miner is full, priority was already set by check_mine ********
     if (mine_check) if (action()) if (is_full()) unload();
     

     // ******** return ********
     if(road_check || mine_check) return true;
     else return false;
}












//check_mine
bool miner::check_mine(int x, int y){
     if(sim_map->get_block_type(x,y) != 2) return false;
     
     //if goal_completed % and current round are lower than ending stage values mine only on safe and resourceful mines
     if( sim_map->goal_completed() < ENDING_STAGE_PERC   &&  *vehicle::current_round < ENDING_STAGE_ROUNDS ){
         if ( sim_map->has_flag(x,y) ) return false;
         
         else if ( !enough_pa() && ((float)sim_map->get_pa(x,y)/(float)extract_rate*100)>=EXTRACT_RATE_USAGE ) 
              { resource_priority = 1; return true; }
         else if ( !enough_ir() && ((float)sim_map->get_ir(x,y)/(float)extract_rate*100)>=EXTRACT_RATE_USAGE )
              { resource_priority = 2; return true; }
         else if ( !enough_pl() && ((float)sim_map->get_pl(x,y)/(float)extract_rate*100)>=EXTRACT_RATE_USAGE )
              { resource_priority = 3; return true; }

         else return false;
     }
     //else mine on any mine that has resources
     else {
          if ( !enough_pa() && sim_map->get_pa(x,y)>0 ) return true;
          else if ( !enough_ir() && sim_map->get_ir(x,y)>0 ) return true;
          else if ( !enough_pl() && sim_map->get_pl(x,y)>0 ) return true;
          else return false;
     }
}


//is_full
bool miner::is_full(){
     if(  (pa_load + ir_load + pl_load) >= max_load  ) return true;
     else return false;
}

//enough resources
bool miner::enough_pa(){
     if( sim_map->get_pa_goal() <= (all_pa_extracted - all_dead_pa_ext) ) return true;
     else return false;
}
bool miner::enough_ir(){
     if( sim_map->get_ir_goal() <= (all_ir_extracted - all_dead_ir_ext) ) return true;
     else return false;
}
bool miner::enough_pl(){
     if( sim_map->get_pl_goal() <= (all_pl_extracted - all_dead_pl_ext) ) return true;
     else return false;
}


//check for dead load
void miner::check_dead_load(){
     if(miner_life  &&  !get_status()){
                    all_dead_pa_ext += pa_load;
                    all_dead_ir_ext += ir_load;
                    all_dead_pl_ext += pl_load;
                    miner_life = false;
                    }
     else if(!miner_life  &&  get_status()){
                          all_dead_pa_ext -= pa_load;
                          all_dead_ir_ext -= ir_load;
                          all_dead_pl_ext -= pl_load;
                          miner_life = true;
                          }
                        
     //check if resources are enough to succeed
     if(sim_map->get_all_pa() - all_dead_pa_ext  <  sim_map->get_pa_goal() 
     || sim_map->get_all_ir() - all_dead_ir_ext  <  sim_map->get_ir_goal()
     || sim_map->get_all_pl() - all_dead_pl_ext  <  sim_map->get_pl_goal() ){
                 //simulation failed
                 sim_map->set_stop(true);
     }
     
     
}                         
 








//get members
int miner::get_pa_load() {
    return pa_load;
}
int miner::get_ir_load() {
    return ir_load;
}
int miner::get_pl_load() {
    return pl_load;
}   
int miner::get_pa_extracted(){
    return pa_extracted;
} 
int miner::get_ir_extracted(){
    return ir_extracted;
}
int miner::get_pl_extracted(){
    return pl_extracted;
}    
