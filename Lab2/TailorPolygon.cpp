#include<GL/glut.h>
#include<iostream>

using namespace std;

// Init the boundary
int boundary_left   = 20;
int boundary_right  = 120;
int boundary_bottom = 20;
int boundary_top    = 120;

// Init the polygon that will be tailored.
int polygon[50][2] = {
    {10, 80},
    {70, 10},
    {80, 80},
    {90, 10},
    {130, 80},
    {80, 130},
};

enum mark {LEFT, BOTTOM, RIGHT, TOP};

int c = -1;

struct node{
    float data[2];
    node* next;
    node* pre;
};

void initNode(node* &head);
void cutEdge(node* &first, node* &head, int &edg, float xl, float xr, float yb, float yt);
void getPoint(node* &head, float xl, float xr, float yb, float yt);
void displayPolygon();
void initBoundary();
void changeSize(int w, int h);

int main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(500, 100);
    glutCreateWindow("TJU_CG_Sutherland-Hodgman");
    glutDisplayFunc(initBoundary);
    glutReshapeFunc(changeSize);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glutMainLoop();
}

// Set the polygon into linked list
void initNode(node* &head){

    head = new node;
    head->data[0] = 0;
    head->data[1] = 0;
    head->next = NULL;
    head->pre = NULL;

    node *p = head;
    node *q = NULL;
    for(int i=0; i<6; i++){
        q = new node;
        q->data[0] = polygon[i][0];
        q->data[1] = polygon[i][1];
        q->pre = p;
        p->next = q;
        p = q;
    }
    p->next = head;
    head->pre = p;

}

