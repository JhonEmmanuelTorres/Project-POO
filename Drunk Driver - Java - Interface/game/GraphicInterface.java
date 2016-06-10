package game;

import org.jsfml.graphics.RenderWindow;
import org.jsfml.audio.Sound;
import org.jsfml.audio.SoundBuffer;
import org.jsfml.window.event.Event;
import org.jsfml.window.Keyboard;
import org.jsfml.window.VideoMode;
import org.jsfml.graphics.Texture;
import org.jsfml.graphics.Sprite;
import org.jsfml.graphics.Color;
import org.jsfml.graphics.Font;
import org.jsfml.graphics.Text;
import java.nio.file.Paths;
import java.io.IOException;

public class GraphicInterface
{
    private boolean keyDownWasPressed;
    private boolean closeGame;
    private RenderWindow window;
    private Texture[] filesT;
    private Sprite[] filesS;
    private SoundBuffer[] gameSoundB;
    private Sound[] gameSoundS;
    private Font fontUbuntu;
    private Text mainT;
    private Text evaluation;
    public static StringBuilder text;


    public GraphicInterface( Deliquent drunk, Authority police )
    {
        window = new RenderWindow( new VideoMode(871, 826), "Drunk Driver" );
        window.setFramerateLimit( 30 );
        window.setVerticalSyncEnabled( true );
        keyDownWasPressed = closeGame = false;
        fontUbuntu = new Font();
        filesT = new Texture[ 5 ];
        filesS = new Sprite[ 5 ];
        gameSoundB = new SoundBuffer[ 3 ];
        gameSoundS = new Sound[ 3 ];
        for( byte i = 0; i < 5; i++ )
            filesT[i] = new Texture();
        for( byte i = 0; i < 3; i++ )
            gameSoundB[i] = new  SoundBuffer();

        try{
            filesT[0].loadFromFile( Paths.get("files/drunk.png") );
            filesT[1].loadFromFile( Paths.get("files/police.png") );
            filesT[2].loadFromFile( Paths.get("files/bottle.png") );
            filesT[3].loadFromFile( Paths.get("files/board.png") );
            filesT[4].loadFromFile( Paths.get("files/drunkCaptured.png") );
            gameSoundB[0].loadFromFile( Paths.get("files/hooter.ogg") );
            gameSoundB[1].loadFromFile( Paths.get("files/brokenBottle.ogg") );
            gameSoundB[2].loadFromFile( Paths.get("files/braking.ogg") );
            fontUbuntu.loadFromFile( Paths.get("files/Ubuntu-L.ttf") );
        }
        catch( IOException e ){
            e.printStackTrace();
        }
        filesS[0] = new Sprite( filesT[0] );
        filesS[1] = new Sprite( filesT[1] );
        filesS[2] = new Sprite( filesT[2] );
        filesS[3] = new Sprite( filesT[3] );
        filesS[4] = new Sprite( filesT[4] );
        gameSoundS[0] = new Sound( gameSoundB[0] );
        gameSoundS[1] = new Sound( gameSoundB[1] );
        gameSoundS[2] = new Sound( gameSoundB[2] );
        mainT = new Text( "", fontUbuntu, 24 );
        mainT.setPosition( 670, 300 );
        mainT.setColor( Color.BLACK );
        mainT.setStyle( Text.BOLD );
        text = new StringBuilder();
        text.append( "\n\n\t\t\t\t\t\t\t\t\t\t\t\tWELCOME!!!" );
        evaluation = new Text( text.toString(), fontUbuntu, 24 );
        evaluation.setPosition( 120, 660 );
        evaluation.setColor( Color.BLACK );
    }

    public void events( )
    {
        for( Event event: window.pollEvents() )
        {
            if( event.type == Event.Type.CLOSED )
                    window.close();

            else if( event.type == Event.Type.KEY_PRESSED && Keyboard.isKeyPressed( Keyboard.Key.DOWN ) )
                keyDownWasPressed = true;
        }
    }

