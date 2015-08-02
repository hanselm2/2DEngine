#include "ApplicationState.h"

ApplicationState* ApplicationState::instance = NULL;
// Adjust this to get the initial ratio

void ApplicationState::addDrawableObject(DrawableObject& drawable)
{
  drawableObjects.push_back(&drawable);

  if (drawable.isClickable())
    addClickableObject(drawable, drawable.getLayer());
}

void  ApplicationState::addClickableObject(DrawableObject& clickable, int z)
{
  if (!clickable.isClickable())
    return;
  clickableObjects[z].insert(clickable);
}

DrawableObject* ApplicationState::getObjectClicked(Vector2d& mousePosition)
{
  DrawableObject *  closestObject = NULL;
  DrawableObject *  clickedObject = NULL;
  int highestLayer = 0;
  for (auto it = clickableObjects.begin(); it != clickableObjects.end(); it++)
  {
    closestObject = it->second.findClosest(mousePosition);

    if (closestObject != NULL && closestObject->isClicked(mousePosition))
    {
      // remember the QuadTrees are indexed on their layers
      if (it->first > highestLayer)
      {
        clickedObject = closestObject;
        highestLayer = it->first;
      }
    }
  }
  return clickedObject;
}

DrawableObject* ApplicationState::getObjectClicked()
{
  DrawableObject *  closestObject = NULL;
  DrawableObject *  clickedObject = NULL;
  int highestLayer = 0;
  for (auto it = clickableObjects.begin(); it != clickableObjects.end(); it++)
  {
    closestObject = it->second.findClosest(mousePosition);

    if (closestObject != NULL && closestObject->isClicked(mousePosition))
    {
      // remember the QuadTrees are indexed on their layers
      if (it->first > highestLayer)
      {
        clickedObject = closestObject;
        highestLayer = it->first;
      }
    }
  }
  return clickedObject;
}

DrawableObject* ApplicationState::getObjectClickedForEditor()
{
  DrawableObject *  clickedObject = NULL;
  int highestLayer = 0;
  for (DrawableObject* obj : drawableObjects)
  {
    if ( obj->isClicked(mousePosition) )
    {
      if (obj->getLayer() > highestLayer)
      {
        clickedObject = obj;
        highestLayer = obj->getLayer();
      }
    }
  }
  return clickedObject;
}

bool ApplicationState::deleteDrawableObject(DrawableObject& obj)
{
  for (auto it = drawableObjects.begin(); it != drawableObjects.end(); it++)
  {
    // Do it Dirty!
    if (&(*(*it)) == &obj)
    {
      delete *it;
      drawableObjects.erase(it);
      return true;
    }
  }
  return false;
}

DrawableObject* ApplicationState::getObjectClicked(Vector2d& mousePosition, int layer)
{
  DrawableObject * closestObject = clickableObjects[layer].findClosest(mousePosition);

  if (closestObject != NULL && closestObject->isClicked(mousePosition))
    return closestObject;
  return NULL;
}