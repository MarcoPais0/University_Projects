/*____________________________________________
    FCTUC
    Marco Pais Nº2019218981
    Meta 2 do Projeto de Computação Gráfica
______________________________________________*/

#ifndef MATERIAIS_H
#define MATERIAIS_H

#include <GL\Glut.h>

GLfloat blackPlasticAmb[] = { 0.0 ,0.0 ,0.0 };
GLfloat blackPlasticDif[] = { 0.01 ,0.01 ,0.01 };
GLfloat blackPlasticSpec[] = { 0.01 ,0.01 ,0.01 };
GLint blackPlasticCoef = 32.0;

GLfloat  redPlasticAmb[] = { 0.3 ,0.0 ,0.0 };
GLfloat  redPlasticDif[] = { 0.5 ,0.0 ,0.0 };
GLfloat  redPlasticSpec[] = { 0.7 ,0.6 ,0.6 };
GLint  redPlasticCoef = 32.0;

GLfloat whitePlasticAmb[] = { 0.5 ,0.5 ,0.5 };
GLfloat whitePlasticDif[] = { 0.55 ,0.55 ,0.55 };
GLfloat whitePlasticSpec[] = { 0.70 ,0.70 ,0.70 };
GLint whitePlasticCoef = 200.0;

#endif
