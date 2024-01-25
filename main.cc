//to linker   -lmingw32 -lSDLmain -lSDL -lSDL_ttf

#include "block.h"
#include "map.h"
#include "map_graphics.h"
#include "map_general_panel.h"
#include "control_panel.h"
#include "secondary_panel.h"
#include "vehicle.h"
#include "scout.h"
#include "miner.h"
#include "rescuer.h"
#include "button.h"
#include "input.h"


#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;
#include <iostream>
using namespace std;
#include <list>
using std::list;


//The attributes of the screen
#define SCREEN_WIDTH 1400
#define SCREEN_HEIGHT 900
//map attributes
#define MAP_OFFSET_X 15
#define MAP_OFFSET_Y 15
//MAP_SIZE is already defined by map_graphics and it's 600pixel square
//general panel attributes
#define GENERAL_PANEL_OFFSET_X 650
#define GENERAL_PANEL_OFFSET_Y 25
//GENERAL_PANEL_SIZE is already define by map_general_panel and it's 700x350
//control panel attributes
#define CONTROL_PANEL_OFFSET_X 15
#define CONTROL_PANEL_OFFSET_Y 625
//CONTROL_PANEL_SIZE is already define by control_panel and it's 600x260
//secondary panel attributes
#define SECONDARY_PANEL_OFFSET_X 650
#define SECONDARY_PANEL_OFFSET_Y 400
//SECONDARY_PANEL_SIZE is already define by secondary_panel and it's 700x400

#define SCREEN_BPP 32


//global variables
//screen
SDL_Surface* screen = NULL;
//event handling
SDL_Event event;
//map
map_graphics* A;
SDL_Rect map_rect;
SDL_Surface* map_surface; //cleared by map_graphics.clean_up()
//general panel
map_general_panel* general_panel;
SDL_Rect general_panel_rect; //for offset
SDL_Surface* general_panel_surface; //cleared by map_general_panel.clean_up()
//secondary panel
secondary_panel* secondary_panel_;
SDL_Rect secondary_panel_rect; //for offset
SDL_Surface* secondary_panel_surface; //cleared by secondary_panel.clean_up()
//control panel
control_panel* control_panel_;
SDL_Rect control_panel_rect;
SDL_Surface* control_panel_surface;
//round counter
int round;
//simulation speed in milliseconds
int sim_speed;
//vehicle list
list<vehicle*> V;
//int Vsize;


//functions
bool init_graphics(); //all graphics related initialization
bool init_simulation(int map_size, int num_miners, int num_scouts, int num_rescuers); //all simulation related initialization
bool init_general_panel(); //panels used for user interaction
bool init_secondary_panel();
bool init_control_panel();
void clean_up(); //cleans up everything
void vehicle_list_move(); //moves all vehicles
void vehicle_list_kill(); //kills all vehicles


