#include "button.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <string>

#ifndef INPUT_H
#define INPUT_H



class input : public button{
      
      //subclass of button
      //enable input only when textfield was clicked
      //else ingore input
      
      private:
              //input size -- offset from button SDL_Rect* get_button_rect();
              SDL_Rect input_rect; //x,y=0
              
              //input field selected - bool was_clicked();
              
              //input surface
              SDL_Surface* input_surface;
              
              //storage string
              std::string str;
              
              //font and color
              TTF_Font* font;
              SDL_Color textColorBlack;
              
              //got input boolean
              bool got_input_var;
              
              
      public:
             //constructor
             input( int x, int y, int w, int h );
             
             //destructor
             ~input();
             
             //clear input
             void clear_input();
             
             //clear input string
             void clear_input_string();
             
             //handle event
             void handle_events(SDL_Event* event);
             
             //get button surface
             SDL_Surface* get_input_surface();
             
             //get input string
             std::string get_input_string();
             
             //get input to int if string is numbers and return true else return false and leave num as is
             bool get_input_int( int* const num );
             
             //get input rectangle
             SDL_Rect* get_input_rect();
             
             //check if input changed
             bool got_input(); //if called once then goes to false
};

#endif
