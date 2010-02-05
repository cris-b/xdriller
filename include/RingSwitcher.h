#ifndef RINGSWITCHER_H
#define RINGSWITCHER_H

#include <vector>
#include <Ogre.h>

#include "RingSwitcherObject.h"


class RingSwitcher
{
    public:
        RingSwitcher(float size = 5);
        ~RingSwitcher();

        void addObject(const Ogre::String &name,const Ogre::String &mesh);

        void update( unsigned long lTimeElapsed );

        void next();
        void prev();

        Ogre::String getCurrentName();

        void setPosition(Ogre::Real x,Ogre::Real y,Ogre::Real z) {mNode->setPosition(x,y,z);}

        void setBlocked(const Ogre::String &name,bool _blocked);
        bool isBlocked(const Ogre::String &name);

        void setMaterialName(const Ogre::String &name, const Ogre::String &material);

    private:

        float size;

        float angle;

        int selected;

        Ogre::SceneNode *mNode;

        Ogre::SceneNode *doubleArrowNode;
        Ogre::Entity *doubleArrowEnt;

        std::vector<RingSwitcherObject*> objects;


};

#endif // RINGSWITCHER_H
