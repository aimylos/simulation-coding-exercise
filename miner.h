#include "map.h"
#include "vehicle.h"

#ifndef MINER_H
#define MINER_H


class miner : public vehicle{

      private:
              int max_load; //max miner load, sum of resources
              int pa_load, ir_load, pl_load; //load of each resource
              int pa_extracted, ir_extracted, pl_extracted; //resources extracted until now
              
              static int all_pa_extracted, all_ir_extracted, all_pl_extracted; //resources extracted from all vehicles until now
              static int all_dead_pa_ext, all_dead_ir_ext, all_dead_pl_ext; //all resources dead vehicle carry
              
              int extract_rate; //function of the vehicle's speed
              float extract_access; //function of vehicle's access coefficient              
              
              int resource_priority; //1:palladium, 2:iridium, 3:platinum, which resources should action extract first
              static const int dir_x[8]; //constant array with 8 possible movements on surrounding blocks starting with 1:(1,0) going clockwise
              static const int dir_y[8];
              
              
              bool check_mine(int x, int y); //if mine is resourcefull, criteria depends goal completed, also sets priority
              bool enough_pa(); //returns true if miner has enough palladium to unload and complete the palladium goal
              bool enough_ir();
              bool enough_pl();
              
              bool miner_life; //same as status
              
              void check_dead_load();  //check for dead load
     
              
              
      public:
             miner(void);
             
             bool action(); //mine a mine, if fail returns false
             bool move(); //if it's on mine then do only action else move position
             
             bool unload(); //moves to base and unloads the resources
             bool is_full(); //returns true if vehicle is full
             
             
             
             int get_pa_load(); //returns palladium load
             int get_ir_load(); //returns iridium load
             int get_pl_load(); //returns platinum load
             
             int get_pa_extracted(); //returns palladium extracted
             int get_ir_extracted(); //returns iridium extracted
             int get_pl_extracted(); //returns platinum extracted
             
             static int get_all_pa_extracted(); //returns all palladium extracted
             static int get_all_ir_extracted(); //returns all iridium extracted
             static int get_all_pl_extracted(); //returns all platinum extracted
             static int get_all_dead_pa_ext(); 
             static int get_all_dead_ir_ext(); 
             static int get_all_dead_pl_ext(); 
             static void reset_miner_class();
             
};





#endif
