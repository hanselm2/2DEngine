#ifndef EDITOR_H
#define EDITOR_H

#include "ActionSet.h"
#include "ApplicationState.h"
#include "../Rendering/DrawableObject.h"
#include "Object.h"
#include "Controller.h"
#include "Utils/Vector2D.h"

#include <vector>
/*
An Ambitious class that will hold mainly functions that will do the actions 
given that we have started editing. As well as basic information about our 
editing/the ability to save and load

In the future we may extend this for things like a level editor, physics editor
and image editor
*/

class Editor : public Controller
{
  public:
    virtual string toString()
    {
      return "Editor";
    }

    virtual string dumpObject()
    {
      if (!Instance()->isPluggedIn())
        return "The Editor is not currently Started";

      return myActions->dumpObject();
    }

    static Editor* Instance()
    {
      static Editor instance;
      return &instance;
    }
    
    //The editor must be able to draw its own unique object, which in this case
    // is the rectange of its drag box
    void drawEditorObjects();
    
  protected:
    Editor();
    virtual void loadActions();
    virtual void unloadActions();

    ActionSet* myActions;
    Vector2d dragStart;
    Vector2d dragEnd;

    /*
    The editor is not part of the game so it will be looking at the
    Application state and will be able to store objects that its selected
    */
    vector< DrawableObject* > selectedObjects;
    
    Vector2d* selectedPoint;

    float squareSize;
    /*
    Helper function that will let us check if we clicked on any of the
    boxes that we draw around a selected objects vertices
    */
    
    // BELOW ARE ALL OF THE ACTION FUNCTIONS
    
    static void loadEnvironment();
    static void saveEnvironment();
    static void clickedMouse();
    static void releasedMouse();
    static void mouseMotion();
    static void texture();
    static void pressedEsc();
    static void pressedDelete();
    static void pressedUp();
    static void pressedDown();
    static void pressedLeft();
    static void pressedRight();
    //TODO add in ability to edit a animation on an object
};

#endif