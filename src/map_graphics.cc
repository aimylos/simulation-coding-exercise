#include "map.h"
#include "map_graphics.h"
#include "vehicle.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>

#include <iostream>
using std::cout;
using std::endl;

#define MAP_PIXEL_SIZE 600  //square map of 600 pixels
#define SCREEN_BPP 32




map_graphics::map_graphics(int s, int x, int y, int* round) : map(s,x,y,round) {
            
            //calcualte map sizes
            map_size = map::get_size();
            block_pixel_size = MAP_PIXEL_SIZE/map_size;
            real_map_pixel_size = block_pixel_size * map_size;
            
            //set font
            //calculate font size
            int font_size=15;
            if(block_pixel_size <= 10) font_size = 4;
            else if(block_pixel_size <= 15) font_size = 6;
            else if(block_pixel_size <= 20) font_size = 8;
            else if(block_pixel_size <= 25) font_size = 12;
            else if(block_pixel_size <= 30) font_size = 14;
            else if(block_pixel_size <= 40) font_size = 20;
            else font_size = 28;
            //load font
            font = TTF_OpenFont( "fonts/arial.ttf", font_size ); //if didn't load then no fonts will appear
            
            //set font colors
            SDL_Color textColor = { 255, 255, 255 };
            textColorWhite = textColor;
            SDL_Color textColor2 = { 0, 0, 0 };
            textColorBlack = textColor2;
           
            //load bitmap textures
            textures_loaded = false;
            load_textures();
            
            //rescale bitmap textures
            textures_rescaled = false;
            rescale_textures();
            
            
            //map surface initialize
            map_surface = SDL_CreateRGBSurface(0, real_map_pixel_size, real_map_pixel_size, SCREEN_BPP, 0, 0, 0, 0);
            update_map();
}


//return map_graphics surface address
SDL_Surface* map_graphics::get_map_surface(){
             return map_surface;
}


//update map surface
bool map_graphics::update_map(){
     if(!textures_loaded || !textures_rescaled) return false;
     
     //offset rectangle
     SDL_Rect offset;
     offset.x = 0;
     offset.y = 0;
     offset.w = block_pixel_size;
     offset.h = block_pixel_size;
     
     //loop to fill map_surface like a grid     
     for(int i=0; i<map_size; i++){
         for(int j=0; j<map_size; j++){
             SDL_BlitSurface( get_pos_texture(j,i), NULL, map_surface, &offset );
             offset.x += block_pixel_size;
         }
         offset.x = 0;
         offset.y += block_pixel_size;
     }
     
     if(map_surface == NULL) return false;
     else return true;     
}



