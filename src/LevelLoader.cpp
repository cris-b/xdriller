#include "LevelLoader.h"
#include "ConfigManager.h"
#include "Brick.h"
#include "Tools.h"
#include "Globals.h"
#include <OgreStringConverter.h>

#define INFINITE_DEPTH 90;

using namespace Ogre;

template<> LevelLoader* Ogre::Singleton<LevelLoader>::msSingleton = 0;
LevelLoader* LevelLoader::getSingletonPtr(void)
{
    return msSingleton;
}
LevelLoader& LevelLoader::getSingleton(void)
{
    assert( msSingleton );
    return ( *msSingleton );
}

LevelLoader::LevelLoader()
{
    gameMode = GAME_MODE_INFINITE;
    levelDifficulty = MEDIUM;

    numLevels = 0;

    cf.loadFromResourceSystem("levels.cfg","General");

    const auto &settings = cf.getSettingsBySection();

    String secName;

    for (const auto &sec : settings) {
        secName = sec.first;
        if(secName != Ogre::BLANKSTRING) {
          levelNames.push_back(secName);
          numLevels++;
        }
    }

    LogManager::getSingleton().logMessage("LevelLoader: Loaded levels.cfg. Number of Levels: "
                                          + StringConverter::toString(numLevels));

    setLevelNum(numLevels-1);

    for(int i = 0; i<numLevels; i++)
    {
        LogManager::getSingleton().logMessage("LevelLoader: Level " + StringConverter::toString(i) + " = " + levelNames[i]);
    }



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

    const auto &settings = cf.getSettingsBySection();

    Ogre::String secName, optName, optValue;

    int n = -1;
    for (const auto &sec : settings)
    {

        secName = sec.first;
        n++;
        for (const auto &set : sec.second)
        {
            optName = set.first;
            optValue = set.second;

            if(levelNum == n-1)
            {
                if(optName == "num_boards") numBoards = StringConverter::parseInt(optValue);
                levelName = secName;

                //Ogre::LogManager::getSingleton().logMessage("SET LEVELNAME TO: " + secName + " " + StringConverter::toString(levelNum));
            }
        }
    }
}

Ogre::String LevelLoader::getLevelName(int level_num)
{

    if(level_num<numLevels) return levelNames[level_num];
    else return "";

}

Ogre::String LevelLoader::getLongName()
{

    return getValue("long_name");

}


Ogre::String LevelLoader::getValue(Ogre::String opt)
{

    if(getGameMode() == GAME_MODE_TUTORIAL)
    {
        if(opt == "background_scene")
        {
            return "tutorial";
        }
    }

    const auto &settings = cf.getSettingsBySection();

    Ogre::String secName, optName, optValue;

    for (const auto &sec : settings)
    {

        secName = sec.first;
        for (const auto &set : sec.second) 
        {
            optName = set.first;
            optValue = set.second;

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

    if(levelName == "tutorial")
    {
        numBoards = 2;
    }
    else
    {
        const auto &settings = cf.getSettingsBySection();

        Ogre::String secName, optName, optValue;

        for (const auto &sec : settings)
        {

            secName = sec.first;
            for (const auto &set : sec.second)
            {
                optName = set.first;
                optValue = set.second;

                if(secName == levelName)
                {
                    if(optName == "num_boards") numBoards = StringConverter::parseInt(optValue);

                }
            }
        }
    }
}

int LevelLoader::getLevelNum(Ogre::String name)
{
    int level_num = -1;

    for(int i = 0; i<numLevels; i++)
    {
        if(levelNames[i] == name)
        {
            level_num = i;
            break;
        }
    }

    return level_num;


};

int LevelLoader::loadBoard()
{
    if(gameMode == GAME_MODE_ADVENTURE || gameMode == GAME_MODE_TIME_ATTACK || gameMode == GAME_MODE_TUTORIAL)
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
                else if(brickColour == ColourValue(0,1,1)) levelData.push_back(BRICK_TUTORIAL);
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
    }
    else if(gameMode == GAME_MODE_INFINITE)
    {
        struct BrickChance
        {
            int red;
            int green;
            int blue;
            int yellow;
            int rock;
            int air;
            int heart;
            int fixed;

        } brickChance;

        levelData.clear();

        //TODO: restarle a heigh la altura que hay entre un nivel y otro
        // para que cuente un numero de profundidades por cada caida
        height = INFINITE_DEPTH; //default board height
        width  = 9;

        //probabilidad de que salga una ficha en concreto entre 1000
        if(levelDifficulty == EASY)
        {
            brickChance.red         = 100; //si el numero aleatorio es menor a esto sale rojo
            brickChance.green       = 500; //si es menor a este sale verde
            brickChance.blue        = 800;
            brickChance.yellow      = 970;
            brickChance.rock        = 990;
            brickChance.air         = 997;
            brickChance.heart       = 1000;
            brickChance.fixed       = 1000;
        }
        if(levelDifficulty == MEDIUM)
        {
            brickChance.red         = 200;
            brickChance.green       = 300;
            brickChance.blue        = 600;
            brickChance.yellow      = 850;
            brickChance.rock        = 979;
            brickChance.air         = 984;
            brickChance.heart       = 985;
            brickChance.fixed       = 1000;
        }
        if(levelDifficulty == HARD)
        {
            brickChance.red         = 200;
            brickChance.green       = 400;
            brickChance.blue        = 600;
            brickChance.yellow      = 800;
            brickChance.rock        = 970;
            brickChance.air         = 979;
            brickChance.heart       = 979;
            brickChance.fixed       = 1000;
        }

        levelData.reserve(height*width);

        for(int j = 0; j < height; j++)
            for(int i = 0; i < width; i++)
            {
                int var = rand() % 1000;



                // no hay BRICK_NONE en modo infinito
                //if(brickColour == ColourValue(0,0,0)) levelData.push_back(BRICK_NONE);
                if(var < brickChance.red)           levelData.push_back(BRICK_RED);
                else if(var < brickChance.green)    levelData.push_back(BRICK_GREEN);
                else if(var < brickChance.blue)     levelData.push_back(BRICK_BLUE);
                else if(var < brickChance.yellow)   levelData.push_back(BRICK_YELLOW);
                else if(var < brickChance.rock)     levelData.push_back(BRICK_ROCK);
                else if(var < brickChance.air)      levelData.push_back(BRICK_AIR);
                else if(var < brickChance.heart)    levelData.push_back(BRICK_HEART);
                else if(var < brickChance.fixed)    levelData.push_back(BRICK_FIXED);
                else //no deberia llegar aqui
                {
                    Ogre::LogManager::getSingleton().logMessage("LevelLoader: Error 'randomizing' infinite level at block x=" +
                            StringConverter::toString(i) + " y=" +
                            StringConverter::toString(j) + ". Value = " +
                            StringConverter::toString(var));

                    levelData.push_back(BRICK_RED);
                }

            }

    }

    return true;
}

int LevelLoader::getNumLevels()
{
    return numLevels;
};

void LevelLoader::setGameMode(int gameMode)
{
    this->gameMode = gameMode;

    if(gameMode == GAME_MODE_TUTORIAL)
    {
        setLevelName("tutorial");
    }
    if(gameMode == GAME_MODE_INFINITE)
    {
        numBoards = 32766; //very big number
    }
}

LevelLoader::~LevelLoader()
{
    levelData.clear();
    levelNames.clear();
}



