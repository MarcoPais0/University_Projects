/*____________________________________________
    FCTUC
    Marco Pais Nº2019218981
    Meta 2 do Projeto de Computação Gráfica
______________________________________________*/

#ifndef META2_H
#define META2_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL\glut.h>
#include "RgbImage.h"

#define PI 3.14159
#define WHITE 1.0, 1.0, 1.0, 1.0
#define GRAY 0.1, 0.1, 0.1, 1.0

//_______________________Variaveis e Constantes
float rotacao = 5;
GLint msec = 10;
int fechar = 0;
int ang_fechar = -20;
int botao = 0;
float depth_botao = 0;
int ecra = 1;
int viewport = 0;
int trans = 0;
int dim = 64, med_dim = dim / 2;
int malha = 0;
char     texto[30];

GLfloat tam = 0.5;

static GLfloat vertices[] = {
    //________________________________Esquerda
    -tam,    -tam,    tam, // 0 
    -tam,    tam,    tam, // 1 
    -tam,    tam,    -tam, // 2
    -tam,    -tam,    -tam, // 3 
    //________________________________Direita
    tam,    -tam,    tam, // 4 
    tam,    tam,    tam, // 5 
    tam,    tam,    -tam, // 6 
    tam,    -tam,    -tam, // 7 
    //________________________________Cima
    -tam,    tam,    tam, // 8 
    -tam,    tam,    -tam, // 9 
    tam,    tam,    -tam, // 10 
    tam,    tam,    tam, // 11 
    //________________________________Baixo
    tam,    -tam,    tam, // 12 
    -tam,    -tam,    tam, // 13 
    -tam,    -tam,    -tam, // 14
    tam,    -tam,    -tam, // 15
    //________________________________Frente
    tam,    tam,    tam, // 16
    -tam,    tam,    tam, // 17 
    -tam,    -tam,    tam, // 18
    tam,    -tam,    tam, // 19
    //________________________________Tras
    tam,    tam,    -tam, // 20
    tam,    -tam,    -tam, // 21 
    -tam,    -tam,    -tam, // 22
    -tam,    tam,    -tam // 23
};

static GLfloat normais[] = {
    //_________________________________Esquerda
    -1.0,    0.0,    0.0,
    -1.0,    0.0,    0.0,
    -1.0,    0.0,    0.0,
    -1.0,    0.0,    0.0,
    //_________________________________Direita
    1.0,    0.0,    0.0,
    1.0,    0.0,    0.0,
    1.0,    0.0,    0.0,
    1.0,    0.0,    0.0,
    //_________________________________Cima
    0.0,    1.0,    0.0,
    0.0,    1.0,    0.0,
    0.0,    1.0,    0.0,
    0.0,    1.0,    0.0,
    //_________________________________Baixo
    0.0,    -1.0,    0.0,
    0.0,    -1.0,    0.0,
    0.0,    -1.0,    0.0,
    0.0,    -1.0,    0.0,
    //_________________________________Frente
    0.0,    0.0,    1.0,
    0.0,    0.0,    1.0,
    0.0,    0.0,    1.0,
    0.0,    0.0,    1.0,
    //_________________________________Tras
    0.0,    0.0,    -1.0,
    0.0,    0.0,    -1.0,
    0.0,    0.0,    -1.0,
    0.0,    0.0,    -1.0
};

static GLfloat textures[] = {
    //______________________Esquerda 
         1,  0,		// 0
         1,  1,		// 1 
         0,  1,		// 2
         0,  0,		// 3
    //________________________Direita
         1,  0,		// 4 
         1,  1,		// 5 
         0,  1,		// 6
         0,  0,		// 7
    //_____________________Cima
         1,  0,		// 8 
         1,  1,		// 9 
         0,  1,		// 10
         0,  0,		// 11
    //_________________________Baixo
         1,  0,		// 12 
         1,  1,		// 13
         0,  1,		// 14
         0,  0,		// 15
    //_______________________Frente
         1,  1,		// 16
         0,  1,		// 17
         0,  0,		// 18
         1,  0,		// 19
    //__________________________Trás
         1,  0,	    // 20
         1,  1,		// 21
         0,  1,		// 22
         0,  0		// 23
};

