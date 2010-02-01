#include "RumbleManager.h"

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "Globals.h"

#define uint16_t unsigned short
#define uint32_t unsigned short

#define BITS_PER_LONG (sizeof(long) * 8)
#define OFF(x)  ((x)%BITS_PER_LONG)
#define BIT(x)  (1UL<<OFF(x))
#define LONG(x) ((x)/BITS_PER_LONG)
#define test_bit(bit, array)    ((array[LONG(bit)] >> OFF(bit)) & 1)

using namespace Ogre;

template<> RumbleManager* Ogre::Singleton<RumbleManager>::ms_Singleton = 0;
RumbleManager* RumbleManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
RumbleManager& RumbleManager::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}


RumbleManager::RumbleManager()
{
    _initialized = false;

    LogManager::getSingleton().logMessage("RubbleManager: Searching input devices for Force Feedback support");


    for(int i=0; i<=9;i++)
    {
        String device;

        device = "/dev/input/event" + StringConverter::toString(i);



        /*#if XDRILLER_DEBUG == 1
            LogManager::getSingleton().logMessage("RubbleManager: Opening " + device);
        #endif*/

        fd = open(device.c_str(), O_RDWR);

        if(fd != -1)
        {
            unsigned long features[4];

            if (ioctl(fd, EVIOCGBIT(EV_FF, sizeof(unsigned long) * 4), features) == -1)
            {
                LogManager::getSingleton().logMessage("RubbleManager: " + device + " has no Force Feedback support");
                break;
            }
            else
            {
                if (test_bit(FF_RUMBLE, features))
                {
                    _initialized = true;
                    LogManager::getSingleton().logMessage("RubbleManager: Force Feedback device with rumble support found on " + device);
                    break;
                }
                else
                {
                    LogManager::getSingleton().logMessage("RubbleManager: " + device + " has no rumble support");
                    close(fd);
                }

                /*#if XDRILLER_DEBUG == 1
                    LogManager::getSingleton().logMessage("RubbleManager: " + device + "does not support FF_RUMBLE");
                #endif*/

            }
        }
        /*#if XDRILLER_DEBUG == 1
        else
        {
            LogManager::getSingleton().logMessage("RubbleManager: Error reading " + device);
        }
        #endif*/
    }


	if(_initialized)
	{
        effects[0].type = FF_RUMBLE;
        effects[0].id = -1;
        effects[0].u.rumble.strong_magnitude = 0;
        effects[0].u.rumble.weak_magnitude = 0x6000;
        effects[0].replay.length = 200;
        effects[0].replay.delay = 0;

        if (ioctl(fd, EVIOCSFF, &effects[0]) == -1)
        {
            LogManager::getSingleton().logMessage("RubbleManager: Effect 0 upload failed");
        }
        effects[1].type = FF_RUMBLE;
        effects[1].id = -1;
        effects[1].u.rumble.strong_magnitude = 0x6000;
        effects[1].u.rumble.weak_magnitude = 0;
        effects[1].replay.length = 200;
        effects[1].replay.delay = 0;

        if (ioctl(fd, EVIOCSFF, &effects[1]) == -1)
        {
            LogManager::getSingleton().logMessage("RubbleManager: Effect 1 upload failed");
        }
        effects[2].type = FF_RUMBLE;
        effects[2].id = -1;
        effects[2].u.rumble.strong_magnitude = 0;
        effects[2].u.rumble.weak_magnitude = 0x8000;
        effects[2].replay.length = 1000;
        effects[2].replay.delay = 0;

        if (ioctl(fd, EVIOCSFF, &effects[2]) == -1)
        {
            LogManager::getSingleton().logMessage("RubbleManager: Effect 2 upload failed");
        }
        effects[3].type = FF_RUMBLE;
        effects[3].id = -1;
        effects[3].u.rumble.strong_magnitude = 0x8000;
        effects[3].u.rumble.weak_magnitude = 0;
        effects[3].replay.length = 1000;
        effects[3].replay.delay = 0;

        if (ioctl(fd, EVIOCSFF, &effects[3]) == -1)
        {
            LogManager::getSingleton().logMessage("RubbleManager: Effect 3 upload failed");
        }

	}
	else
	{
        LogManager::getSingleton().logMessage("RubbleManager: No force Force Feedback device found");
	}
}

RumbleManager::~RumbleManager()
{
    close(fd);
}


int RumbleManager::playEffect(int e)
{
    if(!_initialized) return 1;

    play.type = EV_FF;
    play.code = effects[e].id;
    play.value = 1;

    if (write(fd, (const void*) &play, sizeof(play)) == -1)
    {
        return -1;
    }

    return 0;

}
