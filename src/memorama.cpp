#include <raylib.h>
<<<<<<< HEAD
#include <string.h>
#include <iostream>

#define MAX 10
int id=1;
int renglon;


char nom[MAX+1];

struct Usuarios{
    char nombre[MAX+1];
    char password[MAX+1];
    int id;
    int puntos;
};
Usuarios usuario = {0};
typedef struct{
    const char *texto;
    Rectangle barrera;
    bool activo;
} CuadroOpcion;

typedef enum {
    ELECCION,
    REGISTRARSE,
    INICIAR_SESION
} Opcion;
Opcion opcionActual = ELECCION;
void registro(Usuarios *usuario);
void inicioSesion(void);

=======
#include <iostream>

>>>>>>> c68d2efdaaa2ccd1221ad2bb01dd7d8acc96e433
//--------------------------VARIABLES GLOBALES Y CONSTANTES-----------------
#define IMG 15//cantidad de imagenes
#define SND 10//cantidad de sonidos
#define COLUMNAS 6//para tablero
#define FILAS 4//para tablero
<<<<<<< HEAD
#define TAM_IMG 110//En realidad es más grande que la imagen, para que quede separación entre cartas
#define ESCALA_IMG .2f//Pixeles que se desean entre los originales
=======
#define TAM_IMG 200//En realidad es más grande que la imagen, para que quede separación entre cartas
#define ESCALA_IMG .36f//Pixeles que se desean entre los originales
>>>>>>> c68d2efdaaa2ccd1221ad2bb01dd7d8acc96e433
const int CANT_PARES = (FILAS*COLUMNAS)/2;//Los pares de cartas que se necesitan

Sound sonido[SND];
Texture2D imagenes[IMG];
<<<<<<< HEAD
const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 680;
=======
const int SCREEN_WIDTH = 1620;
const int SCREEN_HEIGHT = 900;
>>>>>>> c68d2efdaaa2ccd1221ad2bb01dd7d8acc96e433

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
<<<<<<< HEAD
Estado estadoActual = MENU;
=======

>>>>>>> c68d2efdaaa2ccd1221ad2bb01dd7d8acc96e433
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

<<<<<<< HEAD
    
=======
    Estado estadoActual = MENU;
>>>>>>> c68d2efdaaa2ccd1221ad2bb01dd7d8acc96e433

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
<<<<<<< HEAD
            if (opcionActual == ELECCION){
                dibujarCargar();
            } else if (opcionActual == REGISTRARSE){
                 registro(&usuario);
                opcionActual = ELECCION;
            } else if (opcionActual == INICIAR_SESION){
                inicioSesion();
                opcionActual = ELECCION;
            }
=======
            dibujarCargar();
>>>>>>> c68d2efdaaa2ccd1221ad2bb01dd7d8acc96e433
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
<<<<<<< HEAD
   
        FILE *archJugadores;
        id=0;
        CuadroOpcion cuadroOpcion[] = {
            {"Registrarse", {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, 200, 20}, false},
            {"Iniciar Sesion", {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 60, 200, 20}, false},
        };
        int tamEl = sizeof(cuadroOpcion) / sizeof(cuadroOpcion[0]);

        
         ClearBackground(RAYWHITE);


            if(opcionActual == ELECCION){
                DrawText("ELIGE ", 300, 20, 40, RED);
                Vector2 mouse = GetMousePosition();

                for (int i = 0; i < tamEl; i++){
                    cuadroOpcion[i].activo = CheckCollisionPointRec(mouse, cuadroOpcion[i].barrera);
                    DrawRectangleRec(cuadroOpcion[i].barrera, cuadroOpcion[i].activo ? SKYBLUE : LIGHTGRAY);
                    DrawText(cuadroOpcion[i].texto, cuadroOpcion[i].barrera.x + 10, cuadroOpcion[i].barrera.y + 2, 18, BLACK);
                }

                for (int i = 0; i < tamEl; i++){
                    if(cuadroOpcion[i].activo && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                            if(i == 0){
                                opcionActual = REGISTRARSE;
                            }else if (i == 1){
                                opcionActual = INICIAR_SESION;
                            }
                            
                        }
                }          
            }
            else if(opcionActual == REGISTRARSE){
               
            } else if(opcionActual == INICIAR_SESION){
                
            }

            
    
}

bool IsAnyKeyPressed(){
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)){
        keyPressed = true;
    } 

    return keyPressed;
}


