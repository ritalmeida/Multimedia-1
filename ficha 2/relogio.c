#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/**************************************
************* CONSTANTE PI ************
**************************************/

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

/**************************************
* AUXILIARES CONVERSÃO GRAUS-RADIANOS *
**************************************/

#define rtd(x) (180 * (x) / M_PI)
#define dtr(x) (M_PI * (x) / 180)

#define DEBUG 1

/**************************************
********** VARIÁVEIS GLOBAIS **********
**************************************/

typedef struct
{
  GLboolean doubleBuffer;
  GLint delay;
} Estado;

typedef struct
{
  float x, y;
} Ponto;

typedef struct
{
  GLint hor, min, seg;
} Horas;

typedef struct
{
  GLint numLados;
  GLfloat raio;
  GLfloat tamLado;
  Ponto pontos[60];
  Horas hora;
} Modelo;

Estado estado;
Modelo modelo;

/**************************************
*** INICIALIZAÇÃO DO AMBIENTE OPENGL **
**************************************/

void init(void)
{

  struct tm *current_time;
  time_t timer = time(0);

  /* Delay para o timer */
  estado.delay = 1000;

  modelo.tamLado = 1;
  modelo.numLados = 60;
  modelo.raio = 0.75;

  /* Ler hora do Sistema */
  current_time = localtime(&timer);
  modelo.hora.hor = current_time->tm_hour;
  modelo.hora.min = current_time->tm_min;
  modelo.hora.seg = current_time->tm_sec;

  glClearColor(0.0, 0.0, 0.0, 0.0);

  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
}

void poligono(GLint n, GLfloat x0, GLfloat y0, GLfloat r)
{
  float t = 0, alpha = 2 * M_PI / n;
  for (int i = 0; i < n; i++)
  {
    modelo.pontos[i].x = r * cos(t) + x0;
    modelo.pontos[i].y = r * sin(t) + y0;
    t += alpha;
    //printf("%f %f\n", modelo.pontos[i].x, modelo.pontos[i].y);
  }

  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_POLYGON);
  for (int i = 0; i < n; i++)
  {
    glVertex2f(modelo.pontos[i].x, modelo.pontos[i].y);
  }
  glEnd();
  glFlush();
}

// Desenhar marcas das horas e minutos
void mostrador() {

  glColor3f(0.0, 0.0, 0.0);
  glLineWidth(0.5);
  glBegin(GL_LINES);

  for (int i = 0; i < 60; i++)
  {
    if (i % 5 == 0)
    {
      glVertex2f(modelo.pontos[i].x, modelo.pontos[i].y);
      glVertex2f(modelo.pontos[i].x * 0.9, modelo.pontos[i].y * 0.9);
    }
    else
    {
      glVertex2f(modelo.pontos[i].x, modelo.pontos[i].y);
      glVertex2f(modelo.pontos[i].x * 0.95, modelo.pontos[i].y * 0.95);
    }
  }
  glEnd();
}

void ponteiros() {

int h, m, s;

  s = 15 - modelo.hora.seg;     //seconds calculation
  if (s < 0) 
  {
    s = 75 - modelo.hora.seg;
  }

  m = 15 - modelo.hora.min;     //minutes calculation
  if (m < 0) 
  {
    m = 75 - modelo.hora.min;
  }

  if(modelo.hora.hor>=12)       //hours calculation 
    h = (modelo.hora.hor - 12) * 5;
  else
    h = modelo.hora.hor * 5;

  if(h == 0){
    h+=15;
  }
  else if(h == 5)
  {
    h+=5;
  }
  else if(h == 10)
  {
    h-=5;
  }
  else if(h == 15)
  {
    h-=15;
  }
  else if(h == 20)
  {
    h+=35;
  }
  else if(h == 25)
  {
    h+=25;
  }
  else if(h == 30)
  {
    h+=15;
  }
  else if(h == 35)
  {
    h+=5;
  }
  else if(h == 40)
  {
    h-=5;
  }
  else if(h == 45)
  {
    h-=15;
  }
  else if(h == 50)
  {
    h-=25;
  }
  else if(h == 55)
  {
    h-=35;
  }


  glColor3f(1.0, 0.0, 0.0);

  //Hours
  glLineWidth(8);
  glBegin(GL_LINES);
  glVertex2f(0, 0);
  glVertex2f(modelo.pontos[h].x * 0.4, modelo.pontos[h].y * 0.4);
  glEnd();

  //Minutes
  glLineWidth(5);
  glBegin(GL_LINES);
  glVertex2f(0, 0);
  glVertex2f(modelo.pontos[m].x * 0.6, modelo.pontos[m].y * 0.6);
  glEnd();

  //Seconds
  glLineWidth(2);
  glBegin(GL_LINES);
  glVertex2f(0, 0);
  glVertex2f(modelo.pontos[s].x * 0.7, modelo.pontos[s].y * 0.7);
  glEnd();
  printf("%d hora %d min %d sec \n", modelo.hora.hor, modelo.hora.min, modelo.hora.seg);

}

