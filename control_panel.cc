#include "map.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "vehicle.h"
#include "scout.h"
#include "miner.h"
#include "rescuer.h"
#include "button.h"
#include "input.h"
#include "control_panel.h"
#include "secondary_panel.h"
#include <string>

#define CONTROL_PANEL_WIDTH 600
#define CONTROL_PANEL_HEIGHT 260
#define SCREEN_BPP 32

control_panel::control_panel(SDL_Rect offset, secondary_panel* sp, map* m, list<vehicle*> *v_list){
             
             //set main screen
             control_panel_surface = SDL_CreateRGBSurface(0, CONTROL_PANEL_WIDTH, CONTROL_PANEL_HEIGHT, SCREEN_BPP, 0, 0, 0, 0);
             
             //set offset rect
             offset_rect = offset;
             offset_rect.w = CONTROL_PANEL_WIDTH;
             offset_rect.h = CONTROL_PANEL_HEIGHT;
             
             //secondary panel
             secd_panel = sp;
             
             //set map
             sim_map = m;
             
             //initialize sim speed
             sim_speed = NULL;
             
             //vehicle list
             vehicle_list = v_list;
             
             //initialize map parameters
             new_map_size = 20;
             new_num_miners = 8;
             new_num_scouts = 8;
             new_num_rescuers = 8;
             
             //initialize vehicle id and map position
             vehicle_id = 0;
             map_x = 0;
             map_y = 0;
             
             //set font and color
             font = TTF_OpenFont( "arial.ttf", 28 );
             header_font = TTF_OpenFont( "arial.ttf", 35);
             SDL_Color textColor = { 0, 0, 0 };
             textColorBlack = textColor;
             
             //toggle vairables
             simulation_paused = false;
             simulation_quit = false;
             simulation_restart = false;
             simulation_surrender = false;                             
             control_panel_state = -1;
             
             //initilize buttons and inputs without offsets
             //main menu
             button_pause = new button(0,0,120,50,"PAUSE");
             button_surrender = new button(0,0,120,50,"SURRENDER");
             button_restart = new button(0,0,120,50,"RESTART");
             button_quit = new button(0,0,120,50,"QUIT");
             button_inc_sim_speed = new button(0,0,160,50,"INCREASE SIMULATION SPEED");
             button_dec_sim_speed = new button(0,0,160,50,"DECREASE SIMULATION SPEED");
             //pause menu
             button_resume = new button(0,0,120,50,"RESUME");
             input_id = new input(0,0,50,35);
             button_vehicle = new button(0,0,90,35,"EDIT / VIEW");
             input_pos_x = new input(0,0,50,35);
             input_pos_y = new input(0,0,50,35);            
             button_pos = new button(0,0,90,35,"EDIT / VIEW");
             button_add_miner = new button(0,0,90,35,"ADD MINER");
             button_add_scout = new button(0,0,90,35,"ADD SCOUT");
             button_add_rescuer = new button(0,0,95,35,"ADD RESCUER");
             //restart menu
             input_map_size = new input(0,0,50,28);
             input_miners = new input(0,0,50,28);
             input_scouts = new input(0,0,50,28);
             input_rescuers = new input(0,0,50,28);
             //vehicle menu
             button_kill_vehicle = new button(0,0,120,40,"KILL VEHICLE");
             button_revive_vehicle = new button(0,0,120,40,"REVIVE VEHICLE");
             button_damage_vehicle = new button(0,0,120,40,"DAMAGE VEHICLE");
             button_repair_vehicle = new button(0,0,120,40,"REPAIR VEHICLE");
             button_move_to_pos = new button(0,0,60,35,"MOVE");
             button_set_speed = new button(0,0,60,35,"SET");
             button_set_access = new button(0,0,60,35,"SET");
             input_move_x = new input(0,0,50,35);
             input_move_y = new input(0,0,50,35);
             input_speed = new input(0,0,50,35);
             input_access = new input(0,0,50,35);
             button_back = new button(0,0,120,50,"BACK");
             //map position menu
             button_set_danger = new button(0,0,60,35,"SET");
             button_set_flag = new button(0,0,120,50,"SET FLAG");
             button_clear_flag = new button(0,0,130,50,"CLEAR FLAG");
             button_set_resources = new button(0,0,60,35,"SET");
             button_set_goal = new button(0,0,100,35,"SET GOAL");
             button_set_unload = new button(0,0,120,35,"ADD RESOURCES");
             input_danger = new input(0,0,50,35);
             input_pa = new input(0,0,50,35);
             input_ir = new input(0,0,50,35);
             input_pl = new input(0,0,50,35);
             
             //clear screen
             clear_control_panel();

             //set up main menu
             set_main_menu();
}


void control_panel::clean_up(){
     //clear surface
     SDL_FreeSurface( control_panel_surface );
     
     //close font
     TTF_CloseFont( font );
     TTF_CloseFont ( header_font );
     
     //delete buttons
     delete button_pause;
     delete button_surrender;
     delete button_restart;
     delete button_quit;
     delete button_inc_sim_speed;
     delete button_dec_sim_speed;
     delete button_resume;
     delete input_id;
     delete button_vehicle;
     delete input_pos_x;
     delete input_pos_y;   
     delete button_pos;
     delete button_add_miner;
     delete button_add_scout;
     delete button_add_rescuer;
     delete input_map_size;
     delete input_miners;
     delete input_scouts;
     delete input_rescuers;
     delete button_kill_vehicle;
     delete button_revive_vehicle;
     delete button_damage_vehicle;
     delete button_repair_vehicle;
     delete button_move_to_pos;
     delete button_set_speed;
     delete button_set_access;
     delete input_move_x;
     delete input_move_y;
     delete input_speed;
     delete input_access;
     delete button_back;
     delete button_set_danger;
     delete button_set_flag;
     delete button_clear_flag;
     delete button_set_resources;
     delete button_set_goal;
     delete button_set_unload;
     delete input_danger;
     delete input_pa;
     delete input_ir;
     delete input_pl;   
}

