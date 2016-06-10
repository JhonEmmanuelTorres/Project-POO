package game;

public class DrunkDriver
{
    public static void main(String[] args)
    {
        Deliquent drunk = new Deliquent();
        Authority police = new Authority( drunk );
        GraphicInterface drunkDriver = new GraphicInterface( drunk, police );
        while( drunkDriver.isOpen() )
        {
            drunkDriver.events();
            drunkDriver.render( drunk, police );
            drunkDriver.update( drunk, police );
        }
    }
}
