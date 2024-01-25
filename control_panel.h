#include "map.h"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "vehicle.h"
#include "scout.h"
#include "miner.h"
#include "rescuer.h"
#include "button.h"
#include "input.h"
#include "secondary_panel.h"
#include <string>


#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

class control_panel {
      //prints derectly on main screen
      private:
                           
              //font and color
              TTF_Font* font;
              TTF_Font* header_font;
              SDL_Color textColorBlack;
              
              //write on control panel panel
              void write_on_panel(std::string str, int x, int y, TTF_Font* f);
              
              //top offset, y axis
              int top_offset; //until last write height
              
              //control panel surface
              SDL_Surface* control_panel_surface;
              
              //offset rectangle
              SDL_Rect offset_rect;
                            
              //simulation map
              map* sim_map;
              
              //simulation speed address
              int* sim_speed;
              
              //vehicle list
              list<vehicle*> *vehicle_list;
              
              //restart parameters
              int new_map_size;
              int new_num_miners;
              int new_num_scouts;
              int new_num_rescuers;
              
              //vehicle id and map position
              int vehicle_id;
              int map_x, map_y;
              
              //secondary panel to control
              secondary_panel* secd_panel;
              
              //toggle variables
              bool simulation_paused;
              bool simulation_quit;
              bool simulation_restart;
              bool simulation_surrender;
              
              //control panel state
              int control_panel_state; //-1:CLEARED SCREEN
                                       //0:MAIN MENU
                                       //1:PAUSED MENU
                                       //2:RESTART MENU
                                       //3:WIN/LOSE MENU
                                       //4:VEHICLE MENU
                                       //5:MAP POSITION MENU
              
              //buttons and inputs, some menu share buttons
              //main menu
              button* button_pause;
              button* button_surrender;
              button* button_restart;
              button* button_quit;
              button* button_inc_sim_speed;
              button* button_dec_sim_speed;
              //pause menu
              button* button_resume;
              input* input_id;
              button* button_vehicle;
              input* input_pos_x;
              input* input_pos_y;
              button* button_pos;
              button* button_add_miner;
              button* button_add_scout;
              button* button_add_rescuer;
              //restart menu
              input* input_map_size;
              input* input_miners;
              input* input_scouts;
              input* input_rescuers;
              //vehicle menu
              button* button_kill_vehicle;
              button* button_revive_vehicle;
              button* button_damage_vehicle;
              button* button_repair_vehicle;
              button* button_move_to_pos;
              button* button_set_speed;
              button* button_set_access;
              input* input_move_x;
              input* input_move_y;
              input* input_speed;
              input* input_access;
              button* button_back;
              //map position menu
              button* button_set_danger;
              button* button_set_flag;
              button* button_clear_flag;
              button* button_set_resources;
              button* button_set_goal;
              button* button_set_unload;
              input* input_danger;
              input* input_pa;
              input* input_ir;
              input* input_pl;
              
              
              
      public:
             //contructor, offset rectangle is needed for buttons and inputs
             control_panel(SDL_Rect offset, secondary_panel* sp, map* m, list<vehicle*> *v_list);
             
             //get control panel surface
             SDL_Surface* get_control_panel_surface();
             
             //handle events
             void handle_events(SDL_Event* event);
             
             //clear control panel area on screen
             void clear_control_panel(); //also sets screen cleared
             
             //set main menu
             void set_main_menu();
             
             //set paused menu
             void set_paused_menu();
             
             //set restart menu
             void set_restart_menu();
             
             //set win/lose menu
             void set_end_menu();
             
             //set vehicle menu
             void set_vehicle_menu();
             
             //set map position menu
             void set_map_pos_menu();
             
             //clean up
             void clean_up();
             
             //set sim speed pointer            
             void set_sim_speed_address(int* s);
                  
             //get toggle variables
             bool user_paused();
             bool user_quit();
             bool user_restart();
             bool user_surrender();
             
             //get restart parameters, map size, number of miners, number of scouts and number of rescuers
             void get_simulaiton_restart(int* m_s, int* n_m, int* n_s, int* n_r);
             
             //get control panel state
             int get_control_panel_state();
             
};

#endif
