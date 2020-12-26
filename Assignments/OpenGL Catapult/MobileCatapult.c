#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include "MobileCatapult.h"

#define SW 1024
#define SH 720
#define NUMPTS 18097
#define MESHDIM 150
#define TERR_D MESHDIM*2.5
#define TERR_W MESHDIM*2.5

//////////////////////////////////////////////////////

//Rotation of the bucket with respect to the arm
int rotBuck = 0;

//Rotation of the arm with respect to the attachment
int rotarm = 0;

//Rotation of the attachment with respect to the base
int rotAttach = 0;

/////////////////////////////////////////////////////

float cam_xrot=-190, cam_yrot=0, cam_zrot=0;
float tyre_rot = 1;
float car_move = 1;
float plat_rot = 1;
float arm1_rot = 0;
float arm2_rot = 0;
float drill_rot = 1;
float terrainHeight[MESHDIM][MESHDIM] = {0,0};
int terrainH , initH;
int xNew, zNew = 0;
float angleY=0 ,  angleZ;
GLdouble pos[3] = {75 , 3 , 75};
GLdouble vel[3] = {0 , 0 , 0};
static GLuint texName[6];
static GLuint texWood, texWall;
void gl_draw();
void gl_init(int w, int h);
void draw_Terrain();
void Normalize3(GLdouble *v);
void resizePointData();
void drawAxes();
void drawCatapult();
void drawBase(double l, double w, double h);
void drawWheel(double r, double h);
void drawBucket(double, double);
GLuint loadTexture(const char* path);
void findNormal(int x , int z , int id, GLdouble *res);

void keyboard (unsigned char key, int x, int y);
void camera();
void reshape (int w, int h);
void cleanup(){}

void bucketSystem();
void wheelSystem(int platL, int platW, int wheelR, int wheelW);


///////////////////////////////////////////////////////////
void resizePointData(){
	float minx=9999,miny=9999,minz=9999,maxx=-9999,maxy=-9999,maxz=-9999;

	for(int t=0; t<NUMPTS; t++){
		minx = minx > dat[t][0] ? dat[t][0] : minx;
		miny = miny > dat[t][1] ? dat[t][1] : miny;
		minz = minz > dat[t][2] ? dat[t][2] : minz;

		maxx = maxx <= dat[t][0] ? dat[t][0] : maxx;
		maxy = maxy <= dat[t][1] ? dat[t][1] : maxy;
		maxz = maxz <= dat[t][2] ? dat[t][2] : maxz;		
	}
	printf("min:(%f,%f,%f)\n max:(%f,%f,%f)\n",minx,miny,minz,maxx,maxy,maxz);

	for(int t=0; t<NUMPTS; t++){
		dat[t][0] = MESHDIM*(dat[t][0]-minx)/(maxx-minx);
		dat[t][1] = MESHDIM*(dat[t][1]-miny)/(maxy-miny);
		dat[t][2] = MESHDIM*(dat[t][2]-minz)/(maxz-minz);
	}	
}

///////////////////////////////////////////////////////////
void findNormal(int x , int z , int id, GLdouble *res){
	GLdouble line1[3] ;
	GLdouble line2[3] ;
	if (id == 1){
		line1[0] = 1; line1[1] = (GLdouble)terrainHeight[x+1][z] - (GLdouble)terrainHeight[x][z+1]; line1[2] = -1;
		line2 [0] = -1; line2[1] = (GLdouble)terrainHeight[x][z] - (GLdouble)terrainHeight[x+1][z]; line2[2] = 0 ;
	}else{
		line1[0] = 0; line1[1] = (GLdouble)terrainHeight[x+1][z] - (GLdouble)terrainHeight[x+1][z+1]; line1[2] = -1;
		line2 [0] = -1; line2[1] = (GLdouble)terrainHeight[x][z+1] - (GLdouble)terrainHeight[x+1][z]; line2[2] = 1 ;		
	}
	res[0] = line1[1]*line2[2] - line1[2]*line2[1];
	res[1] = line1[2]*line2[0] - line1[0]*line2[2];
	res[2] = line1[0]*line2[1] - line1[1]*line2[0];

	Normalize3(res);
}