// Tailor the edge of polygon
void cutEdge(node* &first, node* &head, int &edg, float xl, float xr, float yb, float yt){

    if(edg == 4) return;

    node* p = first;    // end point
    node* q = p->pre;   // start point

    // Handle each point successively.
    while(p != head){
        int x = 0;
        int y = 0;
        
        // node HEAD is NULL
        if(q == head) q = p->pre->pre;
        
        // Define the slope of edge
        float k = 0;
        if(q->data[0] - p->data[0] != 0){
            k = (q->data[1]-p->data[1]) * 1.0 / (q->data[0]-p->data[0]);
        }

        switch(edg){
            case LEFT:
                // If the endpoints of edge aren't in the same side of boundary...
                if(((p->data[0]<=xl) && (q->data[0]>=xl)) || ((p->data[0]>=xl) && (q->data[0]<=xl))){
                    // If the start point is in the right side of left boundary...
                    if((q->data[0]>=xl) && (q->data[0]<=xr)){
                        c++;
                        polygon[c][0] = q->data[0];
                        polygon[c][1] = q->data[1];
                    }
                    // Handle the end point
                    x = xl;
                    y = p->data[1] + (x-p->data[0]) * k;
                    c++;
                    polygon[c][0] = x;
                    polygon[c][1] = y;
                }
                // If the endpoints of edge are both in the boundary...
                else{
                    c++;
                    polygon[c][0] = q->data[0];
                    polygon[c][1] = q->data[1];
                }
                break;
            case RIGHT:
                // If the endpoints of edge aren't in the same side of boundary...
                if(((p->data[0]>=xr) && (q->data[0]<=xr)) || ((p->data[0]<=xr) && (q->data[0]>=xr))){
                    // If the start point is in the left side of right boundary...
                    if((q->data[0]>=xl) && (q->data[0]<=xr)){
                        c++;
                        polygon[c][0] = q->data[0];
                        polygon[c][1] = q->data[1];
                    }
                    // Handle the end point
                    x = xr;
                    y = p->data[1] + (x-p->data[0]) * k;
                    c++;
                    polygon[c][0] = x;
                    polygon[c][1] = y;
                }
                // If the endpoints of edge are both in the boundary...
                else{
                    c++;
                    polygon[c][0] = q->data[0];
                    polygon[c][1] = q->data[1];
                }
                break;
            case BOTTOM:
                // If the endpoints of edge aren't in the same side of boundary...
                if(((p->data[1]<=yb) && (q->data[1]>=yb)) || ((p->data[1]>=yb) && (q->data[1]<=yb))){
                    // If the start point is above the bottom boundary...
                    if((q->data[1]>=yb) && (q->data[1]<=yt)){
                        c++;
                        polygon[c][0] = q->data[0];
                        polygon[c][1] = q->data[1];
                    }

                    // Condiser the condition that the edge is vertical or horizontal
                    if(k == 0){
                        if(p->data[1] != q->data[1]){
                            y = yb;
                            x = p->data[0];
                        }
                        else break;
                    }
                    if(k != 0){
                        y = yb;
                        x = p->data[0] + (y-p->data[1]) / k;
                    }
                    
                    // Handle the end point
                    c++;
                    polygon[c][0] = x;
                    polygon[c][1] = y;
                }
                // If the endpoints of edge are both in the boundary...
                else{
                    c++;
                    polygon[c][0] = q->data[0];
                    polygon[c][1] = q->data[1];
                }
                break;
            case TOP:
                // If the endpoints of edge aren't in the same side of boundary...
                if(((p->data[1]>=yt) && (q->data[1]<=yt)) || ((p->data[1]<=yt) && (q->data[1]>=yt))){
                    // If the start point is under the top boundary...
                    if((q->data[1]>=yb) && (q->data[1]<=yt)){
                        c++;
                        polygon[c][0] = q->data[0];
                        polygon[c][1] = q->data[1];
                    }

                    // Condiser the condition that the edge is vertical or horizontal
                    if(k == 0){
                        if(p->data[1] != q->data[1]){
                            y = yb;
                            x = p->data[0];
                        }
                        else break;
                    }
                    if(k != 0){
                        y = yt;
                        x = p->data[0] + (y-p->data[1]) / k;
                    }
                    
                    // Handle the end point
                    c++;
                    polygon[c][0] = x;
                    polygon[c][1] = y;
                }
                // If the endpoints of edge are both in the boundary...
                else{
                    c++;
                    polygon[c][0] = q->data[0];
                    polygon[c][1] = q->data[1];
                }
                break;
            default:
                break;
        }
        // Update the handled point of the polygon
        p = p->next;
        q = p->pre;
    }

    // Update the input point of the polygon
    p = head;
    node* temp = NULL;
    for(int i=0; i<=c; i++){
        temp = new node;
        temp->data[0] = polygon[i][0];
        temp->data[1] = polygon[i][1];
        temp->pre = p;
        p->next = temp;
        p = temp;
    }
    p->next = head;
    head->pre = p;

    c = -1;
    edg++;
    p = p->next->next;
    
    cutEdge(p, head, edg, xl, xr, yb, yt);

}

// Tailor each edge
void getPoint(node* &head, float xl, float xr, float yb, float yt){
    int n = 0;
    node* p = head->next;
    cutEdge(p, head, n, xl, xr, yb, yt);
}

// Draw the polygon
void displayPolygon(){

    node* h = new node;
    initNode(h);
    node* p = h->next;

    // Draw the raw polygon
    // Set the edge of raw polygon to GREEN
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    while(p != h){
        glVertex2f(p->data[0], p->data[1]);
        p = p->next;
    }
    glEnd();

    // Tailor the polygon
    getPoint(h, boundary_left, boundary_right, boundary_bottom, boundary_top);

    // Draw the polygon that has been tailored
    p = h->next;
    // Set the edge of new polygon to BLUE
    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    while(p != h){
        glVertex2f(p->data[0], p->data[1]);
        p = p->next;
    }
    glEnd();

}

// Draw the boundary
void initBoundary(){
    glClear(GL_COLOR_BUFFER_BIT);
    // Set the boundary to RED
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_LOOP);
    glVertex2f(20.0f, 20.0f);
    glVertex2f(20.0f, 120.0f);
    glVertex2f(120.0f, 120.0f);
    glVertex2f(120.0f, 20.0f);
    glEnd();

    displayPolygon();
    glFlush();
}

void changeSize(int w, int h){
    if(h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w <= h) glOrtho(0.0f, 200.0f, 0.0f, 200.0f*h/w, 1.0f, -1.0f);
    else glOrtho(0.0f, 200.0f*w/h, 0.0f, 200.0f, 1.0f, -1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

