#ifndef BLOCK_H
#define BLOCK_H

class block {
      private:
              int type; // 0 = border
                        // 1 = base
                        // 2 = mine
                        // 3 = road
              bool flag; //for mine only
              float danger; //for mine only
              int pa,ir,pl; //for base and mine
              int goal_pa,goal_ir,goal_pl; //only for base
              int id;
              int vehicle_type;
              
              static int all_pa; //all map resources
              static int all_ir;
              static int all_pl;
              
              static int num_borders;
              static int num_mines;
              static int num_roads;
              static float sum_danger;
              static int num_flags;
              

      public:
             block(int = 0);
             
             static void reset_block_class();
             
             int get_type(); //returns block type
             int get_all_pa();
             int get_all_ir();
             int get_all_pl();
             int get_num_borders();
             int get_num_mines();
             int get_num_roads();
             float get_all_danger(); //avarange danger of all mine blocks
             int get_num_flags(); //returns number of flags on map
             
             
             //base, mine and road only
             int get_id(); //returns vehicle's id on current possition or 0 if position it's empty
             
             //mine and road only
             int clear_id(); //returns cleared id and sets id to 0
             int set_id(int); //sets vehicle's id to block if it's empty, returns id. ONLY ONE VEHICLE IS ALLOWED FOR EACH BLOCK
                              //if current id isn't 0 then returns current id
                              //returns -1 if type is not 2 or 3
             int clear_vehicle_type(); //sets type to 0
             int set_vehicle_type(int); //sets type to 1,2,3
             int get_vehicle_type();
                          
             //mine and base only
             int get_pa(); //returns palladium resources
             int get_ir(); //returns iridium resources
             int get_pl(); //returns platinum resources
             int set_pa(int =0); //returns palladium resources, -1 for random
             int set_ir(int =0); //returns iridium resources
             int set_pl(int =0); //returns platinum resources
             
             //mine only
             int extract_pa(int extract_rate); //subtracts (1 round)*(extract_rate) from block's resources
             int extract_ir(int extract_rate); //returns 0 if extract_rate is incorrect or there are no more resources of the element
             int extract_pl(int extract_rate); //else returns extracted value
             bool has_flag(); //return true if block is flagged as dangerous
             bool set_flag(); //flags the block and returns finals block status
             bool clear_flag(); //unflags the block and returns final block status
             bool is_empty(); //check is mine resources are gone
             bool set_danger(float d); //if d<0 or d>1 then random, false if type isn't mine
             float get_danger(); //returns danger of position, 0 if type isn't mine
             //!!flags will also return false if block isn't type 2
             
             //base only
             int inc_id(); //increases base id which means a vehicle entered, returns final id
             int dec_id(); //decreases base id which means a vehicle left the base
             int unload_pa(int q_pa); //adds vehicle's resource load to base, returns the quantity left
             int unload_ir(int q_ir);
             int unload_pl(int q_pl);
             bool set_goal(int q_pa, int q_ir, int q_pl); //sets mining resources goal, returns false if it's more than all resources             
             int get_pa_goal(); //returns palladium goal
             int get_ir_goal(); //returns iridium goal
             int get_pl_goal(); //returns platinum goal
             bool is_full(); //returns true if base is full, !!returns also false if block is not base!!     
             bool is_full_pa(); //returns true is resoutce is full
             bool is_full_ir();
             bool is_full_pl();
             
             
             //friend class map; //so map can access the blocks            
                          
             void print_block(); //prints block infos on console output stream
};//end of class block

#endif
