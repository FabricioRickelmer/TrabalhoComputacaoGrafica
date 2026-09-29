#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Variáveis de Animação e Interação
float posicaoAviaoZ = -10.0f; 
float anguloHelice = 0.0f;   
float fatorEscala = 1.0f;    
float velocidade = 0.05f;    
int pausado = 0;             

// Posições das nuvens na cena
float nuvensX[] = {-4.0f, 3.5f, -1.0f, -3.0f, 4.2f};
float nuvensY[] = {2.0f, 1.5f, -1.8f, -0.8f, 1.0f};
float nuvensZ[] = {-5.0f, -9.0f, -14.0f, -18.0f, -23.0f};

// Controlo da Câmara 3D
float camX = 0.0f, camY = 0.5f, camZ = 6.0f; 
float yaw = -90.0f;                          
float pitch = -5.0f;                         
float velocidadeCamara = 0.3f;               

// ID da Textura
GLuint IDTexturaMetal;

// --- FUNÇÃO PARA CARREGAR IMAGEM BMP (24-bits) ---
GLuint carregaBMP(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Aviso: Nao foi possivel abrir a textura '%s'. O aviao sera renderizado sem textura.\n", filename);
        return 0;
    }

    unsigned char header[54];
    if (fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M') {
        printf("Erro: Arquivo '%s' nao eh um BMP valido.\n", filename);
        fclose(file);
        return 0;
    }

    int width = *(int*)&(header[18]);
    int height = *(int*)&(header[22]);
    int imageSize = *(int*)&(header[34]);
    if (imageSize == 0) imageSize = width * height * 3;

    unsigned char* data = (unsigned char*)malloc(imageSize);
    fread(data, 1, imageSize, file);
    fclose(file);

    // O formato BMP salva cores em BGR, invertemos para RGB
    for (int i = 0; i < imageSize; i += 3) {
        unsigned char temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Parâmetros de filtragem da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);

    return textureID;
}

void init(void) {
    glEnable(GL_DEPTH_TEST); 
    glClearColor(0.52f, 0.80f, 0.92f, 1.0f); 

    // Carrega a textura da pasta assets
    IDTexturaMetal = carregaBMP("assets/textures/metal.bmp");
}

void desenhaNuvem(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 1.0f);

    glutSolidSphere(0.7f, 12, 12);
    
    glPushMatrix();
    glTranslatef(0.6f, -0.1f, 0.0f);
    glutSolidSphere(0.5f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.6f, -0.1f, 0.0f);
    glutSolidSphere(0.5f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.3f, -0.2f);
    glutSolidSphere(0.45f, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

// Desenha um paralelepípedo mapeando coordenadas UV de textura
void desenhaCaixaTexturizada(float largura, float altura, float profundidade) {
    float x = largura / 2.0f;
    float y = altura / 2.0f;
    float z = profundidade / 2.0f;

    glBegin(GL_QUADS);
    // Face Frontal
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);

    // Face Traseira
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);

    // Face Superior
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-x,  y,  z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( x,  y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y, -z);

    // Face Inferior
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);

    // Face Direita
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( x,  y, -z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y,  z);

    // Face Esquerda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);
    glEnd();
}

// --- MODELO MELHORADO DO AVIÃO ---
void desenhaAviao(void) {
    glPushMatrix();
    
    glTranslatef(0.0f, 0.0f, posicaoAviaoZ);
    glScalef(fatorEscala, fatorEscala, fatorEscala);

    // Ativa mapeamento de textura caso exista
    if (IDTexturaMetal != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, IDTexturaMetal);
        glColor3f(1.0f, 1.0f, 1.0f); 
    } else {
        glColor3f(0.75f, 0.75f, 0.80f); 
    }

    // 1. Fuselagem Aerodinâmica
    glPushMatrix();
    glScalef(0.5f, 0.5f, 2.2f);
    desenhaCaixaTexturizada(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // 2. Cabine do Piloto (Cockpit em Vidro Escuro)
    glDisable(GL_TEXTURE_2D); // Sem textura no vidro
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, 0.2f);
    glColor3f(0.1f, 0.2f, 0.3f); 
    glScalef(0.38f, 0.25f, 0.7f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Reativa textura para as asas
    if (IDTexturaMetal != 0) glEnable(GL_TEXTURE_2D);

    // 3. Asas Principais (Estilo Asas em V)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.2f);
    glScalef(3.2f, 0.06f, 0.7f);
    desenhaCaixaTexturizada(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // 4. Asas Traseiras (Estabilizadores Horizontais)
    glPushMatrix();
    glTranslatef(0.0f, 0.05f, -0.9f);
    glScalef(1.4f, 0.05f, 0.4f);
    desenhaCaixaTexturizada(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    // 5. Estabilizador Vertical (Leme Traseiro)
    glPushMatrix();
    glTranslatef(0.0f, 0.45f, -0.95f);
    glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);
    glScalef(0.06f, 0.55f, 0.4f);
    desenhaCaixaTexturizada(1.0f, 1.0f, 1.0f);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // 6. Bico / Cone Frontal
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.10f);
    glColor3f(0.85f, 0.15f, 0.15f); 
    glutSolidCone(0.25f, 0.45f, 12, 12);
    glPopMatrix();

    // 7. Conjunto da Hélice Tripla
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.45f);
    glRotatef(anguloHelice, 0.0f, 0.0f, 1.0f); 
    glColor3f(0.15f, 0.15f, 0.15f);

    // Pá 1
    glPushMatrix();
    glScalef(1.1f, 0.1f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pá 2
    glPushMatrix();
    glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1.1f, 0.1f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Pá 3
    glPushMatrix();
    glRotatef(240.0f, 0.0f, 0.0f, 1.0f);
    glScalef(1.1f, 0.1f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix(); // Fim Hélice

    glPopMatrix(); // Fim Avião
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 16.0 / 9.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float dirX = cos(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);
    float dirY = sin(pitch * M_PI / 180.0f);
    float dirZ = sin(yaw * M_PI / 180.0f) * cos(pitch * M_PI / 180.0f);

    gluLookAt(camX, camY, camZ,
              camX + dirX, camY + dirY, camZ + dirZ,
              0.0f, 1.0f, 0.0f);

    for (int i = 0; i < 5; i++) {
        desenhaNuvem(nuvensX[i], nuvensY[i], nuvensZ[i]);
    }

    desenhaAviao();

    glutSwapBuffers();
}

void atualizaCena(int value) {
    if (!pausado) {
        posicaoAviaoZ += velocidade;
        
        if (posicaoAviaoZ > 10.0f) {
            posicaoAviaoZ = -25.0f;
        }

        anguloHelice += 30.0f;
        if (anguloHelice > 360.0f) anguloHelice -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, atualizaCena, 0); 
}

void teclado(unsigned char key, int x, int y) {
    float radYaw = yaw * M_PI / 180.0f;
    float dirX = cos(radYaw);
    float dirZ = sin(radYaw);

    switch (key) {
        case 'w': case 'W':
            camX += dirX * velocidadeCamara;
            camZ += dirZ * velocidadeCamara;
            break;
        case 's': case 'S':
            camX -= dirX * velocidadeCamara;
            camZ -= dirZ * velocidadeCamara;
            break;
        case 'a': case 'A':
            camX += dirZ * velocidadeCamara;
            camZ -= dirX * velocidadeCamara;
            break;
        case 'd': case 'D':
            camX -= dirZ * velocidadeCamara;
            camZ += dirX * velocidadeCamara;
            break;
        case 'q': case 'Q':
            camY += velocidadeCamara;
            break;
        case 'z': case 'Z':
            camY -= velocidadeCamara;
            break;
        case 'r': case 'R':
            camX = 0.0f; camY = 0.5f; camZ = 6.0f;
            yaw = -90.0f; pitch = -5.0f;
            break;

        case 'p': case 'P':
            pausado = !pausado;
            break;
        case '+': case '=':
            velocidade += 0.01f;
            break;
        case '-': case '_':
            if (velocidade > 0.01f) velocidade -= 0.01f;
            break;
        case 'e': case 'E':
            fatorEscala = (fatorEscala == 1.0f) ? 1.3f : 1.0f;
            break;
        case 27: 
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void teclasEspeciais(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            yaw -= 3.0f;
            break;
        case GLUT_KEY_RIGHT:
            yaw += 3.0f;
            break;
        case GLUT_KEY_UP:
            pitch += 3.0f;
            if (pitch > 89.0f) pitch = 89.0f;
            break;
        case GLUT_KEY_DOWN:
            pitch -= 3.0f;
            if (pitch < -89.0f) pitch = -89.0f;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("Trabalho PDI - Aviao com Textura");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclasEspeciais);
    glutTimerFunc(0, atualizaCena, 0);

    glutMainLoop();
    return 0;
}