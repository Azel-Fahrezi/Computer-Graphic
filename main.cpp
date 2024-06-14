#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <cmath>
#include <cstdio>

void display();
void init();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void mouseMotion(int x, int y);
void drawRoom();
void drawCupboard();
void drawClock();
void drawDoor();
void drawBed();

float doorAngle = 0.0f;
float angleX = 0.0f;
float angleY = 0.0f;
float zoomZ = 30.0f;

GLuint floorTexture;
GLuint wallTexture;

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D Room");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouseMotion);

    init();

    glutMainLoop();

    return 0;
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    floorTexture = SOIL_load_OGL_texture(
        "wood.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS
    );

    if (floorTexture == 0) {
        printf("Failed to load texture: %s\n", SOIL_last_result());
    }

    wallTexture = SOIL_load_OGL_texture(
        "wall.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS
    );

    if (wallTexture == 0) {
        printf("Failed to load texture: %s\n", SOIL_last_result());
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(0.0, 0.0, zoomZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);

    drawRoom();
    drawCupboard();
    drawClock();
    drawDoor();
    drawBed();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    if (height == 0) height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
        case 'A':    
            angleY -= 5.0f;
            break;
        case 'd':
        case 'D':
            angleY += 5.0f;
            break;
        case 'w':
        case 'W':
            zoomZ -= 1.0f;
            if (zoomZ < 5.0f) zoomZ = 5.0f;
            break;
        case 's':
        case 'S':
            zoomZ += 1.0f;
            if (zoomZ > 100.0f) zoomZ = 100.0f;
            break;
        case 27:
            exit(0);
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    static int lastX = -1;
    static int lastY = -1;

    if (lastX == -1 && lastY == -1) {
        lastX = x;
        lastY = y;
        return;
    }

    int deltaX = x - lastX;
    int deltaY = y - lastY;

    angleY += deltaX * 0.2f;
    angleX += deltaY * 0.2f;

    if (angleX > 85.0f) angleX = 85.0f;
    if (angleX < -85.0f) angleX = -85.0f;

    lastX = x;
    lastY = y;

    glutPostRedisplay();
}

