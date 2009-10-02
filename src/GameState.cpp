#include "GameState.h"

using namespace Ogre;

void GameState::changeState( GameState *state ) {
    GameManager::getSingletonPtr()->changeState( state );
}

void GameState::pushState( GameState *state ) {
    GameManager::getSingletonPtr()->pushState( state );
}

void GameState::popState( void ) {
    GameManager::getSingletonPtr()->popState();
}

void GameState::requestShutdown( void ) {
    GameManager::getSingletonPtr()->requestShutdown();
}

void GameState::fadeState( GameState *state )
{
    fadeTo = state;
}

void GameState::updateStateFader()
{
    if(fadeTo != NULL && Fader::getSingletonPtr()->getFadeState() != FADE_OUT)
        Fader::getSingletonPtr()->fadeOut();

    if(fadeTo != NULL && Fader::getSingletonPtr()->getFadeState() == FADE_FULL)
    {
        GameState *_fadeTo = fadeTo;
        fadeTo = NULL;
        Fader::getSingletonPtr()->fadeIn();
        GameManager::getSingletonPtr()->changeState( _fadeTo );
    }

}
