#ifndef HighScoreState_H
#define HighScoreState_H

#ifndef GameState_H
#include "GameState.h"
#endif

#include "HighScoreTable.h"
#include "EditableText.h"

class HighScoreState : public GameState
{
    public:
        ~HighScoreState( void );

        void enter( void );
        void exit( void );

        void pause( void );
        void resume( void );
        void update( unsigned long lTimeElapsed );

        void keyPressed( const OIS::KeyEvent &e );
        void keyReleased( const OIS::KeyEvent &e );

        void mouseMoved( const OIS::MouseEvent &e );
        void mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
        void mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

        static HighScoreState* getSingletonPtr( void );
    private:
        HighScoreState( void );
        HighScoreState( const HighScoreState& ) { }
        HighScoreState & operator = ( const HighScoreState& );


        Ogre::Root           *mRoot;
        Ogre::Camera         *mCamera;
        Ogre::SceneManager   *mSceneMgr;
        Ogre::Viewport       *mViewport;

        Ogre::OverlayManager    *mOverlayMgr;
        Ogre::Overlay           *mOverlay;

        Ogre::PanelOverlayElement     *mPanel;
        Ogre::TextAreaOverlayElement  *mTextAreaTitle;
        Ogre::TextAreaOverlayElement  *mTextAreaName;



        HighScoreTable *highScoreTable;

        EditableText *mEditableText;

        static HighScoreState    *mHighScoreState;
};

#endif // HighScoreState_H
