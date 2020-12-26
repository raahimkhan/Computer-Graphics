#include <stdlib.h>
#include "GL/glut.h"

static int year = 0;
static int year_jupiter = 0;
static int year_mars = 0;
static int year_neptune = 0;
static int year_venus = 0;
static int year_saturn = 0;
static int year_uranus = 0;
static int year_mercury = 0;
static int year_moon = 0;
static int day = 0;

void init(void)
{
	GLfloat light_position1[] = {50, 50, 0, 1};
    GLfloat light1[] = {0.5, 0.5, 0.5, 1};
    GLfloat light2[] = {0.5, 0.5, .5, 1.0};

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light2);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light1);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light2);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
        glColor3f(1.0, 1.0, 0.0);
        glutSolidSphere(0.8, 20, 12);	// sun
	glPopMatrix();

	glPushMatrix(); // mercury
        glRotatef((GLfloat) year_mercury, 0.0, 1.0, 0.0);
        glTranslatef(0.2, 0.0, 0.0);
        glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
        glColor3f(0.0, 1.0, 0.0);
		glutSolidSphere(0.1, 10, 4);
	glPopMatrix();

	glPushMatrix(); // venus
        glRotatef((GLfloat) year_venus, 0.0, 1.0, 0.0);
        glTranslatef(0.5, 0.0, 0.0);
        glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
        glColor3f(0.0, 0.0, 1.0);
		glutSolidSphere(0.1, 10, 4);
	glPopMatrix();

	glPushMatrix(); // earth around sun  RED ONE IS EARTH
	glRotatef((GLfloat) year, 0.0, 1.0, 0.0);
	glTranslatef(0.8, 0.0, 0.0);
	glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(0.08, 10, 4);

	glPushMatrix(); // moon around earth
	glRotatef((GLfloat) year_moon, 0.0, 1.0, 0.0);
	glTranslatef(0.3, 0.0, 0.0);
	glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
	glColor3f(0.0f, 0.5f, 0.5f);
	glutSolidSphere(0.08, 10, 4);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix(); // mars
        glRotatef((GLfloat) year_mars, 0.0, 1.0, 0.0);
        glTranslatef(1.1, 0.0, 0.0);
        glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
        glColor3f(1.0, 1.0, 1.0);
		glutSolidSphere(0.09, 10, 4);
	glPopMatrix();

	glPushMatrix(); // jupiter
        glRotatef((GLfloat) year_jupiter, 0.0, 1.0, 0.0);
        glTranslatef(1.4, 0.0, 0.0);
        glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
        glColor3f(1.0, 0.0, 1.0);
		glutSolidSphere(0.07, 10, 4);
	glPopMatrix();

	glPushMatrix(); // saturn
        glRotatef((GLfloat) year_saturn, 0.0, 1.0, 0.0);
        glTranslatef(1.7, 0.0, 0.0);
        glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
        glColor3f(0.0, 1.0, 1.0);
		glutSolidSphere(0.05, 10, 4);
	glPopMatrix();

	glPushMatrix(); // uranus
        glRotatef((GLfloat) year_uranus, 0.0, 1.0, 0.0);
        glTranslatef(2.0, 0.0, 0.0);
        glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
        glColor3f(1.0, 0.5, 0.0);
		glutSolidSphere(0.03, 10, 4);
	glPopMatrix();

	glPushMatrix(); // neptune
        glRotatef((GLfloat) year_neptune, 0.0, 1.0, 0.0);
        glTranslatef(2.3, 0.0, 0.0);
        glRotatef((GLfloat) day, 0.0, 1.0, 0.0);
        glColor3f(0.0, 0.5, 0.0);
		glutSolidSphere(0.01, 10, 4);
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
	year_moon = (year_moon + 5) % 360;
    year_mars = (year_mars + 5) % 360; // 4
	year_jupiter = (year_jupiter + 4) % 360; // 5
	year_neptune = (year_neptune + 1) % 360; // 8
	year_venus = (year_venus + 7) % 360; // 2
	year_saturn = (year_saturn + 3) % 360; // 6
	year_uranus = (year_uranus + 2) % 360; // 7
	year_mercury = (year_mercury + 8) % 360; // 1
	year = (year + 6) % 360; // earth (3)
	day = (day + 10) % 360;
	glutPostRedisplay();
	glutTimerFunc(50, timer, 1);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1950, 600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Solar System");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(50, timer, year);
	glutMainLoop();
	return 0;
}