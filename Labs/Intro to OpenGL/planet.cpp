#include <stdlib.h>
#include "GL/glut.h"

static int year = 0;
static int day = 0;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
		glutWireSphere(0.8, 20, 12);	// create sun
		glRotatef((GLfloat) year, 0.0, 1.0, 0.0);
		glTranslatef(2.0, 0.0, 0.0);
		glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
		glutWireSphere(0.2, 10, 4);		// create planet
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0,0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
	gluLookAt(0.0, 0.0, 3.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void timer(int i)
{
	year = (year + 2) % 360;
	day = (day + 10) % 360;
	glutPostRedisplay();
	glutTimerFunc(50, timer, 1);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Solar System");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(50, timer, year);
	glutMainLoop();
	return 0;
}