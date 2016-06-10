#include <ncurses.h>
#include <stdlib.h>  // Random
#include <time.h>    // Different values random
#include <stdbool.h> // to declare bool
#include <string.h>  // strcpy
enum address { RIGHT, DOWN, LEFT, UP };
typedef char byte;

void printBoard( char[] );
char detectDrunk( byte, byte, char[], char[] );
byte findFirstPosition( byte, char [] );
byte persecution( byte, byte, char[], char[] );
byte evaluationOnePosition( byte, byte, byte, char[] );
byte movementPositionOneOrTwo( byte, byte, byte, char[] );
byte evaluationTwoPosition( byte, byte, byte, byte, char[] );

int main()
{
    initscr();			        // Start curses mode
    raw();				        // Line buffering disabled
    keypad( stdscr, true );		// We get F1, F2 etc..
    noecho();			        // Don't echo() while we do getch
    // init the logical's game
    srand( time(NULL) ); // Values random
    byte Drunk, Police, Bottle, turn = 0;
    bool bottleActivated = false, initialPositions = true;
    char *board = (char *)malloc( 121 * sizeof(char) ), *columns = (char *)malloc( 109 * sizeof(char) ), *rows = ( char *)malloc( 109 * sizeof(char) );
    strcpy( board, "XXXXXXXXXXXX         XX XXX XXX XX X     X XX X X X X XX    X    XX X X X X XX X     X XX XXX XXX XX         XXXXXXXXXXXX" );
    strcpy( rows,  "            111111111               22222               3333 4444               55555               666666666" );
    strcpy( columns, "            1   3   6  1   3   6  1 2 3 5 6  1 2 3 5 6  1 2   5 6  1 2 4 5 6  1 2 4 5 6  1   4   6  1   4   6" );
    printw( "\n\t\t\tWelcome to the Drunk Driver!!!" );
    printBoard( board );
    while( true )
    {
        if( getch() == KEY_DOWN )
        {
            erase();
            printw( "\n\t\t\t" );
            if( initialPositions )
            {
                if( turn++ % 2 == 0 )
                {
                    printw( "Random position of Drunk" );
                    Drunk = findFirstPosition( 0, board );
                    board[ Drunk ] = 'D';
                }
                else{
                    printw( "Random position of Police" );
                    Police = findFirstPosition( Drunk, board );
                    board[ Police ] = 'P';
                    initialPositions = false;
                }
            }
            else{
                if( turn++ % 2 == 0 ) // Drunk's turn
                {
                    board[ Drunk ] = ' ';
                    if( Drunk + 1 == Police || Drunk - 1 == Police || Drunk + 11 == Police || Drunk - 11 == Police )
                    {
                        if( Drunk == 49 || Drunk == 59 || Drunk == 61 || Drunk == 70 )
                        {
                            printw( "Drunk cornered. Game Over" );
                            board[ Police ] = ' ';
                            board[ Drunk ] = 'A';
                            printBoard( board );
                            break;
                        }
                        else{
                            printw( "Bottle activated!!!" );
                            Bottle = Drunk;
                            board[ Bottle ] = 'B';
                            Drunk = movementPositionOneOrTwo( Drunk, Police, 2, board ); // Police moved two boxes
                            bottleActivated = true;
                        }
                    }
                    else{
                        printw( "Drunk's turn\n" );
                        Drunk = movementPositionOneOrTwo( Drunk, Police, rand()%2 + 1, board );
                    }
                    board[ Drunk ] = 'D';
                }
                else // Police's turn
                {
                    board[ Police ] = ' ';
                    if( bottleActivated )
                    {
                        printw( "Police loses his turn" );
                        Police = Bottle;
                        bottleActivated = false;
                    }
                    else if( detectDrunk(Drunk,Police,columns, rows) == 'R' || detectDrunk (Drunk,Police,columns, rows) == 'C' && !bottleActivated )
                    {
                        if( Police == Drunk + 1 || Police == Drunk - 1 || Police == Drunk + 2 || Police == Drunk - 2 || Police == Drunk + 11 || Police == Drunk - 11 || Police == Drunk + 22 || Police == Drunk - 22  )
                        {
                            printw( "Drunk caught. Game Over" );
                            board[ Drunk ] = 'A';
                            printBoard( board );
                            break;
                        }
                        else{
                            printw( "Drunk detected \n" );
                            Police = persecution( Drunk, Police, columns, rows );
                        }
                    }
                    else{
                        printw( "Police's turn\n" );
                        Police = movementPositionOneOrTwo( Police, Drunk, 1, board );
                    }
                    board[ Police ] = 'P';
                }
            }
            printBoard( board );
            refresh();
        }
    }
    free( board );
    free( columns );
    free( rows );
    refresh();			// Print it in the real screen
    getch();			// Wait for user input
    endwin();			// End curses mode
    return 0;
}

void printBoard( char board[ 121 ] )
{
    byte j = 0;
    printw( "\n\n\n\t\t\t" );
    for( byte i = 0; i < 11; i++ )
    {
        do{
            if( board[ j ] == 'X' ){
                addch(ACS_CKBOARD);  addch(ACS_CKBOARD);  addch(ACS_CKBOARD);
            }
            else if( board[ j ] == 'B' || board[ j ] == 'D' || board[ j ] == 'P' || board [ j ] == 'A' )
                printw( "%c%c%c", ' ', board[ j ], ' ' );
            else
                printw( "%c%c%c", board[ j ], board[ j ], board[ j ] );
        }while( ++j % 11 != 0 );
        printw( "\n\t\t\t" );
    }
    printw( "\n\t\t\tPress the key down please:\t" );
}

byte findFirstPosition( byte playerBarrier, char board[ 121 ] )
{
    byte Player = rand()%108 + 12;
    while( board[ Player ] == 'X' || playerBarrier == Player)
        Player = rand()%108 + 12;
    return Player;
}

byte movementPositionOneOrTwo( byte Player, byte barrierPlayer, byte quantityBox, char board[ 121 ] )
{
    byte positionOnBoard, positionCurrent, positionRandom = rand()%4; // 0 - 3
    for( byte i = 0; i < 4; i++ )
    {
        positionCurrent = Player;
        printw( "\n   Movement" );
        if( quantityBox == 1 )
            printw( " one " );
        else
            printw( " two " );
        printw( "box " );
        switch( positionRandom )
        {
            case RIGHT:
                printw( "right" );
                positionOnBoard = 1;
                break;
            case DOWN:
                printw( "down" );
                positionOnBoard = 11;
                break;
            case LEFT:
                printw( "left" );
                positionOnBoard = -1;
                break;
            case UP:
                printw( "up" );
                positionOnBoard = -11;
                break;
        }
        if( quantityBox == 1 )
            Player = evaluationOnePosition( positionOnBoard, Player, barrierPlayer, board );
        else
            Player = evaluationTwoPosition( positionOnBoard, Player, barrierPlayer, positionRandom, board );
        if( positionCurrent == Player )
        {
            positionRandom = (positionRandom + 1) % 4;
            printw( ". This movement is not possible!!!" );
        }
        else
            return Player;
    }
    return Player;
}

byte evaluationOnePosition( byte positionOnBoard, byte Player, byte barrierPlayer, char board[ 121 ] )
{
    if( positionOnBoard + Player == barrierPlayer || board[ Player + positionOnBoard ] == 'X' )
        return Player;
    return Player + positionOnBoard;
}

byte evaluationTwoPosition(byte positionOnBoard, byte Player, byte barrierPlayer, byte address, char board[ 121 ] )
{
    if( (2 * positionOnBoard) + Player == barrierPlayer  )
        return Player;
    if( Player == evaluationOnePosition(positionOnBoard,Player,barrierPlayer,board) )
        return Player;
    else
    {
        if( Player == evaluationOnePosition( 2 * positionOnBoard, Player, barrierPlayer, board) )
        {
            byte firstPositionPossible, secondPositionPossible;
            switch( address )
            {
                case RIGHT:
                    firstPositionPossible = -10;
                    secondPositionPossible = 12;
                    break;
                case DOWN:
                    firstPositionPossible = 12;
                    secondPositionPossible = 10;
                    break;
                case LEFT:
                    firstPositionPossible = 10;
                    secondPositionPossible = -12;
                    break;
                case UP:
                    firstPositionPossible = -12;
                    secondPositionPossible = -10;
                    break;
            }
            if( Player != evaluationOnePosition(firstPositionPossible, Player, barrierPlayer, board) )
            {
                printw( "\n\n\t\t\tSpecial movement!!!" );
                return Player + firstPositionPossible;
            }

            if( Player != evaluationOnePosition(secondPositionPossible, Player, barrierPlayer, board) )
            {
                printw( "\n\n\t\t\tSpecial movement!!!" );
                return Player + secondPositionPossible;
            }
        }
        else
            return (2 * positionOnBoard) + Player;
    }
    return Player;
}

char detectDrunk( byte Drunk, byte Police, char columns[ 109 ], char rows[ 109 ] )
{
    if( rows[ Drunk ] == rows[ Police ] && rows[ Drunk ] != ' ' )
        return 'R';
    else if( columns[ Drunk ] == columns[ Police ] && columns[ Police ] != ' ' )
        return 'C';
    else
        return 'N'; //The players aren't in the same row or column
}

byte persecution( byte Drunk, byte Police, char columns[ 109 ], char rows[ 109 ] )
{
    printw( "\n\t\tSpecial movement two box " );
    if( detectDrunk( Drunk, Police, columns, rows ) == 'R' )
    {
        if( Police < Drunk )
        {
            Police += 2;
            printw( "right" );
        }
        else{
            Police -= 2;
            printw( "left" );
        }
    }
    else{
        if( Police < Drunk )
        {
            Police += 22;
            printw( "up" );
        }
        else{
            Police -= 22;
            printw( "down" );
        }
    }
    printw( "!!!" );
    return Police;
}
