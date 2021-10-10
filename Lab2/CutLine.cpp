#include<GL/glut.h>
#include<iostream>

#define LEFT    1       // 0001: left boundary
#define RIGHT   2       // 0010: right boundary
#define BOTTOM  4       // 0100: bottom boundary
#define TOP     8       // 1000: top boundary

using namespace std;

// init boundary: (100,100)->(300,200)
int boundary_left = 100;
int boundary_right = 300;
int boundary_bottom = 100;
int boundary_top = 200;

// init line which has NOT been cut: (150,50)->(50,250)
int raw_X1 = 150;
int raw_Y1 = 50;
int raw_X2 = 50;
int raw_Y2 = 250;

// Set the line which will be cut
int X1 = 150;
int Y1 = 50;
int X2 = 50;
int Y2 = 250;

void endpoint_encode(int x, int y, int &c);
void changeWindowSize(GLsizei w, GLsizei h);
void displayCuttingLine();
void CutLine_CohenSutherland();

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(500, 100);               // Locate the position of window
	glutInitWindowSize(500, 400);                   // Set the size of window
	glutCreateWindow("TJU_CG_Cohen-Sutherland");
	/*
	glutDisplayFunc(displayCuttingLine);            // Call the function of drawing
	glutReshapeFunc(changeWindowSize);              // Reshape the window
	*/
	glClearColor(1.0, 1.0, 1.0, 0.0);               // Set window background to WHITE
	glMatrixMode(GL_PROJECTION);                    // Set projection parameters
	gluOrtho2D(0.0, 600.0, 0.0, 400.0);             // Set the size of stage
	CutLine_CohenSutherland();                      // Cut the line by using Cohen-Sutherland algorithm
	glutDisplayFunc(&displayCuttingLine);           // Call the function of drawing
	glutReshapeFunc(changeWindowSize);              // Reshape the window
	glutMainLoop();

}



/*
Encode the endpoint of line
	1001 | 1000 | 1010
	0001 | 0000 | 0010
	0101 | 0100 | 0110
*/
void endpoint_encode(int x, int y, int &c) {
	c = 0;
	if (x < boundary_left) c |= LEFT;
	else if (x > boundary_right) c |= RIGHT;
	if (y < boundary_bottom) c |= BOTTOM;
	else if (y > boundary_top) c |= TOP;
}

// If we change the window size...
void changeWindowSize(GLsizei w, GLsizei h) {

	if (h == 0) h = 1;

	glViewport(0, 0, w, h);                         // Set the window size
	glMatrixMode(GL_PROJECTION);                	// Reset coordinate system
	glLoadIdentity();

	// Set the range of part space
	if (w <= h) glOrtho(0.0f, 250.0f, 0.0f, 250.0f*h / w, 1.0, -1.0);
	else glOrtho(0.0f, 250.0f*w / h, 0.0f, 250.0f, 1.0, -1.0);

}

// Display the line
void displayCuttingLine() {

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);                       // Set the boundary to RED
	glPointSize(2.0f);                              // Set the size of point in the line

	// Draw the rectangle boundary    
	glBegin(GL_LINE_LOOP);
	glVertex2i(boundary_left, boundary_top);
	glVertex2i(boundary_left, boundary_bottom);
	glVertex2i(boundary_right, boundary_bottom);
	glVertex2i(boundary_right, boundary_top);
	glEnd();
	glFlush();

	// Draw the line which has NOT been cut
    glColor3f(0.0, 1.0, 0.0);                       // Set the boundary to GREEN
	glBegin(GL_LINES);
	glVertex2i(raw_X1, raw_Y1);
	glVertex2i(raw_X2, raw_Y2);
	glEnd();
	glFlush();

    // Draw the line which has been cut
    glColor3f(0.0, 0.0, 1.0);                       // Set the boundary to BLUE
	glBegin(GL_LINES);
	glVertex2i(X1, Y1);
	glVertex2i(X2, Y2);
	glEnd();
	glFlush();

}

// Cut the line by using Cohen-Sutherland algorithm
void CutLine_CohenSutherland() {

	int x, y;
	int code1, code2, code;

	// Calculate the code of endpoint
	endpoint_encode(X1, Y1, code1);
	endpoint_encode(X2, Y2, code2);

	while (code1 != 0 || code2 != 0) {                    // Break the loop only if two endpoints are in the rectangle

		if (code1 & code2) return;

		if (code1 != 0) code = code1;                // Set code=code1 if the first endpoint is not in the rectangle
		else code = code2;

		if (code & LEFT) {                            // Calculate the intersection of line and left boundary
			x = boundary_left;
			y = Y1 + (boundary_left - X1)*(Y2 - Y1) / (X2 - X1);
		}
		else if (code & RIGHT) {                      // Calculate the intersection of line and right boundary
			x = boundary_right;
			y = Y1 + (boundary_right - X1)*(Y2 - Y1) / (X2 - X1);
		}
		else if (code & BOTTOM) {                     // Calculate the intersection of line and bottom boundary
			y = boundary_bottom;
			x = X1 + (boundary_bottom - Y1)*(X2 - X1) / (Y2 - Y1);
		}
		else if (code & TOP) {                        // Calculate the intersection of line and top boundary
			y = boundary_top;
			x = X1 + (boundary_top - Y1)*(X2 - X1) / (Y2 - Y1);
		}

		if (code == code1) {
			X1 = x;
			Y1 = y;
			endpoint_encode(X1, Y1, code1);
		}
		else {
			X2 = x;
			Y2 = y;
			endpoint_encode(X2, Y2, code2);
		}
	}
}