SDL_Surface* map_graphics::get_pos_texture(int x, int y){
         if(!textures_loaded) return NULL;
         
         switch(map::get_block_type(x,y)){
                     case 0:
                           if(x == 0)  written_tx = write_on_tx(border_tx, written_tx, NumToStr(y), textColorWhite);
                           else if(y == 0)  written_tx = write_on_tx(border_tx, written_tx, NumToStr(x), textColorWhite);
                           else return border_tx;
                           
                           if(written_tx == NULL) return border_tx;
                           return written_tx;
                           break;
                     case 1:
                           if(map::get_base_id() == 0) return base_empty_tx;
                           else return base_crowed_tx;                                                
                           break;
                     case 2:
                           if(map::has_flag(x,y))
                                 switch(map::get_vehicle_type(x,y)){
                                        case 0:
                                             return f_mine_tx;
                                             break;
                                        case 1:
                                             if(vehicle::get_vehicle_address(map::get_id(x,y))->get_damaged()){
                                                   written_tx = write_on_tx(scout_on_f_mine_dead_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return scout_on_f_mine_dead_tx;
                                                   else return written_tx;
                                                   }
                                             else {
                                                   written_tx = write_on_tx(scout_on_f_mine_alive_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return scout_on_f_mine_alive_tx;
                                                   else return written_tx;
                                                   }
                                             break;
                                        case 2:
                                             if(vehicle::get_vehicle_address(map::get_id(x,y))->get_damaged()){
                                                   written_tx = write_on_tx(miner_on_f_mine_dead_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return miner_on_f_mine_dead_tx;
                                                   else return written_tx;
                                                   }
                                             else {
                                                   written_tx = write_on_tx(miner_on_f_mine_alive_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return miner_on_f_mine_alive_tx;
                                                   else return written_tx;
                                                   }
                                             break;
                                        case 3:
                                             if(vehicle::get_vehicle_address(map::get_id(x,y))->get_damaged()){
                                                   written_tx = write_on_tx(rescuer_on_f_mine_dead_tx, written_tx, NumToStr(map::get_id(x,y)), textColorBlack);
                                                   if(written_tx == NULL) return rescuer_on_f_mine_dead_tx;
                                                   else return written_tx;
                                                   }
                                             else {
                                                   written_tx = write_on_tx(rescuer_on_f_mine_alive_tx, written_tx, NumToStr(map::get_id(x,y)), textColorBlack);
                                                   if(written_tx == NULL) return rescuer_on_f_mine_alive_tx;
                                                   else return written_tx;
                                                   }
                                             break;
                                        default:
                                             return border_tx;
                                 }           
                           else
                                 switch(map::get_vehicle_type(x,y)){
                                        case 0:
                                             return mine_tx;
                                             break;
                                        case 1:
                                             if(vehicle::get_vehicle_address(map::get_id(x,y))->get_damaged()){
                                                   written_tx = write_on_tx(scout_on_mine_dead_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return scout_on_mine_dead_tx;
                                                   else return written_tx;
                                                   }
                                             else {
                                                   written_tx = write_on_tx(scout_on_mine_alive_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return scout_on_mine_alive_tx;
                                                   else return written_tx;
                                                   }
                                             break;
                                        case 2:
                                             if(vehicle::get_vehicle_address(map::get_id(x,y))->get_damaged()){
                                                   written_tx = write_on_tx(miner_on_mine_dead_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return miner_on_mine_dead_tx;
                                                   else return written_tx;
                                                   }
                                             else {
                                                   written_tx = write_on_tx(miner_on_mine_alive_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return miner_on_mine_alive_tx;
                                                   else return written_tx;
                                                   }
                                             break;
                                        case 3:
                                             if(vehicle::get_vehicle_address(map::get_id(x,y))->get_damaged()){
                                                   written_tx = write_on_tx(rescuer_on_mine_dead_tx, written_tx, NumToStr(map::get_id(x,y)), textColorBlack);
                                                   if(written_tx == NULL) return rescuer_on_mine_dead_tx;
                                                   else return written_tx;
                                                   }
                                             else {
                                                   written_tx = write_on_tx(rescuer_on_mine_alive_tx, written_tx, NumToStr(map::get_id(x,y)), textColorBlack);
                                                   if(written_tx == NULL) return rescuer_on_mine_alive_tx;
                                                   else return written_tx;
                                                   }
                                             break;
                                        default:
                                             return border_tx;
                                 } 
                           break;
                     case 3:
                           switch(map::get_vehicle_type(x,y)){
                                        case 0:
                                             return road_tx;
                                             break;
                                        case 1:
                                             if(vehicle::get_vehicle_address(map::get_id(x,y))->get_damaged()){
                                                   written_tx = write_on_tx(scout_on_road_dead_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return scout_on_road_dead_tx;
                                                   else return written_tx;
                                                   }
                                             else {
                                                   written_tx = write_on_tx(scout_on_road_alive_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return scout_on_road_alive_tx;
                                                   else return written_tx;
                                                   }
                                             break;
                                        case 2:
                                             if(vehicle::get_vehicle_address(map::get_id(x,y))->get_damaged()){
                                                   written_tx = write_on_tx(miner_on_road_dead_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return miner_on_road_dead_tx;
                                                   else return written_tx;
                                                   }
                                             else {
                                                   written_tx = write_on_tx(miner_on_road_alive_tx, written_tx, NumToStr(map::get_id(x,y)), textColorWhite);
                                                   if(written_tx == NULL) return miner_on_road_alive_tx;
                                                   else return written_tx;
                                                   }
                                             break;
                                        case 3:
                                             if(vehicle::get_vehicle_address(map::get_id(x,y))->get_damaged()){
                                                   written_tx = write_on_tx(rescuer_on_road_dead_tx, written_tx, NumToStr(map::get_id(x,y)), textColorBlack);
                                                   if(written_tx == NULL) return rescuer_on_road_dead_tx;
                                                   else return written_tx;
                                                   }
                                             else {
                                                   written_tx = write_on_tx(rescuer_on_road_alive_tx, written_tx, NumToStr(map::get_id(x,y)), textColorBlack);
                                                   if(written_tx == NULL) return rescuer_on_road_alive_tx;
                                                   else return written_tx;
                                                   }
                                             break;
                                        default:
                                             return border_tx;
                           }   
                           break;
                     default: 
                           return border_tx;
         }

}

SDL_Surface* map_graphics::write_on_tx(SDL_Surface* source, SDL_Surface* destination, std::string str, SDL_Color textColor){
             if(source == NULL) { return NULL; }
             if(font == NULL) { return NULL; }
             if(destination == NULL) { return NULL; }
             
            //render text
             SDL_Surface* msg = TTF_RenderText_Solid( font, str.c_str(), textColor );
             
             //center msg
             int w,h; //width and height of text
             TTF_SizeText(font, str.c_str(), &w, &h);
             SDL_Rect temp_rect;
             temp_rect.x = (block_pixel_size - w)/2;
             temp_rect.y = (block_pixel_size - h)/2;
             
             //put text on temporary texture
             SDL_BlitSurface( source, NULL, destination, NULL );
             SDL_BlitSurface( msg, NULL, destination, &temp_rect );
             
             //free surface
             SDL_FreeSurface( msg );
             
             return destination; //return is not needed but return NULL is needed
}
                          
            
   

//load map textures from bitmap images in textures folder
bool map_graphics::load_textures(){
     
     //in case load fails user rectangle
     SDL_Rect temp_rect;
     temp_rect.w = 50;
     temp_rect.h = 50;
     temp_rect.x = 0;
     temp_rect.y = 0;
     
     load_tx( &base_empty_tx, "textures/base_empty.bmp", &temp_rect);
     load_tx( &base_crowed_tx, "textures/base_crowed.bmp", &temp_rect);
     load_tx( &road_tx, "textures/road.bmp", &temp_rect);
     load_tx( &border_tx, "textures/border.bmp", &temp_rect);
     load_tx( &mine_tx, "textures/mine.bmp", &temp_rect);
     load_tx( &f_mine_tx, "textures/f_mine.bmp", &temp_rect);
     load_tx( &miner_on_road_alive_tx, "textures/miner_on_road_alive.bmp", &temp_rect);
     load_tx( &miner_on_mine_alive_tx, "textures/miner_on_mine_alive.bmp", &temp_rect);
     load_tx( &miner_on_f_mine_alive_tx, "textures/miner_on_f_mine_alive.bmp", &temp_rect);
     load_tx( &scout_on_road_alive_tx, "textures/scout_on_road_alive.bmp", &temp_rect);
     load_tx( &scout_on_mine_alive_tx, "textures/scout_on_mine_alive.bmp", &temp_rect);
     load_tx( &scout_on_f_mine_alive_tx, "textures/scout_on_f_mine_alive.bmp", &temp_rect);
     load_tx( &rescuer_on_road_alive_tx, "textures/rescuer_on_road_alive.bmp", &temp_rect);
     load_tx( &rescuer_on_mine_alive_tx, "textures/rescuer_on_mine_alive.bmp", &temp_rect);
     load_tx( &rescuer_on_f_mine_alive_tx, "textures/rescuer_on_f_mine_alive.bmp", &temp_rect);
     load_tx( &miner_on_road_dead_tx, "textures/miner_on_road_dead.bmp", &temp_rect);
     load_tx( &miner_on_mine_dead_tx, "textures/miner_on_mine_dead.bmp", &temp_rect);
     load_tx( &miner_on_f_mine_dead_tx, "textures/miner_on_f_mine_dead.bmp", &temp_rect);
     load_tx( &scout_on_road_dead_tx, "textures/scout_on_road_dead.bmp", &temp_rect);
     load_tx( &scout_on_mine_dead_tx, "textures/scout_on_mine_dead.bmp", &temp_rect);
     load_tx( &scout_on_f_mine_dead_tx, "textures/scout_on_f_mine_dead.bmp", &temp_rect);
     load_tx( &rescuer_on_road_dead_tx, "textures/rescuer_on_road_dead.bmp", &temp_rect);
     load_tx( &rescuer_on_mine_dead_tx, "textures/rescuer_on_mine_dead.bmp", &temp_rect);
     load_tx( &rescuer_on_f_mine_dead_tx, "textures/rescuer_on_f_mine_dead.bmp", &temp_rect);     
     
     written_tx = SDL_CreateRGBSurface(0, 50, 50, SCREEN_BPP, 0, 0, 0, 0); //initialize written
     
     textures_loaded = true;
     return true;
}

//help function for load_textures
bool map_graphics::load_tx(SDL_Surface** surface, std::string filename, SDL_Rect* rect){
     *surface = SDL_LoadBMP( filename.c_str() );
     if(*surface == NULL) {
          *surface = SDL_CreateRGBSurface(0, 50, 50, SCREEN_BPP, 0, 0, 0, 0);
          SDL_FillRect(*surface, rect, SDL_MapRGB((*surface)->format, 255, 255, 255));
          return false;
     }
     else return true;
}
     




//rescale loaded textures to block_pixel_size
bool map_graphics::rescale_textures(){
     if (!textures_loaded) load_textures();
     
     rescale_tx(&road_tx);
     rescale_tx(&base_empty_tx);
     rescale_tx(&base_crowed_tx);
     rescale_tx(&border_tx);
     rescale_tx(&mine_tx);
     rescale_tx(&f_mine_tx);
     rescale_tx(&miner_on_road_alive_tx);
     rescale_tx(&miner_on_mine_alive_tx);
     rescale_tx(&miner_on_f_mine_alive_tx);
     rescale_tx(&scout_on_road_alive_tx);
     rescale_tx(&scout_on_mine_alive_tx);
     rescale_tx(&scout_on_f_mine_alive_tx);
     rescale_tx(&rescuer_on_road_alive_tx);
     rescale_tx(&rescuer_on_mine_alive_tx);
     rescale_tx(&rescuer_on_f_mine_alive_tx);
     rescale_tx(&miner_on_road_dead_tx);
     rescale_tx(&miner_on_mine_dead_tx);
     rescale_tx(&miner_on_f_mine_dead_tx);
     rescale_tx(&scout_on_road_dead_tx);
     rescale_tx(&scout_on_mine_dead_tx);
     rescale_tx(&scout_on_f_mine_dead_tx);
     rescale_tx(&rescuer_on_road_dead_tx);
     rescale_tx(&rescuer_on_mine_dead_tx);
     rescale_tx(&rescuer_on_f_mine_dead_tx);

     rescale_tx(&written_tx);
     
     textures_rescaled = true; //doesn't matter if some didn't rescale, they'll just appear half
     return true;
}

bool map_graphics::rescale_tx(SDL_Surface** surface){
     //temporary surface
     SDL_Surface* temp_surf = ScaleSurface(*surface, block_pixel_size, block_pixel_size);
     if (temp_surf == NULL) {
          SDL_FreeSurface( temp_surf );
          return false;
     }
     else {
          SDL_FreeSurface( *surface ); //free loaded surface
          *surface = temp_surf; //set to rescaled surface
          return true;
     }
}




void map_graphics::clean_up(){
     
     //unload bitmap textures
     SDL_FreeSurface( base_empty_tx );
     SDL_FreeSurface( base_crowed_tx );
     SDL_FreeSurface( border_tx );
     SDL_FreeSurface( mine_tx );
     SDL_FreeSurface( f_mine_tx );
     SDL_FreeSurface( miner_on_road_alive_tx );
     SDL_FreeSurface( miner_on_mine_alive_tx );
     SDL_FreeSurface( miner_on_f_mine_alive_tx );
     SDL_FreeSurface( scout_on_road_alive_tx );
     SDL_FreeSurface( scout_on_mine_alive_tx );
     SDL_FreeSurface( scout_on_f_mine_alive_tx );
     SDL_FreeSurface( rescuer_on_road_alive_tx );
     SDL_FreeSurface( rescuer_on_mine_alive_tx );
     SDL_FreeSurface( rescuer_on_f_mine_alive_tx );
     SDL_FreeSurface( miner_on_road_dead_tx );
     SDL_FreeSurface( miner_on_mine_dead_tx );
     SDL_FreeSurface( miner_on_f_mine_dead_tx );
     SDL_FreeSurface( scout_on_road_dead_tx );
     SDL_FreeSurface( scout_on_mine_dead_tx );
     SDL_FreeSurface( scout_on_f_mine_dead_tx );
     SDL_FreeSurface( rescuer_on_road_dead_tx );
     SDL_FreeSurface( rescuer_on_mine_dead_tx );
     SDL_FreeSurface( rescuer_on_f_mine_dead_tx );
     SDL_FreeSurface( written_tx );
     
     SDL_FreeSurface( map_surface );
     //close font
     TTF_CloseFont( font );
     
}













SDL_Surface* map_graphics::ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
    if(!Surface || !Width || !Height)
        return NULL;
     
    SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
        Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);
 
    double    _stretch_factor_x = (static_cast<double>(Width)  / static_cast<double>(Surface->w)),
        _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(Surface->h));
 
    for(Sint32 y = 0; y < Surface->h; y++)
        for(Sint32 x = 0; x < Surface->w; x++)
            for(Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y)
                for(Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x)
                    DrawPixel(_ret, static_cast<Sint32>(_stretch_factor_x * x) + o_x, 
                        static_cast<Sint32>(_stretch_factor_y * y) + o_y, ReadPixel(Surface, x, y));
 
    return _ret;
}


Uint32 map_graphics::ReadPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void map_graphics::DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}


std::string map_graphics::NumToStr ( int Number )
{
     std::ostringstream ss;
     ss << Number;
     return ss.str();
}
