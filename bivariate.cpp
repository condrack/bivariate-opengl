
/*
Taylor Condrack
Program 5
4230
03/03/16
Program Description: This program displays a graph of a bivariate function.
'u' will rotate the object up, 'd' will rotate down, 'l' will rotate left
'r' will rotate right
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

static int a, b, data,indv=0,width,height;
#define K 100
#define NV (K+1)*(K+1)
static float node[NV][3],node2[NV][3];
#define NT 2*100*100
static int ltri[NT][3];
static int xrot=0,yrot=0;
static int z=0;
void menu(int);
void popupmenu();
void biv();

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
    biv();
}

//creates view volume and rectangle viewport
void reshape(int w, int h)
{
    width=w;
    height=h;
//creates a square viewport and keeps a square when resized
    if(w>h)
        glViewport ((w-h)/2, 0, (GLsizei) h, (GLsizei) h);
    else
        glViewport(0,(h-w)/2,(GLsizei) w, (GLsizei) w);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-10.0, 10.0, -10.0, 10.0, 1, 0);
    glScalef(.9,.9,0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-2.0);
    glRotatef(xrot,1.0,0.0,0.0);
    glRotatef(yrot,0.0,1.0,0.0);
    glTranslatef(-.5,-0.5,0.0);

}
float f(int x,int y)
{
    int z;
    z=0.5*exp(-.04*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2))) *cos(0.15*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2)));
    return z;

}
void norm(float node[])
{
    float l=0;

    for(int i=0;i<3;i++)
        l=l+(node[i]*node[i]);

    l=sqrt(l);

    for(int j=0;j<3;j++)
        node[j]=node[j]/l;
}

void biv()
{
    float y,x,z;
    float h = 1.0/K;
    for (int j=0; j<=K; j++) {
        y = j*h;
        for (int i=0; i<=K; i++) {
            x = i*h;
            z = 0.5*exp(-.04*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2))) *cos(0.15*sqrt(pow(80*x-40, 2) + pow(90*y-45, 2)));
            node[indv][0] = x;
            node[indv][1] = y;
            node[indv][2] = z;
            indv++;
        }
    }
    int indt = 0;
    for (int j=1; j<=K; j++) {
        for (int i=1; i<=K; i++) {
            indv = j*(K+1) + i;
            ltri[indt][0] = indv-K-2;
            ltri[indt][1] = indv-K-1;
            ltri[indt][2] = indv;
            ltri[indt+1][0] = indv-K-2;
            ltri[indt+1][1] = indv;
            ltri[indt+1][2] = indv-1;
            indt += 2;
        }
    }

    for (indv=0; indv<=NV-1; indv++) {
        node2[indv][0] = 0;
        node2[indv][1] = 0;
        node2[indv][2] = 0;
    }

    int i1,i2,i3;
    float tn[3];
    for (indt=0; indt<=NT-1; indt++) {
        i1 = ltri[indt][0];
        i2 = ltri[indt][1];
        i3 = ltri[indt][2];
        tn[0] = (node[i2][1]-node[i1][1]) * (node[i3][2]-node[i1][2]) -
                (node[i2][2]-node[i1][2]) * (node[i3][1]-node[i1][1]);
        tn[1] = (node[i2][2]-node[i1][2]) * (node[i3][0]-node[i1][0]) -
                (node[i2][0]-node[i1][0]) * (node[i3][2]-node[i1][2]);
        tn[2] = (node[i2][0]-node[i1][0]) * (node[i3][1]-node[i1][1]) -
                (node[i2][1]-node[i1][1]) * (node[i3][0]-node[i1][0]);
        norm(tn);
        for (int i=0; i<3; i++) {
            node2[i1][i] += tn[i];
            node2[i2][i] += tn[i];
            node2[i3][i] += tn[i];
        }
    }

    for (indv=0; indv<=NV-1; indv++)
        norm (node2[indv]);
}

void popupmenu()
{
    a= glutCreateMenu(menu);
    glutAddMenuEntry("rotate left",1);
    glutAddMenuEntry("rotate right",2);
    glutAddMenuEntry("zoom +",4);
    glutAddMenuEntry("zoom -",3);
    glutAddMenuEntry("rotate up",5);
    glutAddMenuEntry("rotate down",6);
    glutAddMenuEntry("quit",7);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int in)
{
    if(in==0)
    {
        glutDestroyWindow(b);
        exit(0);
    }
    if(in==1)
    {
      yrot=(yrot+10)%360;
      glutPostRedisplay();
    }

    if(in==2)
    {
        yrot=(yrot-10)%360;
        glutPostRedisplay();
    }

    if(in==3)
    {
        z=z+1;
        glutPostRedisplay();
    }

    if(in==4)
    {
        z=z-1;
        glutPostRedisplay();
    }
    if(in==5)
    {
        xrot=(xrot+10)%360;
        glutPostRedisplay();
    }
    if(in==6)
    {
        xrot=(xrot-10)%360;
        glutPostRedisplay();
    }
    if(in==7)
    {
        exit(0);
    }
    else
    {
        data=in;
    }
    reshape(width,height);
    glutPostRedisplay();
}
void keyboard(unsigned char key,int x,int y)
{
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 'l':
         yrot=(yrot+10)%360;
         glutPostRedisplay();
         break;
        case 'r':
         yrot=(yrot-10)%360;
         glutPostRedisplay();
         break;
        case '+':
         z=z+1;
         glutPostRedisplay();
         break;
        case '-':
         z=z-1;
         glutPostRedisplay();
         break;
        case 'u':
            xrot=(xrot+10)%360;
            glutPostRedisplay();
            break;
        case 'd':
            xrot=(xrot-10)%360;
            glutPostRedisplay();
            break;
      default:
         break;
    }
    reshape(width,height);
    glutPostRedisplay();
}
void display(void)
{
    popupmenu();
    GLubyte color[] = {
            240,1,0,200
    };

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glColorPointer(1, GL_UNSIGNED_BYTE, 0, color);
    glEnableClientState (GL_VERTEX_ARRAY);
    glEnableClientState (GL_NORMAL_ARRAY);
    glVertexPointer (3, GL_FLOAT, 0, node);
    glNormalPointer(GL_FLOAT, 0, node2);
    glDrawElements(GL_TRIANGLES, NT*3, GL_UNSIGNED_INT, ltri);
    glPopMatrix();
    glutSwapBuffers();

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (800, 800);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    popupmenu();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
