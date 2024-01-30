#include "map.h"
#include "vehicle.h"

#ifndef SCOUT_H
#define SCOUT_H



class scout : public vehicle{

      private:
              int num_flags; //number of flags planted from this vehicle
              static int all_num_flags; //number of flags planted from all scouts
              
              
              static const int dir_x[8]; //constant array with 8 possible movements on surrounding blocks starting with 1:(1,0) going clockwise
              static const int dir_y[8];
              
              
      public:
             scout(void);
             
             bool action(); //plants flag to position, if fail returns false
             bool move();
             
             int get_num_flags(); //returns num_flags
             static int get_all_num_flags(); //returns all_num_flags
             static void reset_scout_class();
             
};












#endif
