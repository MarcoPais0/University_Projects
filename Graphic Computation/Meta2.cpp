/*____________________________________________
    FCTUC
    Marco Pais Nº2019218981
    Meta 2 do Projeto de Computação Gráfica
______________________________________________*/

#include "Meta2.h"
#include "RgbImage.h"

void initMaterials(int material);

extern GLint whitePlasticCoef;

void desenhaTexto(char* string, GLfloat x, GLfloat y, GLfloat z) {
    glRasterPos3f(x, y, z);
    while (*string)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
}

void initLights(void) {
    //_______________Ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);

    //_______________Pontual
    glLightfv(GL_LIGHT0, GL_POSITION, PontualPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, PontualCorAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, PontualCorDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, PontualCorEsp);

    //__________________________________Foco
    glLightfv(GL_LIGHT1, GL_POSITION, focoPos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, focoColor);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, focoCA);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, focoLA);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, focoQA);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, focoAperture);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, focoDirection);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, focoExp);
}

void initTextures()
{
    //______________________________________Teclado
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    image.LoadBmpFile("keyboard.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        image.GetNumCols(),
        image.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        image.ImageData());

    //______________________________________Mesa
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    image.LoadBmpFile("chao.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        image.GetNumCols(),
        image.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        image.ImageData());

    //_____________________________________Ecra
    glGenTextures(1, &texture[2]);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    image.LoadBmpFile("ecra.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        image.GetNumCols(),
        image.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        image.ImageData());

    //_____________________________________Plastico
    glGenTextures(1, &texture[3]);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    image.LoadBmpFile("plastic.bmp");
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, 3,
        image.GetNumCols(),
        image.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
        image.ImageData());
}

void init(void) {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_VERTEX_ARRAY);

    glNormalPointer(GL_FLOAT, 0, normais);
    glEnableClientState(GL_NORMAL_ARRAY);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    initLights();

    glTexCoordPointer(2, GL_FLOAT, 0, textures);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    initTextures();
}

void drawEixos() {
    //____________________________________Eixo X
    initMaterials(0);
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(10, 0, 0);
    glEnd();
    //____________________________________Eixo Y
    initMaterials(1);
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 10, 0);
    glEnd();
    //____________________________________Eixo Z
    initMaterials(2);
    glBegin(GL_LINES);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 0, 10);
    glEnd();
}

void drawCube() {
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
}

void drawCube1() {
    glPushMatrix();
    glTranslatef(-tam, 0, 0);
    glRotatef(180, 0, 0, 1);
    glTranslatef(tam, 0, 0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(tam, 0, 0);
    glRotatef(180, 0, 0, 1);
    glTranslatef(-tam, 0, 0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, tam, 0);
    glRotatef(180, 0, 1, 0);
    glTranslatef(0, -tam, 0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -tam, 0);
    glRotatef(180, 1, 0, 0);
    glRotatef(180, 1, 0, 1);
    glTranslatef(0, tam, 0);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
    glPopMatrix();
    glPushMatrix();
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 0, -tam);
    glRotatef(180, 1, 0, 0);
    glTranslatef(0, 0, tam);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
    glPopMatrix();
}

void drawTeclado() {
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
    glDisable(GL_TEXTURE_2D);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
}

void iluminacao() {
    //_________________________________Pontual
    glLightfv(GL_LIGHT0, GL_POSITION, PontualPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, PontualCorAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, PontualCorDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, PontualCorEsp);
    if (ligaPontual)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);

    //__________________________________Foco
    glLightfv(GL_LIGHT1, GL_POSITION, focoPos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, focoColor);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, focoCA);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, focoLA);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, focoQA);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, focoAperture);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, focoDirection);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, focoExp);
    if (ligaFoco)
        glEnable(GL_LIGHT1);
    else
        glDisable(GL_LIGHT1);
}

