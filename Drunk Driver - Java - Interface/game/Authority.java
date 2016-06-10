package game;
import java.util.Scanner;
import java.util.Random;
import java.io.File;
import java.io.IOException;

public class Authority extends Player
{
    // fields
    private char[] rows, columns;

    // Constructor
    public Authority( Deliquent drunk )
    {
        rows = new char[ 109 ];
        columns = new char[ 109 ];
        try{
            Scanner is = new Scanner( new File( "files/rows" ) );
            rows = is.nextLine().toCharArray();
            is = new Scanner( new File( "files/columns" ) );
            columns = is.nextLine().toCharArray();
        }
        catch( IOException e ){
            e.printStackTrace();
        }
        Random r = new Random();
        while( board[ position ] == 'X' || position == drunk.getPosition() )
            position = (byte)(r.nextInt( 96 ) + 12);
    }

    // private methods

    private char detectDrunk( Deliquent drunk )
    {
        if( rows[ drunk.getPosition() ] == rows[ position ] && rows[ position ] != ' ' )
            return 'R';
        else if(  columns[ drunk.getPosition() ] == columns[ position ] && columns[ position ] != ' ' )
            return 'C';
        else
            return 'N'; //The players aren't in the same row or column
    }

    // publics methods

    public void losedTurn( Deliquent drunk )
    {
        position = drunk.getBottle();
    }

    public boolean canCatchTheDeliquent( Deliquent drunk )
    {
        return position == drunk.getPosition() + 1 || position == drunk.getPosition() - 1 || position == drunk.getPosition() + 2 || position == drunk.getPosition() - 2 || position == drunk.getPosition() + 11 || position == drunk.getPosition() - 11 || position == drunk.getPosition() + 22 || position == drunk.getPosition() - 22;
    }

    public boolean detectedTheEnemy( Deliquent drunk )
    {
        return detectDrunk(drunk) == 'R' || detectDrunk(drunk) == 'C' ? true : false;
    }

    public void movement( Deliquent drunk )
    {
        position = movementPositionOneOrTwo( drunk.getPosition(), (byte)1 );
    }

    public void persecution( Deliquent drunk )
    {
        GraphicInterface.text.append( "\n\t\tSpecial movement two box " );
        if( detectDrunk(drunk) == 'R' )
        {
            if( position < drunk.getPosition() )
            {
                position += 2;
                GraphicInterface.text.append( "right" );
            }
            else{
                position -= 2;
                GraphicInterface.text.append( "left" );
            }
        }
        else{
            if( position < drunk.getPosition() )
            {
                position += 22;
                GraphicInterface.text.append( "up" );
            }
            else{
                position -= 22;
                GraphicInterface.text.append( "down" );
            }
        }
        GraphicInterface.text.append( "!!!" );
    }

}
