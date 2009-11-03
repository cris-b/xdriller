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
    private:

        Ogre::String name;

        Ogre::Entity *mEnt;
        Ogre::SceneNode *mNode;

};

#endif // RINGSWITCHEROBJECT_H
