#include "ColoredDrawable.h"

#include "..\..\Dependencies\glew\glew.h"
#include "..\..\Dependencies\freeglut\freeglut.h"

int ColoredDrawable::id = 0;
void ColoredDrawable::drawObject()
{
  glColor4f(r, g, b, a);
  glBegin(GL_POLYGON);
  for (Vector2d vect : points)
  {
    glVertex3f(vect.getX(), vect.getY(), actualZ);
  }
  glEnd();
  #if DEBUG
  if(boundingBox.size() == 4)
    glColor4f(0.f, 1.f, 0.f, 1.0);
  glBegin(GL_LINE_STRIP);
    glVertex3f(boundingBox[0].getX(), boundingBox[0].getY(), actualZ);
    glVertex3f(boundingBox[1].getX(), boundingBox[1].getY(), actualZ);
    glVertex3f(boundingBox[2].getX(), boundingBox[2].getY(), actualZ);
    glVertex3f(boundingBox[3].getX(), boundingBox[3].getY(), actualZ);
    glVertex3f(boundingBox[0].getX(), boundingBox[0].getY(), actualZ);
  glEnd();
  #endif
}