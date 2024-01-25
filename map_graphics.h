#include "map.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "vehicle.h"
#include <string>

#ifndef MAP_GRAPHICS_H
#define MAP_GRAPHICS_H


class map_graphics : public map {
      
      private:
              SDL_Surface* map_surface; //map surface
              
              //blocks
              int block_pixel_size; //pixel size map's blocks
              int map_size; //map size in blocks
              int real_map_pixel_size; //real map size after block division

              //load bimaps
              bool load_textures(); //returns true if all bitmaps were loaded, else false but still initialize textures with colors
              bool load_tx(SDL_Surface** surface, std::string filename, SDL_Rect* rect); //help function for loading bitmap textures
              bool textures_loaded; //returns true if load_textures was called at least once
              
              //rescale
              bool rescale_textures(); 
              bool rescale_tx(SDL_Surface** surface); //help function for rescaling bitmap textures
              bool textures_rescaled;            
              //sdl rescaling function, http://www.sdltutorials.com/sdl-scale-surface
              static SDL_Surface* ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height);
              //http://sdl.beuc.net/sdl.wiki/Pixel_Access
              static Uint32 ReadPixel(SDL_Surface *surface, int x, int y);
              static void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
              
              //road, mine, border
              SDL_Surface* border_tx;
              SDL_Surface* road_tx;
              SDL_Surface* mine_tx;                    
              SDL_Surface* f_mine_tx; //flagged mined
              //alive vehicle
              SDL_Surface* miner_on_road_alive_tx;
              SDL_Surface* miner_on_mine_alive_tx;
              SDL_Surface* miner_on_f_mine_alive_tx;
              SDL_Surface* scout_on_road_alive_tx;
              SDL_Surface* scout_on_mine_alive_tx;
              SDL_Surface* scout_on_f_mine_alive_tx;
              SDL_Surface* rescuer_on_road_alive_tx;
              SDL_Surface* rescuer_on_mine_alive_tx;
              SDL_Surface* rescuer_on_f_mine_alive_tx;
              //dead vehicle
              SDL_Surface* miner_on_road_dead_tx;
              SDL_Surface* miner_on_mine_dead_tx;
              SDL_Surface* miner_on_f_mine_dead_tx;
              SDL_Surface* scout_on_road_dead_tx;
              SDL_Surface* scout_on_mine_dead_tx;
              SDL_Surface* scout_on_f_mine_dead_tx;
              SDL_Surface* rescuer_on_road_dead_tx;
              SDL_Surface* rescuer_on_mine_dead_tx;
              SDL_Surface* rescuer_on_f_mine_dead_tx;
              //base
              SDL_Surface* base_empty_tx;
              SDL_Surface* base_crowed_tx;
              //written texture
              SDL_Surface* written_tx; //a temporary texture that something was written on
              
              //font and colors
              TTF_Font* font;
              SDL_Color textColorBlack;
              SDL_Color textColorWhite;
              
              //get position texture
              SDL_Surface* get_pos_texture(int x, int y); //returns NULL if error
              
              //write on surface
              SDL_Surface* write_on_tx(SDL_Surface* source, SDL_Surface* destination, std::string str, SDL_Color textColor);
              
      public:
             
              map_graphics (int s, int x, int y, int* round); //will call superclass constructor
             
              SDL_Surface* get_map_surface(); // returns map surface address, NULL if error
              
              bool update_map(); //returns true if map_surface successfully updated             
             
             
              //must be caled by destructor
              void clean_up(); //unloads all textures and fonts from memory         
              
              
              
              
              
              //int to string 
              static std::string NumToStr ( int Number );    
             
};









#endif
