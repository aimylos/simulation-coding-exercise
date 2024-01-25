#include "map.h"
#include <list>
using std::list;

#ifndef VEHICLE_H
#define VEHICLE_H

class vehicle {
      private:
              int id; //every vehicle has a unique id based on creation order
              
              //statics
              static int num_vehicles;
              static int num_alive_vehicles;
              static int num_damaged_vehicles;
              static list<vehicle*> vehicle_address_list; //list of vehicle's addresses
              
              //rounds
              int enter_round;
              //speed
              int speed; //takes values between MAX and MIN SPEED
              //status and damage
              bool status; //alive or dead
              bool damaged; //damaged or not
              int damage_round; //round the vehicle was damaged
              //access coefficient
              float access;
              
              //times damaged
              int times_damaged;
              //moved positions
              int moved_pos;
 
              
      protected:
                
                static int num_scouts;
                static int num_miners;
                static int num_rescuers;
                static int num_alive_scouts;
                static int num_alive_miners;
                static int num_alive_rescuers;
                
                //general
                int type; //0:nothing, 1:scout, 2:miner, 3:rescuer
                //position
                int prev_pos_x;
                int prev_pos_y;
                int pos_x;
                int pos_y;
                //map
                map *sim_map;
                //rounds
                int *current_round; //current's round pointer   
                
                //map
                bool in_map; //true if vehicle is in the map             
                
                
      public:
             //constructor
             vehicle(void);             
             
             //general members
             static int get_num_vehicles();
             static int get_num_alive_vehicles();
             static int get_num_alive_scouts();
             static int get_num_scouts();
             static int get_num_miners();
             static int get_num_rescuers();
             static int get_num_alive_miners();
             static int get_num_alive_rescuers();
             static int get_num_damaged_vehicles();
             static void reset_vehicle_class();
             int get_id();
             int get_type();
             
             //get round address
             int* get_round();
             
             //speed
             int set_speed(int); //-1 random, -2 random slow, -3 random fast, if !=limits then random
             int get_speed();
             
             //status
             bool set_status(bool);
             bool get_status();
             
             //damage
             bool set_damaged(bool);
             bool get_damaged();
             
             //access coefficient
             float set_access(float); //-1 random, -2 random low, -3 random high, limits=0.1-1, if !=limits then random
             float get_access();
             
             //map
             bool is_in_map();
             void set_map(map*);
             bool enter_map(int *round, int x, int y); //returns false if fails to enter map
              
             //position
             bool move_to_pos(int x, int y); //moves vehicle to position on map, returns false if the move failed (map rejected the move or vehicle can't move that this speed)
             int get_pos_x();
             int get_pos_y();
             
             //vehicle addresses
             static vehicle* get_vehicle_address(int v_id); //returns vehicle's address from id
             static bool remove_vehicle_address(int v_id); //sets vehicle's address to NULL
             
             //times damaged
             int get_times_damaged();
             //moved positons
             int get_moved_positions();
             
             //virtual members
             //virtual bool action(); //vehicle's action, returns true if action completed successfully
             virtual bool move(); //vehicle's moving function
             
             
             
};
#endif
