#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

// Variáveis de Animação e Interação
float posicaoAviaoZ = -10.0f; // Movimento de translação do avião (aproximação)
float anguloHelice = 0.0f;   // Rotação da hélice
float fatorEscala = 1.0f;    // Escala para efeito dinâmico
float velocidade = 0.05f;    // Velocidade do movimento
int pausado = 0;             // Controle de pausa

// Posições fixas de algumas nuvens na cena
float nuvensX[] = {-3.0f, 2.5f, 0.0f, -2.0f, 3.2f};
float nuvensY[] = {1.5f, 1.0f, -1.2f, -0.5f, 0.5f};
float nuvensZ[] = {-5.0f, -8.0f, -12.0f, -15.0f, -20.0f};

void init(void) {
    glEnable(GL_DEPTH_TEST); // Habilita o teste de profundidade (Essencial para 3D)
    glClearColor(0.52f, 0.80f, 0.92f, 1.0f); // Cor de fundo: Azul Céu
}

// Função para desenhar uma nuvem simples composta por esferas aglutinadas
void desenhaNuvem(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para as nuvens

    // Esferas formando a nuvem
    glutSolidSphere(0.6f, 10, 10);
    
    glPushMatrix();
    glTranslatef(0.5f, -0.1f, 0.0f);
    glutSolidSphere(0.45f, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5f, -0.1f, 0.0f);
    glutSolidSphere(0.45f, 10, 10);
    glPopMatrix();

    glPopMatrix();
}

// Função para desenhar o Avião
void desenhaAviao(void) {
    glPushMatrix();
    
    // Aplicação de Translação (O avião se desloca no eixo Z)
    glTranslatef(0.0f, 0.0f, posicaoAviaoZ);
    
    // Aplicação de Escala (O requisito de escala em ação)
    glScalef(fatorEscala, fatorEscala, fatorEscala);

    // 1. Corpo principal do avião (Fuselagem - Cilindro esticado via escala)
    glPushMatrix();
    glColor3f(0.85f, 0.85f, 0.90f); // Cinza claro metálico
    glScalef(0.4f, 0.4f, 1.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 2. Bico do avião (Cone na frente)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.8f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.80f, 0.20f, 0.20f); // Vermelho detalhe
    glutSolidCone(0.2f, 0.5f, 10, 10);
    glPopMatrix();

    // 3. Asas principais (Paralelepípedo achatado)
    glPushMatrix();
    glTranslatef(0.0f, -0.05f, 0.0f);
    glColor3f(0.90f, 0.90f, 0.95f);
    glScalef(2.2f, 0.05f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 4. Cauda do avião (Estabilizador vertical)
    glPushMatrix();
    glTranslatef(0.0f, 0.3f, -0.7f);
    glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.80f, 0.20f, 0.20f);
    glScalef(0.05f, 0.4f, 0.3f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 5. Hélice em rotação (Requisito de Rotação)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.05f);
    glRotatef(anguloHelice, 0.0f, 0.0f, 1.0f); // Gira em torno do próprio eixo Z
    glColor3f(0.1f, 0.1f, 0.1f);
    glScalef(0.8f, 0.1f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

void display(void) {
    // Limpa os buffers de cor e de profundidade
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Configuração da Projeção Perspectiva
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 1.0, 50.0);

    // Configuração da Câmera Virtual (gluLookAt)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Câmera posicionada um pouco acima e atrás, olhando para a origem da cena
    gluLookAt(0.0f, 1.0f, 4.0f,   // Posição da câmera (Eye)
              0.0f, 0.0f, -5.0f,  // Ponto para onde olha (Center)
              0.0f, 1.0f, 0.0f);  // Vetor "Up" (Orientação)

    // Renderiza o cenário de nuvens estáticas/flutuantes
    for (int i = 0; i < 5; i++) {
        desenhaNuvem(nuvensX[i], nuvensY[i], nuvensZ[i]);
    }

    // Renderiza o avião em movimento
    desenhaAviao();

    glutSwapBuffers();
}

// Função de atualização contínua (Animação)
void atualizaCena(int value) {
    if (!pausado) {
        // Atualiza a translação do avião (aproximando-se da câmera)
        posicaoAviaoZ += velocidade;
        
        // Se o avião passar da câmera, ele reinicia lá no fundo (loop infinito de voo)
        if (posicaoAviaoZ > 5.0f) {
            posicaoAviaoZ = -25.0f;
        }

        // Atualiza a rotação da hélice rapidamente
        anguloHelice += 25.0f;
        if (anguloHelice > 360.0f) anguloHelice -= 360.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, atualizaCena, 0); // ~60 FPS (16ms)
}

// Interação via Teclado
void teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'p':
        case 'P':
            pausado = !pausado; // Pausa ou resume a animação
            break;
        case '+':
        case '=':
            velocidade += 0.01f; // Aumenta a velocidade do voo
            break;
        case '-':
        case '_':
            if (velocidade > 0.01f) velocidade -= 0.01f; // Diminui a velocidade
            break;
        case 'e':
        case 'E':
            // Altera dinamicamente a escala para simular efeito visual
            fatorEscala = (fatorEscala == 1.0f) ? 1.3f : 1.0f;
            break;
        case 27: // Tecla ESC para sair
            exit(0);
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Trabalho CG - Aviao entre as Nuvens");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(teclado);
    glutTimerFunc(0, atualizaCena, 0);

    glutMainLoop();
    return 0;
}

