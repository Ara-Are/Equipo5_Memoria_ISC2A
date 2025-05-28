#include <raylib.h>

typedef struct {
    const char *text;
    Rectangle bounds;
    bool active;
} ElementoMenu;

typedef enum {
    MENU,
    JUGAR,
    CARGAR,
    RANKING,
    OPCIONES
} Estado;

const int SCREEN_WIDTH = 1620;
const int SCREEN_HEIGHT = 900;

void dibujarFondoDegradado(Color topColor, Color bottomColor);


//Funciones para dibujar las diferentes pantallas
void dibujarMenu(ElementoMenu *, int);
void dibujarJugar();
void dibujarCargar();
void dibujarRanking();
void dibujarOpciones();

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Memoria");

    ElementoMenu elementosMenu[] = {
        {"Jugar",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 -100, 400, 40},false},
        {"Cargar",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 40, 400, 40},false},
        {"Ranking",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 20, 400, 40},false},
        {"Opciones",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 80, 400, 40},false},
        {"Salir",{SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 140, 400, 40},false},
    };
    int elementosTam = sizeof(elementosMenu) / sizeof(elementosMenu[0]);

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
                            CloseWindow();
                        }
                    }
                } else {
                    elementosMenu[i].active = false;
                }
            }
        }

        BeginDrawing();
        dibujarFondoDegradado(BLUE, PINK);

        if (estadoActual == MENU) {
            dibujarMenu(elementosMenu, elementosTam);
        } else if (estadoActual == JUGAR) {
            dibujarJugar();
        } else if (estadoActual == CARGAR) {
            dibujarCargar();
        } else if (estadoActual == RANKING) {
            dibujarRanking();
        } else if (estadoActual == OPCIONES) {
            dibujarOpciones();
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

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

void dibujarJugar() {
    DrawText("JUEGO",350,280,60,BLACK);
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

// Funciones para dibujar los diferentes estados del memorama //
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = //