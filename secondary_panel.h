#include "map.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "vehicle.h"
#include "scout.h"
#include "miner.h"
#include "rescuer.h"
#include <string>

#ifndef SECONDARY_PANEL_H
#define SECONDARY_PANEL_H

class secondary_panel {
      //white back ground
      //black letters
      //no user interaction
      //standard size
      private:
              //general panel surface
              SDL_Surface* secondary_panel_surface;
              
              //sendary panel state
              int secondary_panel_state; //0:clear panel
                                         //1:vehicle information
                                         //2:map position information
              
              //font and color
              TTF_Font* font;
              TTF_Font* header_font;
              SDL_Color textColorBlack;
              
              //all perpose rectangle
              SDL_Rect rect;
              
              //top offset, y axis
              int top_offset; //until last write height
              
              //simulation map
              map* sim_map;
              
              //vehicle id and map position
              int vehicle_id;
              int map_x, map_y;
              
              //write functions
              void write_on_panel(std::string str, int x, int y, TTF_Font* f); //x:offset from middle of panel, y:free
              
      public:
              //constructor
              secondary_panel(map* m);
              
              //update panel
              void update_secondary_panel();
              
              //write panel
              void clear_panel();
              void write_vehicle(int id);
              void write_map_pos(int x, int y);
              
              //get general panel surface
              SDL_Surface* get_secondary_panel_surface();
             
              //clean up
              void clean_up();
};

#endif
