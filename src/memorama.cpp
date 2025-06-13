#include <raylib.h>
#include <iostream>

//--------------------------VARIABLES GLOBALES Y CONSTANTES-----------------
#define IMG 15//cantidad de imagenes
#define SND 10//cantidad de sonidos
#define COLUMNAS 6//para tablero
#define FILAS 4//para tablero
#define TAM_IMG 200//En realidad es más grande que la imagen, para que quede separación entre cartas
#define ESCALA_IMG .36f//Pixeles que se desean entre los originales
const int CANT_PARES = (FILAS*COLUMNAS)/2;//Los pares de cartas que se necesitan

Sound sonido[SND];
Texture2D imagenes[IMG];
const int SCREEN_WIDTH = 1620;
const int SCREEN_HEIGHT = 900;

//--------------------------ESTRUCTURAS------------------------------------
typedef struct {
    const char *text;
    Rectangle bounds;
    bool active;
} ElementoMenu;

struct CartasEnTablero {//para asignar las cartas
    int id,disponible;
};

struct Juego {//reprresenta una carta en el tablero
    int id_Carta;
    bool estado;
};

//-------------------------ENUMERACIONES-----------------------------------
typedef enum {
    MENU,
    JUGAR,
    CARGAR,
    RANKING,
    OPCIONES,
    MENUJ,
    FACIL,
    DIFICIL
} Estado;

//--------------------------PROTOTIPOS FUNCIONES----------------------------------------
void dibujarFondoDegradado(Color topColor, Color bottomColor);

//Cargar recursos y liberarlos
void cargarTexturas();
void cargarSonidos();
void liberarTextura();
void liberarSonidos();

//Funciones para dibujar las diferentes pantallas
void dibujarMenu(ElementoMenu *, int);
//void dibujarJugar();
void dibujarCargar();
void dibujarRanking();
void dibujarOpciones();

//Funciones para el funcionamiento del tablero
void inicializarCartas(CartasEnTablero*);//Para asignar las cartas en aleatorio
int IdCarta(CartasEnTablero*);//Verificar si ese id aleatorio no se repita mas de una vez
void llenarTab(Juego[][COLUMNAS], CartasEnTablero*);//asignar el id en el tablero
void mostrarMat(Juego[][COLUMNAS],int,int);//mostrar la matriz y al usuario
void mostarCarta(Juego,int,int,int,float);
void voltearCarta(Juego[][COLUMNAS],int,int,int&,int*);
void evaluarPar(Juego[][COLUMNAS],int*,int&,int&,int,int);

