#include "..\..\Dependencies\glew\glew.h"
#include "..\..\Dependencies\freeglut\freeglut.h"

#include "Editor.h"

/*
Constructor, that initially sets isStarted to false, and loads up its ActionSet.
*/
Editor::Editor()
{
  pluggedIn = false;
  squareSize = 0.1;
  dragStart = Vector2d{ 0.f, 0.f };
  dragEnd = Vector2d{ 0.f, 0.f };
  selectedPoint = NULL;
}

void Editor::loadActions()
{
  myActions = new ActionSet();
  myActions->addKeyPressedAction(vector < int > {12},
    "Opens up a file chooser, that will let you load an environment to the screen",
    loadEnvironment);

  myActions->addKeyPressedAction(vector < int > {19},
    "Opens up a file chooser, that will let you save off the current environment on the screen",
    saveEnvironment);

  myActions->setLeftMouseClickedFunction(
    "Selects whatever object the mouse is on as the selected object and allows editing of it",
    clickedMouse);

  myActions->setLeftMouseReleasedFunction(
    "Sets the dragged start and stopped equal to eachother, and selects whatever objects was within that region",
    releasedMouse);
  
  myActions->setMouseMotionFunction(
    "Passively updates the start and stop of the drag box if the left mouse button is down",
    mouseMotion);

  myActions->addKeyPressedAction(vector < int > {20},
    "Opens up a file chooser, that will let you change the texture if the selected Object is not null and only 1",
    texture);

  myActions->addKeyPressedAction(vector < int > {27},
    "Clears any selected Objects in the editor",
    pressedEsc);

  myActions->addKeyPressedAction(vector < int > {127},
    "Deletes a vertex point of an object if one has been selected",
    pressedDelete);

  myActions->addKeyPressedAction(vector < int > {119},
    "Moves a vertex point of an object if one has been selected up",
    pressedUp);

  myActions->addKeyPressedAction(vector < int > {115},
    "Moves a vertex point of an object if one has been selected down",
    pressedDown);

  myActions->addKeyPressedAction(vector < int > {97},
    "Moves a vertex point of an object if one has been selected left",
    pressedLeft);

  myActions->addKeyPressedAction(vector < int > {100},
    "Moves a vertex point of an object if one has been selected right",
    pressedRight);

  myActions->registerActions("Editor");
}

void Editor::unloadActions()
{
  myActions->unregisterActions("Editor");
}

void Editor::drawEditorObjects()
{
  //For each object that we have selected, we must display the points that make up their
  //vertex, draw a square that we can click on them, and then the  bounding box for the object
  for (DrawableObject* object : selectedObjects)
  {
    vector<Vector2d> boundingBox = object->getBoundingBox();
    float actualZ = object->getLayer() *  0.000000000001f;
    if (boundingBox.size() != 4)
      continue;
    glColor4f(0.f, 1.f, 0.f, 1.0);
    glBegin(GL_LINE_STRIP);
      glVertex3f(boundingBox[0].getX(), boundingBox[0].getY(), actualZ);
      glVertex3f(boundingBox[1].getX(), boundingBox[1].getY(), actualZ);
      glVertex3f(boundingBox[2].getX(), boundingBox[2].getY(), actualZ);
      glVertex3f(boundingBox[3].getX(), boundingBox[3].getY(), actualZ);
      glVertex3f(boundingBox[0].getX(), boundingBox[0].getY(), actualZ);
    glEnd();

    
    //Now draw the square for each point in the bounding box
    glColor4f(0.5f, 0.f, 0.7f, 0.1f);
    for ( Vector2d point : object->getPoints() )
    {
      glBegin(GL_POLYGON);
        glVertex3f(point.getX() - squareSize, point.getY() + squareSize, actualZ);
        glVertex3f(point.getX() + squareSize, point.getY() + squareSize, actualZ);
        glVertex3f(point.getX() + squareSize, point.getY() - squareSize, actualZ);
        glVertex3f(point.getX() - squareSize, point.getY() - squareSize, actualZ);
        glVertex3f(point.getX() - squareSize, point.getY() + squareSize, actualZ);
      glEnd();
    }
  }

  // Now draw our drag box if we have anything to draw
  if (dragStart == dragEnd)
    return;

  glColor4f(1.f, 0.f, 0.f, 1.0);
  glBegin(GL_LINE_STRIP);
  glVertex3f(dragStart.getX(), dragStart.getY(), 0.f);
  glVertex3f(dragEnd.getX(), dragStart.getY(), 0.f);
  glVertex3f(dragEnd.getX(), dragEnd.getY(), 0.f);
  glVertex3f(dragStart.getX(), dragEnd.getY(), 0.f);
  glVertex3f(dragStart.getX(), dragStart.getY(), 0.f);
  glEnd();
}

void Editor::loadEnvironment()
{

}

void Editor::saveEnvironment()
{

}

