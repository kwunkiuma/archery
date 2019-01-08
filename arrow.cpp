#ifdef __APPLE__
#include <GLUT/glut.h> 
#else
#include <GL/glut.h> 
#endif

#include <iostream>

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h> // for usleep
#include <math.h>

//#include "load_and_bind_texture.h"
#include "lights_material.h"

/*
 * Todo
 * release
 */

// variables
size_t bow = 0; // variable for lists
int angle = 0;
float arrowX;
float arrowY;
float veloX = 0;
float veloY = 0;

float uX = 0;
float uY = 0;
float uZ = 0;
float vX = 0;
float vY = 0;
float vZ = 0;
size_t time = 0;

// constants
float cubeSize = 0.0005f;
float width = 0.1f / 2;
float height = 0.1f;
float arrowHead = 0.0004f;
float arrowTip = 0.00005f;
float arrowShaft = 0.005f;
float arrowRadius = 0.00005f;
int windowW = 512;
int windowH = 512;
float range = 0.3;
float fov = 40;
float fovChange = 10;
float playerHeight = 0.017;
int rotateH = 0;
int rotateV = 0;
int strength = 0;
int slices = 32;
int stacks = 32;
bool released = false;
float gravity = -0.098;
float delay = 33 * 1000; // microseconds
float speed = 0.000067 * delay /1000;

//float angle;

enum models {BOW=0, TIP=1, HEAD=2, SHAFT=3, CAP=4};

// LIGHTING
float mat_ambient[] = {0.1, 0.1, 0.1, 1.0};
float mat_diffuse[] = {0.75, 0.75, 0.75, 1.0};
float mat_specular[] = {0.75, 0.75, 0.75, 1.0};
float mat_shininess[] = {50.0};
float light_position[] = {1.0, 1.0, 0.0, 1.0};
float light_ambient[] = {0.1, 0.1, 0.1, 1.0};
float light_diffuse[] = {0.5, 0.5, 0.5, 1.0};

/*
 * 0.1 = 10m
 * 0.01 = 1m
 * 0.001 = 10cm
 */

void idle() {
	if (released) {
		time += 1;
	} else {
		 
	}
	usleep(delay);
	glutPostRedisplay();
}

void load_and_bind_textures() {
	// load all textures here
// 	floor = load_and_bind_texture("../images/grass.png");

}

void make_lists() {
	bow = glGenLists(5);
	
	GLUquadricObj* q_obj = gluNewQuadric();
	gluQuadricNormals(q_obj, GLU_SMOOTH);
	
	glNewList(bow + BOW, GL_COMPILE);
		glPushMatrix();
			glutSolidCube(cubeSize);
			glPushMatrix();
				glTranslatef(0.0f, cubeSize, 0.0f);
				glutSolidCube(cubeSize);
				glTranslatef(0.0f, cubeSize, 0.0f);
				glutSolidCube(cubeSize);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0f, -cubeSize, 0.0f);
				glutSolidCube(cubeSize);
				glTranslatef(0.0f, -cubeSize, 0.0f);
				glutSolidCube(cubeSize);
			glPopMatrix();
			
			glTranslatef(0.0f, 0.0f, cubeSize);
			
			glPushMatrix();
				glTranslatef(0.0f, cubeSize*3, 0.0f);
				glutSolidCube(cubeSize);
				glTranslatef(0.0f, cubeSize, 0.0f);
				glutSolidCube(cubeSize);
				glTranslatef(0.0f, cubeSize, 0.0f);
				glutSolidCube(cubeSize);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0f, -cubeSize*3, 0.0f);
				glutSolidCube(cubeSize);
				glTranslatef(0.0f, -cubeSize, 0.0f);
				glutSolidCube(cubeSize);
				glTranslatef(0.0f, -cubeSize, 0.0f);
				glutSolidCube(cubeSize);
			glPopMatrix();
			
			glTranslatef(0.0f, 0.0f, cubeSize);
			
			glPushMatrix();
				glTranslatef(0.0f, cubeSize*6, 0.0f);
				glutSolidCube(cubeSize);
				glTranslatef(0.0f, cubeSize, 0.0f);
				glutSolidCube(cubeSize);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0f, -cubeSize*6, 0.0f);
				glutSolidCube(cubeSize);
				glTranslatef(0.0f, -cubeSize, 0.0f);
				glutSolidCube(cubeSize);
			glPopMatrix();
		glPopMatrix();
	glEndList();
	
	glNewList(bow + TIP, GL_COMPILE);
		glPushMatrix();
			gluCylinder(q_obj, 0.0f, arrowRadius, arrowTip, slices, stacks);
		glPopMatrix();
	glEndList();
	
	glNewList(bow + HEAD, GL_COMPILE);
		glPushMatrix();
			gluCylinder(q_obj, arrowRadius, arrowRadius, arrowHead, slices, stacks);
		glPopMatrix();
	glEndList();
	
	glNewList(bow + SHAFT, GL_COMPILE);
		glPushMatrix();
			gluCylinder(q_obj, arrowRadius, arrowRadius, arrowShaft, slices, stacks);
		glPopMatrix();
	glEndList();
	
	glNewList(bow + CAP, GL_COMPILE);
		glPushMatrix();
			glRotatef(180, 0.0f, 1.0f, 0.0f);
			gluDisk(q_obj, 0.0, arrowRadius, slices, stacks);
		glPopMatrix();
	glEndList();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 

	// position and orient camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(0, playerHeight, 0, // eye position
			  0, playerHeight, -1, // reference point
			  0, 1, 0  // up vector
		);
	
	// LIGHTING
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
	
	// draw backpane
