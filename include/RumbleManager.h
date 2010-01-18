#ifndef RumbleManager_H
#define RumbleManager_H

#include <Ogre.h>
#include <OgreSingleton.h>


#include <linux/input.h>

class RumbleManager : public Ogre::Singleton<RumbleManager>
{
    public:
        RumbleManager();
        ~RumbleManager();

        static RumbleManager& getSingleton(void);
        static RumbleManager* getSingletonPtr(void);

        int playEffect(int e);

    private:

        int fd;

        struct ff_effect effects[1];
        struct input_event play, stop;

};

#endif // RumbleManager_H