void drawCaixa() {
    //______________________________________Caixa
    glPushMatrix();
    initMaterials(2);
    glTranslatef(4.0 * tam, -0.2 * tam * 2 + 0.01, 3.0 * tam);
    glScalef(5.0, 0.5, 5.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);

    if (malha) {
        glPushMatrix();// Direita
        glTranslatef(5, 0, -5);
        glScalef(5, 10, 5);
        glTranslatef(0, 1, 1);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, -1, -1);
        glNormal3f(-1, 0, 0);
        glBegin(GL_QUADS);
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++) {
                glTexCoord2f((float)j / dim, (float)i / dim);
                glVertex3d(0, (float)j / med_dim, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
                glVertex3d(0, (float)(j + 1) / med_dim, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
                glVertex3d(0, (float)(j + 1) / med_dim, (float)(i + 1) / med_dim);
                glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
                glVertex3d(0, (float)j / med_dim, (float)(i + 1) / med_dim);
            }
        glEnd();
        glPopMatrix();

        glPushMatrix();// Esquerda
        glTranslatef(-5, 0, -5);
        glScalef(5, 10, 5);
        glTranslatef(0, 1, 1);
        glRotatef(90, 1, 0, 0);
        glTranslatef(0, -1, -1);
        glNormal3f(1, 0, 0);
        glBegin(GL_QUADS);
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++) {
                glTexCoord2f((float)j / dim, (float)i / dim);
                glVertex3d(0, (float)j / med_dim, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
                glVertex3d(0, (float)(j + 1) / med_dim, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
                glVertex3d(0, (float)(j + 1) / med_dim, (float)(i + 1) / med_dim);
                glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
                glVertex3d(0, (float)j / med_dim, (float)(i + 1) / med_dim);
            }
        glEnd();
        glPopMatrix();

        glPushMatrix();// Frente
        glTranslatef(-5, 0, 5);
        glScalef(5, 10, 5);
        glNormal3f(0, 0, -1);
        glBegin(GL_QUADS);
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++) {
                glTexCoord2f((float)j / dim, (float)i / dim);
                glVertex3d((float)j / med_dim, (float)i / med_dim, 0);
                glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
                glVertex3d((float)(j + 1) / med_dim, (float)i / med_dim, 0);
                glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
                glVertex3d((float)(j + 1) / med_dim, (float)(i + 1) / med_dim, 0);
                glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
                glVertex3d((float)j / med_dim, (float)(i + 1) / med_dim, 0);
            }
        glEnd();
        glPopMatrix();

        glPushMatrix();// Tras
        glTranslatef(-5, 0, -5);
        glScalef(5, 10, 5);
        glNormal3f(0, 0, 1);
        glBegin(GL_QUADS);
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++) {
                glTexCoord2f((float)j / dim, (float)i / dim);
                glVertex3d((float)j / med_dim, (float)i / med_dim, 0);
                glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
                glVertex3d((float)(j + 1) / med_dim, (float)i / med_dim, 0);
                glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
                glVertex3d((float)(j + 1) / med_dim, (float)(i + 1) / med_dim, 0);
                glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
                glVertex3d((float)j / med_dim, (float)(i + 1) / med_dim, 0);
            }
        glEnd();
        glPopMatrix();

        glPushMatrix();// Baixo
        glTranslatef(-5, 0, -5);
        glScalef(5, 0, 5);
        glNormal3f(0, 1, 0);
        glBegin(GL_QUADS);
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++) {
                glTexCoord2f((float)j / dim, (float)i / dim);
                glVertex3d((float)j / med_dim, 0, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
                glVertex3d((float)(j + 1) / med_dim, 0, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
                glVertex3d((float)(j + 1) / med_dim, 0, (float)(i + 1) / med_dim);
                glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
                glVertex3d((float)j / med_dim, 0, (float)(i + 1) / med_dim);
            }
        glEnd();
        glPopMatrix();

        glPushMatrix();// Cima
        glTranslatef(-5, 20, -5);
        glScalef(5, 0, 5);
        glNormal3f(0, -1, 0);
        glBegin(GL_QUADS);
        for (int i = 0; i < dim; i++)
            for (int j = 0; j < dim; j++) {
                glTexCoord2f((float)j / dim, (float)i / dim);
                glVertex3d((float)j / med_dim, 0, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)i / dim);
                glVertex3d((float)(j + 1) / med_dim, 0, (float)i / med_dim);
                glTexCoord2f((float)(j + 1) / dim, (float)(i + 1) / dim);
                glVertex3d((float)(j + 1) / med_dim, 0, (float)(i + 1) / med_dim);
                glTexCoord2f((float)j / dim, (float)(i + 1) / dim);
                glVertex3d((float)j / med_dim, 0, (float)(i + 1) / med_dim);
            }
        glEnd();
        glPopMatrix();
    }
    else {
        glPushMatrix();
        glTranslatef(0, 10, 0);
        glScalef(10, 20, 10);
        drawCube1();
        glPopMatrix();
    }
    
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawScene() {
    glTranslatef(-4.0 * tam, 0.0, 0.0);

    //_______________________________________Fio
    initMaterials(0);
    glPushMatrix();
    glTranslatef(4.0 * tam, 8.425, 3.0 * tam);
    glScalef(0.3, 2.85, 0.3);
    drawCube();
    glPopMatrix();

    //_____________________________________Lampada
    glPushMatrix();
    if (ligaPontual || Dia) {
        glDisable(GL_LIGHTING);
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, cor_2);
    }
    else if (ligaFoco) {
        glDisable(GL_LIGHTING);
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, cor_1);
    }
    else
        initMaterials(0);
    glTranslatef(4.0 * tam, 7, 3.0 * tam);
    drawCube();
    if (ligaFoco || ligaPontual || Dia) {
        glEnable(GL_LIGHTING);
        glDisableClientState(GL_COLOR_ARRAY);
    }
    glPopMatrix();

    //_______________________________________Trackpad
    initMaterials(0);
    glPushMatrix();
    glTranslatef(1.5 * tam + (4.0 * tam - 1.5 * tam), -0.099 * tam, 0.9 * (3.0 * tam - 0.9 * tam) + 1.4);
    glScalef(1.5, 0.1, 0.9);
    drawCube();
    glPopMatrix();

    //___________________________Teclado
    initMaterials(1);
    glPushMatrix();
    glTranslatef(0, -depth_botao * tam, 0);
    glTranslatef(3.5 * tam + (4.0 * tam - 3.5 * tam), -0.02 * tam, 1.5 * tam + 1.5 * (3.0 * tam - 1.5 * tam) / 4);
    glScalef(3.5, 0.2, 1.5);
    drawTeclado();
    glPopMatrix();

    //_______________________Cubo de baixo
    initMaterials(2);
    glPushMatrix();
    if (trans) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    glTranslatef(4.0 * tam, -0.2 * tam, 3.0 * tam);
    glScalef(4.0, 0.2, 3.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    drawCube();
    glDisable(GL_TEXTURE_2D);
    if (trans) {
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
    glPopMatrix();

    //______________________________Logo
    initMaterials(1);
    glPushMatrix();
    glRotatef(ang_fechar, 1.0, 0.0, 0.0);
    glTranslatef(0.7 * tam + (4.0 * tam - 0.7 * tam), 0.7 * tam + (3.0 * tam - 0.7 * tam), -0.1 * tam - 0.001);
    glScalef(0.7, 0.7, 0.1);
    glRotatef(45, 0.0, 0.0, 1.0);
    drawCube();
    glPopMatrix();

    //_____________________________Ecra
    if (ecra) {
        initMaterials(2);
    }
    else {
        initMaterials(0);
    }
    glPushMatrix();
    glRotatef(ang_fechar, 1.0, 0.0, 0.0);
    glTranslatef(3.6 * tam + (4.0 * tam - 3.6 * tam), 2.6 * tam + (3.0 * tam - 2.6 * tam), -0.1 * tam + 0.001);
    glScalef(3.6, 2.6, 0.1);
    if (ecra) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[2]);
    }
    drawCube();
    if (ecra) {
        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();

    //_______________________Cubo de cima
    initMaterials(2);
    glPushMatrix();
    glRotatef(ang_fechar, 1.0, 0.0, 0.0);
    glTranslatef(4.0 * tam, 3.0 * tam, -0.1 * tam);
    glScalef(4.0, 3.0, 0.1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    drawCube();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void display(void) {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glViewport(0, hScreen/3, wScreen/3, hScreen/3);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-xC, xC, -xC, xC, -zC, zC);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 5, 0, 0, 0, 0, 0, 0, -1);

    //------------------------------------------------ Informacao
    glColor3f(1, 1, 1);			// nao se está a usar LUZ !!   DisableLight
    sprintf_s(texto, 30, "%d - Dia 'd' ", Dia);
    desenhaTexto(texto, -15, 1, -18);
    sprintf_s(texto, 30, "%d - Pontual 'p' ", ligaPontual);
    desenhaTexto(texto, -15, 1, -14);
    sprintf_s(texto, 30, "   %d%d%d - Luz 'RGB' ", PontualR, PontualG, PontualB);
    desenhaTexto(texto, -15, 1, -10);
    sprintf_s(texto, 30, "   %4.2f - Intensidade 'i'", intensidadePontual);
    desenhaTexto(texto, -15, 1, -6);
    sprintf_s(texto, 30, "%d - Foco 's'", ligaFoco);
    desenhaTexto(texto, -15, 1, -2);
    sprintf_s(texto, 30, "%d - Malha 'a'", malha);
    desenhaTexto(texto, -15, 1, 2);
    sprintf_s(texto, 30, "%d - Viewport 'v'", viewport);
    desenhaTexto(texto, -15, 1, 6);
    sprintf_s(texto, 30, "%d - Especularidade 'm'", whitePlasticCoef);
    desenhaTexto(texto, -15, 1, 10);
    sprintf_s(texto, 30, "%d - Transparencia 'w'", trans);
    desenhaTexto(texto, -15, 1, 14);
    glEnable(GL_LIGHTING);

    if (viewport) {
        //_________________________________________Viewport 1
        glViewport(0, hScreen / 4, 6 * (wScreen / 10), hScreen / 2);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3.0 * zC);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);
        //_____________________________Objectos
        iluminacao();
        //drawEixos();
        drawCaixa();
        drawScene();

        //_________________________________________Viewport 2
        glViewport(6 * (wScreen / 10), hScreen / 2, wScreen / 2, hScreen / 2);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-7, 11, -7, 7, -6, 6);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(-1, 5, 0, -1, 0, 0, 0, 0, -1);
        //_____________________________Objectos
        iluminacao();
        //drawEixos();
        drawScene();

        //_________________________________________Viewport 3
        glViewport(6 * (wScreen / 10), 0, wScreen / 2, hScreen / 2);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, (float)wScreen / hScreen, 0.1, 3.0 * zC);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(15, 3, 3, 0, 0, 0, 0, 1, 0);
        //_____________________________Objectos
        iluminacao();
        //drawEixos();
        drawScene();
    }
    else {
        //_________________________________________Viewport 1
        glViewport(0, 0, wScreen, hScreen);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3.0 * zC);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);
        //_____________________________Objectos
        iluminacao();
        //drawEixos();
        drawCaixa();
        drawScene();
    }

    //____________________________Atualizacao
    glutSwapBuffers();
}