//clear screen
void control_panel::clear_control_panel(){
             if(control_panel_surface == NULL) return;
             
             SDL_Rect temp_rect = offset_rect;
             temp_rect.x = 0;
             temp_rect.y = 0;
             SDL_FillRect(control_panel_surface, &temp_rect, SDL_MapRGB((control_panel_surface)->format, 128, 128, 128));

             control_panel_state = -1;
}

//set main menu
void control_panel::set_main_menu(){
          if(control_panel_surface == NULL) return;
          
          //clear screen
          if(control_panel_state != -1) clear_control_panel();
          
          //temp variables
          SDL_Rect temp_rect;
          temp_rect.w = 0;
          temp_rect.h = 0;
          int w = button_pause->get_button_rect()->w;
          int h = button_pause->get_button_rect()->h;
          int ws = button_inc_sim_speed->get_button_rect()->w;
          int hs = button_inc_sim_speed->get_button_rect()->h;
          int x_space = 90; //space between buttons
          int y_space = 30;
          
          //pause button
          temp_rect.x = (CONTROL_PANEL_WIDTH - w - w - x_space)/2;
          temp_rect.y = (CONTROL_PANEL_HEIGHT - h - h - y_space)/2;
          SDL_BlitSurface( button_pause->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_pause->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_pause->get_button_rect()->y = offset_rect.y + temp_rect.y; 

          //surrender button
          temp_rect.x = (CONTROL_PANEL_WIDTH - w - w - x_space)/2 + w + x_space;
          temp_rect.y = (CONTROL_PANEL_HEIGHT - h - h - y_space)/2;
          SDL_BlitSurface( button_surrender->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_surrender->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_surrender->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //restart button
          temp_rect.x = (CONTROL_PANEL_WIDTH - w - w - x_space)/2;
          temp_rect.y = (CONTROL_PANEL_HEIGHT - h - h - y_space)/2 + h + y_space;
          SDL_BlitSurface( button_restart->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_restart->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_restart->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //quit button
          temp_rect.x = (CONTROL_PANEL_WIDTH - w - w - x_space)/2 + w + x_space;
          temp_rect.y = (CONTROL_PANEL_HEIGHT - h - h - y_space)/2 + h + y_space;
          SDL_BlitSurface( button_quit->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_quit->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_quit->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //increase simulation speed button
          temp_rect.x = (CONTROL_PANEL_WIDTH - ws - ws - x_space)/2;
          temp_rect.y = CONTROL_PANEL_HEIGHT -50 ;
          SDL_BlitSurface( button_inc_sim_speed->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_inc_sim_speed->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_inc_sim_speed->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //decrease simulation speed button
          temp_rect.x = (CONTROL_PANEL_WIDTH - ws - ws - x_space)/2 + ws + x_space;
          temp_rect.y = CONTROL_PANEL_HEIGHT - 50 ;
          SDL_BlitSurface( button_dec_sim_speed->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_dec_sim_speed->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_dec_sim_speed->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //unset simulation paused in case it was called from !this
          simulation_paused = false;
          
          //set state to main menu
          control_panel_state = 0;
}

//set paused menu
void control_panel::set_paused_menu(){
          if(control_panel_surface == NULL) return;
          
          //clear screen
          if(control_panel_state != -1) clear_control_panel();
     
          //temp variables
          SDL_Rect temp_rect;
          temp_rect.w = 0;
          temp_rect.h = 0;
          int wbe = button_vehicle->get_button_rect()->w;
          int hbe = button_vehicle->get_button_rect()->h;
          int wbr = button_resume->get_button_rect()->w;
          int hbr = button_resume->get_button_rect()->h;
          int wi = input_id->get_input_rect()->w;
          int hi = input_id->get_input_rect()->h;
          int y_space = 20; //space between rows
          int x_space = 5; //space between input and button
          int x_bspace = 50; //space between add buttons
          int w = 0; int h = 0;
          int vert_offset=0;

          // Vehicle ID: <int_input>  <button Edit/View>
          //put text
          TTF_SizeText(font, "Vehicle ID: ", &w, &h);
          write_on_panel("Vehicle ID: ", (CONTROL_PANEL_WIDTH-w-wi-x_space-wbe)/2, y_space+vert_offset, font);
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_space-wbe)/2 + w;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_id->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_id->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_id->get_input_rect()->y = offset_rect.y + temp_rect.y;
          //put button
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_space-wbe)/2 + w + wi + x_space;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( button_vehicle->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_vehicle->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_vehicle->get_button_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hi;
               
          // Map Position x,y: <int_input><int_input>  <button Edit/View>
          //put text
          TTF_SizeText(font, "Map Position x,y: ", &w, &h);
          write_on_panel("Map Position x,y: ", (CONTROL_PANEL_WIDTH-w-wi-wi-x_space-wbe)/2, y_space+vert_offset, font);
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-wi-x_space-wbe)/2 + w;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_pos_x->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_pos_x->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_pos_x->get_input_rect()->y = offset_rect.y + temp_rect.y;
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-wi-x_space-wbe)/2 + w + wi;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_pos_y->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_pos_y->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_pos_y->get_input_rect()->y = offset_rect.y + temp_rect.y;
          //put button
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-wi-x_space-wbe)/2 + w + wi + wi + x_space;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( button_pos->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_pos->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_pos->get_button_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hi;
          
          //ADD MINER    ADD SCOUT   ADD RESCUER
          //put button
          temp_rect.x = (CONTROL_PANEL_WIDTH-wbe-wbe-wbe-x_bspace-x_bspace)/2;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( button_add_miner->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_add_miner->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_add_miner->get_button_rect()->y = offset_rect.y + temp_rect.y;
          //put button
          temp_rect.x = (CONTROL_PANEL_WIDTH-wbe-wbe-wbe-x_bspace-x_bspace)/2 + wbe + x_bspace;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( button_add_scout->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_add_scout->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_add_scout->get_button_rect()->y = offset_rect.y + temp_rect.y;
          //put button
          temp_rect.x = (CONTROL_PANEL_WIDTH-wbe-wbe-wbe-x_bspace-x_bspace)/2 + wbe + x_bspace + wbe + x_bspace;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( button_add_rescuer->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_add_rescuer->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_add_rescuer->get_button_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hbe;
          
          //resume button
          temp_rect.x = (CONTROL_PANEL_WIDTH - wbr)/2;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( button_resume->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_resume->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_resume->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //set simulation paused in case it was called from !this
          simulation_paused = true;
          
          //set state to pause menu
          control_panel_state = 1;
}

//set restart menu
void control_panel::set_restart_menu(){
          if(control_panel_surface == NULL) return;
          
          //clear screen
          if(control_panel_state != -1) clear_control_panel();
          
          //temp variables
          SDL_Rect temp_rect;
          temp_rect.w = 0;
          temp_rect.h = 0;
          int wbr = button_restart->get_button_rect()->w;
          int wi = input_map_size->get_input_rect()->w;
          int hi = input_map_size->get_input_rect()->h;
          int y_space = 10; //space between rows
          int w = 0; int h = 0;
          int vert_offset=0;
     
          // Enter New Map Parameters
          //put text
          TTF_SizeText(font, "Enter New Map Parameters", &w, &h);
          write_on_panel("Enter New Map Parameters", (CONTROL_PANEL_WIDTH-w)/2, y_space+vert_offset, font);
          vert_offset += y_space + hi;
          
          // Map Size: <int_input>
          //put text
          TTF_SizeText(font, "Map Size: ", &w, &h);
          write_on_panel("Map Size: ", (CONTROL_PANEL_WIDTH-w-wi)/2, y_space+vert_offset, font);
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi)/2 + w;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_map_size->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_map_size->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_map_size->get_input_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hi;

          // Number of Miners: <int_input>
          //put text
          TTF_SizeText(font, "Number of Miners: ", &w, &h);
          write_on_panel("Number of Miners: ", (CONTROL_PANEL_WIDTH-w-wi)/2, y_space+vert_offset, font);
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi)/2 + w;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_miners->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_miners->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_miners->get_input_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hi;

          // Number of Scouts: <int_input>
          //put text
          TTF_SizeText(font, "Number of Scouts: ", &w, &h);
          write_on_panel("Number of Scouts: ", (CONTROL_PANEL_WIDTH-w-wi)/2, y_space+vert_offset, font);
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi)/2 + w;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_scouts->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_scouts->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_scouts->get_input_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hi;

          // Number of Rescuers: <int_input>
          //put text
          TTF_SizeText(font, "Number of Rescuers: ", &w, &h);
          write_on_panel("Number of Rescuers: ", (CONTROL_PANEL_WIDTH-w-wi)/2, y_space+vert_offset, font);
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi)/2 + w;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_rescuers->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_rescuers->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_rescuers->get_input_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hi;

          //restart button
          temp_rect.x = (CONTROL_PANEL_WIDTH - wbr)/2;
          temp_rect.y = vert_offset + y_space;
          SDL_BlitSurface( button_restart->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_restart->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_restart->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //set simulation paused in case it was called from !this
          simulation_paused = true;
          
          //set state to restart menu
          control_panel_state = 2;
}

//set end menu
void control_panel::set_end_menu(){
          if(control_panel_surface == NULL) return;
          
          //clear screen
          if(control_panel_state != -1) clear_control_panel();
          
          //temp variables
          SDL_Rect temp_rect;
          temp_rect.w = 0;
          temp_rect.h = 0;
          int wbr = button_restart->get_button_rect()->w;
          int y_space = 70; //space between rows
          int x_space = 90; //space between buttons
          int w = 0; int h = 0;
          int vert_offset=0;
          
          // Simulation Goal: FAILED/SUCCEEDED
          //put text
          if(sim_map->simulation_status_on_map() == 3){
                TTF_SizeText(font, "Simulation Goal: SUCCEEDED", &w, &h);
                write_on_panel("Simulation Goal: SUCCEEDED", (CONTROL_PANEL_WIDTH-w)/2, y_space+vert_offset, font);
          }         
          else {
                TTF_SizeText(font, "Simulation Goal: FAILED", &w, &h);
                write_on_panel("Simulation Goal: FAILED", (CONTROL_PANEL_WIDTH-w)/2, y_space+vert_offset, font);
          }
          vert_offset += y_space + h;
          
          //restart button
          temp_rect.x = (CONTROL_PANEL_WIDTH - wbr - wbr - x_space)/2;
          temp_rect.y = vert_offset + y_space;
          SDL_BlitSurface( button_restart->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_restart->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_restart->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //quit button
          temp_rect.x = (CONTROL_PANEL_WIDTH - wbr - wbr - x_space)/2 + wbr + x_space;
          temp_rect.y = vert_offset + y_space;
          SDL_BlitSurface( button_quit->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_quit->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_quit->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //set simulation pause in case it was called from !this
          simulation_paused = true;

          //set state to end menu
          control_panel_state = 3;
}

//set vehicle menu
void control_panel::set_vehicle_menu(){
          if(control_panel_surface == NULL) return;
          
          //clear screen
          if(control_panel_state != -1) clear_control_panel();
          
          //temp variables
          SDL_Rect temp_rect;
          temp_rect.w = 0;
          temp_rect.h = 0;
          int wbk = button_kill_vehicle->get_button_rect()->w;
          int hbk = button_kill_vehicle->get_button_rect()->h;
          int wbs = button_set_speed->get_button_rect()->w;
          int hbs = button_set_speed->get_button_rect()->h;
          int wbb = button_back->get_button_rect()->w;
          int hbb = button_back->get_button_rect()->h;
          int wi = input_speed->get_button_rect()->w;
          int hi = input_speed->get_button_rect()->h;
          int y_space = 15; //space between rows
          int x_space = 30; //space between buttons
          int x_ispace = 5; //space between input and button
          int w = 0; int h = 0;
          int vert_offset=0;
          
          //Vehicle Edit
          //put text
          TTF_SizeText(header_font, "Vehicle Edit", &w, &h);
          write_on_panel("Vehicle Edit", (CONTROL_PANEL_WIDTH-w)/2, 4+vert_offset, header_font);
          vert_offset += 4+vert_offset+h;
          
          //KILL VEHICLE    REVIVE VEHICLE    DAMAGE VEHICLE    REPAIR VEHICE
          //kill vehicle button
          temp_rect.x = (CONTROL_PANEL_WIDTH - 4*wbk - 3*x_space)/2;
          temp_rect.y = vert_offset + y_space;
          SDL_BlitSurface( button_kill_vehicle->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_kill_vehicle->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_kill_vehicle->get_button_rect()->y = offset_rect.y + temp_rect.y; 
          //revive vehicle button
          temp_rect.x = (CONTROL_PANEL_WIDTH - 4*wbk - 3*x_space)/2 + wbk + x_space;
          temp_rect.y = vert_offset + y_space;
          SDL_BlitSurface( button_revive_vehicle->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_revive_vehicle->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_revive_vehicle->get_button_rect()->y = offset_rect.y + temp_rect.y; 
          //damage vehicle button
          temp_rect.x = (CONTROL_PANEL_WIDTH - 4*wbk - 3*x_space)/2 + 2*wbk + 2*x_space;
          temp_rect.y = vert_offset + y_space;
          SDL_BlitSurface( button_damage_vehicle->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_damage_vehicle->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_damage_vehicle->get_button_rect()->y = offset_rect.y + temp_rect.y;
          //repair vehicle button
          temp_rect.x = (CONTROL_PANEL_WIDTH - 4*wbk - 3*x_space)/2 + 3*wbk + 3*x_space;
          temp_rect.y = vert_offset + y_space;
          SDL_BlitSurface( button_repair_vehicle->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_repair_vehicle->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_repair_vehicle->get_button_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hbk;

          //Move Vehicle to Position x,y: <input><input>  <button MOVE>
          //put text
          TTF_SizeText(font, "Move Vehicle to Position x,y: ", &w, &h);
          write_on_panel("Move Vehicle to Position x,y: ", (CONTROL_PANEL_WIDTH-w-wi-wi-x_ispace-wbs)/2, y_space+vert_offset, font);
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-wi-x_ispace-wbs)/2 + w;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_move_x->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_move_x->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_move_x->get_input_rect()->y = offset_rect.y + temp_rect.y;
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-wi-x_ispace-wbs)/2 + w + wi;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_move_y->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_move_y->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_move_y->get_input_rect()->y = offset_rect.y + temp_rect.y;
          //put button
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-wi-x_ispace-wbs)/2 + w + wi + wi + x_ispace;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( button_move_to_pos->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_move_to_pos->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_move_to_pos->get_button_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hbs;

          wbs += 200; //displace to the left

          //Set Speed to: <input> <set button>
          //put text
          TTF_SizeText(font, "Set Speed to: ", &w, &h);
          write_on_panel("Set Speed to: ", (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2, y_space+vert_offset, font);
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_speed->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_speed->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_speed->get_input_rect()->y = offset_rect.y + temp_rect.y;
          //put button
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w + wi + x_ispace;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( button_set_speed->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_set_speed->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_set_speed->get_button_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hbs;

          //Set Access to: 1/<input> <set button>
          //put text
          TTF_SizeText(font, "Set Access to: 1/", &w, &h);
          write_on_panel("Set Access to: 1/", (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2, y_space+vert_offset, font);
          //put input
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( input_access->get_input_surface(), NULL, control_panel_surface, &temp_rect );
          input_access->get_input_rect()->x = offset_rect.x + temp_rect.x;
          input_access->get_input_rect()->y = offset_rect.y + temp_rect.y;
          //put button
          temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w + wi + x_ispace;
          temp_rect.y = y_space + vert_offset;
          SDL_BlitSurface( button_set_access->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_set_access->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_set_access->get_button_rect()->y = offset_rect.y + temp_rect.y;
          vert_offset += y_space + hbs;
          
          //back button
          temp_rect.x = CONTROL_PANEL_WIDTH - wbb - 20; //starting from the right
          temp_rect.y = CONTROL_PANEL_HEIGHT - hbb - 20; //starting from bottom
          SDL_BlitSurface( button_back->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_back->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_back->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //set simulation pause in case it was called from !this
          simulation_paused = true;

          //set state to vehicle menu
          control_panel_state = 4;
}

//set map position menu
void control_panel::set_map_pos_menu(){
          if(control_panel_surface == NULL) return;
          
          //clear screen
          if(control_panel_state != -1) clear_control_panel();
          
          //temp variables
          SDL_Rect temp_rect;
          temp_rect.w = 0;
          temp_rect.h = 0;
          int wbsb = button_set_goal->get_button_rect()->w; //set base buttons
          int hbsb = button_set_goal->get_button_rect()->h;
          int wbs = button_set_danger->get_button_rect()->w; //set mine buttons
          int hbs = button_set_danger->get_button_rect()->h;
          int wbb = button_back->get_button_rect()->w; //flag, back buttons
          int hbb = button_back->get_button_rect()->h;
          int wi = input_danger->get_button_rect()->w; //inputs
          int hi = input_danger->get_button_rect()->h;
          int y_space = 5; //space between rows
          int x_space = 20; //space between buttons
          int x_ispace = 5; //space between input and button
          int w = 0; int h = 0;
          int vert_offset = 0;
          
          //map position type
          int pos_type = sim_map->get_block_type(map_x, map_y);
          
          //Base
          if(pos_type == 1){
               //put text Edit Base
               TTF_SizeText(header_font, "Edit Base", &w, &h);
               write_on_panel("Edit Base", (CONTROL_PANEL_WIDTH-w)/2, 4+vert_offset, header_font);
               vert_offset += 4+vert_offset+h;
               
               y_space = 10; //increase row space
               x_space = 15; //decrease button space
               
               //Edit Base Palladium: <input>
               //put text
               TTF_SizeText(font, "Edit Base Palladium: ", &w, &h);
               write_on_panel("Edit Base Palladium: ", (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbsb-wbsb-x_space)/2, y_space+vert_offset, font);
               //put input
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbsb-wbsb-x_space)/2 + w;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( input_pa->get_input_surface(), NULL, control_panel_surface, &temp_rect );
               input_pa->get_input_rect()->x = offset_rect.x + temp_rect.x;
               input_pa->get_input_rect()->y = offset_rect.y + temp_rect.y;
               vert_offset += y_space + hbsb;
               
               //Edit Base Iridium: <input>
               //put text
               TTF_SizeText(font, "Edit Base Iridium: ", &w, &h);
               write_on_panel("Edit Base Iridium: ", (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbsb-wbsb-x_space)/2, y_space+vert_offset, font);
               //put input
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbsb-wbsb-x_space)/2 + w;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( input_ir->get_input_surface(), NULL, control_panel_surface, &temp_rect );
               input_ir->get_input_rect()->x = offset_rect.x + temp_rect.x;
               input_ir->get_input_rect()->y = offset_rect.y + temp_rect.y;
               vert_offset += y_space + hbsb;
               
               //Edit Base Platinum: <input>  <button set goal> <button add rescources>
               //put text
               TTF_SizeText(font, "Edit Base Platinum: ", &w, &h);
               write_on_panel("Edit Base Platinum: ", (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbsb-wbsb-x_space)/2, y_space+vert_offset, font);
               //put input
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbsb-wbsb-x_space)/2 + w;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( input_pl->get_input_surface(), NULL, control_panel_surface, &temp_rect );
               input_pl->get_input_rect()->x = offset_rect.x + temp_rect.x;
               input_pl->get_input_rect()->y = offset_rect.y + temp_rect.y;
               //put button
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbsb-wbsb-x_space)/2 + w + wi + x_ispace;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( button_set_goal->get_button_surface(), NULL, control_panel_surface, &temp_rect );
               button_set_goal->get_button_rect()->x = offset_rect.x + temp_rect.x;
               button_set_goal->get_button_rect()->y = offset_rect.y + temp_rect.y;
               //put button
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbsb-wbsb-x_space)/2 + w + wi + x_ispace + wbsb + x_space;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( button_set_unload->get_button_surface(), NULL, control_panel_surface, &temp_rect );
               button_set_unload->get_button_rect()->x = offset_rect.x + temp_rect.x;
               button_set_unload->get_button_rect()->y = offset_rect.y + temp_rect.y;
               vert_offset += y_space + hbs;    
          }
          //Mine
          else if(pos_type == 2){
               //put text Edit Mine
               TTF_SizeText(header_font, "Edit Mine", &w, &h);
               write_on_panel("Edit Mine", (CONTROL_PANEL_WIDTH-w)/2, 2+vert_offset, header_font);
               vert_offset += 2+vert_offset+h;
               
               wbs += 100; //horizontal offset to the left
               
               //Set Mine Danger: 1/<input> <button>
               //put text
               TTF_SizeText(font, "Set Mine Danger: 1/", &w, &h);
               write_on_panel("Set Mine Danger: 1/", (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2, y_space+vert_offset, font);
               //put input
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( input_danger->get_input_surface(), NULL, control_panel_surface, &temp_rect );
               input_danger->get_input_rect()->x = offset_rect.x + temp_rect.x;
               input_danger->get_input_rect()->y = offset_rect.y + temp_rect.y;
               //put button
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w + wi + x_ispace;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( button_set_danger->get_button_surface(), NULL, control_panel_surface, &temp_rect );
               button_set_danger->get_button_rect()->x = offset_rect.x + temp_rect.x;
               button_set_danger->get_button_rect()->y = offset_rect.y + temp_rect.y;
               vert_offset += y_space + hbs;
               
               //Set Mine's Palladium: <input>
               //put text
               TTF_SizeText(font, "Set Mine's Palladium: ", &w, &h);
               write_on_panel("Set Mine's Palladium: ", (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2, y_space+vert_offset, font);
               //put input
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( input_pa->get_input_surface(), NULL, control_panel_surface, &temp_rect );
               input_pa->get_input_rect()->x = offset_rect.x + temp_rect.x;
               input_pa->get_input_rect()->y = offset_rect.y + temp_rect.y;
               vert_offset += y_space + hbs;
               
               //Set Mine's Iridium: <input>
               //put text
               TTF_SizeText(font, "Set Mine's Iridium: ", &w, &h);
               write_on_panel("Set Mine's Iridium: ", (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2, y_space+vert_offset, font);
               //put input
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( input_ir->get_input_surface(), NULL, control_panel_surface, &temp_rect );
               input_ir->get_input_rect()->x = offset_rect.x + temp_rect.x;
               input_ir->get_input_rect()->y = offset_rect.y + temp_rect.y;
               vert_offset += y_space + hbs;
               
               //Set Mine's Platinum: <input>
               //put text
               TTF_SizeText(font, "Set Mine's Platinum: ", &w, &h);
               write_on_panel("Set Mine's Platinum: ", (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2, y_space+vert_offset, font);
               //put input
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( input_pl->get_input_surface(), NULL, control_panel_surface, &temp_rect );
               input_pl->get_input_rect()->x = offset_rect.x + temp_rect.x;
               input_pl->get_input_rect()->y = offset_rect.y + temp_rect.y;
               //put button
               temp_rect.x = (CONTROL_PANEL_WIDTH-w-wi-x_ispace-wbs)/2 + w + wi + x_ispace;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( button_set_resources->get_button_surface(), NULL, control_panel_surface, &temp_rect );
               button_set_resources->get_button_rect()->x = offset_rect.x + temp_rect.x;
               button_set_resources->get_button_rect()->y = offset_rect.y + temp_rect.y;
               vert_offset += y_space + hbs;
               
               //<button SET FLAG>   <button CLEAR FLAG>
               //put button
               temp_rect.x = (CONTROL_PANEL_WIDTH-3*wbb-x_space)/2;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( button_set_flag->get_button_surface(), NULL, control_panel_surface, &temp_rect );
               button_set_flag->get_button_rect()->x = offset_rect.x + temp_rect.x;
               button_set_flag->get_button_rect()->y = offset_rect.y + temp_rect.y;
               //put button
               temp_rect.x = (CONTROL_PANEL_WIDTH-3*wbb-x_space)/2 + wbb + x_space;
               temp_rect.y = y_space + vert_offset;
               SDL_BlitSurface( button_clear_flag->get_button_surface(), NULL, control_panel_surface, &temp_rect );
               button_clear_flag->get_button_rect()->x = offset_rect.x + temp_rect.x;
               button_clear_flag->get_button_rect()->y = offset_rect.y + temp_rect.y;
               vert_offset += y_space + hbb;     
          }
          //Road
          else if(pos_type == 3){
               //put text   Edit Road
               TTF_SizeText(header_font, "Edit Road", &w, &h);
               write_on_panel("Edit Road", (CONTROL_PANEL_WIDTH-w)/2, 4+vert_offset, header_font);
               vert_offset += 4+vert_offset+h;
               
               //put text   Nothing to edit.
               TTF_SizeText(header_font, "Nothing to edit.", &w, &h);
               write_on_panel("Nothing to edit.", (CONTROL_PANEL_WIDTH-w)/2, 50+vert_offset, header_font);
               vert_offset += 50+vert_offset+h;
          }
          //Border, Default
          else{
               //put text   Edit Border
               TTF_SizeText(header_font, "Edit Border", &w, &h);
               write_on_panel("Edit Border", (CONTROL_PANEL_WIDTH-w)/2, 4+vert_offset, header_font);
               vert_offset += 4+vert_offset+h;
               
               //put text   Nothing to edit.
               TTF_SizeText(header_font, "Nothing to edit.", &w, &h);
               write_on_panel("Nothing to edit.", (CONTROL_PANEL_WIDTH-w)/2, 50+vert_offset, header_font);
               vert_offset += 50+vert_offset+h;
          }
          
          //back button
          temp_rect.x = CONTROL_PANEL_WIDTH - wbb - 10; //starting from the right
          temp_rect.y = CONTROL_PANEL_HEIGHT - hbb - 10; //starting from bottom
          SDL_BlitSurface( button_back->get_button_surface(), NULL, control_panel_surface, &temp_rect );
          button_back->get_button_rect()->x = offset_rect.x + temp_rect.x;
          button_back->get_button_rect()->y = offset_rect.y + temp_rect.y;
          
          //set simulation pause in case it was called from !this
          simulation_paused = true;

          //set state to map position menu
          control_panel_state = 5;
}        

void control_panel::handle_events(SDL_Event* event){
         if(control_panel_surface == NULL) return;
         
         switch(control_panel_state){
                  case 0: //main menu
                       //handle button events
                       button_pause->handle_events(event);
                       button_surrender->handle_events(event);
                       button_restart->handle_events(event);
                       button_quit->handle_events(event);
                       button_inc_sim_speed->handle_events(event);
                       button_dec_sim_speed->handle_events(event);
                       //results
                       if(button_pause->was_clicked()){
                                simulation_paused = true;
                                set_paused_menu();
                                }
                       else if(button_surrender->was_clicked()){
                                simulation_surrender = true;
                                }
                       else if(button_restart->was_clicked()){
                                simulation_paused = true;
                                set_restart_menu();
                                }
                       else if(button_quit->was_clicked()){
                                simulation_quit = true;
                                }
                       else if(button_inc_sim_speed->was_clicked()){
                                if(sim_speed!=NULL && *sim_speed>=200) *sim_speed -= 100;
                                }
                       else if(button_dec_sim_speed->was_clicked()){
                                if(sim_speed!=NULL && *sim_speed<=1200) *sim_speed += 100;
                                }
                       break;
                  case 1: //pause menu
                       button_resume->handle_events(event);
                       input_id->handle_events(event);
                       button_vehicle->handle_events(event);
                       input_pos_x->handle_events(event);
                       input_pos_y->handle_events(event);   
                       button_pos->handle_events(event);
                       button_add_miner->handle_events(event);
                       button_add_scout->handle_events(event);
                       button_add_rescuer->handle_events(event);
                       //results
                       if(button_resume->was_clicked()){
                                simulation_paused = false;
                                set_main_menu();
                                }
                       else if(input_id->got_input()){
                                set_paused_menu();
                                }
                       else if(input_pos_x->got_input()){
                                set_paused_menu();
                                }
                       else if(input_pos_y->got_input()){
                                set_paused_menu();
                                }         
                       else if(button_vehicle->was_clicked()){
                                input_id->get_input_int(&vehicle_id);                              
                                secd_panel->write_vehicle(vehicle_id);
                                input_id->clear_input_string();
                                if(vehicle::get_vehicle_address(vehicle_id)==NULL) set_paused_menu();
                                else set_vehicle_menu();
                                }
                       else if(button_pos->was_clicked()){
                                input_pos_x->get_input_int(&map_x);
                                input_pos_y->get_input_int(&map_y);                     
                                secd_panel->write_map_pos(map_x, map_y);
                                input_pos_x->clear_input_string();
                                input_pos_y->clear_input_string();
                                if(!sim_map->check_coord(map_x, map_y)) set_paused_menu();
                                else set_map_pos_menu();
                                }
                       else if(button_add_miner->was_clicked()){
                                //create new vehicle
                                vehicle_list->push_back(new miner());
                                list<vehicle*>::iterator itr = vehicle_list->end();
                                itr--;
                                (*itr)->set_map(sim_map);
                                (*itr)->enter_map(sim_map->get_round(),-1,-1);
                                //get id and change panels
                                vehicle_id = (*itr)->get_id();
                                secd_panel->write_vehicle(vehicle_id);
                                set_vehicle_menu();
                                }
                       else if(button_add_scout->was_clicked()){
                                //create new vehicle
                                vehicle_list->push_back(new scout());
                                list<vehicle*>::iterator itr = vehicle_list->end();
                                itr--;
                                (*itr)->set_map(sim_map);
                                (*itr)->enter_map(sim_map->get_round(),-1,-1);
                                //get id and change panels
                                vehicle_id = (*itr)->get_id();
                                secd_panel->write_vehicle(vehicle_id);
                                set_vehicle_menu();
                                }
                       else if(button_add_rescuer->was_clicked()){
                                //create new vehicle
                                vehicle_list->push_back(new rescuer());
                                list<vehicle*>::iterator itr = vehicle_list->end();
                                itr--;
                                (*itr)->set_map(sim_map);
                                (*itr)->enter_map(sim_map->get_round(),-1,-1);
                                //get id and change panels
                                vehicle_id = (*itr)->get_id();
                                secd_panel->write_vehicle(vehicle_id);
                                set_vehicle_menu();
                                }
                       break;
                  case 2: //restart menu
                       button_restart->handle_events(event);
                       input_map_size->handle_events(event);
                       input_miners->handle_events(event);
                       input_scouts->handle_events(event);
                       input_rescuers->handle_events(event);
                       //results
                       if(button_restart->was_clicked()){
                                simulation_restart = true;
                                //get inputs and put them in restart parameters
                                if(!input_map_size->get_input_string().empty()) input_map_size->get_input_int(&new_map_size);
                                if(!input_miners->get_input_string().empty()) input_miners->get_input_int(&new_num_miners);
                                if(!input_scouts->get_input_string().empty()) input_scouts->get_input_int(&new_num_scouts);
                                if(!input_rescuers->get_input_string().empty()) input_rescuers->get_input_int(&new_num_rescuers);
                                }
                       else if(input_map_size->got_input()){
                                set_restart_menu();
                                }
                       else if(input_miners->got_input()){
                                set_restart_menu();
                                }
                       else if(input_scouts->got_input()){
                                set_restart_menu();
                                }         
                       else if(input_rescuers->got_input()){
                                set_restart_menu();
                                }
                       break;
                  case 3: //end menu
                       button_restart->handle_events(event);
                       button_quit->handle_events(event);                              
                       if(button_restart->was_clicked()){
                                simulation_paused = true;
                                set_restart_menu();
                                }
                       else if(button_quit->was_clicked()){
                                simulation_quit = true;
                                }
                       break;
                  case 4: //vehicle menu
                       button_kill_vehicle->handle_events(event);
                       button_revive_vehicle->handle_events(event);
                       button_damage_vehicle->handle_events(event);
                       button_repair_vehicle->handle_events(event);
                       button_move_to_pos->handle_events(event);
                       button_set_speed->handle_events(event);
                       button_set_access->handle_events(event);
                       input_move_x->handle_events(event);
                       input_move_y->handle_events(event);
                       input_speed->handle_events(event);
                       input_access->handle_events(event);
                       button_back->handle_events(event);
                       
                       if(input_move_x->got_input()){
                                set_vehicle_menu();
                                }
                       else if(input_move_y->got_input()){
                                set_vehicle_menu();
                                }
                       else if(input_speed->got_input()){
                                set_vehicle_menu();
                                }
                       else if(input_access->got_input()){
                                set_vehicle_menu();
                                }
                       else if(button_back->was_clicked()){
                                set_paused_menu();
                                }
                       else if(button_kill_vehicle->was_clicked()){
                                vehicle* v_address = vehicle::get_vehicle_address(vehicle_id);
                                if(v_address==NULL) break;
                                else v_address->set_status(false);
                                }
                       else if(button_revive_vehicle->was_clicked()){
                                vehicle* v_address = vehicle::get_vehicle_address(vehicle_id);
                                if(v_address==NULL) break;
                                else v_address->set_status(true);
                                }
                       else if(button_damage_vehicle->was_clicked()){
                                vehicle* v_address = vehicle::get_vehicle_address(vehicle_id);
                                if(v_address==NULL) break;
                                else v_address->set_damaged(true);
                                }
                       else if(button_repair_vehicle->was_clicked()){
                                vehicle* v_address = vehicle::get_vehicle_address(vehicle_id);
                                if(v_address==NULL) break;
                                else v_address->set_damaged(false);
                                }
                       else if(button_move_to_pos->was_clicked()){
                                input_move_x->get_input_int(&map_x);
                                input_move_y->get_input_int(&map_y);
                                vehicle* v_address = vehicle::get_vehicle_address(vehicle_id);
                                if(v_address==NULL) break;
                                else v_address->move_to_pos(map_x,map_y);
                                input_move_x->clear_input_string();
                                input_move_y->clear_input_string();
                                set_vehicle_menu();
                                }
                       else if(button_set_speed->was_clicked()){
                                int temp_speed; 
                                input_speed->get_input_int(&temp_speed);
                                vehicle* v_address = vehicle::get_vehicle_address(vehicle_id);
                                if(v_address==NULL) break;
                                else v_address->set_speed(temp_speed);
                                input_speed->clear_input_string();
                                set_vehicle_menu();
                                }
                       else if(button_set_access->was_clicked()){
                                int temp_access; 
                                input_speed->get_input_int(&temp_access);
                                vehicle* v_address = vehicle::get_vehicle_address(vehicle_id);
                                if(v_address==NULL) break;
                                else {v_address->set_access(1.0/temp_access);}
                                input_access->clear_input_string();
                                set_vehicle_menu();
                                }
                       break;
                  case 5: //map position menu
                       button_back->handle_events(event);
                       switch(sim_map->get_block_type(map_x,map_y)){
                             case 1:
                                  input_pa->handle_events(event);
                                  input_ir->handle_events(event);
                                  input_pl->handle_events(event);
                                  button_set_goal->handle_events(event);
                                  button_set_unload->handle_events(event);
                                  break;
                             case 2:
                                   button_set_danger->handle_events(event);
                                   button_set_flag->handle_events(event);
                                   button_clear_flag->handle_events(event);
                                   button_set_resources->handle_events(event);
                                   input_danger->handle_events(event);
                                   input_pa->handle_events(event);
                                   input_ir->handle_events(event);
                                   input_pl->handle_events(event);
                                   break;
                       }
                       if(input_danger->got_input()){
                                set_map_pos_menu();
                                }
                       else if(input_pa->got_input()){
                                set_map_pos_menu();
                                }
                       else if(input_ir->got_input()){
                                set_map_pos_menu();
                                }
                       else if(input_pl->got_input()){
                                set_map_pos_menu();
                                }
                       else if(button_back->was_clicked()){
                                set_paused_menu();
                                }
                       else if(button_set_flag->was_clicked()){
                                sim_map->set_flag(map_x, map_y);
                                set_map_pos_menu();
                                }
                       else if(button_clear_flag->was_clicked()){
                                sim_map->clear_flag(map_x, map_y);
                                set_map_pos_menu();
                                }
                       else if(button_set_danger->was_clicked()){
                                int temp_danger=0;
                                input_danger->get_input_int(&temp_danger);
                                sim_map->set_danger(map_x,map_y,1.0/(float)temp_danger);
                                input_danger->clear_input_string();
                                set_map_pos_menu();
                                }
                       else if(button_set_resources->was_clicked()){
                                int temp_pa=0, temp_ir=0, temp_pl=0;
                                input_pa->get_input_int(&temp_pa);
                                input_ir->get_input_int(&temp_ir);
                                input_pl->get_input_int(&temp_pl);
                                if(!input_pa->get_input_string().empty()) sim_map->set_pa(map_x,map_y,temp_pa);
                                if(!input_ir->get_input_string().empty()) sim_map->set_ir(map_x,map_y,temp_ir);
                                if(!input_pl->get_input_string().empty()) sim_map->set_pl(map_x,map_y,temp_pl);
                                input_pa->clear_input_string();
                                input_ir->clear_input_string();
                                input_pl->clear_input_string();
                                set_map_pos_menu();
                                }
                       else if(button_set_goal->was_clicked()){
                                int temp_pa=0, temp_ir=0, temp_pl=0;
                                input_pa->get_input_int(&temp_pa);
                                input_ir->get_input_int(&temp_ir);
                                input_pl->get_input_int(&temp_pl);
                                sim_map->set_goal((input_pa->get_input_string().empty())?sim_map->get_pa_goal():temp_pa,
                                                  (input_ir->get_input_string().empty())?sim_map->get_ir_goal():temp_ir,
                                                  (input_pl->get_input_string().empty())?sim_map->get_pl_goal():temp_pl);
                                input_pa->clear_input_string();
                                input_ir->clear_input_string();
                                input_pl->clear_input_string();
                                set_map_pos_menu();
                                }
                       else if(button_set_unload->was_clicked()){
                                int temp_pa=0, temp_ir=0, temp_pl=0;
                                input_pa->get_input_int(&temp_pa);
                                input_ir->get_input_int(&temp_ir);
                                input_pl->get_input_int(&temp_pl);
                                if(!input_pa->get_input_string().empty()) sim_map->unload_pa(temp_pa);
                                if(!input_ir->get_input_string().empty()) sim_map->unload_ir(temp_ir);
                                if(!input_pl->get_input_string().empty()) sim_map->unload_pl(temp_pl);
                                input_pa->clear_input_string();
                                input_ir->clear_input_string();
                                input_pl->clear_input_string();
                                set_map_pos_menu();
                                }
                       break;
                  default: //also -1
                       set_main_menu();
         }
}

void control_panel::write_on_panel(std::string str, int x, int y, TTF_Font* f){
     if(f == NULL || control_panel_surface == NULL) return;
     //write str
     SDL_Surface* msg = TTF_RenderText_Solid( f, str.c_str(), textColorBlack );
     //set offset
     SDL_Rect temp_rect;
     temp_rect.x = x;
     temp_rect.y = y;
     temp_rect.w = 0;
     temp_rect.h = 0;
     //put text surface
     SDL_BlitSurface( msg, NULL, control_panel_surface, &temp_rect );
     //clear msg
     SDL_FreeSurface( msg );
}

//get control panel surface
SDL_Surface* control_panel::get_control_panel_surface(){
             return control_panel_surface;
}
//get toggle variables
bool control_panel::user_paused(){
     return simulation_paused;
}
bool control_panel::user_quit(){
     return simulation_quit;
}
bool control_panel::user_restart(){
     return simulation_restart;
}
bool control_panel::user_surrender(){
     if(simulation_surrender) {simulation_surrender = false; return true;}
     else return false;
}
void control_panel::get_simulaiton_restart(int* m_s, int* n_m, int* n_s, int* n_r){
     *m_s = new_map_size;
     *n_m = new_num_miners;
     *n_s = new_num_scouts;
     *n_r = new_num_rescuers;
}
int control_panel::get_control_panel_state(){
    return control_panel_state;
}

void control_panel::set_sim_speed_address(int* s){
     sim_speed = s;
}
