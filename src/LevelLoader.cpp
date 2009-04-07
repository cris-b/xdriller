#include "LevelLoader.h"
#include "ConfigManager.h"
#include "Brick.h"

using namespace Ogre;

LevelLoader::LevelLoader(String levelName)
{
    levelName += ".png";

    levelImage = new Image;

    levelImage->load(levelName , "General");

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

}

LevelLoader::~LevelLoader()
{
    levelData.clear();
}



