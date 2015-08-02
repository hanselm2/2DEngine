#ifndef EVENT_H
#define EVENT_H

#include "Object.h"
#include <map>

/*
An event will be a class and all the "Type" of events will be typedefed in the global 
namespace below at the end of the file.
*/
template <int type>
class Event : public Object
{
  public:

    virtual string toString()
    {
      return "Event"+type;
    }

    virtual string dumpObject()
    {
      return "Events dont really have anything to dump.";
    }
    
    static Event<type>& Instance()
    {
      static Event<type> instance;
      return instance;
    }

    //TODO put more thought into should we spawn threads to do this?
    static void trigger( int key = -1)
    {
      for (auto it = Instance().eventFunctions.begin(); it != Instance().eventFunctions.end(); it++)
      {
        if ( it->first.second == key && it->second != NULL )
          it->second();
      }
    }

    static void registerFunc( void (*func)(), string actionSet , int key = -1)
    {
      if (Instance().eventFunctions.find(std::pair<string, int>{ actionSet, key }) != Instance().eventFunctions.end())
      {
        cout << "ERROR! tried to register a function with an actionSet of " << actionSet << " and a key of "
          << key << " but there already was a function registered to this string" << endl;
        return;
      }
      Instance().eventFunctions[std::pair<string, int>{ actionSet, key }] = func;
    }

    static void unregister(string actionSet, int key = -1)
    {
      if (Instance().eventFunctions.find(std::pair<string, int>{ actionSet, key }) == Instance().eventFunctions.end())
      {
        cout << "WARNING! Tried to unregister a function with an actionSet of " << actionSet << " but it was" <<
          " never registered" << endl;
        return;
      }

      Instance().eventFunctions.erase(std::pair<string, int>{ actionSet, key });
    }
  
  protected:

    Event()
    {
      //NoCode
    }

    typedef void(*eventFunction)();

    map<std::pair<string, int> , eventFunction> eventFunctions;
 
};


enum events{
  leftMouseClicked, leftMouseReleased, rightMouseClicked, rightMouseReleased,
  mouseMovement, keyPressed, keyReleased
};

typedef Event<leftMouseClicked> leftClickedEvent;
typedef Event<mouseMovement> mouseMovementEvent;
typedef Event<leftMouseReleased> leftReleasedEvent;
typedef Event<rightMouseClicked> rightClickedEvent;
typedef Event<rightMouseReleased> rightReleasedEvent;
typedef Event<keyPressed> keyPressedEvent;
typedef Event<keyReleased> keyReleasedEvent;

#endif