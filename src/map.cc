#include <iostream>
using std::cout;
using std::endl;
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;


#include "block.h"
#include "map.h"

#define MIN_SIZE 2   
#define MIN_RAND_SIZE 30
#define MAX_SIZE 100
#define MAX_RAND_SIZE 60
#define MINE_CREATION_CHANCE 30  //30% a mine is created at a mine position 



//map constructor
map::map(int s, int x, int y, int* round){
             //srand(time(NULL)); //seeding random generator
             
             //set current round address
             current_round = round;
             
             //check for correct map size 
             if(s < MIN_SIZE || s > MAX_SIZE)    
                 size = (rand() % (MAX_RAND_SIZE - MIN_RAND_SIZE + 1)) + MIN_RAND_SIZE + 2; //adding 2 for borders
             else
                 size = s + 2;
             
             //check for correct base position
             if(x < 1 || x > size-2)  //base must be inside map area and not on borders
                  base_x = (rand() % (size - 2 - 1)) + 1;
             else
                  base_x = x;
             
             if(y < 1 || y > size-2)
                  base_y = rand() % (size - 2 - 1) + 1;
             else
                  base_y = y;
             
              //initialize temp map
              temp_map = new int*[size];
              for(int i=0;i<size;i++){
                         temp_map[i] = new int[size];
                         for(int j =0;j<size;j++)
                                 temp_map[i][j] = 0;
              }
              
              //assign block types to temp map
              //so that borders go to the side
              //two mines cannot be next to each other
              //base and mine cannot be next to each other
              //the rest is road
             for (int i = 0; i<size; i++){  //borders
                            temp_map[0][i] = 0;
                            temp_map[size-1][i] = 0;
             }
             for (int i=1; i<(size-1); i++){ //x axis
                       temp_map[i][0] = 0;  //borders
                       temp_map[i][size-1] = 0;
                       
                       for(int j=1; j<(size-1); j++){  //y axis
                               if(i==base_x && j==base_y){ //checks for base
                                    temp_map[i][j] = 1;
                               }
                               else{
                                    bool check = false; //checks if mine or base is around
                                    if(temp_map[i][j-1] == 1 || temp_map[i][j-1] == 2) check =true;
                                    if(temp_map[i-1][j-1] == 1 || temp_map[i-1][j-1] == 2) check = true;
                                    if(temp_map[i-1][j] == 1 || temp_map[i-1][j] == 2) check =true;
                                    if(check == true)
                                            temp_map[i][j] = 3;
                                    else {                 //MINE_CREATION_CHANCE% chance of making a mine
                                            int temp_rand = rand() % 101;
                                            if(temp_rand < MINE_CREATION_CHANCE){
                                                  temp_map[i][j] = 2;
                                            }
                                            else{
                                                  temp_map[i][j] = 3;
                                            }
                                    }
                               }
                       }
              }              
                       
              //initialize map         
              block_map = new block*[size];
              for(int i=0;i<size; i++){
                      block_map[i] = new block[size];         
                      for(int j=0; j<size; j++){
                              block_map[i][j] = block(temp_map[i][j]);
                      }
              }
              
              //initialize goal values
              block_map[base_x][base_y].set_goal(-1,-1,-1); //set goal to random 
              
              //initialize stop
              stop = false;
              
              //initialize vehicle counters
              vehicles_in_map = 0;
              vehicles_entered = 0;
                       
}                     

//static members
void map::reset_map_class(){
     block::reset_block_class();
}              
//general members
int* map::get_round(){
    return current_round;
}
bool map::check_coord(int x, int y){
     if(x < 0 || y < 0 || x > size-1 || y > size-1) return false;
     else return true;
} 
int map::get_size(){
    return size;
}
int map::get_base_x(){
    return base_x;
}
int map::get_base_y(){
    return base_y;
}
int map::get_block_type(int x, int y){
    if(check_coord(x,y)) return block_map[x][y].get_type();
    else return -1;
}
int map::get_all_pa(){
    return block_map[0][0].get_all_pa();
}
int map::get_all_ir(){
    return block_map[0][0].get_all_ir();
}
int map::get_all_pl(){
    return block_map[0][0].get_all_pl();
}
int map::get_num_borders(){
    return block_map[0][0].get_num_borders();
}
int map::get_num_mines(){
    return block_map[0][0].get_num_mines();
}
int map::get_num_roads(){
    return block_map[0][0].get_num_roads();
}
float map::get_all_danger(){
    return block_map[0][0].get_all_danger();
}
int map::get_num_flags(){
    return block_map[0][0].get_num_flags();
}
//end of general members
              
              
//type 2 mine
bool map::set_flag(int x, int y){
     if(check_coord(x,y)) return block_map[x][y].set_flag();             
     else return false;
}              