//-----------------------------------MAIN--------------------------------------------
int main(void)
{
    InitAudioDevice();//cosas de sonido
    int x=0,y=0,volteadas=0,cordsArriba1[2],pares=0;//contador para el movimiento en la matriz X y Y
    Juego tablero[FILAS][COLUMNAS];
    CartasEnTablero cartas[CANT_PARES];
    srand(time(0));
    inicializarCartas(cartas);
    llenarTab(tablero, cartas);



    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Memoria");
    cargarSonidos();
    cargarTexturas();

    ElementoMenu elementosMenu[] = {
        {"Jugar",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 -100, 400, 40},false},
        {"Cargar",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 40, 400, 40},false},
        {"Ranking",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 20, 400, 40},false},
        {"Opciones",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 80, 400, 40},false},
        {"Salir",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 140, 400, 40},false},
    };
    ElementoMenu elementosMenuJuego[] = {
        {"Facil",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 -100, 400, 40},false},
        {"Dififcil",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 40, 400, 40},false},
        {"Salir",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 20, 400, 40},false},
    };

    int elementosTam = sizeof(elementosMenu) / sizeof(elementosMenu[0]);
    int elementosTamJuego = sizeof(elementosMenuJuego) / sizeof(elementosMenuJuego[0]);

    Estado estadoActual = MENU;

    SetTargetFPS(60);

    //Mientras la ventana no se intente cerrar, se hará lo que se encuentre dentro del while
    while (!WindowShouldClose()) {

        Vector2 mouse = GetMousePosition();

        if (estadoActual == MENU) {
            for (int i = 0; i < elementosTam; i++) {
                if(CheckCollisionPointRec(mouse, elementosMenu[i].bounds)) {
                    elementosMenu[i].active = true;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        if (i == 0) {
                            estadoActual = JUGAR;
                        } else if (i == 1) {
                            estadoActual = CARGAR;
                        } else if (i == 2) {
                            estadoActual = RANKING;
                        } else if (i == 3) {
                            estadoActual = OPCIONES;
                        } else if (i == 4) {
                            liberarTextura();
                            liberarSonidos();
                            CloseWindow();
                        }
                    }
                } else {
                    elementosMenu[i].active = false;
                }
            }
        }
        else if (estadoActual == MENUJ) {
            for (int i = 0; i < elementosTam; i++) {
                if(CheckCollisionPointRec(mouse, elementosMenuJuego[i].bounds)) {
                    elementosMenuJuego[i].active = true;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        if (i == 0) {
                            estadoActual = FACIL;
                        } else if (i == 1) {
                            estadoActual = DIFICIL;
                        } else if (i == 2) {
                            estadoActual = MENU;
                        }
                    }
                } else {
                    elementosMenuJuego[i].active = false;
                }
            }
        }
        else if (estadoActual == FACIL) {
            if ((IsKeyPressed(KEY_W)|| IsKeyPressed(KEY_UP)) && y>0 ) {//en caso de que suba por el tablero
                y=y-1;
            }
            else if ((IsKeyPressed(KEY_S)|| IsKeyPressed(KEY_DOWN)) && y<FILAS-1 ) {//en caso de que baje por el tablero
                y=y+1;
            }
            if ((IsKeyPressed(KEY_A)|| IsKeyPressed(KEY_LEFT)) && x>0 ) {//en caso de que  valla a la izquierda
                x=x-1;
            }
            else if ((IsKeyPressed(KEY_D)|| IsKeyPressed(KEY_RIGHT)) && x<COLUMNAS-1) {//en caso de que  valla a la derecha
                x=x+1;
            }
        }

        BeginDrawing();
        dibujarFondoDegradado(BLUE, PINK);

        if (IsKeyPressed(KEY_ENTER) && tablero[y][x].estado==false && volteadas<2 && estadoActual==FACIL) {
            voltearCarta(tablero, x, y,volteadas,cordsArriba1);
            evaluarPar(tablero,cordsArriba1,volteadas,pares,x,y);
        }

        if (estadoActual == MENU) {
            dibujarMenu(elementosMenu, elementosTam);
        } else if (estadoActual == JUGAR) {
            estadoActual=MENUJ;
        } else if (estadoActual == CARGAR) {
            dibujarCargar();
        } else if (estadoActual == RANKING) {
            dibujarRanking();
        } else if (estadoActual == OPCIONES) {
            dibujarOpciones();
        }else if (estadoActual == MENUJ) {
            dibujarMenu(elementosMenuJuego, elementosTamJuego);
        }else if (estadoActual == DIFICIL) {
            DrawText("DIFICIL",350,280,60,BLACK);
        }
        if(estadoActual==FACIL) {
            mostrarMat(tablero, x, y);//mostrar la matriz/tablero y al usuario

            if (pares == CANT_PARES) {
                EndDrawing();
                WaitTime(1.0);
                BeginDrawing();
                dibujarFondoDegradado(BLUE, PINK);
                DrawText("Ganaste", 100, 50, 30, BLUE);
                EndDrawing();
            }
        }
        EndDrawing();

    }
    liberarTextura();
    liberarSonidos();
    CloseWindow();
    return 0;
}

//-------------------------------FUNCIONES------------------------------------------

void dibujarFondoDegradado(Color topColor, Color bottomColor) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        float factor = (float)y / (float)SCREEN_HEIGHT;
        Color color = {
            (unsigned char)(topColor.r + (bottomColor.r - topColor.r) * factor),
            (unsigned char)(topColor.g + (bottomColor.g - topColor.g) * factor),
            (unsigned char)(topColor.b + (bottomColor.b - topColor.b) * factor),
            255
        };
        DrawRectangle(0,y,SCREEN_WIDTH,1,color);
    }
}
//Recursos
void cargarTexturas() {
    imagenes[0] = LoadTexture("../imagenes/Reverse.png");
    imagenes[1] = LoadTexture("../imagenes/amatist.png");
    imagenes[2] = LoadTexture("../imagenes/connie.png");
    imagenes[3] = LoadTexture("../imagenes/garnet.png");
    imagenes[4] = LoadTexture("../imagenes/gref.png");
    imagenes[5] = LoadTexture("../imagenes/lars.png");
    imagenes[6] = LoadTexture("../imagenes/lazu.png");
    imagenes[7] = LoadTexture("../imagenes/pearl.png");
    imagenes[8] = LoadTexture("../imagenes/peri.png");
    imagenes[9] = LoadTexture("../imagenes/rosa.png");
    imagenes[10] = LoadTexture("../imagenes/rubi.png");
    imagenes[11] = LoadTexture("../imagenes/sar.png");
    imagenes[12] = LoadTexture("../imagenes/steven.png");
    imagenes[13] = LoadTexture("../imagenes/sug.png");
    imagenes[14] = LoadTexture("../imagenes/zafiro.png");
}
void cargarSonidos() {
    sonido[0]=LoadSound("../sonidos/error.mp3");
    sonido[1]=LoadSound("../sonidos/flip.mp3");
    sonido[2]=LoadSound("../sonidos/correcto.mp3");
}
void liberarTextura() {
    for (int i = 0; i <IMG; i++) {
        UnloadTexture(imagenes[i]);
    }
}
void liberarSonidos() {
    for (int i = 0; i <SND; i++) {
        UnloadSound(sonido[i]);
    }
}

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //
// Funciones para dibujar los diferentes estados del memorama //