void Editor::clickedMouse()
{
  Instance()->dragStart = ApplicationState::Instance().getMousePosition();
  Instance()->dragEnd = ApplicationState::Instance().getMousePosition();
  // Just be safe incase we forget to do this.
  ApplicationState::Instance().setMouseIsDown(true);

  //First check if we already have objects being displayed as editable, if so then 
  // we *may* be able to click on the square displayed around one of the points,
  // but still be outside the point
  DrawableObject* clickable = ApplicationState::Instance().getObjectClickedForEditor();
  if (clickable != NULL)
  {
    Instance()->selectedObjects.clear();
    Instance()->selectedObjects.push_back(clickable);
  }

  // Now lets try to find out if we clicked on a vertix/point of the an object we have displayed
  // Again, were not concerned about speed here cause its the editor
  Instance()->selectedPoint = NULL;
  for (DrawableObject* obj : Instance()->selectedObjects)
  {
    Vector2d mousePos = ApplicationState::Instance().getMousePosition();
    bool foundPoint = false;
    for (Vector2d point : obj->getPoints())
    {
      if (point.isWithinSquareRange(mousePos, Instance()->squareSize*2.f))
      {
        // Bug? if two points are points that are right on top of eachother we select the last
        // one that we see when we iterate
        Instance()->selectedPoint = new Vector2d(point.getX(), point.getY());
        
        foundPoint = true;
      }
    }
    if (foundPoint)
    {
      // Now weve only selected one object for sure
      Instance()->selectedObjects.clear();
      Instance()->selectedObjects.push_back(obj);
      break;
    }
  }
  if (Instance()->selectedPoint != NULL )
    cout << "our vertex point is at " << Instance()->selectedPoint->getX() << " " << Instance()->selectedPoint->getY() << endl;
}

void Editor::releasedMouse()
{
  //The editor does not care about speed so we wont find a fancy way to see that objects weve selected
  // with a quadtree
}

void Editor::mouseMotion()
{
  if (ApplicationState::Instance().getMouseIsDown())
    Instance()->dragEnd = ApplicationState::Instance().getMousePosition();
  else
  {
    Instance()->dragStart = Vector2d{ 0.f, 0.f };
    Instance()->dragEnd = Vector2d{ 0.f, 0.f };
  }
}

void Editor::texture()
{

}

void Editor::pressedEsc()
{
  Instance()->selectedObjects.clear();
}

void Editor::pressedDelete()
{
  if (Instance()->selectedPoint != NULL)
  {
    //Okay now just set the points
    if (Instance()->selectedObjects.size() != 1)
    {
      cout << "ERROR: We tried to delete a point but our selectedObjects wasnt 1 larger" << endl;
      return;
    }
    if (!Instance()->selectedObjects[0]->removePoint(*(Instance()->selectedPoint)))
    {
      cout << "Was not able to remove the point";
    }
    Instance()->selectedPoint = NULL;
  }
  else
  {
    auto it = Instance()->selectedObjects.begin();
    while (it != Instance()->selectedObjects.end())
    {
      ApplicationState::Instance().deleteDrawableObject(*(*it));
      it = Instance()->selectedObjects.erase(it);
    }
  }
}

void Editor::pressedUp()
{
  if (Instance()->selectedPoint != NULL)
  {
    //Okay now just set the points
    if (Instance()->selectedObjects.size() != 1)
    {
      cout << "ERROR: We tried to move a point but our selectedObjects wasnt 1 larger" << endl;
      return;
    }
    if (!Instance()->selectedObjects[0]->movePoint(*(Instance()->selectedPoint), 
                                                    Vector2d{ 0.f, Instance()->squareSize }))
    {
      cout << "Was not able to move the point because we couldnt find it in the object";
    }
  }
  else
  {
    for (DrawableObject* obj : Instance()->selectedObjects)
    {
      obj->move(Vector2d{ 0.f, Instance()->squareSize*2.f });
    }
  }
}

void Editor::pressedDown()
{
  if (Instance()->selectedPoint != NULL)
  {
    //Okay now just set the points
    if (Instance()->selectedObjects.size() != 1)
    {
      cout << "ERROR: We tried to move a point but our selectedObjects wasnt 1 larger" << endl;
      return;
    }
    if (!Instance()->selectedObjects[0]->movePoint(*(Instance()->selectedPoint),
      Vector2d{ 0.f, -Instance()->squareSize }))
    {
      cout << "Was not able to move the point because we couldnt find it in the object";
    }
  }
  else
  {
    for (DrawableObject* obj : Instance()->selectedObjects)
    {
      obj->move(Vector2d{ 0.f, -Instance()->squareSize*2.f });
    }
  }
}

void Editor::pressedRight()
{
  if (Instance()->selectedPoint != NULL)
  {
    //Okay now just set the points
    if (Instance()->selectedObjects.size() != 1)
    {
      cout << "ERROR: We tried to move a point but our selectedObjects wasnt 1 larger" << endl;
      return;
    }
    if (!Instance()->selectedObjects[0]->movePoint(*(Instance()->selectedPoint),
      Vector2d{ Instance()->squareSize, 0.f }))
    {
      cout << "Was not able to move the point because we couldnt find it in the object";
    }
  }
  else
  {
    for (DrawableObject* obj : Instance()->selectedObjects)
    {
      obj->move(Vector2d{ Instance()->squareSize*2.f, 0.f });
    }
  }
}

void Editor::pressedLeft()
{
  if (Instance()->selectedPoint != NULL)
  {
    //Okay now just set the points
    if (Instance()->selectedObjects.size() != 1)
    {
      cout << "ERROR: We tried to move a point but our selectedObjects wasnt 1 larger" << endl;
      return;
    }
    if (!Instance()->selectedObjects[0]->movePoint(*(Instance()->selectedPoint),
      Vector2d{ -Instance()->squareSize, 0.f }))
    {
      cout << "Was not able to move the point because we couldnt find it in the object";
    }
  }
  else
  {
    for (DrawableObject* obj : Instance()->selectedObjects)
    {
      obj->move(Vector2d{ -Instance()->squareSize*2.f, 0.f });
    }
  }
}

