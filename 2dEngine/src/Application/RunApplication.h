#ifndef RUN_APPLICATION_MAIN
#define RUN_APPLICATION_MAIN

/*
The class that starts off and runs the application
*/
class RunApplication{
public:
  
  static void display(void);

  static void reshape(int w, int h);

  static void timer(int v);

  static void keyup(unsigned char key, int x, int y);

  static void keydown(unsigned char key, int x, int y);

  static void mouseUp(int x, int y);

  static void mouseMotion(int x, int y);

  static void mouse(int button, int state, int x, int y);

  static void callKeyEvents();

  static void startStopEditor(); 

};

#endif