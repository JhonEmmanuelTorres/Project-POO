package game;
import java.util.Random;

public class Deliquent extends Player
{
    // fields
    private byte bottle, turn;
    private boolean bottleActivated;

    // Constructor
    public Deliquent()
    {
        turn = 0;
        bottleActivated = false;
    }

    // methods get
    public byte getBottle(){ return bottle; }
    public boolean bottleIsActivated() { return bottleActivated; }

    // methods set
    public void setBottle( byte newBottle ){ bottle = newBottle; }
    public void activateBottle(){ bottleActivated = true; }
    public void deactivateBottle(){ bottleActivated = false; }

    // methods
    public boolean isMyturn()
    {
        return turn++ % 2 == 0 ? true : false;
    }

    public boolean isCornered()
    {
        return position == 49 || position == 59 || position == 61 || position == 71;
    }

    public void movement( Authority police )
    {
        Random r = new Random();
        position = movementPositionOneOrTwo( police.getPosition(), (byte)( r.nextInt(2) + 1 )  );
    }

    public boolean authorityIsNear( Authority police )
    {
        return position + 1 == police.getPosition() || position - 1 == police.getPosition() || position + 11 == police.getPosition() || position - 11 == police.getPosition();
    }

    public void escape( Authority police )
    {
        Random r = new Random();
        position = movementPositionOneOrTwo( police.getPosition(), (byte)( r.nextInt(2) + 1 )  );
    }
}
