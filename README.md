# Trabalho Prático: Avião 3D com Textura (OpenGL)



Este projeto é uma aplicação gráfica interativa desenvolvida em C++ utilizando a biblioteca OpenGL clássica e FreeGLUT. O objetivo é demonstrar a modelação de um avião tridimensional, a aplicação de texturas, a manipulação de câmaras virtuais e o uso de transformações geométricas contínuas (animação).

## Funcionalidades Implementadas

O projeto cumpre todos os requisitos exigidos para a avaliação:

* **Objetos Gráficos Compósitos:** Construção de um avião e nuvens utilizando primitivas geométricas (esferas, cubos, cones e polígonos em modo `GL_QUADS`).


* **Movimento e Animação:** Atualização contínua da cena simulando o voo do avião e a rotação contínua da hélice.


* **Translação:** Deslocamento constante do avião no eixo Z para simular o movimento para a frente.


* **Rotação:** Animação independente das três pás da hélice a girarem em torno do seu eixo.


* **Escala:** Alteração interativa do tamanho do avião através do teclado.


* **Texturização (Mapeamento UV):** Aplicação de uma imagem bitmap (`metal.bmp`) na fuselagem e nas asas do avião, processando a inversão de cores BGR para RGB.


* **Câmara Virtual 3D:** Implementação de uma câmara de navegação livre com cálculo vetorial de posição e orientação (Yaw e Pitch).


* **Projeção e Profundidade:** Utilização da projeção em perspetiva (`gluPerspective`) e teste de profundidade (Z-Buffer) para a correta oclusão visual dos elementos.



## Controlos da Aplicação

A interação com o ambiente virtual é efetuada através do teclado:

| Tecla | Ação |
| --- | --- |
| **W, A, S, D** | Movimentar a câmara (Frente, Esquerda, Trás, Direita) |
| **Q, Z** | Movimentar a câmara verticalmente no eixo Y (Cima, Baixo) |
| **Setas Direcionais** | Rodar a visão da câmara (Cima/Baixo para *Pitch*, Esquerda/Direita para *Yaw*) |
| **R** | Repor a câmara na posição e orientação iniciais |
| **P** | Pausar ou retomar a animação do avião |
| **+ / -** | Aumentar ou diminuir a velocidade de voo do avião |
| **E** | Alternar a escala (tamanho) do modelo do avião |
| **ESC** | Encerrar a aplicação de forma segura |

## Estrutura de Ficheiros

* `src/main.cpp`: Código-fonte principal que contém toda a lógica OpenGL, iluminação, câmara e geometria.


* `assets/textures/metal.bmp`: Ficheiro de textura utilizado para revestir o modelo do avião.


* `CMakeLists.txt`: Script responsável pela configuração e geração dos ficheiros de compilação.

## Como Compilar e Executar

Certifique-se de que possui o **CMake**, um compilador C++ (como o GCC/MinGW) e as bibliotecas **OpenGL** e **FreeGLUT** devidamente configurados no seu sistema.

Abra o terminal (PowerShell) na pasta raiz do projeto e execute os seguintes comandos pela ordem apresentada:

1. **Gerar os ficheiros de compilação:**
```powershell
cmake -B build -G "Ninja"

```


2. **Compilar o código:**
```powershell
cmake --build build

```


3. **Executar a aplicação:**
*(Deve ser sempre executado a partir da raiz do projeto para que a textura seja encontrada corretamente)*
```powershell
.\build\Aviao.exe

```