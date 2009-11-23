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

    mEnt = mSceneMgr->createEntity("ModelSelectorEnt", mesh_name);

    mEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);

    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ModelSelectorNode");

    mNode->attachObject(mEnt);

}

PlayerModelSelector::~PlayerModelSelector()
{
    mNode->detachObject(mEnt);
    mSceneMgr->destroyEntity(mEnt);
    mSceneMgr->destroySceneNode(mNode);
}

void PlayerModelSelector::setPosition(Real x, Real y, Real z)
{
    mNode->setPosition(x,y,z);
}

void PlayerModelSelector::update( unsigned long lTimeElapsed )
{
    mNode->yaw(Radian(lTimeElapsed * 0.001));
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
    ConfigManager::getSingleton().setValue("player_model",model_filenames[playerModelIndex]);

    mNode->detachObject(mEnt);
    mSceneMgr->destroyEntity(mEnt);

    String mesh_name = ConfigManager::getSingleton().getString("player_model") + ".mesh";

    mEnt = mSceneMgr->createEntity("ModelSelectorEnt", mesh_name);

    mEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);

    mNode->attachObject(mEnt);
}
