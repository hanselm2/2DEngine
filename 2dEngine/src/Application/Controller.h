#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Object.h"
#include "ActionSet.h"

/*
A Controller is backed by the notion of the ActionSet class and the event system
It is an abstract class that gaurentees the controler can be plugged in and unplugged
and will have an actionSet that backs it up
*/

class Controller : public Object
{
public:
  virtual string toString()
  {
    return "Editor";
  }

  virtual string dumpObject() = 0;

  ActionSet& getActions()
  {
    return *myActions;
  }

  bool isPluggedIn()
  {
    return pluggedIn;
  }

  virtual void plugIn()
  {
    loadActions();
    pluggedIn = true;
  }

  virtual void unplug()
  {
    unloadActions();
    delete myActions;
    myActions = NULL;
    pluggedIn = false;
  }

protected: 

  ActionSet* myActions;
  virtual void loadActions() = 0;
  virtual void unloadActions() = 0;
  
  bool pluggedIn;

};

#endif