///////////////////////////////////////////////////////////
GLdouble dotProd(GLdouble *a , GLdouble *b){
	return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

///////////////////////////////////////////////////////////
void Normalize3(GLdouble *v){
   GLdouble len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] /= len;
   v[1] /= len;
   v[2] /= len;
}


//////////////////////////////////////////////////////////
void drawAxes(){
   glColor3d(1,0,0);
   glBegin(GL_LINES);
	   glVertex3f(0,0,0);
	   glVertex3f(3,0,0);
   glEnd();

   glColor3d(0,1,0);
   glBegin(GL_LINES);
	   glVertex3f(0,0,0);
	   glVertex3f(0,3,0);
   glEnd();

   glColor3d(0,0,1);
   glBegin(GL_LINES);
	   glVertex3f(0,0,0);
	   glVertex3f(0,0,3);
   glEnd();
}

///////////////////////////////////////////////////////////
void gl_draw(){

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glColor3f(0.0, 1.0, 0.0);
  camera();
  drawAxes();
  glScalef(3,3,3);
  drawCatapult();
  glutSwapBuffers();
}

///////////////////////////////////////////////////////////
void gl_init(int w, int h){
	printf("In gl_init \n");

	// Setup Camera
	glClearColor(0.0, 0.0, 0.0, 0.0);

 	glViewport(0, 0, w, h);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
		
	resizePointData();
}

///////////////////////////////////////////////////////////
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glEnable(GL_DEPTH_TEST);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(120.0, 1.0, 0.0001, MESHDIM*8);
   glMatrixMode(GL_MODELVIEW);
   camera();
}

///////////////////////////////////////////////////////////
void camera(){
   glLoadIdentity();
   glTranslatef (0, 0.0,-MESHDIM);
   glRotatef(cam_xrot,1,0,0);
   glRotatef(cam_yrot,0,1,0);
   glRotatef(cam_zrot,0,0,1);
}

///////////////////////////////////////////////////////////////////////////////
void drawCatapult()
{

int platL = 40, platW = 20, platH = 8, wheelR = 8, wheelW = 3.5;

glTranslatef(0, 0, zNew); //Forward Backward Movement

glPushMatrix(); // Begin Entire Catapult  

  glPushMatrix(); //Begin Wheel System
    wheelSystem(platL,platW,wheelR,wheelW); 
  glPopMatrix(); // End Wheel System
  
  glPushMatrix(); //Begin Catapult Base
    glTranslatef(0, 5,0);
    drawBase(platL, platW, platH);      
  glPopMatrix();  //End Catapult Base

  glPushMatrix(); //Begin Launch System

    glTranslatef(0, 9, 0);    
    glRotatef(rotAttach, 0, 1, 0);
    glTranslatef(0, -9, -15);

    glPushMatrix(); //Begin Arm System

      glTranslatef(0, +16.5 - 7.5 , +13);   
      glRotatef(rotarm, 1, 0, 0);
      glTranslatef(0, -16.5 + 7.5 , -13);

      glPushMatrix(); //Begin Bucket System
        bucketSystem(); 
      glPopMatrix(); //End Bucket System

      glTranslatef(0, 7.5 + 7.5 + 1.5, 0);  
      glRotatef(-60, 1, 0, 0);

      glColor3f(1.0, 0.0, 0.0); // Color Arm
      drawWheel(1, 30); // Draw Arm

    glPopMatrix(); // End Arm System

    glTranslatef(0, 9, 15); //Position arm attachment point
    glColor3f(0.0, 0.0, 1.0); //Color arm attachment point
    drawWheel(3, 3); //Draw arm attachment point
  
  glPopMatrix(); //End Launch System


glPopMatrix(); // End Entire Catapult

}

