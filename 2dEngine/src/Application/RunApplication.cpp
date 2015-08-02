#include <Windows.h>
#include <commctrl.h>

#include "..\..\Dependencies\glew\glew.h"
#include "..\..\Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <cstdlib>

#include "RunApplication.h"

#include "ApplicationState.h"
#include "Utils/AppUtils.h"
#include "../Rendering/ColoredDrawable.h"
#include "Editor.h"
#include "../Rendering/EnvLoader.h"
#include "Utils/QuadTree.h"
#include "../Rendering/TexturedDrawable.h"
#include "../Rendering/AnimatedDrawable.h"

#include "Event.h"

using namespace::std;

void RunApplication::display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset OpenGL transformation matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // reset transformation matrix to identity

  //Use a perspective of a square with the same aspect as our application
  //We dont want to clip anything in the Z direction so thats why its so far back
  gluPerspective(90.f, ApplicationState::Instance().getAspectRatio(), 0, 100 );

	// setup look at transformation so that 
	// eye is at : (0,0, 10)
	// look at center is at : (0,0,0)
	// up direction is +y axis

  //TODO: Find a way to have the camera slowly track the main player
  Vector2d cameraPos = ApplicationState::Instance().getCameraPosition();
  float cameraZ = ApplicationState::Instance().getCameraZ();
	gluLookAt(cameraPos.getX(), cameraPos.getY(), cameraZ+0.00000001f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

  /* Storing for reference will make sure to delete it
	glPushMatrix();
	glTranslatef(ship[1], ship[5], 0.f);
	glRotatef(ship[2], 0.f, 0.f, 1);
	drawship();
	glPopMatrix();
  */

  //TODO: current idea is that we will call the physics engine on each object 
  // in the GameState's physical objects, and continue to move them forward
  // then we will do the rendering for the actual objects so it would look 
  // something like this?
  /*
  
  //TODO: add in code to stop a "lag spiral of death" ie if the physics engine
  // takes 1/30'th a second to determine the positions then we should slow our frame
  // rate to match that
  GameState::Instance().processPhysics();
  for( DrawableObject* drawable : ApplicationState::Instance().getDrawableObjects() )
  {
    if( drawable != NULL ) //Fuck segfaults
    {
      drawable->draw();
    }
  }
  */
#if DEBUG
  ApplicationState::Instance().getClickableObjects(1).drawTree();
#endif
  for (DrawableObject* drawable : ApplicationState::Instance().getDrawableObjects())
  {
    if (drawable != NULL) //Fuck segfaults
    {
      drawable->drawObject();
    }
  }

  if ( Editor::Instance()->isPluggedIn())
  {
   Editor::Instance()->drawEditorObjects();
  }
  glFlush();
	glutSwapBuffers();  // swap front/back framebuffer to avoid flickering 
}

void RunApplication::reshape(int w, int h)
{
	// reset viewport ( drawing screen ) size
	glViewport(0, 0, w, h);
	float fAspect = ((float)w) / h;
  ApplicationState::Instance().setAspectRatio(fAspect);
  ApplicationState::Instance().setYSize(h);
	// reset OpenGL projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.f, fAspect, 0.001f, 30.f);
}

void RunApplication::timer(int v)
{
	glutPostRedisplay(); // trigger display function by sending redraw into message queue
	glutTimerFunc(1000 / ApplicationState::Instance().getFPS(), timer, v); // restart timer again
}

//two functions for buffered key input
void RunApplication::keyup(unsigned char key, int x, int y)
{
  ApplicationState::Instance().releaseKey(key);
  keyReleasedEvent::trigger(key);
  cout << "releasing key " << (int)key << endl;
}