//main
int main( int argc, char* args[] ) {
    int size_of_map = 20;
    int number_of_miners = 8;
    int number_of_scouts = 8;
    int number_of_rescuers = 8;
    bool quit_program = false;
    
    //main program loop
    while(!quit_program){
    
    //quit, pause, restart booleans
    bool quit = false;
    bool paused = false;
    bool restart = false;
    
    //ORDER MATTERS  -  initialize graphics, simulation, general panel
    if( !init_graphics() ) return 1;
    if( !init_simulation(size_of_map, number_of_miners, number_of_scouts, number_of_rescuers) ) return 1;
    if( !init_general_panel() ) return 1;
    if( !init_secondary_panel() ) return 1;
    if( !init_control_panel() ) return 1;
    
    //first round
    //put map and panels on screen
    SDL_BlitSurface( general_panel_surface, NULL, screen, &general_panel_rect);
    SDL_BlitSurface( control_panel_surface, NULL, screen, &control_panel_rect);
    SDL_BlitSurface( secondary_panel_surface, NULL, screen, &secondary_panel_rect);
    SDL_BlitSurface( map_surface, NULL, screen, &map_rect );
    //Update Screen
    SDL_Flip( screen );
    //Pause
    SDL_Delay( 400 );


    
    //main simulation loop
    while(!quit){
        
          if(!paused){
          //RUN SIMULATION ROUND
          //increase round, round shows current round
          round++;
          //move vehicles
          vehicle_list_move();
          //update map and panels
          A->update_map();
          general_panel->update_general_panel();
          secondary_panel_->update_secondary_panel();
          //put map and panels on screen
          SDL_BlitSurface( map_surface, NULL, screen, &map_rect );
          SDL_BlitSurface( general_panel_surface, NULL, screen, &general_panel_rect);
          SDL_BlitSurface( secondary_panel_surface, NULL, screen, &secondary_panel_rect);
          //Update Screen
          SDL_Flip( screen );
          //Pause
          SDL_Delay( sim_speed );
          }
          
          //check simulation status
          switch(A->simulation_status_on_map()){
                  case 0:
                  case 1: //ongoing
                       break;
                  case 2:
                  case -1:
                  case 3: //failed or success
                       if(control_panel_->get_control_panel_state()!=2) control_panel_->set_end_menu();
                       SDL_BlitSurface( control_panel_surface, NULL, screen, &control_panel_rect);
                       SDL_Flip( screen );
                       paused = true;
                       break;
                  default:
                       control_panel_->set_restart_menu();
                       paused = true;
          }
          //since rescuers don't go on mines, they can't die and scouts are useless on their own
          if(vehicle::get_num_alive_miners() == 0) vehicle_list_kill();

          //check events
          while(SDL_PollEvent( &event )){
                  
                  //Quit the program
                  if( event.type == SDL_QUIT ) {quit = true; break;}
                  else control_panel_->handle_events(&event);
                        
                  //end simulation results
                  if(control_panel_->user_quit()) {quit = true; break;}
                  if(control_panel_->user_restart()) {quit = true; restart = true; break;}
                  //pause simulation results
                  if(control_panel_->user_paused()) {paused = true;}
                  else if(control_panel_->user_surrender()) {vehicle_list_kill();}
                  else {paused = false;}
                  
                  SDL_BlitSurface( control_panel_surface, NULL, screen, &control_panel_rect);
                  general_panel->update_general_panel();
                  SDL_BlitSurface( general_panel_surface, NULL, screen, &general_panel_rect);
                  secondary_panel_->update_secondary_panel();
                  SDL_BlitSurface( secondary_panel_surface, NULL, screen, &secondary_panel_rect);
                  A->update_map();
                  SDL_BlitSurface( map_surface, NULL, screen, &map_rect );
                  SDL_Flip( screen );
          }
       }//end of main simulation loop

       //check for restart   
       if(restart) {
                   control_panel_->get_simulaiton_restart(&size_of_map, &number_of_miners, &number_of_scouts, &number_of_rescuers);
                   }
       else quit_program = true;
    
       //Pause
       SDL_Delay( 100 );

       //clear memory
       clean_up();
    
       }//end of program
       return 0; 
}
//end of main

//initialize graphics
bool init_graphics(){
     //Initialize all SDL subsystems
     if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) return false;

     //Set up screen
     screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
     //check for errors
     if( screen == NULL ) return false;

     //Set the window caption
     SDL_WM_SetCaption( "Mining Simulation", NULL );
     
     //Initialize SDL_ttf
     if( TTF_Init() == -1 ) return false;
     
     return true;
}

//initialize simulation  
bool init_simulation(int map_size, int num_miners, int num_scouts, int num_rescuers){
     srand(time(NULL)); //must seed random generator
     
     //initialize round
     round = 1;
     
     //set simulation speed
     sim_speed = 300;
     
     //create a map
     A = new map_graphics(map_size,-1,-1, &round); //size = 20, random coordinates for base
     if(A == NULL) return false;
     //get map surface
     map_surface = A->get_map_surface();
     
     //create vehicles and add to list V
     /*Vsize=0;*/
     for(int i=0; i<num_scouts; i++) {V.push_back(dynamic_cast<vehicle*>(new scout())); /*Vsize++;*/}
     for(int i=0; i<num_miners; i++) {V.push_back(dynamic_cast<vehicle*>(new miner())); /*Vsize++;*/}
     for(int i=0; i<num_rescuers; i++) {V.push_back(dynamic_cast<vehicle*>(new rescuer())); /*Vsize++;*/}

     
     //give map address to vehicles and put vehicles in map
     //entering position variables
     int dir_x[8]={1, 1, 0, -1, -1, -1,  0,  1};
     int dir_y[8]={0, 1, 1,  1,  0, -1, -1, -1};
     int enter_x, enter_y, k, temp_x, temp_y;
     //list itarator
     list<vehicle*>::iterator itr = V.begin();
     //itarator loop
     for(int i=0; /*i<Vsize &&*/ itr!=V.end(); i++, itr++){
             //give map address
             (*itr)->set_map(dynamic_cast<map*>(A));

             //enter map
             enter_x = (rand() % A->get_size()) + 1; //from 1 to map_size
             enter_y = (rand() % A->get_size()) + 1; 
             temp_x = enter_x;
             temp_y = enter_y;
             if(!(*itr)->enter_map(&round,temp_x,temp_y)){
                   int k;
                   for(k=0;k<8;k++) { //try all 9 positions
                      temp_x = enter_x + dir_x[k];
                      temp_y = enter_y + dir_y[k];
                      if((*itr)->enter_map(&round,temp_x,temp_y)) break;
                   }
                   if(k==8) (*itr)->enter_map(&round,-1,-1); //try all positions
             }//if it doesn't enter, vehicle's function will defensively keep the simulation running 
     }
     //map offset
     map_rect.x = MAP_OFFSET_X;
     map_rect.y = MAP_OFFSET_Y;
     map_rect.w = 0;
     map_rect.h = 0;
     
     A->update_map();
     return true;
}    