/*  exercício 1.5
 *  desenhar poligno regular de n lados, com centro no ponto de 
 *  coordenadas (x0,y0) e distancia aos vértices de r
 */
void poligno_alt(GLint n, GLfloat x0, GLfloat y0, GLfloat r) {

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);     //preto e opaco
  glClear(GL_COLOR_BUFFER_BIT);

  GLfloat aux = 2.0f * M_PI;

  glBegin(GL_POLYGON);
  glColor3f(1.0, 1.0, 1.0);         //branco
    glVertex2f(x0, y0);
    for (int i = 0; i <= n; i++) {
      
      glVertex2f(r * cosf(i*aux/n), r * sinf(i*aux/n));
    }
  glEnd();

  glFlush();

}

/*void desenhar_circulo() {

  GLint n = 50;
  GLfloat x0 = 0.0f, y0 = 0.0f, r = 0.9f;
  poligno(n, x0, y0, r);

  mostrador();

  ponteiros();
}*/

/**************************************
***** CALL BACKS DE JANELA/DESENHO ****
**************************************/

/* Callback para redimensionar janela */
void reshape(int width, int height)
{
  GLint size;

  if (width < height)
    size = width;
  else
    size = height;

  /* glViewport(botom, left, width, height)
     Define parte da janela a ser utilizada pelo OpenGL */
  glViewport(0, 0, (GLint)size, (GLint)size);

  /* Matriz Projeção
     Matriz onde se define como o mundo e apresentado na janela */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /* gluOrtho(left,right,bottom,top,near,far);
     Projeção ortogonal 3D, com profundidade (Z) entre -1 e 1 */
  glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);

  /* Matriz Modelview
     Matriz onde são realizadas as tranformações dos modelos desenhados */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/**************************************
** ESPAÇO PARA DEFINIÇÃO DAS ROTINAS **
****** AUXILIARES DE DESENHO ... ******
**************************************/


/* Callback de desenho */
void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  
  /* Espaço para chamada das rotinas auxiliares de desenho ... */


  glBegin(GL_POLYGON);
    /*glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(modelo.tamLado / 2, modelo.tamLado / 2);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(-modelo.tamLado / 2, modelo.tamLado / 2);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-modelo.tamLado / 2, -modelo.tamLado / 2);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex2f(modelo.tamLado / 2, -modelo.tamLado / 2);*/
    poligono(60, 0, 0, modelo.raio);
    mostrador();
    ponteiros();
  glEnd();

  glFlush();

  if (estado.doubleBuffer)
    glutSwapBuffers();
}


/**************************************
******** CALLBACKS TIME/IDLE **********
**************************************/

/* Callback Idle */
void idle(void)
{
  /* Ações a tomar quando o GLUT está idle */

  /* Redesenhar o ecrã */
  // glutPostRedisplay();
}

