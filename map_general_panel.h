#include "map.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "vehicle.h"
#include "scout.h"
#include "miner.h"
#include "rescuer.h"
#include <string>

#ifndef MAP_GENERAL_PANEL_H
#define MAP_GENERAL_PANEL_H

class map_general_panel {
      
      //white back ground
      //black letters
      //no user interaction
      //standard size
            
      private:
              //general panel surface
              SDL_Surface* general_panel_surface;
              
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
             
              //write functions
              void write_on_panel(std::string str, int x, int y, TTF_Font* f); //x:offset from middle of panel, y:free
              
              void clear_panel();
              void write_header();
              void write_simulation_status();
              void write_base_information();
              void write_map_information();
              void write_vehicles_information();
              
      public:
             //constructor
             map_general_panel(map* m);
             
             //get general panel surface
             SDL_Surface* get_general_panel_surface();
             
             //update general panel
             void update_general_panel();
             
             //clean up
             void clean_up();
      
};

#endif
