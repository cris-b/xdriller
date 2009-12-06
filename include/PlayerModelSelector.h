#ifndef PLAYERMODELSELECTOR_H
#define PLAYERMODELSELECTOR_H

#include <Ogre.h>


class PlayerModelSelector
{
    public:
        PlayerModelSelector();
        ~PlayerModelSelector();

        void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);

        void update( unsigned long lTimeElapsed );

        void prev();
        void next();
        void updateModel();

        Ogre::String getName();

    private:

        Ogre::SceneManager *mSceneMgr;

        int playerModelIndex;

        Ogre::SceneNode *mNode;
        Ogre::Entity    *mEnt;
        Ogre::SceneNode *mBaseNode;
        Ogre::Entity    *mBaseEnt;




        Ogre::AnimationState *mAnimationState;

};

#endif // PLAYERMODELSELECTOR_H
