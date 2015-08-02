#ifndef ACTION_SET
#define ACTION_SET

#include "ApplicationState.h"
#include "Object.h"

#include <map>
#include <vector>
/*
A object that will hold all of the actions for button presses
and mouse clicks, it can be switched out for new environments
or change based on other things
*/

class ActionSet : public Object
{
  public:
    virtual string dumpObject();

    virtual string toString()
    {
      return "ActionSet";
    }

    ActionSet()
    {
    }

    /*
    Adds the given function to the map
    Takes the keys you want pressed to activate it eg.(control + save) asci values
    The description of what will happen when you press it
    And the function pointer itself
    */
    void addKeyPressedAction(vector<int> keys, string descript, void(*ActionFunction)());
   
    void addKeyReleasedAction(vector<int> keys, string descript, void(*ActionFunction)());

    /*
    Removes any action for the given buttons
    */
    void removeKeyPressedAction(vector<int> keys);
    
    void removeKeyReleasedAction(vector<int> keys);
    
    /*
    Executes the action for the given set of keys if it has it
    */
    void doKeyPressedAction(vector<int> keys);

    void doKeyReleasedAction(vector<int> keys);
    
    /*
    Executes the action for the given passive mouse function
    */
    void doMouseMotion()
    {
      if (mouseMotionFunction.function != NULL)
        mouseMotionFunction.function();
    }

    /*
    Executes the action for the given clicked mouse function
    */
    void doLeftMouseClicked()
    {
      if (leftClickedMouseFunction.function != NULL)
        leftClickedMouseFunction.function();
    }

    /*
    Executes the action for the given released mouse function
    */
    void doLeftMouseReleased()
    {
      if (leftReleasedMouseFunction.function != NULL)
        leftReleasedMouseFunction.function();
    }

    /*
    Executes the action for the given clicked mouse function
    */
    void doRightMouseClicked()
    {
      if (rightClickedMouseFunction.function != NULL)
        rightClickedMouseFunction.function();
    }

    /*
    Executes the action for the given released mouse function
    */
    void doRightMouseReleased()
    {
      if (rightReleasedMouseFunction.function != NULL)
        rightReleasedMouseFunction.function();
    }

    void setMouseMotionFunction(string des, void(*foo)())
    {
      ActionWrapper newWrapper(des, foo);

      mouseMotionFunction = newWrapper;
    }
    void removeMouseMotionFunction()
    {
      mouseMotionFunction = ActionWrapper();
    }

    void setLeftMouseClickedFunction(string des, void(*foo)())
    {
      ActionWrapper newWrapper(des, foo);

      leftClickedMouseFunction = newWrapper;
    }
    void removeLeftMouseClickedFunction()
    {
      leftClickedMouseFunction = ActionWrapper();
    }

    void setLeftMouseReleasedFunction(string des, void(*foo)())
    {
      ActionWrapper newWrapper(des, foo);

      leftReleasedMouseFunction = newWrapper;
    }
    void removeLeftMouseReleasedFunction()
    {
      leftReleasedMouseFunction = ActionWrapper();
    }

    void setRightMouseClickedFunction(string des, void(*foo)())
    {
      ActionWrapper newWrapper(des, foo);

      rightClickedMouseFunction = newWrapper;
    }
    void removeRightMouseClickedFunction()
    {
      rightClickedMouseFunction = ActionWrapper();
    }

    void setRightMouseReleasedFunction(string des, void(*foo)())
    {
      ActionWrapper newWrapper(des, foo);

      rightReleasedMouseFunction = newWrapper;
    }
    void removeRightMouseReleasedFunction()
    {
      rightReleasedMouseFunction = ActionWrapper();
    }

    /*
    Register's all of the ActionSet's functions to the corresponding events
    */
    void registerActions(string setName);

    void unregisterActions(string setName);

  protected:
    /*
    The action function will act on static/global objects in the GameState namespace
    so it will have to have no parameters and no return type
    */
    typedef void(*ActionFunction) ();

    /*
    We might want to add more things that will be associated with the action function
    but for now, we will just have a string that describes the function and the function 
    itself
    */
    struct ActionWrapper
    {
      ActionWrapper()
      {
        description = "";
        function = NULL;
      }

      ActionWrapper(string des, void(*foo)())
      {
        description = des;
        function = foo;
      }
      string description;
      ActionFunction function;
    };
    map< vector<int>, ActionWrapper > keyPressedMap;
    map< vector<int>, ActionWrapper > keyReleasedMap;
    
    /*
    Three other fucntions are particularly special as they control the mouse.
    1) mouse movement
    2) action on initial clicking
    3) action on dragging while clicked
    */
    ActionWrapper mouseMotionFunction;
    ActionWrapper leftClickedMouseFunction;
    ActionWrapper leftReleasedMouseFunction;
    ActionWrapper rightClickedMouseFunction;
    ActionWrapper rightReleasedMouseFunction;
};

#endif