/////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void helperPolygon(double v11, double v12, double v13, double v21, double v22, double v23, 
  double v31, double v32, double v33, double v41, double v42, double v43, double signal) {

  if (signal == 1) {
    glColor3f(0.545, 0.271, 0.075) ;
    glVertex3f(v11, v12, v13) ;
    glVertex3f(v21, v22, v23) ;
    glVertex3f(v31, v32, v33) ;
    glVertex3f(v41, v42, v43) ;
  }

  else {
    glVertex3f(v11, v12, v13) ;
    glVertex3f(v21, v22, v23) ;
    glVertex3f(v31, v32, v33) ;
    glVertex3f(v41, v42, v43) ;
  }
}

void drawBase(double l1, double w1, double h1)
{
  glBegin(GL_QUADS) ;
    helperPolygon(-l1, -w1, h1, l1, -w1, h1, l1, w1, h1, -l1, w1, h1, 1) ;
    helperPolygon(-l1, -w1, -0.5*h1, -l1, -w1, h1, -l1, w1, h1, -l1, w1, -0.5*h1, 1) ;
    helperPolygon(l1, -w1, -0.5*h1, l1, w1, -0.5*h1, l1, w1, h1, l1, -w1, h1, 1) ;
    helperPolygon(-l1, -w1, -0.5*h1, -l1, w1, -0.5*h1, l1, w1, -0.5*h1, l1, -w1, -0.5*h1, 1) ;
    helperPolygon(l1, -w1, h1, -l1, -w1, h1, -l1, -w1, -0.5*h1, l1, -w1, -0.5*h1, 1) ;
    helperPolygon(l1, w1, -0.5*h1, -l1, w1, -0.5*h1, -l1, w1, h1, l1, w1, h1, 0) ;
  glEnd() ;

  glFlush() ;

}

////////////////////////////////////////////////////////////////////////
void bucketSystem(){

  glPushMatrix() ;
    glTranslatef(0, 23, -13) ;
    glRotatef(90, 1, 0, 0) ;
    drawBucket(4.5, 2.0) ;
  glPopMatrix() ;

}

void bucketHelper(int v11, double v12, double v13, int v21, double v22, double v23, int v31, double v32, double v33, 
int v41, double v42, double v43) {

    glVertex3f(v11, v12, v13) ;
    glVertex3f(v21, v22, v23) ;
    glVertex3f(v31, v32, v33) ;
    glVertex3f(v41, v42, v43) ;
}

void drawBucket(double height1, double radius1){

  glColor3f(0.0, 0.0, 1.0) ;

  glBegin(GL_QUADS) ;
    bucketHelper(-4, -radius1, height1, 1, -radius1, height1, 2, radius1, height1, -4, radius1, height1) ;
    bucketHelper(-4, -radius1, -height1, -4, -radius1, height1, -4, radius1, height1, -4, radius1, -height1) ;
    bucketHelper(4, -radius1, -height1, 4, radius1, -height1, 4, radius1, height1, -4, -radius1, height1) ;
    bucketHelper(-4, -radius1, -height1, -4, radius1, -0.8*height1, 4, radius1, -height1, 4, -radius1, -height1) ;
    bucketHelper(4, -radius1, height1, -4, -radius1, height1, -4, -radius1, -height1, 4, -radius1, -height1) ;
  glEnd() ;

  glFlush() ;

}

////////////////////////////////////////////////////////////////////////////////////////////


void helperWheel(int t1, int t2, int t3, int r1, int r2, int r3, int r4, int d1, int d2) {

  glPushMatrix() ; // wheel
		glTranslatef(t1, t2, t3) ;  // positioning
		glRotatef(r1, r2, r3, r4) ;     // alignment
		drawWheel(d1, d2) ;     // drawing
	glPopMatrix() ;

}

void wheelSystem(int platL, int platW, int wheelR, int wheelW){

  helperWheel(platL-2, -platW+4, 0, wheelR, 0, 1, 0, wheelR, wheelW) ;

  helperWheel(-platL-2, -platW+4, 0, wheelR, 1, 0, 0, wheelR, wheelW) ;

  helperWheel(-platL-2, platW+4, 0, wheelR, 1, 0, 0, wheelR, wheelW) ;

  helperWheel(platL-2, platW+4, 0, wheelR, 0, 1, 0, wheelR, wheelW) ;
}


