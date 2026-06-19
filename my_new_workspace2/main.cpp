//#include <ncurses.h>
#include <ncurses.h>
#include <curses.h>
#include <Option.h>
#include <Text_buffer.h>
#include <Window.h>
#include <Suboptions_Window.h>
#include <File_Dialog.h>
#include <Suboption.h>
#include <Write.h>
#include <Read.h>
#include <iostream>
#include <fstream>
//using namespace std;
#include <vector>

int columns = 0;
int rows = 0;
char text[] = "Plik";
int litera;
int x =0;
int y = 1;
int main()
{
    std::vector<Option*> Options;
    std::vector<Suboption*> File_Suboptions;
    std::vector<Suboption*> Edition_Suboptions;
    Suboption* Write1 = new Write();
    Suboption* Open1 = new Read();
    File_Suboptions.push_back(Write1);
    File_Suboptions.push_back(Open1);
    Suboption* Undo1 = new Write("Cofnij",6);
    Suboption* Redo1 = new Write("Ponow",5);
    Edition_Suboptions.push_back(Undo1);
    Edition_Suboptions.push_back(Redo1);
    Option* Plik = new Option("Plik",4,File_Suboptions);
    Option* Edycja = new Option("Edycja",6,Edition_Suboptions);
    Options.push_back(Plik);
    Options.push_back(Edycja);
    initscr();
    start_color();
    MEVENT event;
    mousemask(ALL_MOUSE_EVENTS,NULL);
    std::vector<Window*> Windows;
    getmaxyx( stdscr, rows, columns );
    keypad( stdscr, TRUE ); 
    
    init_pair(1,0,8);
    init_pair(2,0,4);     
    init_pair(3,0,7);                                                     
    attrset(COLOR_PAIR(1));    
    std::vector<int> Option_points;
    attrset(COLOR_PAIR(2));
    for(int i = 0; i < columns; i++){
        printw(" ");   

    }
    move(0,0);
    for (int i = 0; i < Options.size(); i++) {
        attrset(COLOR_PAIR(1));   
        printw(Options[i]->getName());
        attrset(COLOR_PAIR(2));   
        printw(" ");
    }
    move(y,x);
    attrset(COLOR_PAIR(3));
    for(int i = 0; i < columns; i++){
        for(int j = 1; j < rows; j++){
            printw(" ");   
        }
    }
    move(y,x);
    noecho(); 
    
    
    
    Text_buffer* current_buffer = new Text_buffer(columns,rows);
    Text_buffer* filename = new Text_buffer(23,1);
    Suboption* Featured_Suboption;
    Window* window1;
    bool dialog_displayed = false;
    do{
    

    current_buffer->writeToStaticVars(0,1);
    move(1,0);
    current_buffer->printOutBuffer();
    for(int i = 0; i < Windows.size(); i++){
     
        Windows[i]->displayWindow();
    }
    if(dialog_displayed == true){
        filename->writeToStaticVars(x,y);
        filename->printOutBuffer();
        move(y,x);

    }
    move(y,x);
    litera = getch();
    if(dialog_displayed == true){
       switch(litera){
           case KEY_MOUSE:
            if(getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED){
                if(event.y == rows/2 && x>((columns/2)-10) && x< ((columns/2)+10)){
                  x = event.x;
            } 
            }
           case 263:
                 filename->removeFromBuffer(litera,x,y-1); 
                 x-=1;
                 break;
           case 10:
                 Windows.pop_back();
                 dialog_displayed=false;
                 Featured_Suboption->startSuboption(current_buffer,filename);
           default:
             if(x< ((columns/2)+10)){

               filename->addToBuffer(litera,x-(columns/2)+11,0);
               x+=1;
           }
            
    }
    }
    else{
    switch(litera){
    case KEY_MOUSE:
      if(getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED){
          
          if(event.y != 0){
             y = event.y;
             x = event.x;
             if(!Windows.empty()){
                 if(x >=window1->getDimensions()[0] && x <=(window1->getDimensions()[0]+window1->getDimensions()[2])){
                    if(y >=window1->getDimensions()[1] && y <=(window1->getDimensions()[1]+window1->getDimensions()[3])){
                     Featured_Suboption = window1->getSuboptions()[y-(window1->getDimensions()[1])];
                     
                     vector<Suboption*> window_suboption;
                     window_suboption.push_back(Featured_Suboption);
                     window1 = new File_Dialog((columns/2)-12,(rows/2)-2,25,4,window_suboption);
                     Windows.pop_back();
                     Windows.push_back(window1);
                     dialog_displayed = true;
                     x = (columns/2)-10;
                     y = rows/2;
                     
                 } 
                 }
                 else{
                      Windows.pop_back();
                 }

             }  
          }
          else{
              if(!Windows.empty()){
                Windows.pop_back();  
              }
               int x6 = 0;
               int x_append = 0;
               for(int i =0; i<Options.size();i++){
                   x_append = Options[i]->getLength()+1;
                   if(x6+x_append >= event.x){
                       window1 = new Suboptions_Window(x6,1,10,2,Options[i]->getSuboptions());
                       Windows.push_back(window1);
                       break;
                   }
                   else{
                   x6+=x_append;
                   
                   }
                   
               }
               
          }
      }
    case 260:
        x-=1;
        break;
    case 261:
        x+=1;
        break;
    case 259:
        if(y!=1){
           y-=1;
        }
        break;
    case 258:
        y+=1;
        break;
    case 263:
        current_buffer->removeFromBuffer(litera,x,y-1); 
        x-=1;
        break;
    case 10:
        x=0;
        y+=1;

        break;
    default:
      if(y!=0){
      
    
        current_buffer->addToBuffer(litera,x,y-1);
        x+=1;
        
      }
    }
    }
    
    
    }while(1);
    for (int i = 0; i < Options.size(); i++) {
        delete Options[i];
    }
    endwin();
    return 0;
}