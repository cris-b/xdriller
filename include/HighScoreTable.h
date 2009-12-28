#ifndef HighScoreTable_H
#define HighScoreTable_H

#include <Ogre.h>
#include <OgrePanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>

class HighScoreTable
{
    public:
        HighScoreTable();
        ~HighScoreTable();

        void updateTable(Ogre::String mode, Ogre::String level);


    private:

        Ogre::OverlayManager    *mOverlayMgr;
        Ogre::Overlay           *mOverlay;
        Ogre::PanelOverlayElement     *mPanel;
};

#endif // HighScoreTable_H
