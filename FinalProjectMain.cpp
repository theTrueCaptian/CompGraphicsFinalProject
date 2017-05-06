/*
	Maeda Hanafi
	CSC431 Final Project
	Pacman eating animation
*/
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static float score = 0.0; // Score
static float scale = 0.5; //Pacman's size
static int currI=0, currJ=0; //I and J of the current coin in animation
static float pacX = 0.0, pacY = 4.0, pacZ = -3.0;//Pac man coordinates
static float currX = 0.0, currY = 0.0, currZ = 0.0; //Coordinates of the currently moving coin
static float t=0.0;

static float spotAngle = 90.0; // Spotlight cone half-angle.
static int isAnimate = 0; // Animated?
static float spotExponent = 0.1; // Spotlight exponent = attenuation.
static int spin = 0;

static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
static const int WIDTH = 9, HEIGHT = 10;
static int map[WIDTH][HEIGHT] = {
	{0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,1,1,1,1,1,0},
	{0,1,0,0,1,1,0,0,1,0},
	{0,1,1,1,1,1,1,1,1,0},
	{0,1,0,0,1,1,0,0,1,0},
	{0,1,1,1,1,1,1,1,1,0},
	{0,1,0,0,1,1,0,0,1,0},
	{0,1,1,1,1,1,1,1,1,0},
	{0,0,0,0,0,0,0,0,0,0}
};

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to convert floating point to char string.
void floatToString(char * destStr, int precision, float val) 
{
   sprintf(destStr,"%f",val);
   destStr[precision] = '\0';
}

// Initialization routine.
void setup(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING);

   // Light property vectors.
   float lightAmb[] = { 0.25, 0.25, 0.25, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float globAmb[] = { 0.25, 0.25, 0.25, 1.0 };

   
   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);

   glEnable(GL_LIGHT0); // Enable particular light source.
   
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

   // Material property vectors.
   float matSpec[] = { 0.5, 0.5, 0.5, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties shared by all the spheres.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);

   // Enable color material mode:
   // The ambient and diffuse color of the front faces will track the color set by glColor().
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

   glEnable(GL_BLEND); // Enable blending.
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters. 
  
   
}
//draw a plane
void drawPlane(float xt, float yt, float zt){
	//Draw the plane
	glPushMatrix();
	glTranslatef(xt-0.5, yt, zt-0.5);
	glColor4f(0,0,0,0.5);
	glBegin(GL_QUADS);
		glVertex3f( 0,0, 0);
		glVertex3f( 0,0,1);
		glVertex3f(1,0,1);
		glVertex3f(1,0, 0);
	glEnd();
	glPopMatrix(); 
	//Draw lines
	glPushMatrix();
	glTranslatef(xt-0.5, yt+0.01, zt-0.5);
	glColor4f(0,0,1.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(1,0,0);
	glEnd();
	glPopMatrix(); 
}
//Draws a cube given the translation variables
void drawCube(float xt, float yt, float zt, bool floor){
	
	if(floor){ //draw plane
		glPushMatrix();
	    drawPlane( xt,  yt,  zt);
	    glPopMatrix(); 
	}else{

		glTranslatef(xt, yt, zt);
		glColor4f(0.0, 0.0, 0.0, 1);
		glutSolidCube(0.99);
		glColor4f(0.0, 0.0, 1.0, 1);
		glutWireCube(1.0);
		glPopMatrix(); 
	}
}
//Draws a coin given translation variables
void drawACoin(float xt, float yt, float zt){
	glColor4f(0.75, 0.75, 0.0, 1.0);
	glTranslatef(xt, yt, zt); //
	glutSolidSphere (0.25, 16.0, 16.0);	
}
//draws Pacman given translation vairables
void drawPacman(float xt, float yt, float zt){
	glColor4f(0.75, 0.75, 0.0, 1.0);
	glTranslatef(xt, yt, zt); //
	glutSolidSphere (scale, 16.0, 16.0);	
}

void drawMap(){
	// Draw squares for pacman's map
   int i,j;
   for (i = 0; i < HEIGHT; i++){
      for (j = 0; j < WIDTH; j++){
	     if(map[j][i]==0){ //draw the cubes for the map
			 glPushMatrix();
			 drawCube(-((int)(HEIGHT/2))+i, 0.0, -((int)(WIDTH/2))+j, false);
			 glPopMatrix(); 
		 }else if(map[j][i]==1){ //draw a yellow coin
			 glPushMatrix();
			 if(currI==i && currJ==j){ //if this is the current animated coin, move it
				
				 drawACoin(currX, currY, currZ);				
			 }else
				 drawACoin(-((int)(HEIGHT/2))+i, 0.25, -((int)(WIDTH/2))+j);		
			 glPopMatrix(); 
		 }
		 //draw the shiny "floor"
		 glPushMatrix();
		 drawCube(-((int)(HEIGHT/2))+i, -0.5, -((int)(WIDTH/2))+j, true);
		 glPopMatrix(); 
		 
	  }
   }
}

// Drawing routine.
void drawScene()
{
   float lightPos[] = { 0.0, 5.0, 0.0, 1.0 }; // Spotlight position.
   float spotDirection[] = {0.0, -1.0, 0.0}; // Spotlight direction.   
   GLfloat position[] = { 0.0, 0.0, 1.5, 1.0 };

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   // Write message.
   glDisable(GL_LIGHTING);
   glColor3f(1.0, 1.0, 1.0);
   floatToString(theStringBuffer, 4, score);
   glRasterPos3f(-1.0, 1.0, -2.0);
   writeBitmapString((void*)font, "Pac-Man Cheats! ");  
   writeBitmapString((void*)font, "Score: "); 
   writeBitmapString((void*)font, theStringBuffer); 
   glEnable(GL_LIGHTING);

   gluLookAt (0.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
 
   glPushMatrix();
   
   glPopMatrix();
   
   // Spotlight properties including position.
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);  
   glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
   glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);    
   glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);

   
   glPushMatrix ();
   glPopMatrix();
   

    //draw reflections
   glPushMatrix();
   
   glScalef(1.0, -1.0, 1.0);
   glFrontFace(GL_CW); // Because of reflection front-faces are drawn clockwise. 
   drawPacman(pacX, pacY, pacZ);
   glFrontFace(GL_CCW);  
   glPopMatrix();

   //draw the map
   drawMap();
   
   //draw pacman!
   glPushMatrix();
   drawPacman(pacX, pacY, pacZ);
   glPopMatrix();

   
   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize (int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (float)w/(float)h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
	  case ' ':
		  if (isAnimate) isAnimate = 0;
		  else isAnimate = 1;
          glutPostRedisplay();
		  break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   
   glutPostRedisplay();
}