// 	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex3f(-width, 0.0f, -range);
		glVertex3f(width, 0.0f, -range);
		glVertex3f(width, height, -range);
		glVertex3f(-width, height, -range);
	glEnd();
	
	// draw ground
// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, floor);
	glColor3f(0.3f, 0.5f, 0.1f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-width, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(width, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-width, 0.0f, -1.0f);
	glEnd();
	
// 	glDisable(GL_TEXTURE_2D);
	
	// draw bow
	glEnable(GL_LIGHTING);
	glPushMatrix();
		// translate to player
		glTranslatef(0.0f, playerHeight, 0);
		glTranslatef(0.001f, 0.0f, -0.007f);
		glRotatef(-rotateH, 0.0f, 1.0f, 0.0f);
		glPushMatrix();
			glRotatef(-rotateV, 1.0f, 0.0f, 0.0f);
		
		/* Shake
		float r1 = (rand() % 4) * 0.00001;
		float r2 = (rand() % 4) * 0.00001;
		float r3 = (rand() % 4) * 0.00001;
		glTranslatef(r1*strength, r2*strength, r3*strength);*/
		
			glCallList(bow + BOW);
		if (release)
			glPopMatrix();
		
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.0035f);
			uX = speed * //todo
			for (int i = 0; i < time; i++) {
				std::cerr << i << std::endl;
				glTranslatef(0.0f, 0.0f, -strength*speed);
				glRotatef(-1, 1.0f, 0.0f, 0.0f);
			}
			glPushMatrix();
				glTranslatef(0.0f, 0.0f, 0.0005f * strength);
				glCallList(bow + TIP);
				glTranslatef(0.0f, 0.0f, arrowTip);
				glCallList(bow + HEAD);
				glTranslatef(0.0f, 0.0f, arrowHead);
				glCallList(bow + SHAFT);
				glTranslatef(0.0f, 0.0f, arrowShaft);
				glCallList(bow + CAP);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	if (!release)
		glPopMatrix();
	glDisable(GL_LIGHTING);
	/*
	glPushMatrix();
		glTranslatef(0.0f, playerHeight, 0);
		glTranslatef(0.001f, 0.0f, -0.007f);
	glPopMatrix();*/
	glutSwapBuffers(); 
}

float degToRad(int deg) {
	
	return deg * (atan(1)*4) / 180;
}

float radToDeg(float rad) {
	
	return (rad*180/(atan(1)*4));
}

void moveArrow() {
	
}

void getAngles(int x, int y) {
	float dimension = range * tan(degToRad(fov+fovChange)/2);
	std::cerr << dimension << std::endl;
	float h = x - windowW/2;
// 	std::cerr << h << std::endl;
	h = h / windowW;
// 	std::cerr << h << std::endl;
	h *= dimension;
// 	std::cerr << h << std::endl;
	h = atan(h/range);
// 	std::cerr << h << std::endl;
	rotateH = radToDeg(h) * 1;
	
	
	float v = y - windowH/2;
	v = v / windowH;
	v *=  dimension;
	v = atan(v/range);
	rotateV = radToDeg(v) * 1;
	
	h = x - windowW/2;
	h /= windowW;
	h *= fov;
	
	v = y - windowH/2;
	v /= windowH;
	v *= fov;
	rotateH = h;
	rotateV = v;
	
// 	std::cerr << rotateH << ", " << rotateV << std::endl;
}

void reshape(int w, int h) {
	windowW = w;
	windowH = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	float ratio = w/h;
	gluPerspective(fov+fovChange, ratio, 0.001f, 2.0);

	glutPostRedisplay();
}

void init() {
	load_and_bind_textures();
	// magic invocations...ohmmm
	init_lights();
	init_material();
	glClearColor(0.4f,0.8f,1.0f,1);
	
	// compile display lists for the parts
	make_lists();
	
	// LIGHTING
	// we can do this once if we don't intend to change the light properties
	// incident energy properties of a light
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	// fix the light position
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// enable lighting and turn on the light0
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// so that hidden surfaces are removed
	glEnable(GL_DEPTH_TEST);

	// mode of shading
	glShadeModel(GL_SMOOTH); // can be GL_FLAT, GL_SMOOTH
}

void keyboard(unsigned char key, int, int) {
	switch (key)
	{
		case 'q': 
			fovChange = -fovChange;
			glMatrixMode(GL_PROJECTION); 
			glLoadIdentity();
			gluPerspective(fov+fovChange, 1.0f, 0.001f, 2.0);
			break;
		case 'a':
			strength += 1;
			break;
		case 's':
			if (strength > 0)
			strength -= 1;
			break;
		case ' ':
			released = true;
			break;
// 			pos = -0.4;
// 			exit(1); break;

// 		case ' ': 
// 				g_spinning = !g_spinning;
// 				if (g_spinning)
// 					glutIdleFunc(idle);
// 				else
// 					glutIdleFunc(NULL);
// 				break;
	}
	glutPostRedisplay();
}

void mouse_motion(int x, int y) {
// 	g_cursor_x = x;
// 	g_cursor_y = y;
// 	std::cerr << "\t mouse is at (" << x << ", " << y << ")" << std::endl;
	if (!released)
		getAngles(x, y);
	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 
	glutInitWindowSize(windowW, windowH); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Arrow"); 

	glutKeyboardFunc(keyboard); 
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 
	glutPassiveMotionFunc(mouse_motion); 
					glutIdleFunc(idle);

	init(); 

	glutMainLoop(); 

	return 0; 
}
