#include "glm.h"
#include "math.h"
#include "stdio.h"

typedef struct Ponto    //Struct Ponto
{
    float x, y;
} Ponto;


void desenhar_circunferencia()  //Draw circle
{
    int n = 20;        //Circle vertexes
    Ponto p0 = {0.25, 0.25}, p1 = {-0.25, -0.25};   //Ponto p0 e p1
    
    //Ponto médio 
    float xm = (p0.x + p1.x)/2;
    float ym = (p0.y + p1.y)/2;
    
    //Distância
    float a = p1.x - p0.x;
    float b = p1.y - p0.y;
    float d = sqrt(pow(a, 2) + pow(b, 2));  //distância
    float r = d/2;                          //raio

    //Pontos da circunferencia
    float alpha = 2*M_PI/n;             //conversão
    float c[n][2];
    float x, y, t=0;
    for (int i = 0; i < n; i++)         //percorrer todos os pontos para fazer a circunferÊncia
    {
        x = r * cos(t) + xm;
        y = r * sin(t) + ym;
        c[i][0] = x;
        c[i][1] = y;
        t+=alpha;
        printf("%.2f    %.2f\n", c[i][0], c[i][1]);

    }

    //Diretivas GLUT
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);                   //cor do circulo
    glBegin(GL_POLYGON);
        for (int i = 0; i < n; i++)
        {
            glVertex2f(c[i][0], c[i][1]);       //desenhar o circulo
        }
    
    glEnd();
    glFlush(); 

}
void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);       //Square color
    glBegin(GL_POLYGON);            //Square size and position
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.7, 0.0, 0.0);
        glVertex3f(0.7, 0.7, 0.0);
        glVertex3f(0.0, 0.7, 0.0);
    glEnd();
    glFlush();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(600, 600);       //Window Size
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world");
    glClearColor(1.0, 1.0, 0.0, 0.0);   //Background color
    glutDisplayFunc(desenhar_circunferencia);
    glutMainLoop();
    return 0;
}