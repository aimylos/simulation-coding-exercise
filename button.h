#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <string>

#ifndef BUTTON_H
#define BUTTON_H


class button{
      private:
              //button size and offset
              SDL_Rect box;

              //was pressed
              bool pressed;
              bool clicked;
              
              //button surface
              SDL_Surface* button_surface;
              
              
              
      public:
             //emptry button constructor 
             button(int x, int y, int w, int h);
             
             //constructor
             button( int x, int y, int w, int h, std::string name ); //!!!(x,y) positon of button on the screen!!!, w:width, h:height
             
             //destructor
             ~button();
             
             //handle event
             void handle_events(SDL_Event* event);
             
             //check if button was pressed
             bool was_clicked(); //returns to false when called once
             
             //check if button was selected
             bool was_selected(); //clicked and no other clicks occurred
             
             //get button surface
             SDL_Surface* get_button_surface();
             
             //get button rectangle
             SDL_Rect* get_button_rect();
             
      
};

#endif
