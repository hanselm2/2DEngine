#include "TexturedDrawable.h"

#include "..\..\Dependencies\glew\glew.h"
#include "..\..\Dependencies\freeglut\freeglut.h"

int TexturedDrawable::id = 0;
void TexturedDrawable::drawObject()
{
  if (textureFilePath != "")
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }
  // Use the fact that our bounding box goes from lowestX/Y to highest X
  // to HighestX/Y etc to find the part of the texture for the polygon
  glColor4f(1.f, 1.f, 1.f, 1.f);
  glBegin(GL_POLYGON);
  for (Vector2d vect : points)
  {
    GLdouble x = (vect.getX() - boundingBox[0].getX()) / 
                 (boundingBox[0].getX() - boundingBox[1].getX());
    GLdouble y = (vect.getY() - boundingBox[0].getY()) /
                 (boundingBox[0].getY() - boundingBox[3].getY());
    glTexCoord2d(x, y);

    glVertex3f(vect.getX(), vect.getY(), actualZ);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);

#if DEBUG
  if (boundingBox.size() == 4)
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