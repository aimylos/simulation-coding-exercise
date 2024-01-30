#include <iostream>
using std::cout;
using std::endl;
#include <cstdlib>
using std::rand;
using std::srand;
#include <ctime>
using std::time;

#include "block.h"


#define GOAL_MIN 60 //% of all resources available on map
#define GOAL_MAX 80
#define MAX_RESOURCES 70 //max resources per mine, assuming unit kg since realistic resources are low


//initialize static variables
int block::all_pa=0;
int block::all_ir=0;
int block::all_pl=0;
int block::num_borders=0;
int block::num_mines=0;
int block::num_roads=0;
float block::sum_danger=0.0;
int block::num_flags=0;
void block::reset_block_class(){
     all_pa=0;
     all_ir=0;
     all_pl=0;
     num_borders=0;
     num_mines=0;
     num_roads=0;
     sum_danger=0.0;
     num_flags=0;
}

//block constuctor
block::block(int x){
                 //if (all_pa == 0) srand(time(NULL)); //stop seeding after the first mine block has been created
                 switch (x) {
                        case 0:
                             type = 0; //border
                             num_borders++;
                             break;
                        case 1:
                             type = 1; //base
                             ir = 0; //doens't have any resources yet
                             pa = 0; //goal will be set later
                             pl = 0;
                             id = 0;
                             vehicle_type = 0;
                             break;
                        case 2:
                             type = 2; //mine
                             num_mines++;
                             //initialize resources and fanger coefficient
                             set_danger(-1.0);
                             flag = false;
                             pa = 0;
                             ir = 0;
                             pl = 0;
                             set_pa(-1);
                             set_ir(-1);
                             set_pl(-1);
                             id = 0; //0 means no vehicles
                             vehicle_type = 0;
                             break;
                        case 3:
                             type = 3; //road
                             num_roads++;
                             id = 0;
                             vehicle_type = 0;
                             break;
                        default:
                             type = 0;
                             num_borders++;
                 }
}
//end of constructors

// all types
int block::get_type(){
    return type;
}

int block::get_all_pa(){
    return all_pa;
}
int block::get_all_ir(){
    return all_ir;
}
int block::get_all_pl(){
    return all_pl;
}
float block::get_all_danger(){
    return sum_danger/num_mines;
}
int block::get_num_borders(){
    return num_borders;
}
int block::get_num_mines(){
    return num_mines;
}
int block::get_num_roads(){
    return num_roads;
}
int block::get_num_flags(){
    return num_flags;
}
// end of all types

//type 1,2,3
int block::get_id(){
    if (type==0) return 0;   
    return id;
}
//end of type 1,2,3

// type 0   border
//vehicles must not enter border blocks
// end of type 0

// type 1   base
int block::inc_id(){
    if (type!=1) return -1;
    else return id++;
}
int block::dec_id(){
    if (type!=1) return -1;
    else if (id>0) return --id;
    else return id;
}
bool block::set_goal(int q_pa, int q_ir, int q_pl){
     if (type!=1) return false;
     bool check = true;
     //palladium
     if (q_pa == -1){  //if -1 then random between GOAL_MIN GOAL_MAX
              int temp_perc = rand()%(GOAL_MAX - GOAL_MIN + 1) + GOAL_MIN;
              goal_pa = (temp_perc*all_pa)/100;
              }
     else if (q_pa > all_pa || q_pa < 0)  check = false;  //incorrect input
     else goal_pa = q_pa;
     //iridium
     if (q_ir == -1){
              goal_ir = (((rand() % (GOAL_MAX - GOAL_MIN + 1)) + GOAL_MIN)*all_ir)/100;
              }
     else if (q_ir > all_ir || q_ir < 0) check = false;
     else goal_ir = q_ir;
     //platinum
     if (q_pl == -1){
              goal_pl = (((rand() % (GOAL_MAX - GOAL_MIN + 1)) + GOAL_MIN)*all_pl)/100;
              }     
     else if (q_pl > all_pl || q_pl < 0) check = false;
     else goal_pl = q_pl;
     return check;
}

bool block::is_full(){
     if (type!=1) return false;
     if (pa >= goal_pa && ir >= goal_ir && pl >= goal_pl) return true;
     else return false;
}
bool block::is_full_pa(){
     if (type!=1) return false;
     if (pa >= goal_pa) return true;
     else return false;
}     
bool block::is_full_ir(){
     if (type!=1) return false;
     if (ir >= goal_ir) return true;
     else return false;
}
bool block::is_full_pl(){
     if (type!=1) return false;
     if (pl >= goal_pl) return true;
     else return false;
}
     
