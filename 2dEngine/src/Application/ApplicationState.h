#ifndef APPLICATION_STATE_H
#define APPLICATION_STATE_H

#include <vector>
#include <map>

#include "Object.h"
#include "Utils/Vector2D.h"
#include "Utils/QuadTree.h"
#include "../Rendering/DrawableObject.h"
#include "Event.h"

/*
A singleton class that will hold information about our Application
such as the framerate, and the aspect ration, etc
This class does not hold Game specific information

*/

class ApplicationState : public Object
{
  public: 

    virtual string toString()
    {
      return "ApplicationState";
    }

    virtual string dumpObject()
    {
      string currentState = "";
      currentState += "FPS: " + to_string(FPS) + '\n';
      currentState += "aspect ratio: " + to_string(aspectRatio) + '\n';
      return currentState;
    }

    static ApplicationState& Instance()
    {
      if (instance == NULL)
        instance = new ApplicationState();

      return *instance;
    }

    float getAspectRatio(){ return aspectRatio; }
    void setAspectRatio(float ratio){ aspectRatio = ratio; }

    int getYSize(){ return ySize; }
    void setYSize(int y){ ySize = y; }

    int getFPS(){ return FPS; }
    void setFPS(int newFps){ FPS = newFps; }

    Vector2d getCameraPosition(){ return cameraPosition; }
    void setCameraPosition(Vector2d newPos) { cameraPosition = newPos; }
    void moveCameraPosition(Vector2d difference) { cameraPosition = cameraPosition + difference; }
    
    float getCameraZ(){ return cameraZ; }
    void setCameraZ(float z) { cameraZ = z; }
    
    Vector2d getMousePosition(){ return mousePosition; }
    void setMousePosition(Vector2d newPos) { mousePosition = newPos; }

    Vector2d getMouseScreenPosition(){ return mouseScreenPosition; }
    void setMouseScreenPosition(Vector2d newPos) { mouseScreenPosition = newPos; }
    
    bool getMouseIsDown(){ return mouseIsDown; }
    void setMouseIsDown(bool isDown) { 
      if (isDown != mouseIsDown && isDown == true)
      {
        mouseIsDown = isDown;
        leftClickedEvent::trigger();
      }
      else if (isDown != mouseIsDown&& isDown == false)
      {
        mouseIsDown = isDown;
        leftReleasedEvent::trigger();
      }
      mouseIsDown = isDown;
    }

    void clearAllObjects()
    {
      drawableObjects.clear();
      clickableObjects.clear();
    }

    /*
    Methods that act on the drawable and clickable objects
    */
    vector< DrawableObject* > getDrawableObjects(){ return drawableObjects; }
    void addDrawableObject(DrawableObject& drawable);

    /*
    returns the clickable objects for a given Z buffer
    */
    QuadTree< DrawableObject >& getClickableObjects(int z)
    {
      return clickableObjects[z]; 
    }
    void addClickableObject(DrawableObject& clickable, int z);

    vector<int> getPressedKeys()
    {
      vector<int> pressedKeys;
      for (int i = 0; i < 256; i++)
      {
        if (keyStates[i])
          pressedKeys.push_back(i);
      }
      return pressedKeys;
    }
    void pressKey(int key)
    {
      keyStates[key] = true;
    }
    void releaseKey(int key)
    {
      keyStates[key] = false;
    }
    // The modifiers are ctrl alt and shift
    void pressModifier(int modifier)
    {
      //Modifier defaults to 0 if it is not pressed
      if (modifier == 0)
        return;
      switch (modifier)
      {
      case 1: shiftPressed = true; break;
      case 2: ctrlPressed = true; break;
      case 4: altPressed = true;  break;
      default: cout << "We got a modifier value of " << modifier << " in pressModifier." << endl;
      }
    }
    void releaseModifier(int modifier)
    {
      //Modifier defaults to 0 if it is not pressed
      if (modifier == 0)
        return;
      switch (modifier)
      {
      case 1: shiftPressed = false; break;
      case 2: ctrlPressed = false; break;
      case 4: altPressed = false;  break;
      default: cout << "We got a modifier value of " << modifier << " in releaseModifier." << endl;
      }
    }
    bool isControlPressed()
    {
      return ctrlPressed;
    }
    bool isAltPressed()
    {
      return altPressed;
    }
    bool isShiftPressed()
    {
      return shiftPressed;
    }

