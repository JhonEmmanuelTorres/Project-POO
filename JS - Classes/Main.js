// contants
const RIGHT = 0;
const DOWN  = 1;
const LEFT  = 2;
const UP    = 3;

// variable
var text = "";

// class Environment
function Environment()
{
    this.board = "XXXXXXXXXXXX         XX xxx xxx XX x     x XX x x x x XX    x    XX x x x x XX x     x XX xxx xxx XX         XXXXXXXXXXXX";
}

// methods
Environment.prototype.setBoard = function( position, image )
{
    this.board = this.board.substring( 0, position ) + image + this.board.substring( position + 1 );
}

Environment.prototype.isInTheBoard = function( position )
{
    return this.board[ position ] == "X" || this.board[ position ] == "x";
}

Environment.prototype.print = function()
{
    process.stdout.write( "\n\n" );
    for( var i = 0; i < 11; i++ )
        console.log( "\t\t\t" + this.board.substring( i*11, (i + 1) * 11 ) );
    console.log( text );
    text = "";
    process.stdout.write( "\n\t\t   Press enter please:\t" );
}

var city = new Environment();

// class Player
function Player()
{
    this.initialPosition = true;
    do {
        this.position = Math.floor( (Math.random() * 108) + 12 );
    }while( city.isInTheBoard( this.position ) );
}

// methods
Player.prototype.firstMoveMade = function()
{
    this.initialPosition = false;
}

Player.prototype.isInitialPosition = function()
{
    return this.initialPosition;
}

Player.prototype.evaluationOnePosition = function( positionOnBoard, barrierPlayer )
{
    if( positionOnBoard + this.position == barrierPlayer || city.isInTheBoard( positionOnBoard + this.position ) )
        return + this.position;
    return this.position + positionOnBoard;
}

Player.prototype.evaluationTwoPosition = function( positionOnBoard, barrierPlayer, address )
{
    if( (2 * positionOnBoard) + this.position == barrierPlayer  )
        return this.position;
    if( this.position == this.evaluationOnePosition(positionOnBoard, barrierPlayer) )
        return this.position;
    else
    {
        if( this.position == this.evaluationOnePosition( 2 * positionOnBoard, barrierPlayer ) )
        {
            var firstPositionPossible, secondPositionPossible;
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
            if( this.position != this.evaluationOnePosition(firstPositionPossible, barrierPlayer) )
            {
                text += "\n\t\t\tSpecial movement!!!";
                return this.position + firstPositionPossible;
            }
            if( this.position != this.evaluationOnePosition(secondPositionPossible, barrierPlayer) )
            {
                text += "\n\t\t\tSpecial movement!!!";
                return this.position + secondPositionPossible;
            }
        }
        else
            return (2 * positionOnBoard) + this.position;
    }
    return this.position;
}

Player.prototype.movementPositionOneOrTwo = function( barrierPlayer, quantityBox )
{
    var positionOnBoard, positionCurrent, positionRandom = Math.floor( Math.random() * 3 ); // 0 - 3
    for( var i = 0; i < 4; i++ )
    {
        positionCurrent = this.position;
        text += "\n   Movement";
        text += quantityBox == 1 ? " one " : " two ";
        text += "box ";
        switch( positionRandom )
        {
            case RIGHT:
                text += "right";
                positionOnBoard = 1;
                break;
            case DOWN:
                text += "down";
                positionOnBoard = 11;
                break;
            case LEFT:
                text += "left";
                positionOnBoard = -1;
                break;
            case UP:
                text += "up";
                positionOnBoard = -11;
                break;
        }
        if( quantityBox == 1 )
            this.position = this.evaluationOnePosition( positionOnBoard, barrierPlayer );
        else
            this.position = this.evaluationTwoPosition( positionOnBoard, barrierPlayer, positionRandom );
        if( positionCurrent == this.position )
        {
            positionRandom = (positionRandom + 1) % 4;
            text += ". This movement is not possible!!!";
        }
        else
            return this.position;
    }
    return this.position;
}

// class Deliquent
function Deliquent()
{
    Player.call( this );
    this.turn = -1;
    this.bottleActivated = false;
    this.bottle;
}

// inheritance
Deliquent.prototype = Object.create( Player.prototype );
Deliquent.prototype.constructor = Player;

// methods
Deliquent.prototype.activateBottle = function()
{
    this.bottleActivated = true;
}

Deliquent.prototype.deactivateBottle = function()
{
    this.bottleActivated = false;
}

Deliquent.prototype.bottleIsActivated = function()
{
    return this.bottleActivated;
};

Deliquent.prototype.isMyturn = function()
{
    this.turn++;
    return this.turn % 2 == 0;
}

Deliquent.prototype.isCornered = function()
{
    return this.position == 49 || this.position == 59 || this.position == 61 || this.position == 70;
}

Deliquent.prototype.movement = function()
{
    this.position = this.movementPositionOneOrTwo( police.position, Math.floor( (Math.random() * 2) + 1 ) );
}

Deliquent.prototype.authorityIsNear = function()
{
    return this.position + 1 == police.position || this.position - 1 == police.position || this.position + 11 == police.position || this.position - 11 == police.position;
}

