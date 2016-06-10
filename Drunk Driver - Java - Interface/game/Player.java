package game;

import java.util.Scanner;
import java.util.Random;
import java.io.IOException;
import java.io.File;

public class Player
{
    public boolean initialPosition;
    protected byte position;
    protected char[] board;
    public static final byte RIGHT = 0;
    public static final byte DOWN = 1;
    public static final byte LEFT = 2;
    public static final byte UP = 3;

    // Constructor
    public Player()
    {
        initialPosition = true;
        board = new char[ 121 ];
        try{
            Scanner is = new Scanner( new File( "files/board" ) );
            board = is.nextLine().toCharArray();
            Random r = new Random();
            do{
                position = (byte)( r.nextInt(96) + 12 );
            }while ( board[ position ] == 'X' );
        }
        catch( IOException e ){
            e.printStackTrace();
        }
    }

    // methods get
    public byte getPosition(){ return position; }
    public boolean isInitialPosition(){ return initialPosition; }
    public boolean isInTheBoard( byte position )
    {
        return board[ position ] == 'X' || board[ position ] == 'x' ? true : false;
    }

    // methods set
    public void firstMoveMade(){ initialPosition = false; }

    // methods protected
    protected byte evaluationOnePosition( byte positionOnBoard, byte barrierPlayer )
    {
        if( positionOnBoard + position == barrierPlayer || isInTheBoard( (byte)(positionOnBoard + position ) ) )
            return position;
        return (byte)(positionOnBoard + position );
    }

    protected byte evaluationTwoPosition( byte positionOnBoard, byte barrierPlayer, byte address )
    {
        if( (2 * positionOnBoard) + position == barrierPlayer  )
            return position;
        if( position == evaluationOnePosition(positionOnBoard, barrierPlayer) )
            return position;
        else
        {
            if( position == evaluationOnePosition( (byte)(2 * positionOnBoard), barrierPlayer ) )
            {
                byte firstPositionPossible = 0, secondPositionPossible = 0;
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
                if( position != evaluationOnePosition( firstPositionPossible, barrierPlayer) )
                {
                    GraphicInterface.text.append( "\t\t\tSpecial movement!!!" );
                    return (byte)(position + firstPositionPossible);
                }

                if( position != evaluationOnePosition(secondPositionPossible, barrierPlayer) )
                {
                    GraphicInterface.text.append( "\t\t\tSpecial movement!!!" );
                    return (byte)(position + secondPositionPossible);
                }
            }
            else
                return (byte)(2 * positionOnBoard + position);
        }
        return position;
    }

    protected byte movementPositionOneOrTwo( byte barrierPlayer, byte quantityBox )
    {
        Random r = new Random();
        byte positionOnBoard = 0, positionCurrent, positionRandom = (byte)r.nextInt( 4 ); // 0 - 3
        for( byte i = 0; i < 4; i++ )
        {
            positionCurrent = position;
            GraphicInterface.text.append( "\n   Movement" );
            if( quantityBox == 1 )
                GraphicInterface.text.append( " one " );
            else
                GraphicInterface.text.append( " two " );
            GraphicInterface.text.append( "box " );
            switch( positionRandom )
            {
                case RIGHT:
                    GraphicInterface.text.append( "right" );
                    positionOnBoard = 1;
                    break;
                case DOWN:
                    GraphicInterface.text.append( "down" );
                    positionOnBoard = 11;
                    break;
                case LEFT:
                    GraphicInterface.text.append( "left" );
                    positionOnBoard = -1;
                    break;
                case UP:
                    GraphicInterface.text.append( "up" );
                    positionOnBoard = -11;
                    break;
            }
            if( quantityBox == 1 )
                position = evaluationOnePosition( positionOnBoard, barrierPlayer );
            else
                position = evaluationTwoPosition( positionOnBoard, barrierPlayer, positionRandom );
            if( positionCurrent == position )
            {
                positionRandom = (byte)((positionRandom + 1) % 4);
                GraphicInterface.text.append( ". This movement is not possible!!!" );
            }
            else
                return position;
        }
        return position;
    }
}
