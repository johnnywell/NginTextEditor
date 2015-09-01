//
//  main.cpp
//  NginTextEditor
//
//  Created by Johnny Wellington dos Santos on 8/31/15.
//  Copyright (c) 2015 Johnny Wellington dos Santos. All rights reserved.
//

/* pager functionality by Joseph Spainhour" <spainhou@bellsouth.net> */
#include <ncurses.h>
#include <stdlib.h>

void load_file(WINDOW *p, FILE* f, char* filename);

#define WIDE 100


int main(int argc, char *argv[])
{
    WINDOW *p;
    FILE *f;
    int ch;
    initscr();
    savetty();
    noecho();//disable auto-echoing
    cbreak();//making getch() work without a buffer I.E. raw characters
    keypad(stdscr,TRUE);//allows use of special keys, namely the arrow keys
    clear();    // empty the screen
    timeout(0);
    
    /* create a new pad */
    p = newpad(WIDE,COLS);
    keypad(p, TRUE);
    if( p == NULL )
        printf("Unable to create new pad\n");
    
    load_file(p, f, argv[1]); // open and load the file on the screen
    
    
    /* display the pad’s contents on the screen */
    prefresh(p,0, 0, 0,0, LINES-1,COLS-1);
    int c, cursorY=0, cursorX=0, linseCount = 0;
    while((c=wgetch(p))!='q'){
        switch(c){
            case KEY_LEFT:
                if(cursorX != 0)
                    cursorX--;
                break;
            case KEY_RIGHT:
                if (cursorX != COLS-1)
                    cursorX++;
                break;
            case KEY_UP:
                if(cursorY==0  && linseCount==0)
                    break;
                else if(cursorY == linseCount){
                    cursorY--;
                    linseCount--;
                    prefresh(p,linseCount, 0, 0,0, LINES-1,COLS-1);
                }
                else
                    cursorY--;
                break;
            case KEY_DOWN:
                if(cursorY==LINES-1  && linseCount==WIDE-1)
                    break;
                else if(cursorY == linseCount+LINES-1){
                    cursorY++;
                    linseCount++;
                    prefresh(p,linseCount, 0, 0,0, LINES-1,COLS-1);
                }
                else
                    cursorY++;
                break;
        }
        wmove(p,cursorY,cursorX);
        prefresh(p,linseCount, 0, 0,0, LINES-1,COLS-1);
    }
    endwin();
    return 0;
}

void load_file(WINDOW *p, FILE* f, char* filename){
    
    int ch;
    
    f = fopen(filename, "r");
    if(f == NULL)
    {
        perror("Cannot open input file");
        exit(1);
    }
    
    while( (ch=fgetc(f)) != EOF)
        waddch(p,ch);
    
    fclose(f);
}