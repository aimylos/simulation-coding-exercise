#include "button.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <string>


#define SCREEN_BPP 32

//constructor
button::button( int x, int y, int w, int h, std::string name ){
        //set box size
        box.w = w;
        box.h = h;
        box.x = 0;
        box.y = 0;
        
        //make a white surface for button
        button_surface = SDL_CreateRGBSurface(0, w, h, SCREEN_BPP, 0, 0, 0, 0);
        SDL_FillRect(button_surface, &box, SDL_MapRGB((button_surface)->format, 255, 255, 255));
        
        //set box offset
        box.x = x;
        box.y = y;
                
        //load font, font size is based on 72DPI <=> 1pixel == 1point
        TTF_Font* font = TTF_OpenFont( "arial.ttf", w/name.length()+5 ); //if didn't load then no fonts will appear
            
        //set font color
        SDL_Color textColor = { 0, 0, 0 };
        
        //write str on message
        SDL_Surface* msg = TTF_RenderText_Solid( font, name.c_str(), textColor );
        
        //center msg
        int tw=0,th=0; //width and height of text
        TTF_SizeText(font, name.c_str(), &tw, &th);
        SDL_Rect temp_rect;
        temp_rect.x = (w - tw)/2;
        temp_rect.y = (h - th)/2;
        
        //put msg on surface
        if(msg != NULL) SDL_BlitSurface( msg, NULL, button_surface, &temp_rect );
        
        //free msg surface
        SDL_FreeSurface( msg );
        
        //close font
        TTF_CloseFont( font );   
        
        //set pressed to false 
        pressed = false; 
        clicked = false;    
}


//empty constructor
button::button(int x, int y, int w, int h){
        //set box size and offset
        box.w = w;
        box.h = h;
        box.x = x;
        box.y = y;
                
        //set pressed to false 
        pressed = false; 
        clicked = false;  
        
        //NULL button surface
        button_surface = NULL;
}


//destructor
button::~button(){
        SDL_FreeSurface( button_surface );
}

//get functions
bool button::was_clicked(){
     if(clicked) {clicked = false; return true;}
     else return false;
}

bool button::was_selected(){
     return clicked;
}
             
SDL_Surface* button::get_button_surface(){
             return button_surface;
}
SDL_Rect* button::get_button_rect(){
          return &box;
}


//handle event
void button::handle_events(SDL_Event* event){
     
     //The mouse offsets
     int x = 0, y = 0;

     //If a mouse button was pressed and mouse button was left
     if( event->type == SDL_MOUSEBUTTONDOWN  &&  event->button.button == SDL_BUTTON_LEFT){
            //Get the mouse offsets
            x = event->button.x;
            y = event->button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set button pressed
                pressed = true;
            }
            else {pressed = false; clicked = false;}
     }
     
     //If a mouse button was released and mouse button was left
     if( event->type == SDL_MOUSEBUTTONUP  &&  event->button.button == SDL_BUTTON_LEFT){
            //Get the mouse offsets
            x = event->button.x;
            y = event->button.y;

            //If the mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set button pressed
                if(pressed) clicked = true;
                else {pressed = false; clicked = false;}
            }
            else {pressed = false; clicked = false;}
     }
}




