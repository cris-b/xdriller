#include <Ogre.h>
//#include <OgreException.h>
#ifndef GameManager_H
#include "GameManager.h"
#endif

#ifndef IntroState_H
#include "IntroState.h"
#endif

#include "math.h"
#include "time.h"






#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT ) {
#else
int main( int argc, char **argv ) {
#endif



    srand(time(0));

    GameManager *gameManager = GameManager::getSingletonPtr();


    try {
        // Initialise the game and switch to the first state
        gameManager->startGame( MenuState::getSingletonPtr() );
    }
    catch ( Ogre::Exception& ex ) {
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
                MessageBox( NULL, ex.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL );
        #else
                std::cerr << "An exception has occured: " << ex.getFullDescription();
        #endif
    }



    delete gameManager;
    return 0;
}
