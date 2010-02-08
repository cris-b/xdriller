#ifndef RumbleManager_H
#define RumbleManager_H

#include <Ogre.h>
#include <OgreSingleton.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX

#include <linux/input.h>

class RumbleManager : public Ogre::Singleton<RumbleManager>
{
    public:

        enum Effects {SHORT_WEAK,SHORT_STRONG,LONG_WEAK,LONG_STRONG};

        RumbleManager();
        ~RumbleManager();

        static RumbleManager& getSingleton(void);
        static RumbleManager* getSingletonPtr(void);

        int playEffect(int e);

    private:

        int fd;

        bool _initialized;

        struct ff_effect effects[4];
        struct input_event play, stop;

};

#else

class RumbleManager : public Ogre::Singleton<RumbleManager>
{
    public:

        enum Effects {SHORT_WEAK,SHORT_STRONG,LONG_WEAK,LONG_STRONG};

        RumbleManager() {};
        ~RumbleManager() {};

        static RumbleManager& getSingleton(void);
        static RumbleManager* getSingletonPtr(void);

        int playEffect(int e) {return 0;};

};

#endif // OGRE_PLATFORM == OGRE_PLATFORM_LINUX


#endif // RumbleManager_H
