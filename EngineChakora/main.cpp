/*	OpenGL - przyk³adowy program korzystaj¹cy z biblioteki GLUT
*	(C) 2009 Grzegorz £ukawski, Politechnika Œwiêtokrzyska w Kielcach
*/

// Potrzebne w przypadku CodeBlocks 8.02:
#define GLUT_DISABLE_ATEXIT_HACK

#include <stdio.h>
#include <stdlib.h>
#include <GL\freeglut.h>
#include <GL\glut.h>

#include <windows.h>
// K¹t obserwacji i animacji:
GLfloat lookA = 30;
GLfloat angle;
// Parametry oœwietlenia:
GLfloat lightAmb[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat lightDif[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat lightPos[] = { 0, 0, 800, 1.0 };
GLfloat lightSpec[] = { 1, 1, 1, 1 };
// Listy wyœwietlania:
GLuint obiekt1;

void inicjalizacja() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
	glEnable(GL_LIGHT0);

	glMaterialfv(GL_FRONT, GL_SPECULAR, lightSpec);
	glMateriali(GL_FRONT, GL_SHININESS, 64);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Przygotowanie listy wyœwietlania z szeœcianami:
	obiekt1 = glGenLists(1);
	glNewList(obiekt1, GL_COMPILE);
	for (int i = 0; i < 10; i++) {
		glRotatef(36, 0, 1, 0);
		glPushMatrix();
		glTranslatef(120, 0, 0);
		glColor4f(1, (float)i / 10, 0, 1);
		glutSolidCube(50);
		glPopMatrix();
	}
	glEndList();
}

void rozmiar(int w, int h) {
	if (h == 0)	h = 1;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, (float)w / h, 130, 470);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void rysowanie() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0, 0, -300);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glRotatef(lookA, 1, 0, 0);
	glRotatef(angle, 0, 1, 0);

	// Narysowanie 10 szeœcianów:
	glCallList(obiekt1);

	glutSwapBuffers();
}

// Czas przy poprzedniej ramce:
DWORD _ptt;

void IdleFun() {
	// Czas w ms od uruchomienia systemu:
	DWORD tt = GetTickCount();

	if (_ptt != 0) {
		// Wyznaczenie czasu w sekundach od ostatniej ramki:
		float czass = (tt - _ptt) / 1000.0f;
		// Prêdkoœæ animacji 25 stopni na sekundê:
		angle += 25 * czass;
		if (angle >= 360)	angle -= 360;
	}

	// Bie¿¹cy czas do kolejnego obliczenia:
	_ptt = tt;

	glutPostRedisplay();
}

void KeyFun(unsigned char key, int x, int y) {
	if (key == 0x1B)		exit(0);
}
void SpecialFun(int key, int x, int y) {
	if (key == GLUT_KEY_DOWN)        lookA += 1;
	if (key == GLUT_KEY_UP)          lookA -= 1;
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("gl_timer2");

	glutDisplayFunc(rysowanie);
	glutReshapeFunc(rozmiar);
	glutKeyboardFunc(KeyFun);
	glutSpecialFunc(SpecialFun);

	// Funkcja bezczynnoœci - wywo³ywania ok. 60 razy na sekundê
	// przy w³¹czonej synchronizacji pionowej:
	glutIdleFunc(IdleFun);

	inicjalizacja();

	glutMainLoop();
	return(0);
}