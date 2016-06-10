#include <ctime>    // Random
#include <fstream>  // streams

enum address { RIGHT, DOWN, LEFT, UP };
typedef char byte;
string *text = new string();

class Environment
{
private:
    char board[ 121 ];

public:
    // Constructor
    Environment()
    {
        ifstream is( "board" );
        if( !is.is_open() )
        {
            printw( "Unable to open the file: board\n" );
            getch();			// Wait for user input
            endwin();			// End curses mode
        }
        else{
            is.getline( board, 122 );
            is.close();
        }
    }

    // Methods
    void setBoard( byte position, char Image ){ board[ position ] = Image; }

    bool isInTheBoard( byte position )
    {
        return board[ position ] == 'X' || board[ position ] == 'x' ? true : false;
    }

    void print( )
    {
        byte j = 0;
        printw( "\n\n\t\t\t" );
        for( byte i = 0; i < 11; i++ )
        {
            do{
                switch ( board[ j ] ) {
                    case 'X':
                        attron( COLOR_PAIR(4) );
                        addch(ACS_CKBOARD);  addch(ACS_CKBOARD);  addch(ACS_CKBOARD);
                        break;
                    case 'x':
                        attron( COLOR_PAIR(6) );
                        addch(ACS_CKBOARD);  addch(ACS_CKBOARD);  addch(ACS_CKBOARD);
                        break;
                    case 'B':
                        attron( COLOR_PAIR(3) );
                        printw( " " );       addch(ACS_DIAMOND);   printw( " " );
                        break;
                    case ' ':
                        printw( "%c%c%c", board[ j ], board[ j ], board[ j ] );
                        break;
                    default: // for P and D
                        attron(COLOR_PAIR(1));
                        printw( "%c%c%c", ' ', board[ j ], ' ' );
                        break;
                }
            }while( ++j % 11 != 0 );
            printw( "\n\t\t\t" );
        }
        attron( COLOR_PAIR(2) );
        printw( "%s", text->c_str() );
        text->clear();
        attron( COLOR_PAIR(7) );
        printw( "\n\n\t\t\t   Press the key down please:\t" );
        attroff( COLOR_PAIR(7) );
    }

};

Environment *city = new Environment();

class Player
{
private:
    bool initialPosition;

protected:
    byte position;

    // Declaration of the methods
    byte evaluationOnePosition( byte positionOnBoard, byte  barrierPlayer );
    byte evaluationTwoPosition( byte positionOnBoard, byte barrierPlayer, byte address );
    byte movementPositionOneOrTwo( byte barrierPlayer, byte quantityBox );

public:
    // Constructor
    Player()
    {
        initialPosition = true;
        srand( time(NULL) ); // random values
        do{
            position = rand()%108 + 12;
        }while( city->isInTheBoard(position) );
    }

    // methods
    byte getPosition(){ return position; }
    void firstMoveMade(){ initialPosition = false; }
    bool isInitialPosition(){ return initialPosition; }
};

class Deliquent : public Player
{
private:
    byte bottle, turn;
    bool bottleActivated;

public:
    // Constructor
    Deliquent()
    {
        turn = 0;
        bottleActivated = false;
    }

    // Declaration of the methods
    void movement();
    void escape();
    bool authorityIsNear();

    //Methods
    byte getBottle(){ return bottle; }
    void setBottle( byte newBottle ){ bottle = newBottle; }
    void activateBottle(){ bottleActivated = true; }
    void deactivateBottle(){ bottleActivated = false; }
    bool bottleIsActivated() { return bottleActivated; }

    bool isMyturn()
    {
        return turn++ % 2 == 0 ? true : false;
    }

    bool isCornered()
    {
        return position == 49 || position == 59 || position == 61 || position == 70;
    }

};

Deliquent *drunk = new Deliquent();

class Authority : public Player
{
private:
    char rows[ 109 ], columns[ 109 ];

    //declare the method
    char detectDrunk();

public:
    // Constructor and destroyer
    Authority()
    {
        ifstream is( "rows" );
        if( !is.is_open() )
        {
            printw( "Unable to open the file: rows\n" );
            getch();			// Wait for user input
            endwin();			// End curses mode
        }
        else{
            is.getline( rows, 110 );
            is.close();
        }
        is.open( "columns" );
        if( !is.is_open() )
        {
            printw( "Unable to open the file: columns" );
            getch();			// Wait for user input
            endwin();			// End curses mode
        }
        else{
            is.getline( columns, 110 );
            is.close();
        }
        srand( time(NULL) ); // Values random
        while( city->isInTheBoard(position) || position == drunk->getPosition() )
            position = rand()%108 + 12;
    }

