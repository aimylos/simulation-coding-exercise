#include "map.h"
#include "vehicle.h"

#ifndef RESCUER_H
#define RESCUER_H



class rescuer : public vehicle{
      
      private:
      
              int num_rescued; //number of vehicles rescued by this rescuer
              static int all_num_rescued; //number of vehicles rescued by all rescuers
      
              
              static const int dir_x[8]; //constant array with 8 possible movements on surrounding blocks starting with 1:(1,0) going clockwise
              static const int dir_y[8];
      
              int rescue_pos_x; //position of vehicle that needs to be rescued
              int rescue_pos_y; //no position is (-1,-1)
              
              bool surround_check(int x, int y); //returns true if (x,y) is around rescuer's positions
      
      public:
             
             rescuer(void);
      
      
             bool action(); //plants flag to position, if fail returns false
             bool move();
             
             
             bool set_rescue_pos(int x, int y); //returns false if vehicle already has position to rescue or coordinates are false
             bool force_set_rescue_pos(int x, int y); //returns false if coordinates are false
             
             
             int get_num_rescued();
             static int get_all_num_rescued();
             static void reset_rescuer_class();
      
      
};




#endif