int block::unload_pa( int q_pa ){
     if (type!=1) return q_pa;
     pa += q_pa;
     if (is_full_pa()) {
            int left_pa = pa - goal_pa;
            pa -= left_pa;
            return left_pa;
            }
     else
         return 0;
}
int block::unload_ir( int q_ir ){
     if (type!=1) return q_ir;
     ir += q_ir;
     if (is_full_ir()) {
            int left_ir = ir - goal_ir;
            ir -= left_ir;
            return left_ir;
            }
     else
         return 0;
}
int block::unload_pl( int q_pl ){
     if (type!=1) return q_pl;
     pl += q_pl;
     if (is_full_pl()) {
            int left_pl = pl - goal_pl;
            pl -= left_pl;
            return left_pl;
            }
     else
         return 0;
}

int block::get_pa_goal(){
    if (type!=1) return 0;
    return goal_pa;
}
int block::get_ir_goal(){
    if (type!=1) return 0;
    return goal_ir;
}
int block::get_pl_goal(){
    if (type!=1) return 0;
    return goal_pl;
}
//end of type 1

//type 2   mine
bool block::set_flag(){
     if(type!=2) return false;
     if (flag == false) num_flags++;
     flag = true;
     return flag;
}
bool block::clear_flag(){
     if(type!=2) return false;
     if (flag == true) num_flags--;
     flag = false;
     return flag;
}
bool block::has_flag(){
     if(type!=2) return false;
     return flag;
}

bool block::is_empty(){
     if(type!=2) return true;
     else if(pa == 0 && ir == 0 && pl == 0) return true;
     else return false;
}

bool block::set_danger(float d){
      if(type!=2) return false;
      if (d < 0.0 || d > 0.9){
          danger = (double)rand() / RAND_MAX;
          if (danger > 0.9) danger = 0.9;
          }
      else
          danger = d;
      
      sum_danger += danger;
      return true;
}
float block::get_danger(){
      if(type!=2) return 0.0;
      return danger;
}
 

int block::extract_pa(int extract_rate){
     if (type!=2 || extract_rate<0) return 0;
     if (pa >= extract_rate){
           pa -= extract_rate;
           return extract_rate;
     } 
     else {
          extract_rate = pa;
          pa = 0;
          return extract_rate;
     }
}          
int block::extract_ir(int extract_rate){
     if (type!=2 || extract_rate<0) return 0;
     if (ir >= extract_rate){
           ir -= extract_rate;
           return extract_rate;
     } 
     else {
          extract_rate = ir;
          ir = 0;
          return extract_rate;
     }
}
int block::extract_pl(int extract_rate){
     if (type!=2 || extract_rate<0) return 0;
     if (pl >= extract_rate){
           pl -= extract_rate;
           return extract_rate;
     } 
     else {
          extract_rate = pl;
          pl = 0;
          return extract_rate;
     }
}
//end of type 2

//type 2 and 3    mine and road
int block::set_id(int x){
    if ((type!=2 && type!=3) || x<0) return -1;
    if (id!=0) return id;
    id = x;
    return x;
}

int block::clear_id(){
    if (type!=2 && type!=3) return -1;
    int temp = id;
    id = 0;
    return temp;
}

int block::clear_vehicle_type(){
    int temp = vehicle_type;
    vehicle_type = 0;
    return temp;
}
int block::set_vehicle_type(int x){
    if (type!=2 && type!=3) return -1;
    if(x < 0 || x > 3) return -1;
    vehicle_type = x;
    return vehicle_type;
}
int block::get_vehicle_type(){
    if (type!=2 && type!=3) return -1;
    return vehicle_type;
}
//end of type 2 and 3

//type 1 and 2    base and mine
int block::get_pa(){
    if (type!=1 && type!=2) return 0;
    return pa;
}
int block::get_ir(){
    if (type!=1 && type!=2) return 0;
    return ir;
}
int block::get_pl(){
    if (type!=1 && type!=2) return 0;
    return pl;
}

int block::set_pa(int x){
    if (type!=1 && type!=2) return 0;
    if (x==-1){
         all_pa -= pa;
         pa = rand() % (MAX_RESOURCES + 1) + 1;
         all_pa += pa;
         }
    else if (x>=0) {
         all_pa += x - pa;
         pa = x;
         }
    return pa;
}
int block::set_ir(int x){
    if (type!=1 && type!=2) return 0;
    if (x==-1){
         all_ir -= ir;
         ir = rand() % (MAX_RESOURCES + 1) + 1;
         all_ir += ir;
         }
    else if (x>=0) {
         all_ir += x - ir;
         ir = x;
         }
    return ir;
}
int block::set_pl(int x){
    if (type!=1 && type!=2) return 0;
    if (x==-1){
         all_pl -= pl;
         pl = rand() % (MAX_RESOURCES + 1) + 1;
         all_pl += pl;
         }
    else if (x>=0) {
         all_pl += x - pl;
         pl = x;
         }
    return pl;
}
//end of type 1 and 2








void block::print_block(){
     cout << type << " and " << danger << " and " << pa << " " << ir << " " << pl << endl;
}
