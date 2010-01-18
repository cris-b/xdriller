#include "RumbleManager.h"

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

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



	fd = open("/dev/input/event9", O_RDWR);
	if (fd == -1) {
		LogManager::getSingleton().logMessage("RubbleManager: Open device file");
		return;
	}
	else
	{
        effects[0].type = FF_RUMBLE;
        effects[0].id = -1;
        effects[0].u.rumble.strong_magnitude = 0x5000;
        effects[0].u.rumble.weak_magnitude = 0;
        effects[0].replay.length = 1000;
        effects[0].replay.delay = 0;

        if (ioctl(fd, EVIOCSFF, &effects[0]) == -1) {
            LogManager::getSingleton().logMessage("RubbleManager: Effect upload failed");
        }

	}
}

RumbleManager::~RumbleManager()
{
    //dtor
}


int RumbleManager::playEffect(int e)
{

    play.type = EV_FF;
    play.code = effects[e].id;
    play.value = 1;

    if (write(fd, (const void*) &play, sizeof(play)) == -1) {
        return 1;
    }

    return 0;

}
