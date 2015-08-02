#ifndef APPLICATION_UTILS_H
#define APPLICATION_UTILS_H

#include <stdlib.h>
#include <process.h>
#include <vector>
#include "..\..\Dependencies\glew\glew.h"
#include "..\..\Dependencies\freeglut\freeglut.h"

#include "Vector2D.h"

#define PI 3.14159265

/*
A Class that holds static methods for common utilities any application 
might need. There will be a seperate game Utils class that should hold
static methods specific to the implementation of the game
*/

class AppUtils
{
  public:
    /*
    Uses rand(), so the entropy is pretty low and predictable so 
    it also uses the processID of the application itself
    */
    static bool randomBool()
    {
      return ( ( ( rand()*_getpid() ) % 2 ) == 1);
    }

    /*
    Uses rand(), so the entropy is pretty low and predictable so 
    it also uses the processID of the application itself

    Returns number from -1 to 1
    */
    static float randomFloat()
    {
      if (randomBool())
      {
        return ( ( (float)rand()*_getpid() ) /  (float)RAND_MAX );
      }
      else return -( ( (float)rand()*_getpid() ) / (float)RAND_MAX );
    }

    /*
    Helper function that is used to take a vector of 2d Vectors and give us the least X and largest
    X by reference. Useful for bounding boxes
    */
    static void findLeastAndLargestX(vector<Vector2d> vector, float& leastX, float& largestX)
    {
      if (vector.size() == 0)
        return;
      
      leastX = vector[0].getX();
      largestX = vector[0].getX();
      for (Vector2d vect : vector)
      {
        if (vect.getX() < leastX)
          leastX = vect.getX();
        else if (vect.getX() > largestX)
          largestX = vect.getX();
      }
    }

    /*
    Helper function that is used to take a vector of 2d Vectors and give us the least Y and largest
    Y by reference. Useful for bounding boxes
    */
    static void findLeastAndLargestY(vector<Vector2d> vector, float& leastY, float& largestY)
    {
      if (vector.size() == 0)
        return;

      leastY = vector[0].getY();
      largestY = vector[0].getY();
      for (Vector2d vect : vector)
      {
        if (vect.getY() < leastY)
          leastY = vect.getY();
        else if (vect.getY() > largestY)
          largestY = vect.getY();
      }
    }

    /*
    Helper Function that takes checks if a value is between two bounding values.
    */
    static bool valueIsBounded(float value, float lowerBound, float higherBound)
    {
      return (value >= lowerBound && value <= higherBound);
    }

    /* 
    Function that will take in the path of a bmp file and give us back an array
    that holds its pixel values. Then will call fill up that GL Texture and return the 
    texture Id that openGL creates

    For quesstions or forgetfulness wikipedia BMP file format
    */
    static GLuint loadTextureFromBMPFile(const char* imagePath)
    {
      unsigned char header[54]; // Header for the BMP format is 54 bytes
      unsigned int width, height, imageSize;
      unsigned char * data;

      FILE * file = fopen(imagePath, "rb");
      if (!file)
      {
        cout << "Could not open a file for the path " << imagePath << endl;
        return 0;
      }

      if (fread(header, 1, 54, file) != 54)
      {
        cout << "Could not open the 54 bits for the BMP's header." << endl;
        return 0;
      }

      if (header[0] != 'B' || header[1] != 'M')
      {
        cout << "Not BM as the first two bytes this is not the correct file format." << endl;
        return 0;
      }

      imageSize = *(int*)&(header[0x22]);
      width = *(int*)&(header[0x12]);
      height = *(int*)&(header[0x16]);

      //if the imageSize isnt given we can just fill it in 
      if (imageSize == 0)
      {
        imageSize = height * width * 3;
      }

      data = new unsigned char[imageSize];
      if (fread(data, 1, imageSize, file) != imageSize)
      {
        cout << "Was unable to read the " << imageSize << " bytes of data we were expecting" << endl;
      }
      fclose(file);
     
      // The actual id of the texture we want, what were returning
      GLuint textureId;
      glGenTextures(1, &textureId);

      glBindTexture(GL_TEXTURE_2D, textureId);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

      return textureId;
    }

    /*
    A helper function that will by default tokenize the given string around the space character.
    It can be passed in any string however and we will return a vector that is tokenized around that string
    */
    static vector<string> tokenizeString(const string& str, const string& delimiters = " ")
    {
      vector<string> tokens;
      // Skip any delims to start
      string::size_type lastPos = str.find_first_not_of(delimiters, 0);
      // find next delimiter
      string::size_type pos = str.find_first_of(delimiters, lastPos);

      //Now cycle it up and put it into a vector
      while (string::npos != pos || string::npos != lastPos)
      {
        tokens.push_back(str.substr(lastPos, pos - lastPos));

        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
      }
      return tokens;
    }
};

#endif