bool map::clear_flag(int x, int y){
     if(check_coord(x,y)) return block_map[x][y].clear_flag();             
     else return false;
}

bool map::has_flag(int x, int y){
     if(check_coord(x,y)) return block_map[x][y].has_flag();             
     else return false;
}

bool map::set_danger(int x, int y, float d){
     if(check_coord(x,y)) return block_map[x][y].set_danger(d);
     else return false;
}
float map::get_danger(int x, int y){
      if(check_coord(x,y)) return block_map[x][y].get_danger();
      else return 0;
}

int map::extract_pa(int x, int y, int extract_rate){
     if(check_coord(x,y)) return block_map[x][y].extract_pa(extract_rate);
     else return 0;
}
int map::extract_ir(int x, int y, int extract_rate){
     if(check_coord(x,y)) return block_map[x][y].extract_ir(extract_rate);
     else return 0;
}
int map::extract_pl(int x, int y, int extract_rate){
     if(check_coord(x,y)) return block_map[x][y].extract_pl(extract_rate);
     else return 0;
}
//end of type 2


//type 1 and 2
int map::get_pa(int x, int y){
     if(check_coord(x,y)) return block_map[x][y].get_pa();             
     else return 0;
}
int map::get_ir(int x, int y){
     if(check_coord(x,y)) return block_map[x][y].get_ir();             
     else return 0;
}
int map::get_pl(int x, int y){
     if(check_coord(x,y)) return block_map[x][y].get_pl();             
     else return 0;
}
int map::set_pa(int x, int y, int q){
     if(check_coord(x,y)) {
              q = block_map[x][y].set_pa(q);
              if( !set_goal(get_pa_goal(),get_ir_goal(),get_pl_goal()) )
                   set_goal(get_all_pa(),get_ir_goal(),get_pl_goal());
              return q;
     }            
     else return 0;
}
int map::set_ir(int x, int y, int q){
     if(check_coord(x,y)) {
              q = block_map[x][y].set_ir(q);
              if( !set_goal(get_pa_goal(),get_ir_goal(),get_pl_goal()) )
                   set_goal(get_pa_goal(),get_all_ir(),get_pl_goal());
              return q;
     }            
     else return 0;
}
int map::set_pl(int x, int y, int q){
     if(check_coord(x,y)) {
              q = block_map[x][y].set_pl(q);
              if( !set_goal(get_pa_goal(),get_ir_goal(),get_pl_goal()) )
                   set_goal(get_pa_goal(),get_ir_goal(),get_all_pl());
              return q;
     }            
     else return 0;
}
//end of type 1 and 2
     
//type 1
bool map::set_goal(int q_pa, int q_ir, int q_pl){
     return block_map[base_x][base_y].set_goal(q_pa, q_ir, q_pl);  
}
int map::get_pa_goal(){
     return block_map[base_x][base_y].get_pa_goal();
}
int map::get_ir_goal(){
     return block_map[base_x][base_y].get_ir_goal();
}
int map::get_pl_goal(){
     return block_map[base_x][base_y].get_pl_goal();
}
int map::get_base_pa(){
    return block_map[base_x][base_y].get_pa();
}
int map::get_base_ir(){
    return block_map[base_x][base_y].get_ir();
}
int map::get_base_pl(){
    return block_map[base_x][base_y].get_pl();
}
int map::get_base_id(){
    return get_id(base_x, base_y);
}
bool map::is_full(){
     return block_map[base_x][base_y].is_full();
}
bool map::is_full_pa(){
     return block_map[base_x][base_y].is_full_pa();
}
bool map::is_full_ir(){
     return block_map[base_x][base_y].is_full_ir();
}
bool map::is_full_pl(){
     return block_map[base_x][base_y].is_full_pl();
}
float map::goal_completed(){
      int goal_sum = get_pa_goal() + get_ir_goal() + get_pl_goal();
      int resources_sum = get_base_pa() + get_base_ir() + get_base_pl();
      return (float)resources_sum / (float)goal_sum * 100.0;
}
int map::unload_pa(int q_pa){
    return block_map[base_x][base_y].unload_pa(q_pa);
}
int map::unload_ir(int q_ir){
    return block_map[base_x][base_y].unload_ir(q_ir);
}
int map::unload_pl(int q_pl){
    return block_map[base_x][base_y].unload_pl(q_pl);
}