void Timer(int value) {
    //______________________________Botao
    if (depth_botao >= 0.11) {
        botao = 0;
    }
    if (fechar == 1) {
        botao = 0;
        depth_botao = 0;
    }
    if (botao == 0 && depth_botao > 0) {
        depth_botao -= 0.01;
    }
    if (botao == 1 && depth_botao < 0.12) {
        depth_botao += 0.01;
    }
    if (ang_fechar > 85) {
        glTranslatef(0, -0.12 * tam, 0);
    }

    //___________________________________________Ecra
    if (fechar == 0 && ang_fechar != -20) {
        ang_fechar -= rotacao;
    }
    else if (fechar == 1 && ang_fechar != 90) {
        ang_fechar += rotacao;
    }

    glutPostRedisplay();
    glutTimerFunc(msec, Timer, 1);
}

void updateLuz() {
    PontualCorAmb[0] = PontualR * intensidadePontual;
    PontualCorAmb[1] = PontualG * intensidadePontual;
    PontualCorAmb[2] = PontualB * intensidadePontual;
    PontualCorDif[0] = PontualR * intensidadePontual;
    PontualCorDif[1] = PontualG * intensidadePontual;
    PontualCorDif[2] = PontualB * intensidadePontual;
    PontualCorEsp[0] = PontualR * intensidadePontual;
    PontualCorEsp[1] = PontualG * intensidadePontual;
    PontualCorEsp[2] = PontualB * intensidadePontual;;
    glLightfv(GL_LIGHT0, GL_AMBIENT, PontualCorAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, PontualCorDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, PontualCorEsp);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a': case 'A'://
        malha = !malha;
        break;
    case 'v': case 'V'://
        viewport = !viewport;
        break;
    case '-'://
        if (angZoom < 120)
            angZoom += 5;
        break;

    case '+'://
        if (angZoom > 15)
            angZoom -= 5;
        break;

    case'f': case'F'://
        fechar = !fechar;
        break;

    case'e': case'E'://
        botao = 1;
        break;

    case 'd': case 'D'://
        Dia = !Dia;
        if (Dia) {
            luzGlobalCorAmb[0] = 1.0;
            luzGlobalCorAmb[1] = 1.0,
            luzGlobalCorAmb[2] = 1.0;
        }
        else {
            luzGlobalCorAmb[0] = 0.0;
            luzGlobalCorAmb[1] = 0.0;
            luzGlobalCorAmb[2] = 0.0;
        }
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
        break;

    case 'p':case 'P'://
        ligaPontual = !ligaPontual;
        updateLuz();
        break;

    case 's': case 'S'://
        ligaFoco = !ligaFoco;
        updateLuz();
        break;

    case 'i': case 'I'://
        intensidadePontual = intensidadePontual + 0.1;
        if (intensidadePontual > 2) intensidadePontual = 0.2;
        updateLuz();
        break;

    case 'r':case 'R'://
        PontualR = (PontualR + 1) % 2;
        updateLuz();
        break;
    
    case 'g':case 'G'://
        PontualG = (PontualG + 1) % 2;
        updateLuz();
        break;
    
    case 'b':case 'B'://
        PontualB = (PontualB + 1) % 2;
        updateLuz();
        break;
    
    case 'w': case'W'://
        trans = !trans;
        break;

    case 'q':case 'Q'://
        ecra = !ecra;
        break;

    case 'm':case 'M'://
        if (whitePlasticCoef > 32) whitePlasticCoef = whitePlasticCoef - 10;
        else whitePlasticCoef = 200;
        break;

    case 27:
        //__________________________________________________Escape
        exit(0);
        break;
    }
}

void teclasNotAscii(int key, int x, int y) {
    if (key == GLUT_KEY_UP)
        obsP[1] = (obsP[1] + 0.1);

    if (key == GLUT_KEY_DOWN)
        obsP[1] = (obsP[1] - 0.1);

    if (key == GLUT_KEY_LEFT)
        aVisao = (aVisao + 0.05);

    if (key == GLUT_KEY_RIGHT)
        aVisao = (aVisao - 0.05);

    if (obsP[1] > yC)
        obsP[1] = yC;
    if (obsP[1] < -yC)
        obsP[1] = -yC;

    obsP[0] = rVisao * cos(aVisao);
    obsP[2] = rVisao * sin(aVisao);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(wScreen, hScreen);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Marco Pais - 2019218981     |Observador - 'SETAS'| |Zoom - '+/-'|  |Viewport - 'v'| |Fechar - 'f'| |Botao - 'e'| |Ecra - 'q'|");

    init();

    glutSpecialFunc(teclasNotAscii);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(msec, Timer, 1);

    glutMainLoop();

    return 0;
}
