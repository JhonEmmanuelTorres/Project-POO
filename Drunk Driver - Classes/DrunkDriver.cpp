#include <ncurses.h>
using namespace std;
#include "classesDD.h"

int main()
{
    initscr();			        // Start curses mode
    raw();				        // Line buffering disabled
    keypad( stdscr, true );		// We get F1, F2 etc..
    noecho();			        // Don't echo() while we do getch
    if( !has_colors() )
    {
        printw( "Your terminal does not support color\n" );
        getch();
        endwin();
    }
    start_color();              // Start color
    init_pair( 1, COLOR_RED, COLOR_BLACK );
    init_pair( 2, COLOR_GREEN, COLOR_BLACK );
    init_pair( 3, COLOR_YELLOW, COLOR_BLACK );
    init_pair( 4, COLOR_BLUE , COLOR_BLACK );
    init_pair( 6, COLOR_CYAN, COLOR_BLACK );
    init_pair( 7, COLOR_WHITE, COLOR_BLACK );
    attron( COLOR_PAIR(1) );
    printw( "\n\t\t\tWELCOME TO THE DRIVER DRUNK!!!" );
    city->print();
    while( true )
    {
        if( getch() == KEY_DOWN )
        {
            erase();
            attron( COLOR_PAIR(1) );
            printw( "\n\t\t\t    " );
            if( police->isInitialPosition() || drunk->isInitialPosition() )
            {
                if( drunk->isMyturn() )
                {
                    printw( "RANDOM POSITION OF DRUCK" );
                    city->setBoard( drunk->getPosition(), 'D' );
                    drunk->firstMoveMade();

                }
                else{
                    printw( "RANDOM POSITION OF POLICE" );
                    city->setBoard( police->getPosition(), 'P' );
                    police->firstMoveMade();
                }
            }
            else{
                if( drunk->isMyturn() ) // Drunk's turn
                {
                    city->setBoard( drunk->getPosition(), ' ' );
                    if( drunk->authorityIsNear( ) )
                    {
                        if( drunk->isCornered() && police->canCatchTheDeliquent() )
                        {
                            printw( "DRUNK CORNERED. GAME OVER" );
                            city->setBoard( police->getPosition(), ' ' );
                            city->setBoard( drunk->getPosition(), 'A' );
                            city->print();
                            break;
                        }
                        else{
                            printw( "BOTTLE ACTIVATED!!!" );
                            drunk->setBottle( drunk->getPosition() );
                            city->setBoard( drunk->getBottle(), 'B' );
                            drunk->escape();// two boxes
                            drunk->activateBottle();
                        }
                    }
                    else{
                        printw( "DRUNK'S TURN" );
                        drunk->movement();
                    }
                    city->setBoard( drunk->getPosition(), 'D' );
                }
                else{ // Turn's police
                    city->setBoard( police->getPosition(), ' ' );
                    if( drunk->bottleIsActivated() )
                    {
                        printw( "POLICE LOST HIS TURN" );
                        police->losedTurn();
                        drunk->deactivateBottle();
                    }
                    else if( police->detectedTheEnemy() && !drunk->bottleIsActivated() )
                    {
                        if( police->canCatchTheDeliquent() )
                        {
                            printw( "DRUNCK CAUGHT. GAME OVER" );
                            city->setBoard( drunk->getPosition(), 'A' );
                            city->print();
                            break;
                        }
                        else{
                            printw( "DRUNK DETECTED" );
                            police->persecution();
                        }
                    }
                    else{
                        printw( "POLICE'S TURN" );
                        police->movement();
                    }
                    city->setBoard( police->getPosition(), 'P' );
                }
            }
            city->print();
            refresh();
        }
    }
    delete drunk;
    delete police;
    delete city;
    delete text;
    refresh();			// Print it in the real screen
    getch();			// Wait for user input
    endwin();			// End curses mode
    return 0;
}