//initialize general panel
bool init_general_panel(){
     //create a general panel
     general_panel = new map_general_panel(dynamic_cast<map*>(A));
     
     //offset rect
     general_panel_rect.x = GENERAL_PANEL_OFFSET_X; 
     general_panel_rect.y = GENERAL_PANEL_OFFSET_Y;
     general_panel_rect.w = 0;
     general_panel_rect.h = 0;
     
     //get general panel surface
     general_panel_surface = general_panel->get_general_panel_surface();
     if(general_panel_surface == NULL) return false;
     
     return true;
}

//initialize secondary panel
bool init_secondary_panel(){
     //create a secondary panel
     secondary_panel_ = new secondary_panel(dynamic_cast<map*>(A));
     
     //offset rect
     secondary_panel_rect.x = SECONDARY_PANEL_OFFSET_X;
     secondary_panel_rect.y = SECONDARY_PANEL_OFFSET_Y;
     secondary_panel_rect.w = 0;
     secondary_panel_rect.h = 0;
     
     //get secondary panel surface
     secondary_panel_surface = secondary_panel_->get_secondary_panel_surface();
     if(secondary_panel_surface == NULL) return false;
     
     return true;
}

//initialize control panel
bool init_control_panel(){
     //offset rect
     control_panel_rect.x = CONTROL_PANEL_OFFSET_X;
     control_panel_rect.y = CONTROL_PANEL_OFFSET_Y;
     control_panel_rect.w = 0;
     control_panel_rect.h = 0;
     
     //create a control panel
     control_panel_ = new control_panel(control_panel_rect, secondary_panel_, dynamic_cast<map*>(A), &V);
     
     //set simulation speed address
     control_panel_->set_sim_speed_address(&sim_speed);
     
     //get control panel surface
     control_panel_surface = control_panel_->get_control_panel_surface();
     if(control_panel_surface == NULL) return false;
     
     return true;
}

//clean up everything and quit    
void clean_up(){
     //delete vehicles
     vehicle::reset_vehicle_class();
     miner::reset_miner_class();
     scout::reset_scout_class();
     rescuer::reset_rescuer_class();
     //list itarator
     list<vehicle*>::iterator itr = V.begin();
     //itarator loop
     for(int i=0; /*i<Vsize &&*/ itr!=V.end(); i++, itr++) delete *itr;
     V.clear();
     
     //clear map graphics and delete map
     map::reset_map_class();
     A->clean_up();
     delete A;
     
     //clear general panel
     general_panel->clean_up();
     delete general_panel;
     
     //clear secondary panel
     secondary_panel_->clean_up();
     delete secondary_panel_;
     
     //clear control panel
     control_panel_->clean_up();
     delete control_panel_;
     
     //delete screen
     SDL_FreeSurface( screen );
     
     //Quit SDL and TTF systems
     SDL_Quit();
     TTF_Quit();   
}

//simulation functions
void vehicle_list_move(){
         //list iterator
         list<vehicle*>::iterator itr = V.begin();
         //iterator loop 
         for(int i=0; /*i<Vsize &&*/ itr!=V.end(); i++, itr++) (*itr)->move(); 
}

void vehicle_list_kill(){
         //list iterator
         list<vehicle*>::iterator itr = V.begin();
         //iterator loop 
         for(int i=0; /*i<Vsize &&*/ itr!=V.end(); i++, itr++) (*itr)->set_status(false); 
}