void RunApplication::keydown(unsigned char key, int x, int y)
{
  ApplicationState::Instance().pressKey((int)key);
  callKeyEvents();
  /*
  //TODO
  OPENFILENAME ofn;       // common dialog box structure
  char szFile[260];       // buffer for file name
  HANDLE hf;              // file handle

  // Initialize OPENFILENAME
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.lpstrFile = szFile;
  // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
  // use the contents of szFile to initialize itself.
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  // Display the Open dialog box. 

  if (GetOpenFileName(&ofn) == TRUE)
    hf = CreateFile(ofn.lpstrFile,
    GENERIC_READ,
    0,
    (LPSECURITY_ATTRIBUTES)NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    (HANDLE)NULL);
    */
}

/*
I feel like this is really really bad style, but i want if you press and hold button1 then
button 2 and hold it, and let go of button 1, keyDown on button 1 is not still triggered
*/
void RunApplication::callKeyEvents()
{
  for (int key : ApplicationState::Instance().getPressedKeys())
  {
    cout << "event on key" << key << endl;
    keyPressedEvent::trigger(key);
  }
}

void RunApplication::mouseUp(int x, int y)
{
  //TODO
}

void RunApplication::mouseMotion(int x, int y)
{
  Vector2d mousePosition(x, y);
  ApplicationState::Instance().setMouseScreenPosition(mousePosition);
  ApplicationState::Instance().convertScreenVecToWorld(mousePosition);
  ApplicationState::Instance().setMousePosition(mousePosition);
  mouseMovementEvent::trigger();
}



void RunApplication::mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    
    ApplicationState::Instance().setMouseIsDown(true);
    
    glutMotionFunc(mouseMotion);
  }
  else if (state == GLUT_UP)
  {
    ApplicationState::Instance().setMouseIsDown(false);
  }
}

//TODO move these fucntions to somewhere else? or just leave this here
// cause the editor is a developer tool
void RunApplication::startStopEditor()
{
  if ( !Editor::Instance()->isPluggedIn())
  {
    cout << "Weaboo" << endl;
    Editor::Instance()->plugIn();
  }
  else if ( Editor::Instance()->isPluggedIn())
  {
    cout << "Yata" << endl;
    Editor::Instance()->unplug();
  }
}


int main(int argc, char* argv[])
{
	srand((unsigned int)time(0));  // seeding RNG

	glutInit(&argc, (char**)argv);

	//set up for double-buffering & RGB color buffer & depth test
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  ApplicationState::Instance().setYSize(700);
  ApplicationState::Instance().setAspectRatio(1.5);

	glutInitWindowSize( (ApplicationState::Instance().getYSize()*ApplicationState::Instance().getAspectRatio()) ,
                       ApplicationState::Instance().getYSize());
	glutInitWindowPosition(100, 100);
	glutCreateWindow((const char*)"2D Asteroid");

  // init data, setup OpenGL environment here
  glClearColor(0.3, 0.3, 0.3, 1.0); // clear color is gray
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // uncomment this function if you only want to draw wireframe model

  ApplicationState::Instance().setFPS(60);
  ApplicationState::Instance().setCameraZ(10.f);

  //TODO Remove this? set up the editor
  keyPressedEvent::registerFunc(RunApplication::startStopEditor, "MainApplication", 5);

	// set up the call-back functions 
  glutDisplayFunc(RunApplication::display);  // called when drawing 
  glutReshapeFunc(RunApplication::reshape);  // called when change window size
  glutKeyboardFunc(RunApplication::keydown); // called when received keyboard interaction
  glutKeyboardUpFunc(RunApplication::keyup);
  glutMouseFunc(RunApplication::mouse);       // called when received mouse interaction
  glutPassiveMotionFunc(RunApplication::mouseMotion);
  glutTimerFunc(100, RunApplication::timer, ApplicationState::Instance().getFPS()); // a periodic timer. Usually used for updating animation

  
  EnvLoader::Instance().loadEnvironment("environment.txt", "testEnv");
	glutMainLoop(); // start the main message-callback loop

  keyPressedEvent::unregister("MainApplication", 5);

	return 0;
}