#ifndef RINGSWITCHEROBJECT_H
#define RINGSWITCHEROBJECT_H

#include <Ogre.h>

#include "EntityMaterialInstance.h"

class RingSwitcherObject
{
    public:
        RingSwitcherObject(const Ogre::String &name, const Ogre::String &mesh, Ogre::SceneNode *parent);
        ~RingSwitcherObject();

        void setPosition(const Ogre::Vector3 &pos);

        Ogre::String getName() {return name;}

        void setScale(Ogre::Real s) {mNode->setScale(s,s,s);}

        void setBlocked(bool _blocked);
        bool isBlocked();

        void setMaterialName(const Ogre::String &material);


    private:

        bool blocked;

        Ogre::String name;

        Ogre::Entity *mEnt;
        Ogre::SceneNode *mNode;

        EntityMaterialInstance *mEmi;
};

#endif // RINGSWITCHEROBJECT_H
