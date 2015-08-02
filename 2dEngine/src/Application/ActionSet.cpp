#include "ActionSet.h"
#include "Event.h"

string ActionSet::dumpObject()
{
  string currentState = "";
  for (auto it = keyPressedMap.begin(); it != keyPressedMap.end(); it++)
  {
    if (it->second.function != NULL)
    {
      currentState += "Keys ";
      for (int i : it->first)
      {
        char asciChar = i;
        currentState += asciChar + " ";
      }
      currentState += "are mapped to function with description: " + it->second.description + "When pressed\n";
    }
  }
  for (auto it = keyReleasedMap.begin(); it != keyReleasedMap.end(); it++)
  {
    if (it->second.function != NULL)
    {
      currentState += "Keys ";
      for (int i : it->first)
      {
        char asciChar = i;
        currentState += asciChar + " ";
      }
      currentState += "are mapped to function with description: " + it->second.description + "When released\n";
    }
  }
  if (mouseMotionFunction.function != NULL)
    currentState += "passive Mouse Fuction: " + mouseMotionFunction.description; +'\n';
  if (leftClickedMouseFunction.function != NULL)
    currentState += "clicked Mouse Function: " + leftClickedMouseFunction.description + '\n';
  if (leftReleasedMouseFunction.function != NULL)
    currentState += "released Mouse Function: " + leftReleasedMouseFunction.description + '\n';
  if (rightClickedMouseFunction.function != NULL)
    currentState += "clicked Mouse Function: " + rightClickedMouseFunction.description + '\n';
  if (rightReleasedMouseFunction.function != NULL)
    currentState += "released Mouse Function: " + rightReleasedMouseFunction.description + '\n';
  return currentState;
}

void ActionSet::addKeyPressedAction(vector<int> keys, string descript, void(*ActionFunction)())
{
  if (keyPressedMap.find(keys) != keyPressedMap.end())
  {
    cout << "replacing function that did " << keyPressedMap[keys].description << " when the keys ";
    for (int i : keys)
      cout << (char)i << " ";
    cout << "were pressed." << endl;
  }

  ActionWrapper newWrapper(descript, ActionFunction);

  keyPressedMap[keys] = newWrapper;
}

void ActionSet::addKeyReleasedAction(vector<int> keys, string descript, void(*ActionFunction)())
{
  if (keyReleasedMap.find(keys) != keyReleasedMap.end())
  {
    cout << "replacing function that did " << keyReleasedMap[keys].description << " when the keys ";
    for (int i : keys)
      cout << (char)i << " ";
    cout << "were released." << endl;
  }

  ActionWrapper newWrapper(descript, ActionFunction);

  keyReleasedMap[keys] = newWrapper;
}

void ActionSet::removeKeyPressedAction(vector<int> keys)
{
  // if we cant remove anything then dont
  if (keyPressedMap.find(keys) == keyPressedMap.end())
    return;
  keyPressedMap.erase(keys);
}

void ActionSet::removeKeyReleasedAction(vector<int> keys)
{
  // if we cant remove anything then dont
  if (keyReleasedMap.find(keys) == keyReleasedMap.end())
    return;
  keyReleasedMap.erase(keys);
}

void ActionSet::doKeyPressedAction(vector<int> keys)
{
  if (keyPressedMap.find(keys) == keyPressedMap.end())
    return;
  if (keyPressedMap[keys].function != NULL)
  {
    keyPressedMap[keys].function();
  }
}

void ActionSet::doKeyReleasedAction(vector<int> keys)
{
  if (keyReleasedMap.find(keys) == keyReleasedMap.end())
    return;
  if (keyReleasedMap[keys].function != NULL)
  {
    keyReleasedMap[keys].function();
  }
}

void ActionSet::registerActions(string setName)
{
  leftClickedEvent::registerFunc(leftClickedMouseFunction.function, setName);
  mouseMovementEvent::registerFunc(mouseMotionFunction.function, setName);
  leftReleasedEvent::registerFunc(leftReleasedMouseFunction.function, setName);
  rightClickedEvent::registerFunc(rightClickedMouseFunction.function, setName);
  rightReleasedEvent::registerFunc(rightReleasedMouseFunction.function, setName);

  //Take the example that Control + S will lauch a save prompt, this function can be triggered
  //when either the control button or S button is pressed
  for (auto it = keyPressedMap.begin(); it != keyPressedMap.end(); it++)
  {
    if (it->second.function != NULL)
    {
      for (int i : it->first)
      {
        keyPressedEvent::registerFunc(it->second.function, setName, i);
      }
    }
  }

  for (auto it = keyReleasedMap.begin(); it != keyReleasedMap.end(); it++)
  {
    if (it->second.function != NULL)
    {
      for (int i : it->first)
      {
        keyReleasedEvent::registerFunc(it->second.function, setName, i);
      }
    }
  }
}

void ActionSet::unregisterActions(string setName)
{
  leftClickedEvent::unregister(setName);
  mouseMovementEvent::unregister(setName);
  leftReleasedEvent::unregister(setName);
  rightClickedEvent::unregister(setName);
  rightReleasedEvent::unregister(setName);

  //Take the example that Control + S will lauch a save prompt, this function can be triggered
  //when either the control button or S button is pressed
  for (auto it = keyPressedMap.begin(); it != keyPressedMap.end(); it++)
  {
    if (it->second.function != NULL)
    {
      for (int i : it->first)
      {
        keyPressedEvent::unregister(setName, i);
      }
    }
  }

  for (auto it = keyReleasedMap.begin(); it != keyReleasedMap.end(); it++)
  {
    if (it->second.function != NULL)
    {
      for (int i : it->first)
      {
        keyReleasedEvent::unregister(setName, i);
      }
    }
  }
}