Deliquent.prototype.escape = function()
{
    this.position = this.movementPositionOneOrTwo( police.position, 2 );
}

var drunk = new Deliquent();

// class Authority
function Authority()
{
    Player.call( this );
    this.rows    = "            111111111               22222               3333 4444               55555               666666666";
    this.columns = "            1   3   6  1   3   6  1 2 3 5 6  1 2 3 5 6  1 2   5 6  1 2 4 5 6  1 2 4 5 6  1   4   6  1   4   6";
    this.possiblePos = [ -1, 1, 2, -2, 11, -11, 22, -22 ];
    while( city.isInTheBoard( this.position ) || this.position == drunk.position )
        position = Math.floor( (Math.random() * 108) + 12 );
}

// 	inheritance

Authority.prototype = Object.create( Player.prototype );
Authority.prototype.constructor = Player;

// methods
Authority.prototype.losedTurn = function()
{
    this.position = drunk.bottle;
}

Authority.prototype.canCatchTheDeliquent = function()
{
    return this.position == drunk.position + 1 || this.position == drunk.position - 1 || this.position == drunk.position + 2 || this.position == drunk.position - 2 || this.position == drunk.position + 11 || this.position == drunk.position - 11 || this.position == drunk.position + 22 || this.position == drunk.position - 22;
}

Authority.prototype.detectedTheEnemy = function()
{
    return this.detectDrunk() == "R" || this.detectDrunk() == "C";
}

Authority.prototype.movement = function()
{
    this.position = this.movementPositionOneOrTwo( drunk.position, 1 );
}

Authority.prototype.detectDrunk = function()
{
    if( this.rows[ drunk.position ] == this.rows[ this.position ] && this.rows[ this.position ] != " " )
        return "R";
    else if( this.columns[ drunk.position ] == this.columns[ this.position ] && this.columns[ this.position ] != " " )
        return "C";
    return "N";
}

Authority.prototype.persecution = function()
{
    text += "\n\t\tSpecial movement two box ";
    if( this.detectDrunk() == 'R' )
    {
        if( this.position < drunk.position )
        {
            this.position += 2;
            text += "right";
        }
        else {
            this.position -= 2;
            text += "left";
        }
    }
    else {
        if( this.position < drunk.position )
        {
            this.position += 22;
            text += "up";
        }
        else{
            this.position -= 22;
            text += "down";
        }
    }
    text += "!!!";
}

var police = new Authority();

// logic game

process.stdout.write( "\n\t\tWELCOME TO THE DRIVER DRUNK!!!" );
city.print();

const readline = require( 'readline' );

const rl = readline.createInterface( {
    input: process.stdin,
    output: process.stdout
} );

rl.on( 'line', ( input ) => {
    main();
} );

function main()
{
    process.stdout.write( '\033c' ); // clear screen
    process.stdout.write(  "\n\t\t " );
    if( police.isInitialPosition() || drunk.isInitialPosition() )
    {
        if( drunk.isMyturn() )
        {
            process.stdout.write( "RANDOM POSITION OF DRUNK" );
            city.setBoard( drunk.position, "D" );
            drunk.firstMoveMade();
        }
        else {
            process.stdout.write( "RANDOM POSITION OF POLICE" );
            city.setBoard( police.position, "P" );
            police.firstMoveMade();
        }
    }
    else {

        if( drunk.isMyturn() )
        {
            city.setBoard( drunk.position, " " );
            if( drunk.authorityIsNear() )
            {
                if( drunk.isCornered() && police.canCatchTheDeliquent() )
                {
                    process.stdout.write( "DRUNK CORNERED. GAME OVER" );
                    city.setBoard( police.position, " " );
                    city.setBoard( drunk.position, "C" );
                    city.print();
                    process.exit( 1 );
                }
                else {
                    process.stdout.write( "BOTTLE ACTIVATED!!!" );
                    drunk.bottle = drunk.position;
                    city.setBoard( drunk.bottle, "B" );
                    drunk.escape();
                    drunk.activateBottle();
                }
            }
            else {
                process.stdout.write( "DRUNK'S TURN" );
                drunk.movement();
            }
            city.setBoard( drunk.position, "D" );
        }
        else { // Turn's police
            city.setBoard( police.position, " " );
            if( drunk.bottleIsActivated() )
            {
                process.stdout.write( "POLICE LOST HIS TURN" );
                police.losedTurn();
                drunk.deactivateBottle();
            }
            else if( police.detectedTheEnemy() && !drunk.bottleIsActivated() )
            {
                if( police.canCatchTheDeliquent() )
                {
                    process.stdout.write( "DRUNCK CAUGHT. GAME OVER" );
                    city.setBoard( drunk.position, "C" );
                    city.print();
                    process.exit( 1 );
                }
                else {
                    process.stdout.write( "DRUNK DETECTED" );
                    police.persecution();
                }
            }
            else {
                process.stdout.write( "POLICE'S TURN" );
                police.movement();
            }
            city.setBoard( police.position, "P" );
        }
    }
    city.print();
}
