#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <Ogre.h>

#include <vector>

class LevelLoader
{
    public:
        LevelLoader(Ogre::String levelName);
        ~LevelLoader();

        int getHeight() {return height;}
        int getWidth() {return width;}

        int getBrickType(int index) {return levelData[index];}

    private:

        int height, width;

        Ogre::Image *levelImage;

        std::vector<int> levelData;

};

#endif // LEVELLOADER_H
