#include "MenuButton.h"

int menuButtonCount = 0;

MenuButton::MenuButton(UTFString caption)
{

    String name = "MenuButton_" + StringConverter::toString(menuButtonCount);
    menuButtonCount++;

    mNode = Root::getSingleton().getSceneManager( "ST_GENERIC" )->getRootSceneNode()->createChildSceneNode(name + "_Node");
    mTextNode = mNode->createChildSceneNode();


    text = new MovableText(name,caption);



    mTextNode->attachObject(text);

    mTextNode->scale(0.5,0.5,0.5);

    AxisAlignedBox aabb = text->GetAABB();

    mTextNode->translate(-(aabb.getMinimum().x+aabb.getMaximum().x)/4.0,0,0);
    //mTextNode->translate(-(aabb.getMinimum().x+aabb.getMaximum().x)/3.0,0,0);

    //LogManager::getSingleton().logMessage(StringConverter::toString(this->text->getWidth()));

    // Create a manual object for 2D
    frame = Root::getSingleton().getSceneManager( "ST_GENERIC" )->createManualObject(name + "_frame");

    // Use identity view/projection matrices
    //frame->setUseIdentityProjection(true);
    //frame->setUseIdentityView(true);

    frame->begin("button", RenderOperation::OT_TRIANGLE_STRIP);

    frame->position(-3, -0.4, -0);
    frame->textureCoord(0, 0);
    frame->position( 3, -0.4, -0);
    frame->textureCoord(1, 0);
    frame->position( 3,  0.4, -0);
    frame->textureCoord(1, 1);
    frame->position(-3,  0.4, -0);
    frame->textureCoord(0, 1);

    frame->index(0);
    frame->index(1);
    frame->index(2);
    frame->index(3);
    frame->index(0);

    frame->end();


    frame->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);
    text->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);



    //mTextNode->needUpdate();

    mNode->attachObject(frame);
    //mNode->needUpdate();



    //mNode->showBoundingBox(true);
    //mTextNode->showBoundingBox(true);

    state = BSTATE_INACTIVE;
    dest_reached = false;

    speed = Vector3(0,0,0);

}

MenuButton::~MenuButton()
{

    mTextNode->detachAllObjects();
    mNode->detachAllObjects();
    delete text;
    Root::getSingleton().getSceneManager( "ST_GENERIC" )->destroyManualObject( frame );
    mNode->removeAndDestroyAllChildren();
    mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());

    menuButtonCount--;

}

void MenuButton::setState(int state)
{
    this->state = state;

    if(state == BSTATE_ACTIVE)
    {
        text->setColor(ColourValue(1,1,0));
    }
    else if(state == BSTATE_INACTIVE)
    {
        text->setColor(ColourValue::White);
    }

}

void MenuButton::update(unsigned long lTimeElapsed)
{
    if(dest_reached == false)
    {
        Vector3 pos = mNode->getPosition();
        Vector3 dir = dest-pos;

        dir.normalise();

        speed += dir*lTimeElapsed*0.0001;

        if(speed.length() > Vector3(pos-dest).length())
        {
            speed = Vector3(0,0,0);
            dest_reached = true;
            mNode->setPosition(dest);
        }
        else
        {
            mNode->setPosition(pos+speed);
        }

    }

}

void MenuButton::setDest(Vector3 dest)
{
    this->dest = dest;
    dest_reached = false;
}

void MenuButton::setDest(Real x,Real y ,Real z)
{
    Vector3 dest(x,y,z);
    this->dest = dest;
    dest_reached = false;
}








