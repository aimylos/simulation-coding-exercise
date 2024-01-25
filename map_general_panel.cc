#include "map.h"
#include "map_general_panel.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "vehicle.h"
#include "scout.h"
#include "miner.h"
#include "rescuer.h"
#include <string>
#include <sstream>

#define GENERAL_PANEL_WIDTH 700
#define GENERAL_PANEL_HEIGHT 350 
#define SCREEN_BPP 32

map_general_panel::map_general_panel(map* m){

         //simulation map
         sim_map = m;
         
         //top offset
         top_offset = 0;
         
         //load font and color
         font = TTF_OpenFont( "arial.ttf", 18 ); //if didn't load then no fonts will appear
         header_font = TTF_OpenFont( "arial.ttf", 28 );
         SDL_Color textColor = { 0, 0, 0 };
         textColorBlack = textColor;

         //make white surface
         general_panel_surface = SDL_CreateRGBSurface(0, GENERAL_PANEL_WIDTH, GENERAL_PANEL_HEIGHT, SCREEN_BPP, 0, 0, 0, 0);
         
         //update general panel
         update_general_panel();
}

SDL_Surface* map_general_panel::get_general_panel_surface(){
             return general_panel_surface;
}

void map_general_panel::update_general_panel(){
     clear_panel();
     write_header();
     write_simulation_status();
     write_base_information();
     write_map_information();
     write_vehicles_information();
}


//clear surface
void map_general_panel::clear_panel(){
         if (general_panel_surface == NULL) return;
         rect.w = GENERAL_PANEL_WIDTH;
         rect.h = GENERAL_PANEL_HEIGHT;
         rect.x = 0;
         rect.y = 0;
         SDL_FillRect(general_panel_surface, &rect, SDL_MapRGB((general_panel_surface)->format, 255, 255, 255));
}

//write header
void map_general_panel::write_header(){
         if(header_font == NULL || general_panel_surface == NULL) return; 
         write_on_panel("General Information", 0, 3, header_font);
}

//write simulation status
void map_general_panel::write_simulation_status(){
         if(font == NULL || general_panel_surface == NULL) return;
         //make output string
         std::ostringstream os; //string stream
         //get simulation status
         switch(sim_map->simulation_status_on_map()){
                  case 0:
                       os << "Simulation Status: " << "NOT STARTED" ;
                       break;
                  case 1:
                       os << "Simulation Status: " << "ONGOING" ;
                       break;
                  case 2:
                       os << "Simulation Status: " << "FAILED" ;
                       break;
                  case 3:
                       os << "Simulation Status: " << "SUCCEEDED" ;
                       break;
                  case -1:
                       os << "Simulation Status: " << "UNKNOWN" ;
                       break;
                  default:
                       os << "Simulation Status: " << "UNKNOWN" ;
         }
         // add Goal Completed: <float>%
         os << "       Goal Completed: " << sim_map->goal_completed() << "%       Round: " << *sim_map->get_round();
         write_on_panel(os.str(), 0, top_offset+10, font);
}

//write base information
void map_general_panel::write_base_information(){
         if(font == NULL || general_panel_surface == NULL) return;
         //make output string
         std::ostringstream os; //string stream
         //Base: Coordinates(x,y)       Vehicles in base: <int>
         os << "---->  Base:    Coordinates:(" << sim_map->get_base_x() << "," 
                         << sim_map->get_base_y() << ")      Vehicles in base: " << sim_map->get_base_id() << "  <----";
         write_on_panel(os.str(), 0, top_offset+20, font);
         //clear os stream
         os.str("");
         os.clear();
         //Resources Goal:     Palladium: <int> , Iridium: <int> , Platinum: <int>
         os << "Resources Goal:      Palladium: " << sim_map->get_pa_goal() << ",  Iridium: " << sim_map->get_ir_goal() 
                         << ",  Platinum: " << sim_map->get_pl_goal();
         write_on_panel(os.str(), 0, top_offset+2, font);
         //clear os stream
         os.str("");
         os.clear();
         //Resources in Base:     Palladium: <int> , Iridium: <int> , Platinum: <int>
         os << "Resources in Base:      Palladium: " << sim_map->get_base_pa() << ",  Iridium: " << sim_map->get_base_ir() 
                         << ",  Platinum: " << sim_map->get_base_pl();
         write_on_panel(os.str(), 0, top_offset+2, font);
}