void resetTranslation(){
	currX = -((int)(HEIGHT/2))+currI;
	currY = 0.25 ;
	currZ = -((int)(WIDTH/2))+currJ;
	t=0;
}


// Timer function.
void animate(int value)
{
   if (isAnimate) 
   {
	   //rotate light1
	   spin = (spin + 30) % 360;
	   if(map[currJ][currI]==1){ //Animate coins only
		  
		   float a = pacX-currX, b=pacY-currY, c=pacZ-currZ;
		   currX = -((int)(HEIGHT/2))+currI +a*t;
		   currY =  0.25 + currY+b*t;
		   currZ =-((int)(WIDTH/2))+currJ + c*t;
		  
		   t=t+0.5;
		   
			//if the coin hits Pacman
			if((currY) >= pacY){
				//set coin on map to 0
				map[currJ][currI] = 2;
				//move to next coin
				currJ++;
				//reset
				resetTranslation();
				//inc score
				score = score +1000;
				//make pacman bigger
				scale=scale+0.05;
			}
		}else{ //if it is not a coin, then move to the next one
			currJ++;
	    }
		//decide the next coin if currJ exceeds map's bounds
		if(currJ>=WIDTH-1){
			currI++;
			//reset
			resetTranslation();
			currJ = 0;
		}
		if(currI>=HEIGHT-1){ 
			currI = 0;
			currJ = 0;
			//reset
			resetTranslation();
			isAnimate = 0;
		}	   
   }
   glutTimerFunc(50, animate, 1);
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to start!" << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Pacman!");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutTimerFunc(5, animate, 1);
   glutSpecialFunc(specialKeyInput);
    
   glutMainLoop();
   
   return 0;
}