    public void render( Deliquent drunk, Authority police )
    {
        window.clear();
        window.draw(filesS[3]);
        filesS[1].setPosition( setPositionBoardX( police.getPosition() ) + 4, setPositionBoardY( police.getPosition() ) + 11 );
        if( closeGame )
        {
            filesS[4].setPosition( setPositionBoardX( drunk.getPosition() ), setPositionBoardY( drunk.getPosition() ) );
            window.draw( filesS[4] );
        }
        else
            filesS[0].setPosition( setPositionBoardX( drunk.getPosition() ) + 4, setPositionBoardY( drunk.getPosition() ) + 20 );

        if( !police.isInitialPosition() && !closeGame )
            window.draw(filesS[1]);
        if( !drunk.isInitialPosition() && !closeGame  )
            window.draw(filesS[0]);

        if( drunk.bottleIsActivated() )
        {
            filesS[2].setPosition( setPositionBoardX( drunk.getBottle() ) + 4, setPositionBoardY( drunk.getBottle() ) + 4 );
            window.draw( filesS[2] );
        }
        window.draw( evaluation );
        window.draw( mainT );
        window.display();
    }


    public void update( Deliquent drunk, Authority police )
    {
        if( keyDownWasPressed )
        {
            keyDownWasPressed = false;
            text = new StringBuilder(); // Se borra el texto
            if( police.isInitialPosition() || drunk.isInitialPosition() )
            {
                if( drunk.isMyturn() )
                {
                    mainT.setString( "RANDOM \nPOSITION \nOF DRUCK" );
                    drunk.firstMoveMade();
                }
                else{
                    mainT.setString( "RANDOM \nPOSITION \nOF POLICE" );
                    police.firstMoveMade();
                }
            }
            else{
                if( drunk.isMyturn() ) // Drunk's turn
                {
                    if( drunk.authorityIsNear( police ) )
                    {
                        if( drunk.isCornered() && police.canCatchTheDeliquent( drunk ) )
                        {
                            mainT.setString( "DRUNK CORNERED.\n GAME OVER" );
                            closeGame = true;
                        }
                        else{
                            mainT.setString( "  BOTTLE \nACTIVATED!!!" );
                            drunk.setBottle( drunk.getPosition() );
                            drunk.escape( police );// two boxes
                            drunk.activateBottle();
                            gameSoundS[1].play(); // brokenBottle
                        }
                    }
                    else{
                        mainT.setString( "DRUNK'S \n  TURN" );
                        drunk.movement( police );
                    }
                }
                else{ // Turn's police
                    if( drunk.bottleIsActivated() )
                    {
                        mainT.setString( "POLICE LOST \n  HIS TURN" );
                        police.losedTurn( drunk );
                        drunk.deactivateBottle();
                        gameSoundS[2].play(); // braking
                    }
                    else if( closeGame )
                        window.close();

                    else if( police.detectedTheEnemy( drunk ) && !drunk.bottleIsActivated() )
                    {
                        if( police.canCatchTheDeliquent( drunk ) )
                        {
                            mainT.setString( "   DRUNCK \n   CAUGHT. \nGAME OVER" );
                            drunk.isMyturn();
                            closeGame = true;
                        }
                        else{
                            mainT.setString( "  DRUNK\n DETECTED" );
                            police.persecution( drunk );
                            gameSoundS[0].play(); // hooter
                        }
                    }

                    else{
                        mainT.setString( "POLICE'S\n  TURN" );
                        police.movement( drunk );
                    }
                }
            }
            evaluation.setString( text.toString() );
        }
    }

    public boolean isOpen()
    {
        return window.isOpen();
    }

    private float setPositionBoardX( byte position )
    {
        return (float)( (position % 11) * 60 );
    }

    private float setPositionBoardY( byte position )
    {
        return (float)( (position / 11) * 60 );
    }
}
