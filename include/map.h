#include "block.h"

#ifndef MAP_H
#define MAP_H



class map{
    private:
      block** block_map; 
      int base_x, base_y;      
      int** temp_map;
      int size;
      int* current_round;
      bool stop; //stop simulation by reporting unknown
      int vehicles_in_map; //number of vehicles in map
      int vehicles_entered; //number of vehicles enter the map until now
      
      
      
    public:        
      map(int s, int x, int y, int* round);  //s:map size, x,y:base coordinates, -1 for random, (x,y) -> (1:size,1:size), round address
      //borders coordinates have 0 or s+1
      //map coordinates have (x,y)==(1:s,1:s)
      
      static void reset_map_class();
      
      //general members
      int* get_round();
      bool check_coord(int x, int y); //checks if coordinates are correct
      int get_size();
      int get_base_x(); //get base x axis coordinate
      int get_base_y();      
      int get_block_type(int x, int y); //returns -1 if coordinates are wrong
      int get_all_pa(); //get whole map info
      int get_all_ir();
      int get_all_pl();
      int get_num_borders();
      int get_num_mines();
      int get_num_roads();
      float get_all_danger(); //average danger
      int get_num_flags();
      int simulation_status_on_map(); //returns  0:not started, means no vehicle has enter
                                      //         1:ongoing, means vehicles are on map and goal hasn't been reached
                                      //         2:dead area, means vehicles were on map, but all died   
                                      //         3:goal completed, means base goal reached
                                      //        -1:stop
                                      //        -2:if unknown
      void set_stop(bool x); //set to unknown
      
      //mine only
      bool set_flag(int x, int y);//will only set on type 2
      bool clear_flag(int x, int y);//returns current flag status
      bool has_flag(int x, int y);//!will also return false if type!=2
      bool set_danger(int x, int y, float d); //d>1 or d<0 sets random danger, returns false if block isn't mine
      float get_danger(int x, int y);
      int extract_pa(int x, int y, int extract_rate); //returns extracted resource or 0 if failed
      int extract_ir(int x, int y, int extract_rate);
      int extract_pl(int x, int y, int extract_rate);
      
          
      //mine and base only
      int get_pa(int x, int y);
      int get_ir(int x, int y);
      int get_pl(int x, int y);
      int set_pa(int x, int y, int q);//q:quantity, q=-1 for random
      int set_ir(int x, int y, int q);
      int set_pl(int x, int y, int q);
      
      //base only
      bool set_goal(int q_pa, int q_ir, int q_pl); //sets mining resources goal, returns false if it's more than all resources, -1 for random             
      int get_pa_goal(); //returns palladium goal
      int get_ir_goal(); //returns iridium goal
      int get_pl_goal(); //returns platinum goal
      int get_base_pa(); //returns base resources
      int get_base_ir();
      int get_base_pl();
      int get_base_id();
      bool is_full(); //returns true if base is full, !!returns also false if block is not base!!     
      bool is_full_pa(); //returns true if resource is full
      bool is_full_ir();
      bool is_full_pl();
      float goal_completed(); //returns % of goal completed
      int unload_pa(int q_pa); //unload resource to base, returns resource left
      int unload_ir(int q_ir); //!!unload doesn't check if vehicle is actually on base or not!!
      int unload_pl(int q_pl);
      
            
      //mine and road only
      bool insert_vehicle(int x, int y, int id, int v_type); //insert vehicle in map, returns false if move failed
      bool move_vehicle(int x, int y, int prev_x, int prev_y, int id, int v_type); //move vehicle
      bool remove_vehicle(int x, int y); //removes vehicle on position (x,y), returns false if nothing removed
      int get_id(int x, int y); //return vehicle id on position
      int get_vehicle_type(int x, int y); //returns vehicle type on position or 0 if no vehicle, else returns -1
            
      
    
      
      void print_map();
      
      
    
};



#endif
