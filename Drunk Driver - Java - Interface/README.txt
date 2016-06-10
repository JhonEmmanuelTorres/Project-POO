
*******************************************************************
*                             EXECUTE                             *
*******************************************************************

This program was made in java, therefore is necesary to have it 
installed.

#########
# LINUX #
#########

# to compile #
$ make

#  to run   #
$ sh run.sh


  //////////////////////////////////
 //         OTHERS OS            //
//////////////////////////////////

# to compile #
$ javac -d classes -cp classes/jsfml.jar game/*.java

#  to run   #
$ java -cp classes:classes/jsfml.jar game.DrunkDriver
