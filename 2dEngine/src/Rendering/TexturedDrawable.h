#ifndef TEXTURED_DRAWABLE_H
#define TEXTURED_DRAWABLE_H

#include "DrawableObject.h"
#include "../Application/ApplicationState.h"
#include "../Application/Utils/AppUtils.h"

/*
* A class that implements Drawable object but only draws a simple colored object
* defined by a singular RGBA color
*/
class TexturedDrawable : public DrawableObject
{
public:
  /*
  * Methods inherited from Object
  */
  virtual string toString()
  {
    return "TexturedDrawable" + to_string(myId);
  }

  virtual string dumpObject()
  {
    string currentState = DrawableObject::dumpObject();

    currentState += "myId: " + myId + '\n';
    currentState += "myTextureFilePath " + textureFilePath + '\n';
    currentState += "myTextureId" + to_string(textureId) + '\n';

    return currentState;
  }

  /*
  * Method inherited from DrawableObject
  */
  virtual void drawObject();

  TexturedDrawable() : DrawableObject()
  {
    id++;
    myId = id;
    textureFilePath = "";
    textureId = 0;
  }

  TexturedDrawable(vector<Vector2d> points, int z, string filePath) :
    DrawableObject(points, z)
  {
    textureFilePath = filePath;
    textureId = AppUtils::loadTextureFromBMPFile(textureFilePath.c_str());
    id++;
    myId = id;
  }

  /*
  * getter and setter methods
  */
  string getTextureFilePath() { return textureFilePath; }
  void setTextureFilePath(char * newPath)
  {
    textureFilePath = newPath;
    textureId = AppUtils::loadTextureFromBMPFile(textureFilePath.c_str() );
  }

  GLuint getTextureId(){ return textureId; }
  void setTextureId(GLuint newId){ textureId = newId; }

  int getMyId() { return myId; }

protected:
  string textureFilePath;
  GLuint textureId;

  /*
  * A static int that will allow us to identify the object. Possibly a bad idea
  */
  static int id;
  int myId;

};
#endif