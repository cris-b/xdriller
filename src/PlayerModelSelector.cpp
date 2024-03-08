#include "PlayerModelSelector.h"
#include "ConfigManager.h"


using namespace Ogre;


//sitio terrible pa definir esto
//deberia haber una clase playerLoader o algo asi que leyera de un archivo .cfg
#define NUM_PLAYER_MODELS               2

const char model_filenames[NUM_PLAYER_MODELS][10] = {"tux", "tom"};
const char model_names    [NUM_PLAYER_MODELS][10] = {"Tux", "Tom"};
//---

PlayerModelSelector::PlayerModelSelector()
{

    mSceneMgr = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" );

    //player model cual de la lista
    for(int i=0; i<NUM_PLAYER_MODELS; i++)
    {
        if(ConfigManager::getSingleton().getString("player_model") == model_filenames[i]) playerModelIndex = i;
    }

    String mesh_name = ConfigManager::getSingleton().getString("player_model") + ".mesh";

    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ModelSelectorNode");

    mNode->pitch(Degree(10));

    mEnt = mSceneMgr->createEntity("ModelSelectorEnt", mesh_name);
    mEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);
    mEnt->setCastShadows(true);

    mAnimationState = mEnt->getAnimationState("Idle");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);

    mNode->attachObject(mEnt);

    mNode->setScale(3,3,3);


    //setup base
    mBaseNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ModelSelectorBaseNode");

    mBaseNode->pitch(Degree(10));

    mBaseEnt = mSceneMgr->createEntity("ModelSelectorBaseEnt", "basecool.mesh");
    mBaseEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);
    mBaseEnt->setCastShadows(false);

    mBaseNode->attachObject(mBaseEnt);
    //mBaseNode->setScale(2,2,2);
    mBaseNode->translate(0,-1.5,0);

    Light *light = mSceneMgr->createLight("PlayerModelSelectorLight");
    light->setType(Light::LT_SPOTLIGHT);
    light->setDiffuseColour(0,0,0);

    SceneNode *lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "PlayerModelSelectorNode" );
    lightNode->attachObject(light);

    lightNode->setPosition(Vector3(0, 10, 0));
    lightNode->setDirection(Vector3(0,-1,0));


    mSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_MODULATIVE);

}

PlayerModelSelector::~PlayerModelSelector()
{

    mNode->detachObject(mEnt);
    mSceneMgr->destroyEntity(mEnt);
    mSceneMgr->destroySceneNode(mNode);

    mBaseNode->detachObject(mBaseEnt);
    mSceneMgr->destroyEntity(mBaseEnt);
    mSceneMgr->destroySceneNode(mBaseNode);

    mSceneMgr->destroyLight("PlayerModelSelectorLight");

    mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);
}

void PlayerModelSelector::setPosition(Real x, Real y, Real z)
{
    mNode->setPosition(x,y,z);
}

void PlayerModelSelector::update( unsigned long lTimeElapsed )
{
    //static float base_a = 0;
    static Real time_idle = 0;

    time_idle += lTimeElapsed * 0.001;

    //base_a += lTimeElapsed * 0.001;
    //if(base_a > M_PI*2) base_a -= M_PI*2;

    //float base_scale = 1+sin(base_a)/4.0;
    //mBaseNode->setScale(base_scale,base_scale,base_scale);


    mBaseNode->yaw(Radian(lTimeElapsed * -0.0001));
    mNode->yaw(Radian(lTimeElapsed * -0.0001));

    if(time_idle > 3.0)
    {
        int oneOfThree = rand() % 3;

        mAnimationState->setEnabled(false);
        if(oneOfThree == 0) mAnimationState = mEnt->getAnimationState("Bored_1");
        if(oneOfThree == 1) mAnimationState = mEnt->getAnimationState("Bored_2");
        if(oneOfThree == 2) mAnimationState = mEnt->getAnimationState("Bored_3");
        mAnimationState->setLoop(false);
        mAnimationState->setEnabled(true);


    }


    if(mAnimationState->getAnimationName() == "Bored_1" ||
       mAnimationState->getAnimationName() == "Bored_2" ||
       mAnimationState->getAnimationName() == "Bored_3")
    {
        time_idle = 0;

        if(mAnimationState->hasEnded())
        {
            mAnimationState->setEnabled(false);
            mAnimationState = mEnt->getAnimationState("Idle");
            time_idle = 0;
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);
        }

    }

    mAnimationState->addTime(lTimeElapsed*0.001);


};

void PlayerModelSelector::prev()
{
    playerModelIndex--;
    if(playerModelIndex < 0) playerModelIndex = NUM_PLAYER_MODELS-1;

    updateModel();
}

void PlayerModelSelector::next()
{
    playerModelIndex++;
    if(playerModelIndex > NUM_PLAYER_MODELS-1) playerModelIndex = 0;

    updateModel();

}

String PlayerModelSelector::getName()
{
    return model_names[playerModelIndex];
}

void PlayerModelSelector::updateModel()
{
    mAnimationState->setEnabled(false);

    ConfigManager::getSingleton().setValue("player_model",model_filenames[playerModelIndex]);

    mNode->detachObject(mEnt);
    mSceneMgr->destroyEntity(mEnt);

    /*mMirrorNode->detachObject(mMirrorEnt);
    mSceneMgr->destroyEntity(mMirrorEnt);*/

    String mesh_name = ConfigManager::getSingleton().getString("player_model") + ".mesh";

    mEnt = mSceneMgr->createEntity("ModelSelectorEnt", mesh_name);
    mEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);
    mEnt->setCastShadows(true);

    mNode->attachObject(mEnt);


    mAnimationState = mEnt->getAnimationState("Idle");
    mAnimationState->setLoop(true);

    /*mMirrorEnt = mSceneMgr->createEntity("ModelSelectorMirrorEnt", mesh_name);
    mMirrorEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);
    mMirrorEnt->setCastShadows(false);

    mMirrorNode->attachObject(mMirrorEnt);*/



}
