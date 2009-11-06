#ifndef RINGSWITCHER_H
#define RINGSWITCHER_H

#include <vector>
#include <Ogre.h>

#include "RingSwitcherObject.h"


class RingSwitcher
{
    public:
        RingSwitcher(float radius = 1.7);
        ~RingSwitcher();

        void addObject(const Ogre::String &name,const Ogre::String &mesh);

        void update( unsigned long lTimeElapsed );

        void next();
        void prev();

        Ogre::String getCurrentName();

        void setPosition(Ogre::Real x,Ogre::Real y,Ogre::Real z) {mNode->setPosition(x,y,z);}

    private:

        float radius;

        int selected;

        float angle;

        float desired_angle;


        Ogre::SceneNode *mNode;

        std::vector<RingSwitcherObject*> objects;


};

#endif // RINGSWITCHER_H
