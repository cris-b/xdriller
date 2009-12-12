#include "LevelLoader.h"
#include "ConfigManager.h"
#include "Brick.h"
#include "Tools.h"

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
    numLevels = -1;

    cf.loadFromResourceSystem("levels.cfg","General");

   ConfigFile::SectionIterator seci = cf.getSectionIterator();

   while (seci.hasMoreElements())
   {
        numLevels++;
        seci.getNext();

   }

   Ogre::LogManager::getSingleton().logMessage("LevelLoader: Loaded levels.cfg. Number of Levels: "
                                                + StringConverter::toString(numLevels));

    setLevelNum(numLevels-1);

}

void LevelLoader::nextLevel()
{
    levelNum ++;
    if(levelNum >= numLevels)
        levelNum = 0;

    setLevelNum(levelNum);
}

void LevelLoader::prevLevel()
{
    levelNum --;
    if(levelNum < 0)
        levelNum = numLevels-1;

    setLevelNum(levelNum);
}

void LevelLoader::setLevelNum(int levelNum)
{
    this->levelNum = levelNum;
    setBoardNum(0);

   ConfigFile::SectionIterator seci = cf.getSectionIterator();

   Ogre::String secName, optName, optValue;

   int n = -1;
   while (seci.hasMoreElements())
   {

       secName = seci.peekNextKey();
       ConfigFile::SettingsMultiMap *settings = seci.getNext();
       ConfigFile::SettingsMultiMap::iterator i;

       n++;
       for (i = settings->begin(); i != settings->end(); ++i)
       {
           optName = i->first;
           optValue = i->second;

           if(levelNum == n-1)
           {
                if(optName == "num_boards") numBoards = StringConverter::parseInt(optValue);
                levelName = secName;

                //Ogre::LogManager::getSingleton().logMessage("SET LEVELNAME TO: " + secName + " " + StringConverter::toString(levelNum));
           }
       }


   }


}

Ogre::String LevelLoader::getLongName()
{

    return getValue("long_name");

}


Ogre::String LevelLoader::getValue(Ogre::String opt)
{

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
                if(optName == opt) return optValue;
           }
       }
   }

   return "";

}

void LevelLoader::setLevelName(String levelName)
{
    this->levelName = levelName;
    setBoardNum(0);

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

            roundColourValue(&brickColour);

            if(brickColour == ColourValue(0,0,0)) levelData.push_back(BRICK_NONE);
            else if(brickColour == ColourValue(1,0,0)) levelData.push_back(BRICK_RED);
            else if(brickColour == ColourValue(0,1,0)) levelData.push_back(BRICK_GREEN);
            else if(brickColour == ColourValue(0,0,1)) levelData.push_back(BRICK_BLUE);
            else if(brickColour == ColourValue(1,1,0)) levelData.push_back(BRICK_YELLOW);
            else if(brickColour == ColourValue(1,0,1)) levelData.push_back(BRICK_ROCK);
            else if(brickColour == ColourValue(1,1,1)) levelData.push_back(BRICK_AIR);
            else if(brickColour == ColourValue(0.5,0,0)) levelData.push_back(BRICK_HEART);
            else if(brickColour == ColourValue(0.5,0.5,0.5)) levelData.push_back(BRICK_FIXED);
            else
            {
                Ogre::LogManager::getSingleton().logMessage("LevelLoader: Wrong color at x=" +
                                                StringConverter::toString(i) + " y=" +
                                                StringConverter::toString(j) + " on level image " +
                                                levelName + " : r = " +
                                                StringConverter::toString(brickColour.r) + ", g = " +
                                                StringConverter::toString(brickColour.g) + ", b = " +
                                                StringConverter::toString(brickColour.b) + ", a = " +
                                                StringConverter::toString(brickColour.a) + ".");

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