//write map information
void map_general_panel::write_map_information(){
         if(font == NULL || general_panel_surface == NULL) return;
         //make output string
         std::ostringstream os; //string stream
         //Map:  Number of Mines: <int>,   Number of Flags: <int>,   Average Danger: <float>
         os << "---->  Map:    Number of Mines: " << sim_map->get_num_mines() << ",  Number of Flags: " << sim_map->get_num_flags()
                         << "  Average Danger: " << sim_map->get_all_danger() << "  <----";
         write_on_panel(os.str(), 0, top_offset+20, font);
         //clear os stream
         os.str("");
         os.clear();
         //All Map Resources:     Palladium: <int> , Iridium: <int> , Platinum: <int>
         os << "All Map Resources:   Palladium: " << sim_map->get_all_pa()<< ",  Iridium: " << sim_map->get_all_ir() 
                         << ",  Platinum: " << sim_map->get_all_pl();
         write_on_panel(os.str(), 0, top_offset+2, font);
}

//write vehicles information
void map_general_panel::write_vehicles_information(){
         if(font == NULL || general_panel_surface == NULL) return;
         //make output string
         std::ostringstream os; //string stream
         //Vehicles:   Alive Vehicles: <int>,  Damaged Vehicles: <int>,  Dead Vehicles: <int>
         os << "---->  Vehicles:   Alive Vehicles: " << vehicle::get_num_alive_vehicles() << ",  Damaged Vehicles: "
                         << vehicle::get_num_damaged_vehicles() << ",  Dead Vehicles: " 
                         << (vehicle::get_num_vehicles() - vehicle::get_num_alive_vehicles()) << "  <----";
         write_on_panel(os.str(), 0, top_offset+20, font);
         //clear os stream
         os.str("");
         os.clear();
         //Alive/Dead Miners: <int>/<int>,   Alive/Dead Scouts: <int>/<int>,   Alive/Dead Rescuers: <int>/<int>
         os << "Alive/Dead Miners: " << vehicle::get_num_alive_miners() << "/" << (vehicle::get_num_miners() - vehicle::get_num_alive_miners())
                         << ",   Alive/Dead Scouts: " << vehicle::get_num_alive_scouts() << "/" 
                         << (vehicle::get_num_scouts() - vehicle::get_num_alive_scouts())
                         << ",   Alive/Dead Rescuers: " << vehicle::get_num_alive_rescuers() << "/" 
                         << (vehicle::get_num_rescuers() - vehicle::get_num_alive_rescuers());
         write_on_panel(os.str(), 0, top_offset+2, font);
         //clear os stream
         os.str("");
         os.clear();
         //Miners' Extracted Rescources:    Palladium: <int> , Iridium: <int> , Platinum: <int>
         os << "Miners' Extracted Rescources:    Palladium: " << miner::get_all_pa_extracted() << ",  Iridium: " << miner::get_all_ir_extracted() 
                         << ",  Platinum: " << miner::get_all_pl_extracted();
         write_on_panel(os.str(), 0, top_offset+2, font);
         //clear os stream
         os.str("");
         os.clear();
         //Rescuers' Rescued Vehicles: <int>
         os << "Rescuers' Rescued Vehicles: " << rescuer::get_all_num_rescued();
         write_on_panel(os.str(), 0, top_offset+2, font);
}


void map_general_panel::clean_up(){
     //clear general panel surface
     SDL_FreeSurface( general_panel_surface );
     
     //close font
     TTF_CloseFont( font );
     TTF_CloseFont( header_font );
}


void map_general_panel::write_on_panel(std::string str, int x, int y, TTF_Font* f){
     if(f == NULL || general_panel_surface == NULL) return;
     SDL_Surface* msg = TTF_RenderText_Solid( f, str.c_str(), textColorBlack );
     //align msg top center
     int w,h; //width and height of text
     TTF_SizeText(f, str.c_str(), &w, &h);
     rect.x = (GENERAL_PANEL_WIDTH - w)/2 - x;
     rect.y = y;
     rect.w = 0;
     rect.h = 0;
     //increase top offset
     top_offset = h + rect.y;
     //put text surface
     SDL_BlitSurface( msg, NULL, general_panel_surface, &rect );
     //clear msg
     SDL_FreeSurface( msg );
}
