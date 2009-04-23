#include "LevelLoader.h"
#include "ConfigManager.h"
#include "Brick.h"

using namespace Ogre;

template<> LevelLoader* Ogre::Singleton<LevelLoader>::ms_Singleton = 0;
LevelLoader* LevelLoader::getSingletonPtr(void)
{
    return ms_Singleton;
}
LevelLoader& LevelLoader::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}

LevelLoader::LevelLoader()
{

}

void LevelLoader::setLevelName(String levelName)
{
    this->levelName = levelName;
    setBoardNum(0);

    cf.loadFromResourceSystem("levels.cfg","General");

   ConfigFile::SectionIterator seci = cf.getSectionIterator();

   Ogre::String secName, optName, optValue;

   while (seci.hasMoreElements())
   {

       secName = seci.peekNextKey();
       ConfigFile::SettingsMultiMap *settings = seci.getNext();
       ConfigFile::SettingsMultiMap::iterator i;

       for (i = settings->begin(); i != settings->end(); ++i)
       {
           optName = i->first;
           optValue = i->second;

           if(secName == levelName)
           {
                if(optName == "num_boards") numBoards = StringConverter::parseInt(optValue);
           }
       }
   }

}

int LevelLoader::loadBoard()
{
    levelData.clear();

    levelImage = new Image;

    levelImage->load(levelName + StringConverter::toString(boardNum) + ".png", "General");

    height = levelImage->getHeight();
    width  = levelImage->getWidth();

    levelData.reserve(height*width);

    for(int j = 0; j < height; j++)
        for(int i = 0; i < width; i++)
        {
            ColourValue brickColour = levelImage->getColourAt(i,j,0);

            if(brickColour == ColourValue(0,0,0)) levelData.push_back(BRICK_NONE);
            else if(brickColour == ColourValue(1,0,0)) levelData.push_back(BRICK_RED);
            else if(brickColour == ColourValue(0,1,0)) levelData.push_back(BRICK_GREEN);
            else if(brickColour == ColourValue(0,0,1)) levelData.push_back(BRICK_BLUE);
            else if(brickColour == ColourValue(1,1,0)) levelData.push_back(BRICK_YELLOW);
            else if(brickColour == ColourValue(1,0,1)) levelData.push_back(BRICK_ROCK);
            else if(brickColour == ColourValue(1,1,1)) levelData.push_back(BRICK_AIR);
            else
            {
                Ogre::LogManager::getSingleton().logMessage("LevelLoader: Wrong color at x=" +
                                                StringConverter::toString(i) + " y=" +
                                                StringConverter::toString(j) + " on level image " + "levelName");

                levelData.push_back(BRICK_NONE);

            }

        }

    delete levelImage;

    return true;
}

LevelLoader::~LevelLoader()
{
    levelData.clear();
}