void drawWheel(double r, double h)
{
  

  glBegin(GL_QUAD_STRIP);
  for(int i = 0; i < 360; i+=2){
    glVertex3f(r*cos(i), h/2, r*sin(i));
    glVertex3f(r*cos(i), -h/2, r*sin(i));
    glVertex3f(r*cos(i+1), -h/2, r*sin(i+1));
    glVertex3f(r*cos(i+1), h/2, r*sin(i+1));
  }
  glEnd();
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0, h/2, 0);  
  for(int i = 0; i < 360; i++){
    glVertex3f(r*cos(i), h/2, r*sin(i));    
  }
  glEnd();


  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0, -h/2, 0); 
  for(int i = 0; i < 360; i++){
    glVertex3f(r*cos(i), -h/2, r*sin(i)); 
  }
  glEnd();

glFlush();


}

///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void specialkeys(int key, int x, int y)
{
 switch (key) {
  case GLUT_KEY_UP:
      zNew++; 
    break;

  case GLUT_KEY_DOWN:
    //TYPE YOUR CODE HERE
  zNew--;
    break;
  case GLUT_KEY_RIGHT:      ///Move cart
    rotAttach += 1;
    if(rotAttach > 25){ rotAttach = 25; }
    break;
  case GLUT_KEY_LEFT:     // Move cart
    //TYPE YOUR CODE HERE
  rotAttach -= 1;
    if(rotAttach < -25){ rotAttach = -25; }
    break;
  case GLUT_KEY_F1:     //Rotate arm
  rotarm += 3;
    if(rotarm > 32) { rotarm = 32; }
  break;
  case GLUT_KEY_F2:   //Rotate arm
  rotarm -= 1;
    if(rotarm < -19) { rotarm = -19; }
  break;
  case GLUT_KEY_F3:   //Rotate Bucket
  rotBuck += 1;
    if(rotBuck > 35) { rotBuck = 35; }
  break;
  case GLUT_KEY_F4:     //Rotate Bucket
  rotBuck -= 1;
    if(rotBuck < -35) { rotBuck = -35; }
  break;
  case GLUT_KEY_F5:     //FIRE 

  rotarm = 32;  
  break;

  default:
    break;
  }
  glutPostRedisplay();
}

///////////////////////////////////////////////////////////
void keyboard (unsigned char key, int x, int y)
{   
   // Camera controls - Rotation along principle axis
   switch (key) {
      case 'q':
    	 cam_xrot += 1;
    	 if (cam_xrot >360) cam_xrot -= 360;
         break;
      case 'z':
       	 cam_xrot -= 1;
    	 if (cam_xrot < -360) cam_xrot += 360;
         break;
      case 'a':
    	 cam_yrot += 1;
    	 if (cam_yrot >360) cam_yrot -= 360;
         break;
      case 'd':
       	 cam_yrot -= 1;
    	 if (cam_yrot < -360) cam_yrot += 360;
         break;
      case 'w':
    	 cam_zrot += 1;
    	 if (cam_zrot >360) cam_zrot -= 360;
         break;
      case 'x':
       cam_zrot -= 1;
    	 if (cam_zrot < -360) cam_zrot += 360;
         break;
	      //TYPE YOUR CODE HERE
    default:
         break;
	 }
	 glutPostRedisplay();
}

///////////////////////////////////////////////////////////
int main(int argc, char **argv){
	printf("In main \n");

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize(SW, SH);
   glutInitWindowPosition(100, 50);
   glutCreateWindow(argv[0]);

   glutDisplayFunc(gl_draw);
   glutReshapeFunc(reshape);
   glutSpecialFunc(specialkeys);
   glutKeyboardFunc(keyboard);   

   gl_init(SW, SH);

   glutMainLoop();

   return EXIT_SUCCESS;
}