static GLfloat cor_1[] = {
    //_________________________________Esquerda
    GRAY, // 0 
    GRAY, // 1 
    GRAY, // 2 
    GRAY, // 3 
    //_________________________________Direita
    GRAY, // 4
    GRAY, // 5
    GRAY, // 6
    GRAY, // 7
    //__________________________________Cima
    GRAY, // 8 
    GRAY, // 9 
    GRAY, // 10 
    GRAY, // 11
    //________________________________Baixo
    WHITE, // 12
    WHITE, // 13
    WHITE, // 14
    WHITE, // 15
    //__________________________________Frente
    GRAY, // 16
    GRAY, // 17
    GRAY, // 18 
    GRAY, // 19
    //________________________________Tras
    GRAY, // 20
    GRAY, // 21
    GRAY, // 22
    GRAY // 23
};

static GLfloat cor_2[] = {
    //_________________________________Esquerda
    WHITE, // 0 
    WHITE, // 1 
    WHITE, // 2 
    WHITE, // 3 
    //_________________________________Direita
    WHITE, // 4
    WHITE, // 5
    WHITE, // 6
    WHITE, // 7
    //__________________________________Cima
    WHITE, // 8 
    WHITE, // 9 
    WHITE, // 10 
    WHITE, // 11
    //________________________________Baixo
    WHITE, // 12
    WHITE, // 13
    WHITE, // 14
    WHITE, // 15
    //__________________________________Frente
    WHITE, // 16
    WHITE, // 17
    WHITE, // 18 
    WHITE, // 19
    //________________________________Tras
    WHITE, // 20
    WHITE, // 21
    WHITE, // 22
    WHITE // 23
};

static GLuint esquerda[] = { 0, 1, 2, 3 };
static GLuint direita[] = { 4, 7, 6, 5 };
static GLuint cima[] = { 8, 11, 10, 9 };
static GLuint baixo[] = { 12, 13, 14, 15 };
static GLuint frente[] = { 16, 17, 18, 19 };
static GLuint tras[] = { 20, 21, 22, 23 };

//_____________________________________________________________Sistema Coordenadas e Objectos
GLint wScreen = 1280, hScreen = 720; // Janela
GLfloat xC = 20.0, yC = 20.0, zC = 20.0; // Mundo

//____________________________________________________________________Observador 
GLfloat rVisao = 10, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat obsP[] = { rVisao * cos(aVisao), 6.0, rVisao * sin(aVisao) };
GLfloat angZoom = 90;
GLfloat incZoom = 3;

//_______________________________Texturas
RgbImage image;
GLuint texture[4];

//________________________Luz Ambiente
GLint   Dia = 1;     // 'D'  
GLfloat intensidadeDia = 1.0;
GLfloat luzGlobalCorAmb[4] = {intensidadeDia, intensidadeDia,intensidadeDia, 1.0};

//_______________________________Luz pontual (eixo Y)
GLint   ligaPontual = 1;		 // 'P'  
GLfloat intensidadePontual = 0.5;  // 'I'  
GLint   PontualR = 1;		 	 // 'R'  
GLint   PontualG = 1;			 // 'G'  
GLint   PontualB = 1;			 // 'B'  
GLfloat PontualPos[4] = {0.0, 10, 0, 1.0};
GLfloat PontualDir1[4] = { 0.0, 0, 2, 1.0 };
GLfloat PontualDir2[4] = { 0.0, 4, 2, 1.0 };
GLfloat PontualCorAmb[4] = {PontualR * intensidadePontual, PontualG * intensidadePontual, PontualB * intensidadePontual, 0.0};
GLfloat PontualCorDif[4] = {PontualR * intensidadePontual, PontualG * intensidadePontual, PontualB * intensidadePontual, 1.0};
GLfloat PontualCorEsp[4] = {PontualR * intensidadePontual, PontualG * intensidadePontual, PontualB * intensidadePontual, 1.0};

//________________________________________________Foco
GLint ligaFoco = 1;
GLfloat focoAperture = 60.0;
GLfloat focoPos[] = { 0, 10, 0, 1.0 };
GLfloat focoDirection[] = { 0.0, -1.0, 0.0, 0.0 };
GLfloat focoColor[] = { 100, 100, 100, 1.0 };
GLfloat focoCA = 1;
GLfloat focoLA = 0.5;
GLfloat focoQA = 0.2;
GLfloat focoExp = 2.0;

#endif
