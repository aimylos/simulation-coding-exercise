#include "input.h"
#include "button.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>

#define SCREEN_BPP 32
#define MAX_INPUT_SIZE 10


input::input( int x, int y, int w, int h ) : button(x,y,w,h){
              
        //temp rectanlge for filling
        input_rect.w = w;
        input_rect.h = h;
        input_rect.x = 0;
        input_rect.y = 0;
        
        //make input surface
        input_surface = SDL_CreateRGBSurface(0, w, h, SCREEN_BPP, 0, 0, 0, 0);
        clear_input();
                
        //load font, font size is based on 72DPI <=> 1pixel == 1point
        font = TTF_OpenFont( "fonts/arial.ttf", h-5 ); //if didn't load then no fonts will appear
            
        //set font color
        SDL_Color textColor = { 0, 0, 0 };
        textColorBlack = textColor;
        
        //Initialize string
        str = "";
    
        //Enable Unicode
        SDL_EnableUNICODE( SDL_ENABLE ); 
        
        //initialze 
        got_input_var = false;
}

//destructor
input::~input(){
        //call button destructor
        //button::~button();               

        //close font
        TTF_CloseFont( font );
        
        //free input surface
        SDL_FreeSurface( input_surface );
        
        //Disable Unicode
        SDL_EnableUNICODE( SDL_DISABLE ); 
}

//clear input
void input::clear_input(){
     if(input_surface == NULL) return;
     //make a white surface with black borders for input
     SDL_FillRect(input_surface, &input_rect, SDL_MapRGB((input_surface)->format, 255, 33, 33));
     SDL_Rect temp_rect;
     temp_rect.w = input_rect.w-2;
     temp_rect.h = input_rect.h-2;
     temp_rect.x = 1;
     temp_rect.y = 1;
     SDL_FillRect(input_surface, &temp_rect, SDL_MapRGB((input_surface)->format, 255, 255, 255));
}
//clear input string
void input::clear_input_string(){
     str.clear();
     clear_input();
}

//get input surface
SDL_Surface* input::get_input_surface(){
             return input_surface;
}

//get input string
std::string input::get_input_string(){
            return str;
}

SDL_Rect* input::get_input_rect(){
          return button::get_button_rect();
}

bool input::got_input(){
     if(got_input_var) {got_input_var=false; return true;}
     else return false;
}

//handle event
void input::handle_events(SDL_Event* event){
     //call button event handling
     button::handle_events(event);
     
     //if input field was selected and a key was pressed
     if( button::was_selected() && event->type == SDL_KEYDOWN ){
        //keep a copy of the current version of the string
        std::string temp = str;
        
        //if the string less than maximum size
        if( str.length() <= MAX_INPUT_SIZE ){
            //if the key is a space
            if( event->key.keysym.unicode == (Uint16)' ' )
            {
                //append the character
                str += (char)event->key.keysym.unicode;    
            }
            //if the key is a number
            else if( ( event->key.keysym.unicode >= (Uint16)'0' ) && ( event->key.keysym.unicode <= (Uint16)'9' ) )
            {
                //append the character
                str += (char)event->key.keysym.unicode;
            }
            //if the key is a uppercase letter
            else if( ( event->key.keysym.unicode >= (Uint16)'A' ) && ( event->key.keysym.unicode <= (Uint16)'Z' ) )
            {
                //append the character
                str += (char)event->key.keysym.unicode;
            }
            //if the key is a lowercase letter
            else if( ( event->key.keysym.unicode >= (Uint16)'a' ) && ( event->key.keysym.unicode <= (Uint16)'z' ) )
            {
                //append the character
                str += (char)event->key.keysym.unicode;
            }
        }
        //if backspace was pressed and the string isn't blank
        if( ( event->key.keysym.sym == SDLK_BACKSPACE ) && ( str.length() != 0 ) )
        {
            //Remove a character from the end
            str.erase( str.length() - 1 );
        }
        //if the string was changed
        if( str != temp )
        {
            //set got input
            got_input_var=true;
            
            //clear old input
            clear_input();
            
            //write str on message
            SDL_Surface* msg = TTF_RenderText_Solid( font, str.c_str(), textColorBlack );
        
            //center msg
            int tw=0,th=0; //width and height of text
            TTF_SizeText(font, str.c_str(), &tw, &th);
            SDL_Rect temp_rect;
            temp_rect.x = (input_rect.w - tw)/2;
            temp_rect.y = (input_rect.h - th)/2;
        
            //put msg on surface
            if(msg != NULL) SDL_BlitSurface( msg, NULL, input_surface, &temp_rect );
        
            //free msg surface
            SDL_FreeSurface( msg );
        }
    }
}

bool input::get_input_int( int* const num ){
     // stringstream used for the conversion initialized with the contents of Text
     std::stringstream convert(str); 
     int result;
     //give the value to result using the characters in the string
     if ( !(convert >> result) ) return false;
     else {
          *num = result;
          return true;
          }
}