//end of type 1

//type 2 and 3
bool map::insert_vehicle(int x, int y, int id, int v_type){
     if(!check_coord(x,y)) return false; //check for correct coordinates
     if(block_map[x][y].get_type() != 3) return false; //vehicles enters only on road
     if(block_map[x][y].get_id() != 0) return false; //block must be empty 
     
     block_map[x][y].set_id(id);
     block_map[x][y].set_vehicle_type(v_type);
     vehicles_in_map++; //increase vehicles in map
     vehicles_entered++; //increase vehicles entered
     return true;
}

bool map::move_vehicle(int x, int y, int prev_x, int prev_y, int id, int v_type){
     if(!check_coord(x,y)) return false;
     else if(x == prev_x  &&  y == prev_y) return false; //vehicle didn't move
     else if(block_map[x][y].get_type() == 1) { //check for base
          block_map[prev_x][prev_y].clear_id();
          int t = block_map[prev_x][prev_y].clear_vehicle_type();
          block_map[x][y].inc_id();
          return true;
          } 
     else if(block_map[prev_x][prev_y].get_type() != 1  &&  block_map[prev_x][prev_y].get_id() != id)  //check if the vehicle was in the previous position, exept if it was base
          return false;
     else if(block_map[x][y].get_id() != 0) //check if new position is available
          return false; 
     else { //move
          if(block_map[prev_x][prev_y].get_type() == 1) //if previous position was base decrease id
               block_map[prev_x][prev_y].dec_id();
          else {  //else clear previous postion
               block_map[prev_x][prev_y].clear_id();
               block_map[prev_x][prev_y].clear_vehicle_type();
               }
          block_map[x][y].set_id(id);
          block_map[x][y].set_vehicle_type(v_type);
          return true;
     }
}
bool map::remove_vehicle(int x, int y){
     if(!check_coord(x,y)) return false;
     else if(block_map[x][y].get_type() == 1 && block_map[x][y].get_id() > 0){
          block_map[x][y].dec_id();
          vehicles_in_map--; //decrease vehicles in map 
          return true;
          }
     else if(block_map[x][y].get_id() > 0){
          block_map[x][y].clear_id();
          block_map[x][y].clear_vehicle_type();
          vehicles_in_map--; //decrease vehicles in map
          return true;
          }
     else
         return false;
}
         
int map::get_id(int x, int y){
    if(check_coord(x,y)) return block_map[x][y].get_id();
    else return -1;
}
int map::get_vehicle_type(int x, int y){
    if(check_coord(x,y)) return block_map[x][y].get_vehicle_type();
    else return -1;
}



int map::simulation_status_on_map(){
    //stop simulation on map
    if(stop) return -1;
    
    //not started
    else if(vehicles_entered == 0) return 0;
    
    //goal completed
    else if(is_full()) return 3;
    
    //ongoing
    else if(vehicles_in_map > 0) return 1;
    
    //dead area
    else if(vehicles_in_map == 0) return 2;
    
    //unknown
    else return -2;
}

void map::set_stop(bool x){
     stop = x;
}













              
void map::print_map(){
     
     
     
                     // cout << block_map[i][j].get_type();
     
     
     
     
     
                     /* cout << "Size: " << size << "base x y: " << base_x << " " << base_y << endl;
     
     
     
                   /*for(int i=0;i<size; i++){
                           
                      for(int j=0; j<size; j++){
                              block_map[i][j].print_block();
                              cout << block_map[i][j].get_type() << endl;
                      }
              }*/
              }                       
              
