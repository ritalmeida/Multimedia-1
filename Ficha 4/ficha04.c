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

#define RAD(x) (M_PI * (x) / 180)
#define GRAUS(x) (180 * (x) / M_PI)

/**************************************
******** CONSTANTES AUXILIARES ********
**************************************/

#define DEBUG 1
#define LADO_MAXIMO     2
#define LADO_MINIMO     0.3
#define DELTA_LADO      0.1

/**************************************
********** VARIÁVEIS GLOBAIS **********
**************************************/

typedef struct {
  GLboolean   doubleBuffer;
  GLint       delayMovimento;
  GLboolean   debug;
  GLboolean   movimentoTranslacao;      // se os cubinhos se movem
  GLboolean   movimentoRotacao;         // se o cubo grande roda;
}Estado;


typedef struct {
  GLfloat   theta[3];     // 0-Rotação em X; 1-Rotação em Y; 2-Rotação em Z
  
  GLint     eixoRodar;    // eixo que está a rodar (mudar com o rato)
  GLfloat   ladoCubo;     // comprimento do lado do cubo
  GLfloat   deltaRotacao; // incremento a fazer ao angulo quando roda
  GLboolean sentidoTranslacao; //sentido da translação dos cubos pequenos
  GLfloat   translacaoCubo; //
  GLfloat   deltaTranslacao; // incremento a fazer na translacao
  GLboolean sentidoRotacao;  //sentido da rotação dos cubos pequenos
  GLfloat   thetaCubo;     // Rotação dos cubinhos
}Modelo;

Estado estado;
Modelo modelo;
GLfloat aux;

/**************************************
*** INICIALIZAÇÃO DO AMBIENTE OPENGL **
**************************************/

void inicia_modelo()
{
  estado.delayMovimento=50;
  estado.movimentoTranslacao=GL_FALSE;
  estado.movimentoRotacao=GL_FALSE;

  modelo.theta[0]=0;
  modelo.theta[1]=0;
  modelo.theta[2]=0;
  modelo.eixoRodar=0;  // eixo de X;
  modelo.ladoCubo=1;
  modelo.deltaRotacao = 5;
  modelo.deltaTranslacao = -0.1;
  modelo.translacaoCubo = 0;
  modelo.sentidoTranslacao = GL_FALSE;
  modelo.sentidoRotacao = GL_TRUE;
  aux = modelo.thetaCubo;
}

void init(void)
{
  inicia_modelo();
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_DEPTH_TEST);

}

/**************************************
***** CALL BACKS DE JANELA/DESENHO ****
**************************************/