    /*
    Gets the object clicked for a specified layer
    */
    DrawableObject* getObjectClicked(Vector2d& mousePosition, int layer);

    /*
    Gets the Object clicked for the largest layer where the mouse is. This method is much slower
    than specifying the layer as it checks each quadtree
    */
    DrawableObject* getObjectClicked(Vector2d& mousePosition);

    /*
    Gets the Object clicked for the stored application mouse position and the largest layer
    */
    DrawableObject* getObjectClicked();

    /*
    A method that should only really be used by the Editor because it goes against the idea
    that objects can be drawn but not clicked. This method will check for all objects that we 
    draw even if they arent clicked and return the drawable object.

    Note: Currently this function does not take advantage of our quadtree, we should probably
    change this.
    */
    DrawableObject* getObjectClickedForEditor();

    bool deleteDrawableObject(DrawableObject& obj);

    /*
    Helper function that uses the fact that our application will use a projection matrix with
    a set distance for the camera and everything at Z = 0 to convert mouse coordinates to
    the worldView? cordinates
    */
    static float convertScreenXToWorld(int screenX)
    {
      float screenRatio = (float)screenX / (float)(ApplicationState::Instance().getYSize() *
        ApplicationState::Instance().getAspectRatio());

      return 2.f * ((screenRatio - 0.5f) * ApplicationState::Instance().getCameraZ() *
        ApplicationState::Instance().getAspectRatio());
    }

    /*
    Helper function that uses the fact that our application will use a projection matrix with
    a set distance for the camera and everything at Z = 0 to convert mouse coordinates to
    the worldView? cordinates
    */
    static float convertScreenYToWorld(int screenY)
    {
      // The y is reversed the top of the screen is 0 pixels but positive y
      return -2.f * (((float)screenY / (float)ApplicationState::Instance().getYSize() - 0.5f) *
        ApplicationState::Instance().getCameraZ());
    }

    /*
    Helper function that uses the fact that our application will use a projection matrix with
    a set distance for the camera and everything at Z = 0 to convert mouse coordinates to
    the worldView? cordinates

    Uses passing by reference to affect the vector
    */
    static void convertScreenVecToWorld(Vector2d& screenVector)
    {
      screenVector.setX(convertScreenXToWorld(screenVector.getX()));
      screenVector.setY(convertScreenYToWorld(screenVector.getY()));
    }
    
  protected:
    static ApplicationState* instance;

    // Is the mouse down right now?
    bool mouseIsDown;
  
    // The width/height is the aspect ratio
    float aspectRatio;
    int ySize;
    int FPS;

    //Cameras X and Y and depth.
    Vector2d cameraPosition;
    float cameraZ;

    //Mouse positions, the screen and the "world view"
    Vector2d mousePosition;
    Vector2d mouseScreenPosition;

    //Array that holds info on all 256 possible key presses, True = down
    bool* keyStates;
    //GLUT doesnt natively check to see if Control, Shift or Alt are pressed
    bool ctrlPressed;
    bool shiftPressed;
    bool altPressed;

    //All of the objects that the application can draw
    vector< DrawableObject* > drawableObjects;
    
    //All of the objects that the application can click
    map< int, QuadTree<DrawableObject> > clickableObjects;

  private:
    
    ApplicationState()
    {
      keyStates = new bool[256];
      for (int i = 0; i < 256; i++)
      {
        keyStates[i] = false;
      }
      ctrlPressed = false;
      shiftPressed = false;
      altPressed = false;
    }

};

#define DEBUG 0

#endif