void drawRoom() {
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glBegin(GL_QUADS);

    glColor3ub(255, 255, 255);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, -5.0, 10.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, 10.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0, -6.0, 10.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(15.0, -5.0, 10.0);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, -5.0, -10.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, -10.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0, -6.0, -10.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(15.0, -5.0, -10.0);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, -5.0, -10.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -5.0, 10.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0, -5.0, 10.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(15.0, -5.0, -10.0);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, -6.0, -10.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, 10.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0, -6.0, 10.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(15.0, -6.0, -10.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glBegin(GL_QUADS);

    glColor3ub(255, 255, 255);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, 8.0, -10.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, -10.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0, -6.0, -10.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(15.0, 8.0, -10.0);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, 8.0, -11.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, -11.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0, -6.0, -11.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(15.0, 8.0, -11.0);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, 8.0, -11.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, -11.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0, -6.0, -10.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0, 8.0, -10.0);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(15.0, 8.0, -10.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(15.0, -6.0, -10.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(15.0, -6.0, -11.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(15.0, 8.0, -11.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, wallTexture);
    glBegin(GL_QUADS);

    glColor3ub(255, 255, 255);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, 8.0, -10.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, -10.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0, -6.0, 10.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0, 8.0, 10.0);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, 8.0, 10.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, 10.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0, -6.0, 11.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0, 8.0, 11.0);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, 8.0, 11.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, 11.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0, -6.0, 10.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0, 8.0, 10.0);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(-15.0, 8.0, -10.0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-15.0, -6.0, -10.0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-15.0, -6.0, -11.0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-15.0, 8.0, -11.0);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void drawCupboard() {
    glBegin(GL_QUADS);
    glColor3ub(80, 50, 20);
    glVertex3f(-12.0, -0.5, -1.5);
    glVertex3f(-12.0, -5.5, -1.5);
    glVertex3f(-12.0, -5.5, -5.5);
    glVertex3f(-12.0, -0.5, -5.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(80, 50, 20);
    glVertex3f(-14.5, -0.5, -1.5);
    glVertex3f(-14.5, -5.5, -1.5);
    glVertex3f(-14.5, -5.5, -5.5);
    glVertex3f(-14.5, -0.5, -5.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(80, 50, 20);
    glVertex3f(-12.0, -0.5, -5.5);
    glVertex3f(-12.0, -5.5, -5.5);
    glVertex3f(-14.5, -5.5, -5.5);
    glVertex3f(-14.5, -0.5, -5.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(80, 50, 20);
    glVertex3f(-12.0, -0.5, -1.5);
    glVertex3f(-12.0, -5.5, -1.5);
    glVertex3f(-14.5, -5.5, -1.5);
    glVertex3f(-14.5, -0.5, -1.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(161, 108, 59);
    glVertex3f(-14.5, -0.5, -1.5);
    glVertex3f(-12.0, -0.5, -1.5);
    glVertex3f(-12.0, -0.5, -5.5);
    glVertex3f(-14.5, -0.5, -5.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(161, 108, 59);
    glVertex3f(-11.8, -1.0, -2.0);
    glVertex3f(-11.8, -2.0, -2.0);
    glVertex3f(-11.8, -2.0, -5.0);
    glVertex3f(-11.8, -1.0, -5.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(161, 108, 59);
    glVertex3f(-11.8, -2.5, -2.0);
    glVertex3f(-11.8, -4.5, -2.0);
    glVertex3f(-11.8, -4.5, -5.0);
    glVertex3f(-11.8, -2.5, -5.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(235, 235, 235);
    glVertex3f(-11.7, -1.5, -3.1);
    glVertex3f(-11.7, -1.8, -3.1);
    glVertex3f(-11.7, -1.8, -4.1);
    glVertex3f(-11.7, -1.5, -4.1);
    glEnd();
}

void drawClock() {
    glPushMatrix();
    glTranslatef(-5.0f, 5.0f, -9.0f);
    
    glColor3ub(255, 255, 255);
    glutSolidTorus(0.1f, 1.0f, 20, 50);

    glColor3ub(0, 0, 0);
    glLineWidth(2.0f);

    glPushMatrix();
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.7f, 0.0f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

void drawDoor() {
    glBegin(GL_QUADS);
    glColor3ub(80, 50, 20);
    glVertex3f(-14.5, 6.0, -9.5);
    glVertex3f(-14.5, -6.0, -9.5);
    glVertex3f(-9.0, -6.0, -9.5);
    glVertex3f(-9.0, 6.0, -9.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(80, 50, 20);
    glVertex3f(-9.0, 6.0, -9.5);
    glVertex3f(-9.0, -6.0, -9.5);
    glVertex3f(-9.0, -6.0, -10.0);
    glVertex3f(-9.0, 6.0, -10.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(80, 50, 20);
    glVertex3f(-14.5, 6.0, -9.5);
    glVertex3f(-14.5, -6.0, -9.5);
    glVertex3f(-14.5, -6.0, -10.0);
    glVertex3f(-14.5, 6.0, -10.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(80, 50, 20);
    glVertex3f(-14.5, 6.0, -9.5);
    glVertex3f(-14.5, 6.0, -10.0);
    glVertex3f(-9.0, 6.0, -10.0);
    glVertex3f(-9.0, 6.0, -9.5);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(163, 131, 91);
    glVertex3f(-13.5, 5.0, -9.4);
    glVertex3f(-13.5, 2.0, -9.4);
    glVertex3f(-10.0, 2.0, -9.4);
    glVertex3f(-10.0, 5.0, -9.4);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(163, 131, 91);
    glVertex3f(-13.5, 0.0, -9.4);
    glVertex3f(-13.5, -4.0, -9.4);
    glVertex3f(-10.0, -4.0, -9.4);
    glVertex3f(-10.0, 0.0, -9.4);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(163, 131, 91);
    glVertex3f(-10.0, 1.5, -9.4);
    glVertex3f(-10.0, 0.5, -9.4);
    glVertex3f(-9.5, 0.5, -9.4);
    glVertex3f(-9.5, 1.5, -9.4);
    glEnd();
}

void drawBed(){
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-15.0, 2.0, 8.0);
    glVertex3f(-15.0, -5.0, 8.0);
    glVertex3f(-13.5, -5.0, 8.0);
    glVertex3f(-13.5, 2.0, 8.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-15.0, 2.0, 6.5);
    glVertex3f(-15.0, -5.0, 6.5);
    glVertex3f(-13.5, -5.0, 6.5);
    glVertex3f(-13.5, 2.0, 6.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-15.0, 2.0, 8.0);
    glVertex3f(-15.0, 2.0, 6.5);
    glVertex3f(-13.5, 2.0, 6.5);
    glVertex3f(-13.5, 2.0, 8.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-13.5, 2.0, 8.0);
    glVertex3f(-13.5, -5.0, 8.0);
    glVertex3f(-13.5, -5.0, 6.5);
    glVertex3f(-13.5, 2.0, 6.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-13.5, -1.5, 8.0);
    glVertex3f(-13.5, -3.5, 8.0);
    glVertex3f(1.0, -3.5, 8.0);
    glVertex3f(1.0, -1.5, 8.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(1.0, 2.0, 8.0);
    glVertex3f(1.0, -5.0, 8.0);
    glVertex3f(2.5, -5.0, 8.0);
    glVertex3f(2.5, 2.0, 8.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(1.0, 2.0, 6.5);
    glVertex3f(1.0, -5.0, 6.5);
    glVertex3f(2.5, -5.0, 6.5);
    glVertex3f(2.5, 2.0, 6.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(1.0, 2.0, 8.0);
    glVertex3f(1.0, 2.0, 6.5);
    glVertex3f(2.5, 2.0, 6.5);
    glVertex3f(2.5, 2.0, 8.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(2.5, 2.0, 8.0);
    glVertex3f(2.5, -5.0, 8.0);
    glVertex3f(2.5, -5.0, 6.5);
    glVertex3f(2.5, 2.0, 6.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(1.0, 2.0, 8.0);
    glVertex3f(1.0, -5.0, 8.0);
    glVertex3f(1.0, -5.0, 6.5);
    glVertex3f(1.0, 2.0, 6.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(2.5, 1.0, 6.5);
    glVertex3f(2.5, -3.5, 6.5);
    glVertex3f(2.5, -3.5, 2.0);
    glVertex3f(2.5, 1.0, 2.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(1.0, 2.0, 2.0);
    glVertex3f(1.0, -5.0, 2.0);
    glVertex3f(2.5, -5.0, 2.0);
    glVertex3f(2.5, 2.0, 2.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(1.0, 2.0, 0.5);
    glVertex3f(1.0, -5.0, 0.5);
    glVertex3f(2.5, -5.0, 0.5);
    glVertex3f(2.5, 2.0, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(1.0, 2.0, 2.0);
    glVertex3f(1.0, 2.0, 0.5);
    glVertex3f(2.5, 2.0, 0.5);
    glVertex3f(2.5, 2.0, 2.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(2.5, 2.0, 2.0);
    glVertex3f(2.5, -5.0, 2.0);
    glVertex3f(2.5, -5.0, 0.5);
    glVertex3f(2.5, 2.0, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(1.0, 2.0, 2.0);
    glVertex3f(1.0, -5.0, 2.0);
    glVertex3f(1.0, -5.0, 0.5);
    glVertex3f(1.0, 2.0, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-13.5, -1.5, 0.5);
    glVertex3f(-13.5, -3.5, 0.5);
    glVertex3f(1.0, -3.5, 0.5);
    glVertex3f(1.0, -1.5, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-15.0, 2.0, 2.0);
    glVertex3f(-15.0, -5.0, 2.0);
    glVertex3f(-13.5, -5.0, 2.0);
    glVertex3f(-13.5, 2.0, 2.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-15.0, 2.0, 0.5);
    glVertex3f(-15.0, -5.0, 0.5);
    glVertex3f(-13.5, -5.0, 0.5);
    glVertex3f(-13.5, 2.0, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-15.0, 2.0, 2.0);
    glVertex3f(-15.0, 2.0, 0.5);
    glVertex3f(-13.5, 2.0, 0.5);
    glVertex3f(-13.5, 2.0, 2.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-13.5, 2.0, 2.0);
    glVertex3f(-13.5, -5.0, 2.0);
    glVertex3f(-13.5, -5.0, 0.5);
    glVertex3f(-13.5, 2.0, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(101, 67, 33);
    glVertex3f(-14.5, 1.0, 6.5);
    glVertex3f(-14.5, -3.5, 6.5);
    glVertex3f(-14.5, -3.5, 2.0);
    glVertex3f(-14.5, 1.0, 2.0);
    glEnd();

    glBegin(GL_QUADS);
    glColor3ub(255, 0, 0);
    glVertex3f(-10.0, 0.0, 0.5);
    glVertex3f(-10.0, -1.5, 0.5);
    glVertex3f(1.0, -1.5, 0.5);
    glVertex3f(1.0, 0.0, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 0, 0);
    glVertex3f(-10.0, 0.0, 8.0);
    glVertex3f(-10.0, -1.5, 8.0);
    glVertex3f(1.0, -1.5, 8.0);
    glVertex3f(1.0, 0.0, 8.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 0, 0);
    glVertex3f(-10.0, 0.0, 0.5);
    glVertex3f(-10.0, 0.0, 8.0);
    glVertex3f(1.0, 0.0, 8.0);
    glVertex3f(1.0, 0.0, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex3f(-14.5, 0.0, 0.5);
    glVertex3f(-14.5, -1.5, 0.5);
    glVertex3f(-10.0, -1.5, 0.5);
    glVertex3f(-10.0, 0.0, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex3f(-14.5, 0.0, 8.0);
    glVertex3f(-14.5, -1.5, 8.0);
    glVertex3f(-10.0, -1.5, 8.0);
    glVertex3f(-10.0, 0.0, 8.0);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(255, 255, 255);
    glVertex3f(-14.5, 0.0, 0.5);
    glVertex3f(-14.5, 0.0, 8.0);
    glVertex3f(-10.0, 0.0, 8.0);
    glVertex3f(-10.0, 0.0, 0.5);
    glEnd();
}