void registro(Usuarios *usuario){
    memset(usuario, 0, sizeof(Usuarios));
    FILE *archJugadores;

    Usuarios temp;
    char nomTemp[MAX+1];
    bool existe = false, verificado = false, nomExistente = false, terminado = false;


    archJugadores = fopen("jugadores.dat", "rb+");

    if(archJugadores == NULL){
        archJugadores = fopen("jugadores.dat", "wb+");
    }
    bool datosGuardados = false;


    rewind(archJugadores);
        while (fread(&temp, sizeof(Usuarios), 1, archJugadores)) {
            if (temp.id >= id) {
                id = temp.id + 1;
            }
        }

    const int screenWidth = 800;
    const int screenHeight = 450;

    
 
    int letras = 0;

    Rectangle textBox = { screenWidth/2.0f - 100, 180, 250, 50 };
    bool mouseOnText = false;

    int framesCounter = 0;
    bool ingresoNombre = true;

    
    while (!terminado){    
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText){
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0){
                if ((key >= 32) && (key <= 125) && (letras < MAX)){
                    if(ingresoNombre){
                        usuario->nombre[letras] = (char)key;
                        usuario->nombre[letras+1] = '\0'; 
                    }
                    else{
                         usuario->password[letras] = (char)key;
                        usuario->password[letras + 1] = '\0';
                    }
                     letras++;
                }

                key = GetCharPressed(); 
            }

            if(IsKeyPressed(KEY_BACKSPACE)){
                letras--;
                if (letras < 0) letras = 0;
                if(ingresoNombre){
                    usuario->nombre[letras] = '\0';
                }
                else{
                    usuario->password[letras] = '\0';
                }
                
            }

            if(IsKeyPressed(KEY_ENTER)){
                if (letras > 0 && ingresoNombre) {
                    letras = 0;

                    existe = false;

                    rewind(archJugadores);
                    while(fread(&temp, sizeof(Usuarios), 1, archJugadores)){
                    if(strcmp(temp.nombre, usuario->nombre) == 0){
                        existe = true;
                        break;
                    }
                }
                verificado = true;

                if(!existe){
                    ingresoNombre = false;
                    nomExistente = false;
                    letras = 0;
                }
                else{
                    nomExistente = true;
                }
                }
            }

        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);


      if (!ingresoNombre && IsKeyPressed(KEY_ENTER) && letras > 0) {
        usuario->id = id++;
        //usuario->renglon = renglon++;
        fseek(archJugadores, 0, SEEK_END);
        fwrite(usuario, sizeof(Usuarios), 1, archJugadores);
        datosGuardados = true;
        terminado = true;
        
    }



        if (mouseOnText) framesCounter++;
        else framesCounter = 0;
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("REGISTRO", 300, 20, 40, RED);

            DrawText("COLOCA EL MOUSE EN LA CAJA!", 240, 90, 20, GRAY);

            if(ingresoNombre){
               // DrawText("Ingresa tu nombre de usuario y presiona ENTER", 300, 180, 30, DARKGRAY);
                DrawText("Nombre: ", 330, 135, 40, BLACK);
            }
            else{
                    DrawText("Contraseña: ", 280, 135, 40, BLACK);
                
            }
            

            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            if(ingresoNombre){
                DrawText(usuario->nombre, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
            }
            else{
                DrawText(usuario->password, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
            }

            if(nomExistente){
                DrawText("Este nombre ya está registrado. Intenta con otro!", 150, 400, 20, RED);
            }

            DrawText(TextFormat("CARACTERES DISPONIBLES: %i/%i", letras, MAX), 300, 250, 15, DARKGRAY);

            if (mouseOnText){
                if(ingresoNombre){
                    if (letras < MAX){
                        if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(usuario->nombre, 40), (int)textBox.y + 12, 40, MAROON);
                     }
                    else DrawText("Presiona BACKSPACE para borrar...", 230, 300, 20, GRAY);
                } 
                else{
                  if (letras < MAX){
                      if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(usuario->password, 40), (int)textBox.y + 12, 40, MAROON);
                       }
                  else DrawText("Presiona BACKSPACE para borrar...", 230, 300, 20, GRAY);
                  }
            }
                

        if(datosGuardados){
            strcpy(nom, usuario->nombre);
            terminado = true;
        }

        EndDrawing();
        
    }

     fclose(archJugadores);

    //CloseWindow();        

}


