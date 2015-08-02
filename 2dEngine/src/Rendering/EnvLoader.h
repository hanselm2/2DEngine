#ifndef ENVLOADER_H
#define ENVLOADER_H

#include "../Application/Object.h"
#include "../Application/Utils/Vector2D.h"

#include <vector>
#include <map>


class EnvLoader : public Object
{
public:
  virtual string dumpObject()
  {
    return "EnvLoader: TODO";
  }

  virtual string toString()
  {
    return "EnvLoader";
  }

  static EnvLoader& Instance()
  {
    static EnvLoader instance;
    return instance;
  }

  void loadEnvironment(string fileName, string envName);
  
  void saveEnvironment(string fileName, string envName);
protected:

  /*
  Helper function that will tell us if we are currently in any block
  */
  bool inAnyBlock();

  bool inAnimationBlock;
  bool inVectorBlock;

  map<string, vector<string>> animationMap;

  map<string, vector<Vector2d>> vectorMap;

};
#endif