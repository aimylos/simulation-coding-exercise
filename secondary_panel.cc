#include "map.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "vehicle.h"
#include "scout.h"
#include "miner.h"
#include "rescuer.h"
#include "secondary_panel.h"
#include <string>
#include <sstream>

#define SECONDARY_PANEL_WIDTH 700
#define SECONDARY_PANEL_HEIGHT 400
#define SCREEN_BPP 32

secondary_panel::secondary_panel(map* m){

         //simulation map
         sim_map = m;
         
         //initialize parameters
         vehicle_id = 0;
         map_x = 0;
         map_y = 0;
         
         //top offset
         top_offset = 0;
         
         //load font and color
         font = TTF_OpenFont( "arial.ttf", 20 ); //if didn't load then no fonts will appear
         header_font = TTF_OpenFont( "arial.ttf", 30 );
         SDL_Color textColor = { 0, 0, 0 };
         textColorBlack = textColor;

         //make white surface
         secondary_panel_surface = SDL_CreateRGBSurface(0, SECONDARY_PANEL_WIDTH, SECONDARY_PANEL_HEIGHT, SCREEN_BPP, 0, 0, 0, 0);
         
         //update general panel
         secondary_panel_state = 0;
         clear_panel();
}

SDL_Surface* secondary_panel::get_secondary_panel_surface(){
             return secondary_panel_surface;
}

void secondary_panel::clean_up(){
     //clear general panel surface
     SDL_FreeSurface( secondary_panel_surface );
     
     //close font
     TTF_CloseFont( font );
     TTF_CloseFont( header_font );
}

void secondary_panel::update_secondary_panel(){
     switch(secondary_panel_state){
            case 0:
                 break;
            case 1:
                 write_vehicle(vehicle_id);
                 break;
            case 2:
                 write_map_pos(map_x, map_y);
                 break;
            default:
                 clear_panel();
     }
}

void secondary_panel::clear_panel(){
     if (secondary_panel_surface == NULL) return;
     rect.w = SECONDARY_PANEL_WIDTH;
     rect.h = SECONDARY_PANEL_HEIGHT;
     rect.x = 0;
     rect.y = 0;
     SDL_FillRect(secondary_panel_surface, &rect, SDL_MapRGB((secondary_panel_surface)->format, 255, 255, 255));
     
     secondary_panel_state = 0;
}

void secondary_panel::write_vehicle(int id){
     if (secondary_panel_surface == NULL) return;
     //clean panel
     clear_panel();
     
     //keep vehicle id
     vehicle_id = id;
     
     //get vehicle address
     vehicle* v_address = vehicle::get_vehicle_address(vehicle_id);
     if(v_address==NULL) return;
     
     //set secondary panel state to 1
     secondary_panel_state = 1;
     
     //make output string
     std::ostringstream os; //string stream
     
     //set top_offset until last write height
     top_offset = 0;
     
     //Vehicle Information
     write_on_panel("Vehicle Information", 0, top_offset+15, header_font);
     
     //id: <int>    Type: Scout/Miner/Rescuer (1/2/3)   Status: Alive/Dead     In Map: YES/NO
     os << "ID: " << vehicle_id << "        Type: ";
     switch(v_address->get_type()){
           case 1:
                os << "Scout (1)";
                break;
           case 2:
                os << "Miner (2)";
                break;
           case 3:
                os << "Rescuer (3)";
                break;
           case 0:
           default:
                os << "No type (0)";
     }
     os << "        Status: ";
     if(v_address->get_status()) os << "Alive";
     else os << "Dead";
     os << "        In Map: ";
     if(v_address->is_in_map()) os <<"YES";
     else os << "NO";
     write_on_panel(os.str(), 0, top_offset+40, font);
     //clear os stream
     os.str("");
     os.clear();
     
     //Damaged: YES/NO    Times Damaged: <int>    Access Coefficient: <float>
     os << "Damaged: ";
     if(v_address->get_damaged()) os << "YES";
     else os << "NO";
     os << "        Times Damaged: " << v_address->get_times_damaged() << "        Access Coefficient: " << v_address->get_access();
     write_on_panel(os.str(), 0, top_offset+20, font);
     //clear os stream
     os.str("");
     os.clear();
     
     //Current Position: (<int>,<int>)    Positions Moved: <int>    Speed: <int>
     os << "Current Position: (" << v_address->get_pos_x() << "," << v_address->get_pos_y() << ")        Positions Moved: " 
                   << v_address->get_moved_positions() << "        Speed: " << v_address->get_speed();
     write_on_panel(os.str(), 0, top_offset+20, font);
     //clear os stream
     os.str("");
     os.clear();
     
     //Use of !!!risky cast!!!
     switch(v_address->get_type()){
             case 1:
                  //Number of Flags Planted: <int>
                  os << "Number of Flags Planted: " << dynamic_cast<scout*>(v_address)->get_num_flags();
                  write_on_panel(os.str(), 0, top_offset+20, font);
                  //clear os stream
                  os.str(""); os.clear();
                  break;
             case 2:
                  //Miner's Load:     Palladium: <int>     Iridium: <int>     Platinum: <int>
                  os << "Miner's Load:     Palladium: " << dynamic_cast<miner*>(v_address)->get_pa_load() << "     Iridium: "
                     << dynamic_cast<miner*>(v_address)->get_ir_load() << "     Platinum: " << dynamic_cast<miner*>(v_address)->get_pl_load();
                  write_on_panel(os.str(), 0, top_offset+20, font);
                  //clear os stream
                  os.str(""); os.clear();
                  
                  //Miner's All Extracted Resources: Palladium: <int>  Iridium: <int>  Platinum: <int>
                  os << "Miner's All Extracted Resources: Palladium: " << dynamic_cast<miner*>(v_address)->get_pa_extracted() << "  Iridium: "
                     << dynamic_cast<miner*>(v_address)->get_ir_extracted() << "  Platinum: " << dynamic_cast<miner*>(v_address)->get_pl_extracted();
                  write_on_panel(os.str(), 0, top_offset+20, font);
                  //clear os stream
                  os.str(""); os.clear();
                  break;
             case 3:
                  //Number of Vehicles Rescued: <int>
                  os << "Number of Vehicles Rescued: " << dynamic_cast<rescuer*>(v_address)->get_num_rescued();
                  write_on_panel(os.str(), 0, top_offset+20, font);
                  //clear os stream
                  os.str(""); os.clear();
                  break;
     }
}