/* Callback de temporizador */
void timer(int value)
{
  glutTimerFunc(estado.delay, timer, 0);
  /* Acções do temporizador ...
     Não colocar aqui primitivas OpenGL de desenho glBegin, glEnd, etc.
     Simplesmente alterar os valores de modelo.hora.hor, modelo.hora.min e modelo.hora.seg */

  modelo.hora.seg++;
  if (modelo.hora.seg > 59) {   //quando faz 60 segundos

    modelo.hora.seg = 0;
    modelo.hora.min++;
    if (modelo.hora.min > 59) { //quando passam os 60 minutos

      modelo.hora.min = 0;
      modelo.hora.hor++;
      if (modelo.hora.hor > 23) {   //24horas

        modelo.hora.hor = 0;
      } 
  }
  
  
  /* Redesenhar o ecrã (invoca o callback de desenho) */
  glutPostRedisplay();
  }
}

/**************************************
*********** FUNÇÃO AJUDA **************
**************************************/

void imprime_ajuda(void)
{
  printf("\n\nDesenho de um quadrado\n");
  printf("h,H - Ajuda \n");
  printf("+   - Aumentar tamanho do Lado\n");
  printf("-   - Diminuir tamanho do Lado\n");
  printf("ESC - Sair\n");
}

/**************************************
********* CALLBACKS TECLADO ***********
**************************************/

/* Callback para interação via teclado (carregar na tecla) */
void key(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27: // Tecla Escape
    exit(1);
    /* Ações sobre outras teclas */

  case 'h':
  case 'H':
    imprime_ajuda();
    break;

  case '+':
    if (modelo.tamLado < 1.8)
    {
      modelo.tamLado += 0.05;
      glutPostRedisplay(); 
    }
    break;
  case '-':
    if (modelo.tamLado > 0.2)
    {
      modelo.tamLado -= 0.05;
      glutPostRedisplay(); 
    }
    break;
  }

  if (DEBUG)
    printf("Carregou na tecla %c\n", key);
}

/* Callback para interação via teclado (largar a tecla) */
void keyUp(unsigned char key, int x, int y)
{
  if (DEBUG)
    printf("Largou a tecla %c\n", key);
}

/* Callback para interacção via teclas especiais (carregar na tecla) */
void specialKey(int key, int x, int y)
{
  /* Ações sobre outras teclas especiais
      GLUT_KEY_F1 ... GLUT_KEY_F12
      GLUT_KEY_UP
      GLUT_KEY_DOWN
      GLUT_KEY_LEFT
      GLUT_KEY_RIGHT
      GLUT_KEY_PAGE_UP
      GLUT_KEY_PAGE_DOWN
      GLUT_KEY_HOME
      GLUT_KEY_END
      GLUT_KEY_INSERT */

  switch (key)
  {
    /* Redesenhar o ecrã */
    //glutPostRedisplay();
  }

  if (DEBUG)
    printf("Carregou na tecla especial %d\n", key);
}

/* Callback para interação via teclas especiais (largar a tecla) */
void specialKeyUp(int key, int x, int y)
{

  if (DEBUG)
    printf("Largou a tecla especial %d\n", key);
}


/**************************************
************ FUNÇÃO MAIN **************
**************************************/

int main(int argc, char **argv) {


  estado.doubleBuffer = GL_FALSE; // Colocar GL_TRUE para ligar o Double Buffer

  glutInit(&argc, argv);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(300, 300);
  glutInitDisplayMode(((estado.doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE) | GLUT_RGB);
  if (glutCreateWindow("Relógio") == GL_FALSE)
    exit(1);

  init();

  imprime_ajuda();

  /* Registar callbacks do GLUT */

  /* callbacks de janelas/desenho */
  glutReshapeFunc(reshape);
  glutDisplayFunc(draw);

  //glutDisplayFunc(desenhar_circulo);

  /* Callbacks de teclado */
  glutKeyboardFunc(key);
  //glutKeyboardUpFunc(keyUp);
  //glutSpecialFunc(specialKey);
  //glutSpecialUpFunc(specialKeyUp);

  /* Callbacks timer/idle */
  //glutTimerFunc(estado.delay, timer, 0);
  //glutIdleFunc(idle);

  /* Começar loop */
  glutMainLoop();

  return 0;
}
