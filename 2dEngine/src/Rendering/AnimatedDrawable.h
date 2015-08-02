#ifndef ANIMATED_DRAWABLE_H
#define ANIMATED_DRAWABLE_H

#include "DrawableObject.h"
#include "../Application/ApplicationState.h"
#include "../Application/Utils/AppUtils.h"

/*
* A class that implements Drawable object but only draws a simple colored object
* defined by a singular RGBA color
*/
class AnimatedDrawable : public DrawableObject
{
public:
  /*
  * Methods inherited from Object
  */
  virtual string toString()
  {
    return "AnimatedDrawable" + to_string(myId);
  }

  virtual string dumpObject()
  {
    string currentState = DrawableObject::dumpObject();

    currentState += "myId: " + myId + '\n';
    currentState += "myTextureFilePaths \n";
    for (string filePath : textureFilePathArray)
    {
      currentState += filePath + '\n';
    }
    currentState += "myTextureIds \n";
    for (GLuint textId : textureIdArray)
    {
      currentState += textId + 'n';
    }

    return currentState;
  }

  /*
  * Method inherited from DrawableObject
  */
  virtual void drawObject();

  AnimatedDrawable() : DrawableObject()
  {
    id++;
    myId = id;
    currentTexture = 0;
    frameCount = 0;
    framesPerTexture = 0;
  }

  AnimatedDrawable(vector<Vector2d> points, int z, vector<string> filePathArray, 
                   unsigned int changeRate) :
    DrawableObject(points, z)
  {
    textureFilePathArray = filePathArray;

    for (string filePath : textureFilePathArray)
    {
      textureIdArray.push_back(AppUtils::loadTextureFromBMPFile(filePath.c_str()));
    }
    currentTexture = 0;
    frameCount = 0;
    framesPerTexture = changeRate;
    id++;
    myId = id;
  }

  /*
  * getter and setter methods
  */
  vector<string> getTextureFilePath() { return textureFilePathArray; }
  void setTextureFilePath(vector<string> newPathArray)
  {
    textureFilePathArray = newPathArray;
    for (string filePath : textureFilePathArray)
    {
      textureIdArray.push_back(AppUtils::loadTextureFromBMPFile(filePath.c_str()));
    }
  }

  vector<GLuint> getTextureId(){ return textureIdArray; }
  void setTextureId(vector<GLuint> newId){ textureIdArray = newId; }

  int getMyId() { return myId; }

protected:
  vector<string> textureFilePathArray;
  vector<GLuint> textureIdArray;

  /*
  Integer that tells us what indice of ID's we are currently on
  */
  unsigned int currentTexture;
  /* 
  Integer that tells us how many frames its been since we changed the texture
  */
  unsigned int frameCount;

  /*
  Integer that tells us how long we wait for before trying the next texture
  */
  unsigned int framesPerTexture;

  /*
  * A static int that will allow us to identify the object. Possibly a bad idea
  */
  static int id;
  int myId;

};
#endif