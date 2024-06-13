#include <stdlib.h> // standard definitions
#include <math.h>   // math definitions
#include <stdio.h>  // standard I/O

#ifdef APPLE
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Escape key
#define ESC 27

// Camera position
float x = -8.0, y = 7.0, z = 10.0;
float deltaMove = 0.0;

// Camera direction
float lx = 0.0, ly = 1.0, lz = -1.0;
float angle = 0.0;
float deltaAngle = 0.0;

// Mouse drag control
int isDragging = 0;
int xDragStart = 0;

void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, ratio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void update(void)
{
    if (deltaMove)
    {
        x += deltaMove * lx * 0.01;
        y += deltaMove * ly * 0.01;
        z += deltaMove * lz * 0.01;
    }
    glutPostRedisplay();
}

void drawCube(float sx, float sy, float sz, float tx, float ty, float tz, float angle = 0.0, float ax = 0.0, float ay = 0.0, float az = 1.0)
{
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glRotatef(angle, ax, ay, az);
    glScalef(sx, sy, sz);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawFilledCylinder(float radius, float height, float tx, float ty, float tz)
{
    GLUquadric *quadric = gluNewQuadric();
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluDisk(quadric, 0, radius, 32, 1); // Base
    gluCylinder(quadric, radius, radius, height, 32, 32);
    glTranslatef(0, 0, height);
    gluDisk(quadric, 0, radius, 32, 1); // Top
    glPopMatrix();
    gluDeleteQuadric(quadric);
}

void drawRoom()
{
    // Floor
    glColor3f(0.6, 0.4, 0.2); // Brown color
    drawCube(8.0, 0.1, 8.0, 0.0, -1.0, 0.0);

    // Walls
    glColor3f(0.8, 0.8, 0.8);
    drawCube(8.0, 4.0, 0.1, 0.0, 1.0, -4.0); // Back wall
    glColor3f(0.0, 0.5, 0.0);                // Green wall
    drawCube(0.1, 4.0, 8.0, 4.0, 1.0, 0.0);  // Right wall
}

void drawSofa()
{
    glColor3f(0.5, 0.5, 0.8);
    drawCube(1.5, 0.3, 2.5, 3, -0.5, 0);  // Seat
    drawCube(0.3, 0.7, 2.5, 3.6, 0, 0);   // Backrest
    drawCube(1.5, 1, 0.3, 3, -0.5, 1.4);  // Armrest 1
    drawCube(1.5, 1, 0.3, 3, -0.5, -1.4); // Armrest 2

    // Add cushions to sofa
    glColor3f(0.9, 0.9, 0.9); // Light grey color for the cushions
    drawCube(0.5, 0.5, 0.1, 3.1, -0.1, 1.0, 60, 1.0, 0.0, 1.0); // First cushion on sofa
    drawCube(0.5, 0.5, 0.1, 3.1, -0.1, -0.9, -60, 1.0, 1.0, 1.0); // Second cushion on sofa
}

void drawTable()
{
    glColor3f(0.6, 0.3, 0.2);
    drawFilledCylinder(1.0, 0.2, 0.0, -0.4, 0.5); // Table top (cylinder)
    drawCube(0.1, 0.6, 0.1, 0.7, -0.7, 0.9);      // Table leg 1
    drawCube(0.1, 0.6, 0.1, -0.7, -0.7, 0.9);     // Table leg 2
    drawCube(0.1, 0.6, 0.1, 0.7, -0.7, 0.1);      // Table leg 3
    drawCube(0.1, 0.6, 0.1, -0.7, -0.7, 0.1);     // Table leg 4
}

void drawChair()
{
    // Draw the chair as per the reference image
    glColor3f(0.6, 0.4, 0.2);                  // Light brown color for the frame
    drawCube(0.1, 1.7, 0.1, -2.1, -0.1, -2.6); // Back left leg
    drawCube(0.1, 1.7, 0.1, -2.9, -0.1, -2.6); // Back right leg
    drawCube(0.1, 1, 0.1, -2.0, -0.5, -1.6);   // Front left leg
    drawCube(0.1, 1, 0.1, -3.0, -0.5, -1.6);   // Front right leg
    drawCube(1.0, 0.1, 1.0, -2.5, -0.5, -2.0); // Seat
    drawCube(0.1, 0.1, 1, -3.0, -0.1, -2.1);   // Arm support
    drawCube(0.1, 0.1, 1, -2.0, -0.1, -2.1);   // Arm support

    glColor3f(0.9, 0.9, 0.9);                  // Light grey color for the cushion
    drawCube(0.9, 0.2, 0.9, -2.5, -0.3, -2.0); // Seat cushion
    drawCube(0.9, 1.0, 0.2, -2.5, 0.3, -2.4);  // Back cushion

    // Add cushion to chair
    drawCube(0.4, 0.4, 0.1, -2.3, 0, -2.1, -45, 1.0, 1.0, 1.0); // Cushion on chair
}

void drawLamp()
{
    glColor3f(0.9, 0.6, 0.3);
    drawCube(0.1, 2, 0.1, 3.3, 0, -3.3);          // Lamp pole
    drawFilledCylinder(0.5, 0.5, 3.4, 0.8, -3.4); // Lamp shade
}

void drawWindow()
{
    // Draw the window with brown frame and blue glass
    glColor3f(0.5, 0.35, 0.05);             // Brown color for the frame
    drawCube(2.2, 2.2, 0.1, 0, 1.0, -3.95); // Frame
    // Inner frame vertical line
    drawCube(0.1, 2.2, 0.1, 0, 1.0, -3.89);
    // Inner frame horizontal line
    drawCube(2.2, 0.1, 0.1, 0, 1.0, -3.89);

    glColor3f(0.7, 0.9, 1.0);               // Blue color for the glass
    drawCube(2.0, 2.0, 0.1, 0, 1.0, -3.90); // Glass

    // Draw curtains
    glColor3f(0.9, 0.8, 0.7);                 // Beige color for the curtains
    drawCube(0.7, 3.2, 0.1, -1.2, 1.0, -3.8); // Left curtain
    drawCube(0.7, 3.2, 0.1, 1.2, 1.0, -3.8);  // Right curtain
    // Curtain holder
    glColor3f(0.5, 0.35, 0.05);
    drawCube(2.2, 0.1, 0.1, 0, 2.4, -3.89);
}

void drawBookshelf()
{
    glColor3f(0.5, 0.35, 0.05);
    drawCube(1.0, 0.1, 1, -3.0, 1.5, -3.8); // Shelf base
    glColor3f(1.0, 0.0, 0.0);               // Red book
    drawCube(0.1, 0.6, 0.5, -3.3, 1.8, -3.7);
    glColor3f(0.0, 1.0, 0.0); // Green book
    drawCube(0.1, 0.6, 0.5, -3.0, 1.8, -3.7);
    glColor3f(0.0, 0.0, 1.0); // Blue book
    drawCube(0.1, 0.6, 0.5, -2.7, 1.8, -3.7);
}

void drawPicture()
{
    glColor3f(0.5, 0.35, 0.05); // Frame
    drawCube(0.1, 1.5, 2.0, 3.8, 2.0, 0.0);
    glColor3f(0.0, 0.5, 0.5); // Picture
    drawCube(0.01, 1.3, 1.8, 3.75, 2.0, 0.01);
}

void drawRug()
{
    glColor3f(0.8, 0.8, 0.8); // Rug
    drawCube(5.5, 0.1, 5.5, 0.0, -0.95, 0.2);
}

void drawScene(void)
{
    // Clear color and depth buffers
    glClearColor(0.7, 0.7, 0.7, 1.0); // Background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera
    gluLookAt(
        x, y, z,
        0, 0, 0,
        0, 1, 0);

    drawRoom();
    drawSofa();
    drawTable();
    drawChair();
    drawLamp();
    drawWindow();
    drawBookshelf();
    drawPicture();
    drawRug();

    glutSwapBuffers();
}

void processNormalKey(unsigned char key, int xx, int yy)
{
    if (key == ESC || key == 'q' || key == 'Q')
        exit(0);
}

void pressSpecialKey(int key, int xx, int yy)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        deltaMove = 1.0;
        break;
    case GLUT_KEY_DOWN:
        deltaMove = -1.0;
        break;
    }
}

void releaseSpecialKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        deltaMove = 0.0;
        break;
    case GLUT_KEY_DOWN:
        deltaMove = 0.0;
        break;
    }
}

void mouseMove(int x, int y)
{
    if (isDragging)
    {
        deltaAngle = (x - xDragStart) * 0.005;
        lx = -sin(angle + deltaAngle);
        ly = cos(angle + deltaAngle);
    }
}

void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            isDragging = 1;
            xDragStart = x;
        }
        else
        {
            angle += deltaAngle;
            isDragging = 0;
        }
    }
}

void lights()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light1_diffuse[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat light1_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position1[] = {-2.0, -2.0, 5.0, 1.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lmodel_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
}

void lightoff(unsigned char key, int x, int y)
{
    if (key == 'a')
    {
        glDisable(GL_LIGHT0);
    }
    if (key == 's')
    {
        glEnable(GL_LIGHT0);
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    printf("\n\
-----------------------------------------------------------------------\n\
  OpenGL Sample Program:\n\
  - Hold up-arrow/down-arrow to move camera n\
  - q or ESC to quit\n\
-----------------------------------------------------------------------\n");

    // General initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 600);
    glutCreateWindow("OpenGL/GLUT Sample Program");
    lights();

    // Register callbacks
    glutReshapeFunc(changeSize);
    glutDisplayFunc(drawScene);
    glutIdleFunc(update);
    glutIgnoreKeyRepeat(1);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(processNormalKey);
    glutSpecialFunc(pressSpecialKey);
    glutSpecialUpFunc(releaseSpecialKey);
    glutKeyboardFunc(lightoff);

    // OpenGL init
    glEnable(GL_DEPTH_TEST);

    // Enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}