void dibujarMenu(ElementoMenu *elementosMenu, int elementosTam) {
    for (int i = 0; i < elementosTam; i++) {
        if (elementosMenu[i].active) {
            DrawRectangleRec(elementosMenu[i].bounds,PINK);
        } else {
            DrawRectangleRec(elementosMenu[i].bounds,BLUE);
        }
        DrawText(elementosMenu[i].text,elementosMenu[i].bounds.x+20, elementosMenu[i].bounds.y+20,20,BLACK);
    }
}

void dibujarCargar() {
    DrawText("CARGAR",350,280,60,BLACK);
}
void dibujarRanking() {
    DrawText("RANKING",350,280,60,BLACK);
}
void dibujarOpciones() {
    DrawText("OPCIONES",350,280,60,BLACK);
}

//Funciones para el tablero de memorama--------------------------------
void inicializarCartas(CartasEnTablero*cartas) {
    for (int i = 0; i < (CANT_PARES); i++) {
        cartas[i].id = i;
        cartas[i].disponible = 2;
    }
}
int IdCarta(CartasEnTablero*cartas) {
    int id=0,contrl=0;
    do {
        id=rand()%(CANT_PARES);
        if(cartas[id].disponible>0) {
            cartas[id].disponible-=1;
            contrl=1;
        }
        else {
            contrl=0;
        }
    }while(contrl!=1);
    return id;
}
void llenarTab(Juego tablero[][COLUMNAS], CartasEnTablero cartas[CANT_PARES]) {
    for (int i = 0; i <FILAS; i++) {
        for (int j = 0; j <COLUMNAS; j++) {
            tablero[i][j].id_Carta =IdCarta(cartas);
            tablero[i][j].estado = false;//Volteado
        }
    }
}
void mostrarMat(Juego tablero[][COLUMNAS],int x,int y) {//mostrar matriz y al usuario
    // Dibujar la matriz
    for (int i = 0; i <FILAS ; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i==y && j==x) {//lo que se muestra en la posición del usuario
                mostarCarta(tablero[i][j],i,j,90,0.4f);
            }
            else if (i!=y || j!=x) {//para evitar dibujar empalmado
                mostarCarta(tablero[i][j],i,j,100,ESCALA_IMG);
            }
        }
    }
}
void mostarCarta(Juego tablero, int i, int j,int tamEspacio,float escala) {
    if (tablero.estado == false) {
        DrawTextureEx(imagenes[0], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
    }
    else {
        switch (tablero.id_Carta) {
            case 0: {
                DrawTextureEx(imagenes[1], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 1: {
                DrawTextureEx(imagenes[2], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 2: {
                DrawTextureEx(imagenes[3], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 3: {
                DrawTextureEx(imagenes[4], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 4: {
                DrawTextureEx(imagenes[5], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 5: {
                DrawTextureEx(imagenes[6], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 6: {
                DrawTextureEx(imagenes[7], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 7: {
                DrawTextureEx(imagenes[8], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 8: {
                DrawTextureEx(imagenes[9], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 9: {
                DrawTextureEx(imagenes[10], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 10: {
                DrawTextureEx(imagenes[11], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 11: {
                DrawTextureEx(imagenes[12], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 12: {
                DrawTextureEx(imagenes[13], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }
            case 13: {
                DrawTextureEx(imagenes[14], (Vector2){(float)tamEspacio+j*TAM_IMG,(float)tamEspacio+i*TAM_IMG},0,escala,WHITE);
                break;
            }

        }
    }
}
void voltearCarta(Juego tablero[FILAS][COLUMNAS], int x, int y, int &volteadas, int cordArriba1[2]) {
    PlaySound(sonido[1]);
    if (volteadas == 1) {
        tablero[y][x].estado= true;
        volteadas++;
    }
    else if (volteadas == 0) {
        tablero[y][x].estado= true;
        cordArriba1[0] = x;//coordenada en x de la primer carta volteada
        cordArriba1[1] = y;//coordenada en y de la primer carta volteada
        volteadas++;
    }

}
void evaluarPar(Juego tablero[FILAS][COLUMNAS], int cordsArriba1[2], int &volteadas,int &pares,int x,int y) {
    if (volteadas==2) {
        volteadas=0;
        if (tablero[y][x].id_Carta==tablero[cordsArriba1[1]][cordsArriba1[0]].id_Carta) {
            WaitTime(.5);
            PlaySound(sonido[2]);
            mostrarMat(tablero, x, y);
            pares++;
        }
        else {
            mostrarMat(tablero, x, y);
            WaitTime(1);
            PlaySound(sonido[0]);
            tablero[y][x].estado=false;
            tablero[cordsArriba1[1]][cordsArriba1[0]].estado=false;
        }
    }
}