    // Declaration of the methods
    void movement();
    void persecution();

    // methods
    void losedTurn()
    {
        position = drunk->getBottle();
    }

    bool canCatchTheDeliquent()
    {
        return position == drunk->getPosition() + 1 || position == drunk->getPosition() - 1 || position == drunk->getPosition() + 2 || position == drunk->getPosition() - 2 || position == drunk->getPosition() + 11 || position == drunk->getPosition() - 11 || position == drunk->getPosition() + 22 || position == drunk->getPosition() - 22;
    }

    bool detectedTheEnemy()
    {
        return detectDrunk() == 'R' || detectDrunk() == 'C' ? true : false;
    }
};

Authority *police = new Authority();

// Player's methods

byte Player::evaluationOnePosition( byte positionOnBoard, byte barrierPlayer )
{
    if( positionOnBoard + position == barrierPlayer || city->isInTheBoard( positionOnBoard + position ) )
        return position;
    return position + positionOnBoard;
}

byte Player::evaluationTwoPosition( byte positionOnBoard, byte barrierPlayer, byte address )
{
    if( (2 * positionOnBoard) + position == barrierPlayer  )
        return position;
    if( position == evaluationOnePosition(positionOnBoard, barrierPlayer) )
        return position;
    else
    {
        if( position == evaluationOnePosition( 2 * positionOnBoard, barrierPlayer ) )
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
            if( position != evaluationOnePosition(firstPositionPossible, barrierPlayer) )
            {
                text->append( "\n\t\t\tSpecial movement!!!" );
                return position + firstPositionPossible;
            }

            if( position != evaluationOnePosition(secondPositionPossible, barrierPlayer) )
            {
                text->append( "\n\t\t\tSpecial movement!!!" );
                return position + secondPositionPossible;
            }
        }
        else
            return (2 * positionOnBoard) + position;
    }
    return position;
}


byte Player::movementPositionOneOrTwo( byte barrierPlayer, byte quantityBox )
{
    srand( time(NULL) ); // Random values
    byte positionOnBoard, positionCurrent, positionRandom = rand()%4; // 0 - 3
    for( byte i = 0; i < 4; i++ )
    {
        positionCurrent = position;
        text->append( "\n   Movement" );
        if( quantityBox == 1 )
            text->append( " one " );
        else
            text->append( " two " );
        text->append( "box " );
        switch( positionRandom )
        {
            case RIGHT:
                text->append( "right" );
                positionOnBoard = 1;
                break;
            case DOWN:
                text->append( "down" );
                positionOnBoard = 11;
                break;
            case LEFT:
                text->append( "left" );
                positionOnBoard = -1;
                break;
            case UP:
                text->append( "up" );
                positionOnBoard = -11;
                break;
        }
        if( quantityBox == 1 )
            position = evaluationOnePosition( positionOnBoard, barrierPlayer );
        else
            position = evaluationTwoPosition( positionOnBoard, barrierPlayer, positionRandom );
        if( positionCurrent == position )
        {
            positionRandom = (positionRandom + 1) % 4;
            text->append( ". This movement is not possible!!!" );
        }
        else
            return position;
    }
    return position;
}

// Deliquent's methods

void Deliquent::movement()
{
    srand( time(NULL) ); // Values random
    position = movementPositionOneOrTwo( police->getPosition(), rand()%2 + 1 );
}

bool Deliquent::authorityIsNear()
{
    return position + 1 == police->getPosition() || position - 1 == police->getPosition() || position + 11 == police->getPosition() || position - 11 == police->getPosition();
}

void Deliquent::escape()
{
    position = movementPositionOneOrTwo( police->getPosition(), 2 );
}

// Authority's methods

void Authority::movement()
{
    position = movementPositionOneOrTwo( drunk->getPosition(), 1 );
}


char Authority::detectDrunk( )
{
    if( rows[ drunk->getPosition() ] == rows[ position ] && rows[ position ] != ' ' )
        return 'R';
    else if(  columns[ drunk->getPosition() ] == columns[ position ] && columns[ position ] != ' ' )
        return 'C';
    else
        return 'N'; //The players aren't in the same row or column
}

void Authority::persecution( )
{
    text->append( "\n\t\tSpecial movement two box " );
    if( detectDrunk( ) == 'R' )
    {
        if( position < drunk->getPosition() )
        {
            position += 2;
            text->append( "right" );
        }
        else{
            position -= 2;
            text->append( "left" );
        }
    }
    else{
        if( position < drunk->getPosition() )
        {
            position += 22;
            text->append( "up" );
        }
        else{
            position -= 22;
            text->append( "down" );
        }
    }
    text->append( "!!!" );
}