void inicioSesion(){
    FILE *archJugadores;
    
     Usuarios temp;
    char nomTemp[MAX+1];
    bool existe = false, verificado = false, nomExistente = false, usuarioExiste = false, terminado = false;


    archJugadores = fopen("jugadores.dat", "rb");

    if(archJugadores == NULL){
        //archJugadores = fopen("jugadores.dat", "wb+");
    }
    bool datosGuardados = false;


    char nomSesion[MAX+1] = {0};
    char contSesion[MAX+1] = {0};

    const int screenWidth = 800;
    const int screenHeight = 450;

    //InitWindow(screenWidth, screenHeight, "raylib - inicio sesion");
 
    int letras = 0;

    Rectangle textBox = { screenWidth/2.0f - 100, 180, 250, 50 };
    bool mouseOnText = false;

    int framesCounter = 0;
    bool ingresoNombre = true;
    bool error = false;
            

    
    while (!terminado){    
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText){
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0){
                if ((key >= 32) && (key <= 125) && (letras < MAX)){
                    if(ingresoNombre){
                        nomSesion[letras] = (char)key;
                        nomSesion[letras+1] = '\0'; 
                    }
                    else{
                        contSesion[letras] = (char)key;
                        contSesion[letras + 1] = '\0';
                    }
                     letras++;
                }

                key = GetCharPressed(); 
            }

            if(IsKeyPressed(KEY_BACKSPACE)){
                letras--;
                if (letras < 0) letras = 0;
                if(ingresoNombre){
                    nomSesion[letras] = '\0';
                }
                else{
                    contSesion[letras] = '\0';
                }
                
            }

            if(IsKeyPressed(KEY_ENTER)){
                if (letras > 0 && ingresoNombre) {
                    letras = 0;
                    ingresoNombre = false;
                }
                else{
                    if(letras > 0 && !ingresoNombre){
                        rewind(archJugadores);
                        while(fread(&temp, sizeof(Usuarios), 1, archJugadores)){
                            if(strcmp(temp.nombre, nomSesion) == 0 && strcmp(temp.password, contSesion) == 0){
                                usuarioExiste = true;
                                break;
                            }
                        }
                    }
                    if (usuarioExiste) {
                        fclose(archJugadores);
                        strcpy(nom, nomSesion);
                        terminado = true;
                    } 
                    else {
                        error = true;
                        ingresoNombre = true;
                        letras = 0;
                     }
                }

            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText) framesCounter++;
        else framesCounter = 0;
        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("INICIAR SESION", 200, 20, 40, RED);

            DrawText("COLOCA EL MOUSE EN LA CAJA!", 240, 90, 20, GRAY);

            if(ingresoNombre){
               // DrawText("Ingresa tu nombre de usuario y presiona ENTER", 300, 180, 30, DARKGRAY);
                DrawText("Nombre: ", 330, 135, 40, BLACK);
            }
            else{
                    DrawText("Contraseña: ", 280, 135, 40, BLACK);
                
            }
            

            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            if(ingresoNombre){
                DrawText(nomSesion, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
            }
            else{
                DrawText(contSesion, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
            }

            if(error){
                DrawText("Usuario o contraseña incorrecta!", 150, 400, 20, RED);
            }

            DrawText(TextFormat("CARACTERES DISPONIBLES: %i/%i", letras, MAX), 300, 250, 15, DARKGRAY);

            if (mouseOnText){
                if(ingresoNombre){
                    if (letras < MAX){
                        if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(nomSesion, 40), (int)textBox.y + 12, 40, MAROON);
                     }
                    else DrawText("Presiona BACKSPACE para borrar...", 230, 300, 20, GRAY);
                } 
                else{
                  if (letras < MAX){
                      if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(contSesion, 40), (int)textBox.y + 12, 40, MAROON);
                       }
                  else DrawText("Presiona BACKSPACE para borrar...", 230, 300, 20, GRAY);
                  }
            }
             

        EndDrawing();
        
        
    }

     fclose(archJugadores);

    //CloseWindow();        
}

=======
    DrawText("CARGAR",350,280,60,BLACK);
}
>>>>>>> c68d2efdaaa2ccd1221ad2bb01dd7d8acc96e433
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
<<<<<<< HEAD
                mostarCarta(tablero[i][j],i,j,35,0.21f);
            }
            else if (i!=y || j!=x) {//para evitar dibujar empalmado
                mostarCarta(tablero[i][j],i,j,50,ESCALA_IMG);
=======
                mostarCarta(tablero[i][j],i,j,90,0.4f);
            }
            else if (i!=y || j!=x) {//para evitar dibujar empalmado
                mostarCarta(tablero[i][j],i,j,100,ESCALA_IMG);
>>>>>>> c68d2efdaaa2ccd1221ad2bb01dd7d8acc96e433
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