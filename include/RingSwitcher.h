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

    private:

        float radius;

        int selected;


        Ogre::SceneNode *mNode;

        std::vector<RingSwitcherObject*> objects;


};

#endif // RINGSWITCHER_H
