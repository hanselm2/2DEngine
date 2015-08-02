#include <fstream>
#include <sstream>

#include "../Application/Utils/AppUtils.h"
#include "AnimatedDrawable.h"
#include "DrawableObject.h"
#include "ColoredDrawable.h"
#include "TexturedDrawable.h"
#include "EnvLoader.h"

void EnvLoader::loadEnvironment(string fileName, string envName)
{
  ifstream infile("lib\\" + fileName, ifstream::in);
  string line;
  bool lookingForEnv = false;
  while (getline(infile, line) && !lookingForEnv)
  {
    // We first look for the environment Name ie Environment "envName" start
    if (line.find("Environment "+envName+" start") != string::npos)
    {
      lookingForEnv = true;
      continue;
    }
  }

  bool inAnimationBlock = false;
  bool inVectorBlock = false;
  
  string currentKey = "";
  // Now process the entire block for the given environment
  while (getline(infile, line) && lookingForEnv)
  { 
    // exit if we hit the end
    if (line.find("Environment " + envName + " end") != string::npos)
    {
      lookingForEnv = false;
      continue;
    }

    //First check to see if we are starting or stopping a block
    if (line.find("VectorPoints") != string::npos)
    {
      if (currentKey == "" && !inAnyBlock())
      {
        if (line.find("start") != string::npos)
        {
          vector<string> tokens = AppUtils::tokenizeString(line);
          if (tokens.size() != 3)
          {
            cout << "ERROR! Tried to tokenize the line " << line <<
              " and got back a token string that was not of 3 tokens" << endl;
            break;
          }
          currentKey = tokens[1];
          inVectorBlock = true;
        }
        else
        {
          cout << "ERROR! Parsing the line " << line << " and we saw the key work VectorPoints\n" <<
            "which means we are starting or ending a VectorPoints block, and we expected\n to be starting one" <<
            " but we cant find the words start on this line" << endl;
          break;
        }
      }
      else if (currentKey != "" && inVectorBlock)
      {
        if (line.find("end") != string::npos)
        {
          vector<string> tokens = AppUtils::tokenizeString(line);
          if (tokens.size() != 3)
          {
            cout << "ERROR! Tried to tokenize the line " << line <<
              " and got back a token string that was not of 3 tokens" << endl;
            break;
          }
          if (tokens[1] != currentKey)
          {
            cout << "ERROR! Read in the line " << line << " and saw that we are ending\n" <<
              "a VectorPointsBlock but the given name we tokenized is " << tokens[1] << " and\n" <<
              "this doesn't match our current blockKey of " << currentKey << endl;
            break;
          }
          currentKey = "";
          inVectorBlock = false;
        }
      }
      continue;
    }
    else if (line.find("Animation") != string::npos)
    {
      if (currentKey == "" && !inAnyBlock())
      {
        if (line.find("start") != string::npos)
        {
          vector<string> tokens = AppUtils::tokenizeString(line);
          if (tokens.size() != 3)
          {
            cout << "ERROR! Tried to tokenize the line " << line <<
              " and got back a token string that was not of 3 tokens" << endl;
            break;
          }
          currentKey = tokens[1];
          inAnimationBlock = true;
        }
        else
        {
          cout << "ERROR! Parsing the line " << line << " and we saw the key word Animation\n" <<
            "which means we are starting or ending a Animation block, and we expected\n to be starting one" <<
            " but we cant find the word start on this line" << endl;
          break;
        }
      }
      else if (currentKey != "" && inAnimationBlock)
      {
        if (line.find("end") != string::npos)
        {
          vector<string> tokens = AppUtils::tokenizeString(line);
          if (tokens.size() != 3)
          {
            cout << "ERROR! Tried to tokenize the line " << line <<
              " and got back a token string that was not of 3 tokens" << endl;
            break;
          }
          if (tokens[1] != currentKey)
          {
            cout << "ERROR! Read in the line " << line << " and saw that we are ending\n" <<
              "a AnimationBlock but the given name we tokenized is " << tokens[1] << " and\n" <<
              "this doesn't match our current blockKey of " << currentKey << endl;
            break;
          }
          currentKey = "";
          inAnimationBlock = false;
        }
      }
      continue;
    }

    // Process the line if we are in an animation block
    if ( inVectorBlock )
    {
      vector<string> tokens = AppUtils::tokenizeString(line);
      if (tokens.size() != 2)
      {
        cout << "ERROR! Read in the line " << line << " and processing it inside a Vector Block\n" <<
          "but we did not find the 2 tokens representing floats to make up the Vector 2d" << endl;
        break;
      }

      Vector2d vecToAdd = Vector2d{ stof(tokens[0]), stof(tokens[1]) };
      vectorMap[currentKey].push_back(vecToAdd);
      continue;
    }
    else if (inAnimationBlock)
    {
      vector<string> tokens = AppUtils::tokenizeString(line);
      if (tokens.size() != 1)
      {
        cout << "ERROR! Read in the line " << line << " and processing it inside an Animation Block\n" <<
          "and we were expecting only 1 token, the name of the file in our lib to load for the animation,\n" <<
          "but we got a size of " << tokens.size() << endl;
        break;
      }
      
      animationMap[currentKey].push_back("lib\\" + tokens[0]);
      continue;
    }

    // Now actually make the objects that we need for this environment and push them 
    // into the application state

    // The format for ColoredDrawable is ColoredDrawable <vectorName> <layer> <r> <g> <b> <a> <clickable (t/f)>
    if ( line.find("ColoredDrawable") != string::npos )
    {
      if (currentKey != "" || inAnyBlock())
      {
        cout << "ERROR! Tried to declare a ColoredDrawable Object with line " << line << "while\n" <<
          "the currentKey was not reset, or we were in a block" << endl;
        break;
      }
      
      vector<string> tokens = AppUtils::tokenizeString(line);
      if (tokens.size() != 8)
      {
        cout << "ERROR! The Line " << line << " is being processed as a ColoredDrawable declaration but the\n" <<
          "tokenized string is not 8 tokens long, the declaration must follow the format\n" <<
          "ColoredDrawable <vectorName> <layer> <r> <g> <b> <a> <clickable (t/f)>" << endl;
        break;
      }
      if (vectorMap[tokens[1]].size() == 0)
      {
        cout << "ERROR! The Line " << line << " has been tokenized and we looked for the VectorPoints\n" <<
          tokens[1] << " but we could not find a previous declaration of a VectorPoints block with this name" << endl;
        break;
      }
      else if (vectorMap[tokens[1]].size() < 3)
      {
        cout << "WARNING! The Line " << line << "has been tokenized and we looked for the VectorPoints\n" <<
          tokens[1] << " but the vector that was declared has been found the contain less than 3 points\n" <<
          "This is viable behavior but makes no sense as it will only appear as a line" << endl;
        break;
      }
      //The 7th parameter for ColoredDrawable should be true for false 
      if (tokens[7] != "true" && tokens[7] != "false")
      {
        cout << "WARNING! The Line " << line << "must end with true or false\n" <<
          "indicating wheather the object can be clicked."<< endl;
      }

      DrawableObject* coloredDraw = new ColoredDrawable(vectorMap[tokens[1]], stoi(tokens[2]), stof(tokens[3]),
                                                        stof(tokens[4]), stof(tokens[5]), stof(tokens[6]) );
      coloredDraw->setClickable(tokens[7] == "true");
      ApplicationState::Instance().addDrawableObject(*coloredDraw);
    }

    // The format for TexturedDrawable is TexturedDrawable <vectorName> <layer> <string in lib> <clickable (t/f)>
    if (line.find("TexturedDrawable") != string::npos)
    {
      if (currentKey != "" || inAnyBlock())
      {
        cout << "ERROR! Tried to declare a TexturedDrawable Object with line " << line << "while\n" <<
          "the currentKey was not reset, or we were in a block" << endl;
        break;
      }

      vector<string> tokens = AppUtils::tokenizeString(line);
      if (tokens.size() != 5)
      {
        cout << "ERROR! The Line " << line << " is being processed as a TexturedDrawable declaration but the\n" <<
          "tokenized string is not 5 tokens long, the declaration must follow the format\n" <<
          "TexturedDrawable <vectorName> <layer> <string in lib> <clickable (t/f)>" << endl;
        break;
      }
      if (vectorMap[tokens[1]].size() == 0)
      {
        cout << "ERROR! The Line " << line << " has been tokenized and we looked for the VectorPoints\n" <<
          tokens[1] << " but we could not find a previous declaration of a VectorPoints block with this name" << endl;
        break;
      }
      else if (vectorMap[tokens[1]].size() < 3)
      {
        cout << "WARNING! The Line " << line << "has been tokenized and we looked for the VectorPoints\n" <<
          tokens[1] << " but the vector that was declared has been found the contain less than 3 points\n" <<
          "This is viable behavior but makes no sense as it will only appear as a line" << endl;
        break;
      }
      //The 5th parameter for TexturedDrawable should be true for false 
      if (tokens[4] != "true" && tokens[4] != "false")
      {
        cout << "WARNING! The Line " << line << "must end with true or false\n" <<
          "indicating wheather the object can be clicked." << endl;
      }

      string filePath = "lib\\" + tokens[3];
      DrawableObject* texDraw = new TexturedDrawable(vectorMap[tokens[1]], stoi(tokens[2]), filePath );
      texDraw->setClickable(tokens[4] == "true");
      ApplicationState::Instance().addDrawableObject(*texDraw);
    }

    // The format for AnimatedDrawable is AnimatedDrawable <vectorName> <layer> <animationName> <framesPerChange> <clickable (t/f)>
    if (line.find("AnimatedDrawable") != string::npos)
    {
      if (currentKey != "" || inAnyBlock())
      {
        cout << "ERROR! Tried to declare a AnimatedDrawable Object with line " << line << "while\n" <<
          "the currentKey was not reset, or we were in a block" << endl;
        break;
      }

      vector<string> tokens = AppUtils::tokenizeString(line);
      if (tokens.size() != 6)
      {
        cout << "ERROR! The Line " << line << " is being processed as an AnimatedDrawable declaration but the\n" <<
          "tokenized string is not 6 tokens long, the declaration must follow the format\n" <<
          "AnimatedDrawable <vectorName> <layer> <animationName> <framesPerChange> <clickable (t/f)>" << endl;
        break;
      }
      if (vectorMap[tokens[1]].size() == 0)
      {
        cout << "ERROR! The Line " << line << " has been tokenized and we looked for the VectorPoints\n" <<
          tokens[1] << " but we could not find a previous declaration of a VectorPoints block with this name" << endl;
        break;
      }
      else if (vectorMap[tokens[1]].size() < 3)
      {
        cout << "WARNING! The Line " << line << "has been tokenized and we looked for the VectorPoints\n" <<
          tokens[1] << " but the vector that was declared has been found the contain less than 3 points\n" <<
          "This is viable behavior but makes no sense as it will only appear as a line" << endl;
 
      }
      //The 5th parameter for TexturedDrawable should be true for false 
      if (tokens[5] != "true" && tokens[4] != "false")
      {
        cout << "WARNING! The Line " << line << "must end with true or false\n" <<
          "indicating wheather the object can be clicked." << endl;
      }
      // Make sure we have an animation with the given name
      if (animationMap[tokens[3]].size() == 0)
      {
        cout << "ERROR! The Line " << line << " has been tokenized and we looked for the Animation\n" <<
          tokens[3] << " but the animation that was declared has not been declared." << endl;
        break;
      }

      DrawableObject* animDraw = new AnimatedDrawable(vectorMap[tokens[1]], stoi(tokens[2]), animationMap[tokens[3]], 
                                                      stoi(tokens[4]) );
      animDraw->setClickable(tokens[5] == "true");
      ApplicationState::Instance().addDrawableObject(*animDraw);
    }
  }

}

void EnvLoader::saveEnvironment(string fileName, string envName)
{

}

bool EnvLoader::inAnyBlock()
{
  return (inAnimationBlock || inVectorBlock);
}