#ifndef RINGSWITCHEROBJECT_H
#define RINGSWITCHEROBJECT_H

#include <Ogre.h>

class RingSwitcherObject
{
    public:
        RingSwitcherObject(const Ogre::String &name, const Ogre::String &mesh, Ogre::SceneNode *parent);
        ~RingSwitcherObject();

        void setPosition(const Ogre::Vector3 &pos);

        Ogre::String getName() {return name;}

        void setScale(Ogre::Real s) {mNode->setScale(s,s,s);}
    private:

        Ogre::String name;

        Ogre::Entity *mEnt;
        Ogre::SceneNode *mNode;

};

#endif // RINGSWITCHEROBJECT_H
