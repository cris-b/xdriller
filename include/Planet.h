#ifndef PLANET_H
#define PLANET_H


#include <Ogre.h>



class Planet
{
    public:
        Planet();
        ~Planet();

        void update( unsigned long lTimeElapsed );

        void setOrientation(Ogre::Quaternion q);

    private:

        float size;

        float angle;

        int selected;

        Ogre::SceneNode *mNode;

        Ogre::Entity *mEnt;

        Ogre::Quaternion from;
        Ogre::Quaternion to;

        Ogre::Real rot_progress;


};

#endif // RINGSWITCHER_H
