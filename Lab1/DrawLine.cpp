#include<GL\glut.h>
#include<iostream>
#include<cmath>

using namespace std;

void swapValue(int *a, int *b);
void lineOfDDA(int x1, int y1, int x2, int y2);
void lineOfBresenham(int x1, int y1, int x2, int y2);
void changeWindowSize(GLsizei w, GLsizei h);
void displayLine(void);

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(200, 200);               // Locate the position of window
	glutInitWindowSize(400, 400);                   // Set the size of window
	glutCreateWindow("TJU_CG_DDA&Bresenham");
	glutDisplayFunc(displayLine);                   // Call the function of drawing
	glutReshapeFunc(changeWindowSize);              // Reshape the window
	glClearColor(1.0, 1.0, 1.0, 0.0);		        // Set window background to WHITE
	glMatrixMode(GL_PROJECTION);			        // Set projection parameters
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);             // Set the size of stage
	glutMainLoop();

	return 0;

}

void swapValue(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

// Draw a line by using DDA algorithm
void lineOfDDA(int x1, int y1, int x2, int y2) {

	glColor3f(1.0, 0.0, 0.0);                       // Set the line of DDA to RED
	glPointSize(2.0f);                              // Set the size of point in the line

	// We should guarantee that the slope of this line to be less than 1
	int dm = 0;
	if (abs(x2 - x1) >= abs(y2 - y1)) dm = abs(x2 - x1);
	else dm = abs(y2 - y1);
	float x = x1;
	float y = y1;
	float dx = (float)(x2 - x1) / dm;
	float dy = (float)(y2 - y1) / dm;

	for (int i = 0; i < dm; i++) {
		glBegin(GL_POINTS);
		glVertex2f((int)x, (int)y);
		glEnd();
		glFlush();
		x += dx;
		y += dy;
	}
}

// Draw a line by using Bresenham algorithm
void lineOfBresenham(int x1, int y1, int x2, int y2) {

	glColor3f(0.0, 0.0, 1.0);                       // Set the line of Bresenham to BLUE
	glPointSize(2.0f);                              // Set the size of point in the line

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);

	// If the two point is same...
	if (dx == 0 && dy == 0) {
		glBegin(GL_POINTS);
		glVertex2f(x1, y1);
		glEnd();
		glFlush();
		return;
	}

	// We should guarantee that the slope of this line to be less than 1
	int flag = 0;
	if (dx < dy) {
		flag = 1;
		swapValue(&x1, &y1);
		swapValue(&x2, &y2);
		swapValue(&dx, &dy);
	}

	int tx = (x2 - x1) > 0 ? 1 : -1;
	int ty = (y2 - y1) > 0 ? 1 : -1;
	int x = x1;
	int y = y1;
	int ds = dy * 2;
	int dt = (dy - dx) * 2;
	int d = ds - dx;

	while (x != x2) {

		if (d < 0) d += ds;
		else {
			y += ty;
			d += dt;
		}

		if (flag) {
			glBegin(GL_POINTS);
			glVertex2f(y, x);
			glEnd();
			glFlush();
		}
		else {
			glBegin(GL_POINTS);
			glVertex2f(x, y);
			glEnd();
			glFlush();
		}

		x += tx;

	}

}

// If we change the window size...
void changeWindowSize(GLsizei w, GLsizei h) {

	if (h == 0) h = 1;

	glViewport(0, 0, w, h);                         // Set the window size
	glMatrixMode(GL_PROJECTION);                    // Reset coordinate system
	glLoadIdentity();

	// Set the range of part space
	if (w <= h) glOrtho(0.0f, 250.0f, 0.0f, 250.0f*h / w, 1.0, -1.0);
	else glOrtho(0.0f, 250.0f*w / h, 0.0f, 250.0f, 1.0, -1.0);

}

// Display the line
void displayLine(void) {

	// Set the color of background
	glClear(GL_COLOR_BUFFER_BIT);

	int DDA_x1 = 20;
	int DDA_y1 = 20;
	int DDA_x2 = 160;
	int DDA_y2 = 80;
	lineOfDDA(DDA_x1, DDA_y1, DDA_x2, DDA_y2);

	int Bre_x1 = 20;
	int Bre_y1 = 40;
	int Bre_x2 = 160;
	int Bre_y2 = 100;
	lineOfBresenham(Bre_x1, Bre_y1, Bre_x2, Bre_y2);

}