/* Callback para redimensionar janela */
void reshape(int width, int height)
{
  glViewport(0, 0, (GLint) width, (GLint) height);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (width < height)
     glOrtho(-5, 5, -5*(GLdouble)height/width, 5*(GLdouble)height/width,-10,10);
  else
     glOrtho(-5*(GLdouble)width/height, 5*(GLdouble)width/height,-5, 5, -10,10);

   
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

/**************************************
** ESPAÇO PARA DEFINIÇÃO DAS ROTINAS **
****** AUXILIARES DE DESENHO ... ******
**************************************/

void desenhaPoligono(GLfloat a[], GLfloat b[], GLfloat c[], GLfloat  d[], GLfloat cor[])
{
  /* draw a polygon via list of vertices */

  glBegin(GL_POLYGON);
    glColor3fv(cor);
    glVertex3fv(a);
    glVertex3fv(b);
    glVertex3fv(c);
    glVertex3fv(d);
  glEnd();
}

void cubo()
{
 GLfloat vertices[][3] = {{-0.5,-0.5,-0.5},   //bottom left vertex
                          {0.5,-0.5,-0.5},    //bottom right vertex
                          {0.5,0.5,-0.5},     //top right vertex
                          {-0.5,0.5,-0.5},    //top left vertex
                          
                          //os outros 4 vértices, os de trás

                          {-0.5,-0.5,0.5},    
                          {0.5,-0.5,0.5},
                          {0.5,0.5,0.5},
                          {-0.5,0.5,0.5},
                          };   

  GLfloat cores[][3] = {{0.0,1.0,1.0},        //azul claro
                        {1.0,0.0,0.0},        //vermelho
                        {1.0,1.0,0.0},        //amarelo
                        {0.0,1.0,0.0},        //verde
                        {1.0,0.0,1.0},        //rosa
                        {0.0,0.0,1.0},        //azul
                        {1.0,1.0,1.0}};       //branco

  /*desenhaPoligono(vertices[4],vertices[5],vertices[6],vertices[7],cores[2]);  //amarelo
  desenhaPoligono(vertices[0],vertices[1],vertices[2],vertices[3],cores[0]);  //azul claro
  desenhaPoligono(vertices[4],vertices[0],vertices[3],vertices[7],cores[4]);  //rosa
  desenhaPoligono(vertices[5],vertices[1],vertices[2],vertices[6],cores[3]);  //verde
  desenhaPoligono(vertices[0],vertices[4],vertices[5],vertices[1],cores[1]);  //vermelho
  desenhaPoligono(vertices[7],vertices[6],vertices[2],vertices[3],cores[5]);  //azul*/
	
    desenhaPoligono(vertices[0], vertices[1], vertices[2], vertices[3], cores[0]);
    desenhaPoligono(vertices[4], vertices[5], vertices[6], vertices[7], cores[1]);
    desenhaPoligono(vertices[1], vertices[2], vertices[6], vertices[5], cores[2]);
    desenhaPoligono(vertices[0], vertices[3], vertices[7], vertices[4], cores[3]);
    desenhaPoligono(vertices[2], vertices[3], vertices[7], vertices[6], cores[4]);
    desenhaPoligono(vertices[0], vertices[1], vertices[5], vertices[4], cores[5]);




  glPolygonMode(GL_BACK, GL_LINE);
  
}

void desenhaEixos(GLfloat a[], GLfloat b[], GLfloat c[], GLfloat d[])   //draw the axis via a list of vertexes
{
  glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3fv(a);
    glVertex3fv(b);

    glColor3f(0.0, 1.0, 0.0);
    glVertex3fv(a);
    glVertex3fv(c);

    glColor3f(0.0, 2.0, 1.0);
    glVertex3fv(a);
    glVertex3fv(d);
    glEnd();
}

void eixos()
{
  GLfloat vertices[][3] = { {0.0, 0.0, 0.0},
                            {2.0, 0.0, 0.0},
                            {0.0, 2.0, 0.0},
                            {0.0, 0.0, 2.0}};
  desenhaEixos(vertices[0], vertices[1], vertices[2], vertices[3]);
}

// ... Definição das rotinas auxiliares de desenho ...

/* Callback de desenho */
void draw(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ... chamada das rotinas auxiliares de desenho ...

  glPushMatrix();
    glRotatef(modelo.theta[0],1,0,0);                           //rotation in the x axis
    glRotatef(modelo.theta[1],0,1,0);                           //rotation in the y axis
    glRotatef(modelo.theta[2],0,0,1);                           //rotation in the z axis
    glScalef(modelo.ladoCubo,modelo.ladoCubo,modelo.ladoCubo);  //scaling the cube
    cubo();                                                     //drawing the cube
    eixos();                                                    //drawing the axis in the middle of the cube

    glPushMatrix();                                             //small cube in the X axis
      glTranslatef(2.0, 0.0, 0.0);                              //translating the drawing point to the tip of the x axis
      glScalef(0.5, 0.5, 0.5);                                  //scaling the small cube
      glTranslatef(modelo.translacaoCubo, 0.0, 0.0);            //moving the cube in the axis
      glRotatef(modelo.thetaCubo, modelo.sentidoRotacao, 0, 0); //rotating the cube in the axis
      cubo();                                                   //drawing the small cube
    glPopMatrix();
    
    glPushMatrix();                                             //small cube in the y axis
      glTranslatef(0.0, 2.0, 0.0);                              //translating the drawing point to the tip of the y axis
      glScalef(0.5, 0.5, 0.5);                                  //scaling the small cube
      glTranslatef(0.0, modelo.translacaoCubo, 0.0);            //moving the cube in the axis
      glRotatef(modelo.thetaCubo, 0, modelo.sentidoRotacao, 0); //rotating the cube in the axis
      cubo();                                                   //drawing the small cube
    glPopMatrix();

    glPushMatrix();                                             //small cube in the z axis 
      glTranslatef(0.0, 0.0, 2.0);                              //translating the drawing point to the tip of the y axis
      glScalef(0.5, 0.5, 0.5);                                  //scaling the small cube
      glTranslatef(0.0, 0.0, modelo.translacaoCubo);            //moving the cube in the axis
      glRotatef(modelo.thetaCubo, 0, 0, modelo.sentidoRotacao); //rotating the cube in the axis
      cubo();                                                   //drawing the small cube
    glPopMatrix();
       
  glPopMatrix();

  glLoadIdentity();
  glPushMatrix();                                               //draw the axis in the corner of the window
    glTranslatef(3.0, 3.0, 0.0);                                //translate to the corner
    glScalef(0.5, 0.5, 0.5);                                    //scaling the axis
    glRotatef(modelo.theta[0],1,0,0);                           //rotate in the x axis
    glRotatef(modelo.theta[1],0,1,0);                           //rotate in the y axis
    glRotatef(modelo.theta[2],0,0,1);                           //rotate in the z axis
    eixos();                                                    //draw the axis
  glPopMatrix();


  if (estado.doubleBuffer)
    glutSwapBuffers();
  else
    glFlush();
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
  glutTimerFunc(estado.delayMovimento, timer, 0);
  /* ... accoes do temporizador não colocar aqui transformações, alterar
     somente os valores das variáveis ... */

  // alterar o modelo.theta[] usando a variável modelo.eixoRodar como indice
  if (modelo.eixoRodar != -1)
  {
    modelo.theta[modelo.eixoRodar] += modelo.deltaRotacao;    //rotates the cube in the selected axis
  }

  /*moves the small cubes along their respective axis*/
  modelo.translacaoCubo = (modelo.translacaoCubo + modelo.deltaTranslacao); 

  /*checks the position of the small cubes along the axis*/
  if(modelo.translacaoCubo >= 0 || modelo.translacaoCubo <= -4 + modelo.ladoCubo + modelo.ladoCubo/2) {
    modelo.deltaTranslacao = -modelo.deltaTranslacao;     //inverts the direction of the small cubes' movement
    modelo.sentidoTranslacao = !modelo.sentidoTranslacao; //inverts the direction of the small cubes' movement
  }

  /*checks if the cubes are going towards or away from the big cube*/
  if(modelo.sentidoTranslacao != GL_TRUE)
    modelo.thetaCubo += modelo.deltaRotacao;              //else they continue rotating
  
  /* redesenhar o ecrã */
  glutPostRedisplay();
}


/**************************************
*********** FUNÇÃO AJUDA **************
**************************************/

void imprime_ajuda(void)
{
  printf("\n\nDesenho de um quadrado\n");
  printf("h,H - Ajuda \n");
  printf("F1  - Reiniciar \n");
  printf("F2  - Poligono Fill \n");
  printf("F3  - Poligono Line \n");
  printf("F4  - Poligono Point \n");
  printf("+   - Aumentar tamanho dos Cubos\n");
  printf("-   - Diminuir tamanho dos Cubos\n");
  printf("i,I - Reiniciar Variáveis\n");
  printf("p,p - Iniciar/Parar movimento dos cubinhos\n");
  printf("ESC - Sair\n");
  printf("teclas do rato para iniciar/parar rotação e alternar eixos\n");

}

/**************************************
********* CALLBACKS TECLADO ***********
**************************************/

/* Callback para interação via teclado (carregar na tecla) */
void key(unsigned char key, int x, int y)
{
  switch (key) {
    case 27:
      exit(1);
    /* ... accoes sobre outras teclas ... */

    case 'h' :
    case 'H' :
                imprime_ajuda();
            break;
    case '+':
            if(modelo.ladoCubo<LADO_MAXIMO)
            {
              modelo.ladoCubo+=DELTA_LADO;
              glutPostRedisplay();
            }
          break;

    case '-':
            if(modelo.ladoCubo>LADO_MINIMO)
            {
              modelo.ladoCubo-=DELTA_LADO;
              glutPostRedisplay();
            }
          break;

    case 'i' :
    case 'I' :
            inicia_modelo();
            glutPostRedisplay();
          break;
    case 'p' :
    case 'P' :
            estado.movimentoTranslacao=!estado.movimentoTranslacao;
          break;
    case '5':                     //rotation on the z axis
        if(modelo.eixoRodar == 1)
          modelo.eixoRodar = -1;
        else if(modelo.eixoRodar != 1)
          modelo.eixoRodar = 1;        
        break;
  }

  if(DEBUG)
    printf("Carregou na tecla %c\n",key);
}

/* Callback para interação via teclado (largar a tecla) */
void keyUp(unsigned char key, int x, int y)
{
  if(DEBUG)
    printf("Largou a tecla %c\n",key);
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
    case GLUT_KEY_F1 :
            inicia_modelo();
            glutPostRedisplay();
        break;

    case GLUT_KEY_F2 :
            inicia_modelo();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glutPostRedisplay();
        break;
    case GLUT_KEY_F3 :
            inicia_modelo();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glutPostRedisplay();
        break;
    case GLUT_KEY_F4 :
            inicia_modelo();
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            glutPostRedisplay();
        break;
  
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
*********** CALLBACKS RATO ************
**************************************/

void mouseMotion(int x, int y)
{
    /* x,y    => coordenadas do ponteiro quando se move no rato
                 a carregar em teclas
    */

  if(DEBUG)
    printf("Mouse Motion %d %d\n",x,y);

}

void mousePassiveMotion(int x, int y)
{
    /* x,y    => coordenadas do ponteiro quando se move no rato
                 sem estar a carregar em teclas
    */

  if(DEBUG)
    printf("Mouse Passive Motion %d %d\n",x,y);

}

void mouse(int button, int state, int x, int y)
{
  /* button => GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
     state  => GLUT_UP, GLUT_DOWN
     x,y    => coordenadas do ponteiro quando se carrega numa tecla do rato
  */

  // alterar o eixo que roda (variável modelo.eixoRodar)

  switch(button){
    case GLUT_LEFT_BUTTON :               //rotation in the x axis
      if(state == GLUT_DOWN)
      {       
        if(modelo.eixoRodar == 0) {
          modelo.eixoRodar = -1;
        }
        else if(modelo.eixoRodar != 0) {
          modelo.eixoRodar = 0;
        }  
      }
     break;
    /*case GLUT_MIDDLE_BUTTON:      
      if(state == GLUT_DOWN)
      {
        if(modelo.eixoRodar == 1)
          modelo.eixoRodar = -1;
        else if(modelo.eixoRodar != 1)
          modelo.eixoRodar = 1;        
      }
      break;*/
    case GLUT_RIGHT_BUTTON :              //rotation in the y axis
      if(state == GLUT_DOWN)
      {        
        if(modelo.eixoRodar == 2)
          modelo.eixoRodar = -1;
        else if(modelo.eixoRodar !=2)
          modelo.eixoRodar = 2;
      }
      break;
  }
  if(DEBUG)
    printf("Mouse button:%d state:%d coord:%d %d\n",button,state,x,y);
}

/**************************************
************ FUNÇÃO MAIN **************
**************************************/

int main(int argc, char **argv)
{
  estado.doubleBuffer = GL_TRUE;

  glutInit(&argc, argv);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(400,400);
  glutInitDisplayMode(((estado.doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE) | GLUT_RGB | GLUT_DEPTH) ;
  if (glutCreateWindow("Cubo") == GL_FALSE)
    exit(1);

  init();

  imprime_ajuda();

  /* Registar callbacks do GLUT */

  // Callbacks de janelas/desenho
  glutReshapeFunc(reshape);
  glutDisplayFunc(draw);

  // Callbacks de teclado
  glutKeyboardFunc(key);
  //glutKeyboardUpFunc(keyUp);
  glutSpecialFunc(specialKey);
  //glutSpecialUpFunc(SpecialKeyUp);

  // Callbacks do rato
  //glutPassiveMotionFunc(MousePassiveMotion);
  //glutMotionFunc(MouseMotion);
  glutMouseFunc(mouse);

  // Callbacks timer/idle
  glutTimerFunc(estado.delayMovimento, timer, 0);
  //glutIdleFunc(Idle);

  // Começar
  glutMainLoop();

  return 0;
}
