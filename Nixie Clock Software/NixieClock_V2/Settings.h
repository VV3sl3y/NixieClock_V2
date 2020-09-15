#include <EEPROM.h>
/*
   Debugmode: If needed, this makes sure feedback is received via Serial
   mode 1: Minimalistic: (Check functionallity) Just the basic returns like time, date connection
   mode 2: Intermediate: (Check for errors) Debugging purposes when the clock does not work
   mode 3: Full: (Dive deep) returns all possible states and information during the programm in case mode 2 seems fine
*/
#define DebugMode 3

#define NumberOfNixies 6

#define BRIGHTNESS  32

#define timezone 1      // change to your timezone

//Times are in milliseconds
#define DisplayTimeUpdateInterval 100
#define DisplayDateUpdateInterval 900
#define SwitchDateTimeInterval 300000

//Tweakable settings for on/off time for each nixie for refresh vs brightness
#define NIXIE_OFF_TIME 100
#define NIXIE_ON_TIME 900

/*
   AnimationPCP: Makes sure feedback is received via Serial
   mode 1: Counting up (using a counter to fill the number values ascending)
   mode 2: Counting down (using a counter to fill the number values descending)
   mode 3: Randomized (using a randomizer to fill the number values)
*/
#define AnimationPCP  1
#define cyclePCP 20
#define PCP_INTERVAL 50