void secondary_panel::write_map_pos(int x, int y){
     if (secondary_panel_surface == NULL) return;
     
     //clean panel
     clear_panel();
     
     //check map address
     if(sim_map==NULL) return;
     
     //check if position exists
     if(!sim_map->check_coord(x,y)) return;
     
     //keep map position
     map_x = x;
     map_y = y;
     
     //get position type
     int pos_type = sim_map->get_block_type(map_x,map_y);
     
     //set secondary panel state to 2
     secondary_panel_state = 2;
     
     //make output string
     std::ostringstream os; //string stream
     
     //set top_offset until last write height
     top_offset = 0;
     
     //Border/Road/Mine/Base Information
     switch(pos_type){
            case 1:
                 write_on_panel("Base Information", 0, top_offset+15, header_font);
                 break;
            case 2:
                 write_on_panel("Mine Information", 0, top_offset+15, header_font);
                 break;
            case 3:
                 write_on_panel("Road Information", 0, top_offset+15, header_font);
                 break;
            case 0:
            default:
                 write_on_panel("Border Information", 0, top_offset+15, header_font);
     }

     //Position x,y: (<int>,<int>)     Vehicle on Position: YES/NO     ID:/COUNT: <int>
     os << "Position x,y: (" << map_x << "," << map_y << ")       Vehicle on Position: " << ((sim_map->get_id(map_x,map_y)>0)?"YES":"NO");
     os << ((pos_type==1)?"       COUNT: ":"       ID: ");
     os << ((pos_type==0)?0:sim_map->get_id(map_x,map_y));
     write_on_panel(os.str(), 0, top_offset+30, font);
     //clear os stream
     os.str(""); os.clear();
     
     if(pos_type == 1){
           //Base resources:     Palladium: <int>     Iridium: <int>     Platinum: <int>
           os << "Base resources:     Palladium: " << sim_map->get_base_pa() << "     Iridium: "
                       << sim_map->get_base_ir() << "     Platinum: " << sim_map->get_base_pl();
           write_on_panel(os.str(), 0, top_offset+20, font);
           //clear os stream
           os.str(""); os.clear();
                       
           //Base/Simulation Goal:     Palladium: <int>     Iridium: <int>     Platinum: <int>
           os << "Base/Simulation Goal:    Palladium: " << sim_map->get_pa_goal() << "    Iridium: "
                       << sim_map->get_ir_goal() << "    Platinum: " << sim_map->get_pl_goal();
           write_on_panel(os.str(), 0, top_offset+20, font);
           //clear os stream
           os.str(""); os.clear();
           
           //Goal Completed: <float> %
           os << "Goal Completed: " << sim_map->goal_completed() << "%";
           write_on_panel(os.str(), 0, top_offset+20, font);
           //clear os stream
           os.str(""); os.clear();
     }
     else if(pos_type == 2){
           //Mine available resources:    Palladium: <int>    Iridium: <int>    Platinum: <int>
           os << "Mine available resources:    Palladium: " << sim_map->get_pa(map_x,map_y) << "     Iridium: "
                       << sim_map->get_ir(map_x,map_y) << "     Platinum: " << sim_map->get_pl(map_x,map_y);
           write_on_panel(os.str(), 0, top_offset+20, font);
           //clear os stream
           os.str(""); os.clear();
           
           //Flaged Mine: YES/NO     Danger Coefficient: <float>
           os << "Flaged Mine: " << (sim_map->has_flag(map_x,map_y)?"YES":"NO") << "     Danger Coefficient: "
                       << sim_map->get_danger(map_x,map_y);
           write_on_panel(os.str(), 0, top_offset+20, font);
           //clear os stream
           os.str(""); os.clear();
     }
}

void secondary_panel::write_on_panel(std::string str, int x, int y, TTF_Font* f){
     if(f == NULL || secondary_panel_surface == NULL) return;
     SDL_Surface* msg = TTF_RenderText_Solid( f, str.c_str(), textColorBlack );
     //align msg top center
     int w,h; //width and height of text
     TTF_SizeText(f, str.c_str(), &w, &h);
     rect.x = (SECONDARY_PANEL_WIDTH - w)/2 - x; //starting from the middle
     rect.y = y;
     rect.w = 0;
     rect.h = 0;
     //increase top offset
     top_offset = h + rect.y;
     //put text surface
     SDL_BlitSurface( msg, NULL, secondary_panel_surface, &rect );
     //clear msg
     SDL_FreeSurface( msg );
}
