//---------------------------------LBRERIAS---------------------------------
#include <raylib.h>
#include <string.h>
#include <iostream>

//--------------------------VARIABLES GLOBALES Y CONSTANTES-----------------
#define MAX 10
#define IMG 15//cantidad de imagenes
#define SND 10//cantidad de sonidos
#define COLUMNAS 6//para tablero
#define FILAS 4//para tablero
#define TAM_IMG 500//Tamaño en pixeles de las cartas originalmnete
const int CANT_PARES = (FILAS*COLUMNAS)/2;//Los pares de cartas que se necesitan

// -- + -- + -- + -- + -- + -- + -- + -- + -- + -- + -- + -- + -- + -- //
// Funciones y structs para el uso de listas doblemente enlazadas

#define type int

typedef struct Element {
    type data;
    type x;
    type y;
    struct Element *left;
    struct Element *right;
} Nodo;

Nodo *ini = nullptr;
Nodo *end = nullptr;

Nodo *createNode(type, Nodo *, Nodo *,type,type);
bool isEmpty();
void displayListLTR();
void displayListRTL();
void insertIni(type,type,type);
void insertEnd(type,type,type);
void insertSort(type);
bool exist(type,type,type);
void borrar(type,type,type);
void freeList();
void nextTo(type,type);
void prevTo(type,type);
void compactList();
bool tieneUnElemento();


// Funciones y structs para el uso de listas doblemente enlazadas
// -- + -- + -- + -- + -- + -- + -- + -- + -- + -- + -- + -- + -- + -- //

Sound sonido[SND];
Texture2D imagenes[IMG];
int SCREEN_WIDTH = 1200;
int SCREEN_HEIGHT = 680;

int * semilla = (int*)malloc(CANT_PARES*sizeof(int));

int puntuacion=0;
int puntuacionMac=0;
int pares=0;
bool machine=false;

int id=1;
int renglon;
char nom[MAX+1];
int idActual = 0;

bool sesionIniciada = false;
bool aiuda=false;


//--------------------------ESTRUCTURAS------------------------------------
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

typedef enum {
    ELECCION,
    REGISTRARSE,
    INICIAR_SESION,
    CERRAR_SESION,
    VOLVER
} Opcion;

Estado estadoActual = MENU;
Opcion opcionActual = ELECCION;
//--------------------------PROTOTIPOS FUNCIONES----------------------------------------
void dibujarFondoDegradado(Color topColor, Color bottomColor);

//Cargar recursos y liberarlos
void cargarTexturas();
void cargarSonidos();
void liberarTextura();
void liberarSonidos();

//Funciones para dibujar las diferentes pantallas
void dibujarMenu(ElementoMenu *, int);
void dibujarCargar();
void dibujarRanking();

void dibujarInstrucciones();//pantalla de instrucciones

//Funciones para el funcionamiento del tablero
void reiniciarPtsTab(CartasEnTablero*,Juego[][COLUMNAS],int&,int&,int&,bool&);//para reiniciar lo que se necesita para jugar
void inicializarCartas(CartasEnTablero*);//Para asignar las cartas en aleatorio
int IdCarta(CartasEnTablero*);//Verificar si ese id aleatorio no se repita mas de una vez
void llenarTab(Juego[][COLUMNAS], CartasEnTablero*);//asignar el id en el tablero
void mostrarMat(Juego[][COLUMNAS],int,int);//mostrar la matriz y al usuario
void voltearCarta(Juego[][COLUMNAS],int,int,int&,int*);
void evaluarPar(Juego[][COLUMNAS],int*,int&,int&,int,int);

void maquina(Juego[][COLUMNAS],int*,int&,int&,int&,int&,bool&);
bool buscar(Juego [][COLUMNAS], int &, int*, int&);

//Funciones usuario
void registro(Usuarios *usuario);
void inicioSesion(void);

//Funcion para el ranking
bool rankGuardado(Usuarios[], Usuarios);


//-----------------------------------MAIN--------------------------------------------
int main()
{
    InitAudioDevice();//cosas de sonido
    int x=0,y=0,volteadas=0,cordsArriba1[2];//contador para el movimiento en la matriz X y Y
    semilla[1]=CANT_PARES;
    bool terminado=false;
    Juego tablero[FILAS][COLUMNAS];
    CartasEnTablero cartas[CANT_PARES];

    Usuarios usuarioR;



    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Memoria");
    //tomar los tamaños de la pantalla en donde se muestra
    SCREEN_WIDTH = GetMonitorWidth(0);
    SCREEN_HEIGHT = GetMonitorHeight(0);
    SetWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);//reajustar la ventana
    SetWindowPosition(0,0);//posicionar para que se muestre bien

    cargarSonidos();
    cargarTexturas();

    ElementoMenu elementosMenu[] = {
        {"Jugar",{(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 -100, 400, 40},false},
        {"Cargar",{(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 - 40, 400, 40},false},
        {"Ranking",{(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 + 20, 400, 40},false},
        {NULL,{NULL, NULL, NULL, NULL},NULL},
        {"Salir",{(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 + 80, 400, 40},false},
    };
    ElementoMenu elementosMenuJuego[] = {
        {"Facil",{(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 -100, 400, 40},false},
        {"Dififcil",{(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 - 40, 400, 40},false},
        {"Regresar",{(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 + 20, 400, 40},false},
    };

    int elementosTam = sizeof(elementosMenu) / sizeof(elementosMenu[0]);
    int elementosTamJuego = sizeof(elementosMenuJuego) / sizeof(elementosMenuJuego[0]);



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
                            reiniciarPtsTab(cartas,tablero,x,y,volteadas,terminado);
                        } else if (i == 1) {
                            estadoActual = DIFICIL;
                            reiniciarPtsTab(cartas,tablero,x,y,volteadas,terminado);
                        } else if (i == 2) {
                            estadoActual = MENU;
                        }
                    }
                } else {
                    elementosMenuJuego[i].active = false;
                }
            }
        }
        else if (estadoActual == FACIL || estadoActual == DIFICIL) {

            if (((IsKeyPressed(KEY_W)|| IsKeyPressed(KEY_UP)) && y>0) && !machine ) {//en caso de que suba por el tablero
                y=y-1;
            }
            else if ((IsKeyPressed(KEY_S)|| IsKeyPressed(KEY_DOWN)) && y<FILAS-1 && !machine) {//en caso de que baje por el tablero
                y=y+1;
            }
            else if ((IsKeyPressed(KEY_A)|| IsKeyPressed(KEY_LEFT)) && x>0 && !machine) {//en caso de que  valla a la izquierda
                x=x-1;
            }
            else if ((IsKeyPressed(KEY_D)|| IsKeyPressed(KEY_RIGHT)) && x<COLUMNAS-1 && !machine) {//en caso de que  valla a la derecha
                x=x+1;
            }
            else if (IsKeyPressed(KEY_I)) {//para ver instrucciones
                if (!aiuda) {
                    aiuda=true;
                }
                else {
                    aiuda=false;
                }
            }
            else if (IsKeyPressed(KEY_BACKSPACE)) {//Para salir del juego estando jugando
                estadoActual = MENUJ;
            }
            else if (IsKeyPressed(KEY_ENTER) && tablero[y][x].estado == false && !machine) {
                if (volteadas < 2  ) {
                    voltearCarta(tablero, x, y, volteadas, cordsArriba1);

                    // Si es la segunda carta
                    if (volteadas == 2) {
                        // Mostrar ambas cartas volteadas
                        mostrarMat(tablero, x, y);
                        EndDrawing();

                        // Pausa antes de evaluar si es o no par
                        WaitTime(0.1);
                        BeginDrawing();
                        dibujarFondoDegradado(BLUE, PINK);

                        evaluarPar(tablero, cordsArriba1, volteadas, pares, x, y);
                        //Checar si se completo
                        if (pares == CANT_PARES) {
                            terminado=true;
                            FILE *archJugadores = fopen("jugadores.dat", "rb+");

                            //busca el usuario en el archivo y le escibe su puntuacion
                            rewind(archJugadores);
                            while(fread(&usuarioR, sizeof(Usuarios), 1, archJugadores)){
                                if((strcmp (nom, usuarioR.nombre) == 0) && usuarioR.puntos < puntuacion){
                                    usuarioR.puntos = puntuacion;

                                    fseek(archJugadores, -sizeof(Usuarios), SEEK_CUR);
                                    fwrite(&usuarioR, sizeof(Usuarios), 1, archJugadores);
                                }
                            }
                            fclose(archJugadores);

                        }
                        machine = true;
                    }
                }
            }
            else if(machine && estadoActual == FACIL) {
                maquina(tablero, cordsArriba1, volteadas, pares, x, y,terminado);//La función maquia escoge dos cartas aleatoriamentes

            }
            else if(machine && estadoActual == DIFICIL) {

                if(tieneUnElemento() || isEmpty()) {
                    maquina(tablero, cordsArriba1, volteadas, pares, x, y,terminado);//La función maquia escoge dos cartas aleatoriamentes
                }
                if (!buscar(tablero, volteadas, cordsArriba1,pares)) {//La función buscar lo que hace es ya que se voltearon dos cartas iguales, la maquina esoge esa
                    maquina(tablero, cordsArriba1, volteadas, pares, x, y,terminado);//La función maquia escoge dos cartas aleatoriamentes
                } else {
                    machine=false;
                }
            }
        }

        BeginDrawing();
        dibujarFondoDegradado(BLUE, PINK);

        if (estadoActual == MENU) {
            dibujarMenu(elementosMenu, elementosTam);
        } else if (estadoActual == JUGAR) {
            if(sesionIniciada){
                estadoActual = MENUJ;
            }else{
                DrawText("No se ha iniciado seion", 500, 1000, 60, RED);
                estadoActual = CARGAR;
                opcionActual = ELECCION;
            }
        } else if (estadoActual == CARGAR) {
            if (opcionActual == ELECCION){
                dibujarCargar();
                DrawText("Presiona ENTER para volver", 500, 600, 20, DARKGRAY);
                if (IsKeyPressed(KEY_ENTER)) {
                    estadoActual = MENU;
                }
            } else if (opcionActual == REGISTRARSE){
                 registro(&usuario);
                 if(sesionIniciada){                   
                    estadoActual = MENU;  
                }
                opcionActual = ELECCION;

            } else if (opcionActual == INICIAR_SESION){
                inicioSesion();
                if(sesionIniciada){
                    estadoActual = MENU;
                }
                opcionActual = ELECCION;
               
            } else if(opcionActual == CERRAR_SESION){
                sesionIniciada = false;
                opcionActual = ELECCION;

            }
        } else if (estadoActual == RANKING) {
            dibujarRanking();
            DrawText("Presiona ENTER para volver", 500, 600, 20, DARKGRAY);
            if (IsKeyPressed(KEY_ENTER)) {
                estadoActual = MENU;
            }
        }else if (estadoActual == MENUJ) {
            dibujarMenu(elementosMenuJuego, elementosTamJuego);
        }
        if(estadoActual==FACIL || estadoActual==DIFICIL) {
            mostrarMat(tablero, x, y);//mostrar la matriz/tablero y al usuario
            if (aiuda) {
                dibujarInstrucciones();
            }
        }
        EndDrawing();

        if (terminado) {//si ya termino el juego
            WaitTime(1.0);
            BeginDrawing();
            dibujarFondoDegradado(BLUE, PINK);
            if (puntuacion >= puntuacionMac) {
                DrawText("Ganaste", (float)SCREEN_WIDTH/3 , (float)SCREEN_HEIGHT/3, 170, BLACK);
                PlaySound(sonido[3]);
            } else {
                DrawText("Perdiste", (float)SCREEN_WIDTH/3 , (float)SCREEN_HEIGHT/3, 170, BLACK);
                PlaySound(sonido[4]);
            }
            DrawText("Puntos:", (float)SCREEN_WIDTH/3 , (float)SCREEN_HEIGHT/3+155, 110, BLACK);
            DrawText(TextFormat("Tu: %d",puntuacion), (float)SCREEN_WIDTH/3-210 , (float)SCREEN_HEIGHT/3+375, 85, BLACK);
            DrawText(TextFormat("Maquina: %d",puntuacionMac), (float)SCREEN_WIDTH/3+360 , (float)SCREEN_HEIGHT/3+375, 85, BLACK);
            EndDrawing();
            WaitTime(6.0);
            estadoActual = MENUJ;
            pares=0;
            volteadas=0;
            terminado=false;
        }

    }
    liberarTextura();
    liberarSonidos();
    CloseWindow();
    freeList();
    free(semilla);
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
//Función para cargar las imagenes dentro de un vector
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
//función para cargar los sonidos en un vector
void cargarSonidos() {
    sonido[0]=LoadSound("../sonidos/error.mp3");
    sonido[1]=LoadSound("../sonidos/flip.mp3");
    sonido[2]=LoadSound("../sonidos/correcto.mp3");
    sonido[3]=LoadSound("../sonidos/ganar.mp3");
    sonido[4]=LoadSound("../sonidos/perder.mp3");
}
//función para liberar el espacio usado en las imagenes
void liberarTextura() {
    for (int i = 0; i <IMG; i++) {
        UnloadTexture(imagenes[i]);
    }
}
//función para liberar el espacio usado en los sonidos
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

        FILE *archJugadores;
        id=0;
        CuadroOpcion cuadroOpcion[] = {
            {"Registrarse", {(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 - 100, 200, 30}, false},
            {"Iniciar Sesion", {(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 - 60, 200, 30}, false},
            {"Cerrar Sesion", {(float)SCREEN_WIDTH/2 - 100, (float)SCREEN_HEIGHT/2 - 20, 200, 30}, false},
        };
        int tamEl = sizeof(cuadroOpcion) / sizeof(cuadroOpcion[0]);


        dibujarFondoDegradado(BLUE,PURPLE);

            if(opcionActual == ELECCION){
                DrawText("ELIGE ", 300, 20, 40, RED);
                Vector2 mouse = GetMousePosition();

                for (int i = 0; i < tamEl; i++){
                    cuadroOpcion[i].activo = CheckCollisionPointRec(mouse, cuadroOpcion[i].barrera);
                    DrawRectangleRec(cuadroOpcion[i].barrera, cuadroOpcion[i].activo ? SKYBLUE : LIGHTGRAY);
                    DrawText(cuadroOpcion[i].texto, cuadroOpcion[i].barrera.x + 10, cuadroOpcion[i].barrera.y + 2, 20, BLACK);
                }

                for (int i = 0; i < tamEl; i++){
                    if(cuadroOpcion[i].activo && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                            if(i == 0){
                                opcionActual = REGISTRARSE;
                            }else if (i == 1){
                                opcionActual = INICIAR_SESION;
                            }else if (i == 2){
                                opcionActual = CERRAR_SESION;
                            }

                        }
                }
            }
           

}

void maquina(Juego tablero[][6], int * cordsArriba1, int &volteadas, int &pares, int &x, int &y, bool& terminado) {
    WaitTime(1);
    srand(time(nullptr));
    do {
        x = rand()%COLUMNAS;
        y = rand()%FILAS;
    }while((x<0 || x>=COLUMNAS) || ( y<0 || y>=FILAS));
    if(tablero[y][x].estado == false) {
        voltearCarta(tablero, x, y, volteadas, cordsArriba1);
        // Si es la segunda carta
        if (volteadas == 2) {
            // Mostrar ambas cartas volteadas
            mostrarMat(tablero, x, y);
            EndDrawing();

            // Pausa antes de evaluar si es o no par
            WaitTime(0.1);
            BeginDrawing();
            dibujarFondoDegradado(BLUE, PINK);

            evaluarPar(tablero, cordsArriba1, volteadas, pares, x, y);
            machine = false;
            //Checar si se completo
            if (pares == CANT_PARES) {
                terminado=true;
                PlaySound(sonido[3]);
            }
        }
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


//funcion para registrar un nuevo usuario
void registro(Usuarios *usuario){
    memset(usuario, 0, sizeof(Usuarios));
    FILE *archJugadores;

    Usuarios temp;
    char nomTemp[MAX+1];
    bool existe = false, verificado = false, nomExistente = false, terminado = false;


    //se abre el archivo como lecura/escritura
    archJugadores = fopen("jugadores.dat", "rb+");

    if(archJugadores == NULL){
        //si no existe, lo crea y lo abre para lectura/escritura
        archJugadores = fopen("jugadores.dat", "wb+");
    }
    bool datosGuardados = false;

    //se va hasta el inicio del archivo
    rewind(archJugadores);
        //recorre el archivo para "posicionar" el id
        while (fread(&temp, sizeof(Usuarios), 1, archJugadores)) {
            //se guarda el id, en cada usuario nuevo, el id aumenta en 1
            if (temp.id >= id) {
                id = temp.id + 1;
            }
        }

    const int screenWidth = 800;

    int letras = 0;

    Rectangle textBox = { screenWidth/2.0f - 100, 180, 250, 50 };
    bool mouseOnText = false;

    int framesCounter = 0;
    bool ingresoNombre = true;  //variable bool que indica si se esta ingresando el nombre o la contrasena

    //mientras no se haya terminado el registro 
    while (!terminado){
        //si el mouse esta situado en la caja de texto
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText){
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0){
                //si se esta presionando una letra y no se ha alcanzado el maximo de caracteres, se escribe el nombre o la contrasena 
                if ((key >= 32) && (key <= 125) && (letras < MAX)){
                    //en la posicion letras, se mete el caracter que se este presionando y en la siguiente posicion se escribe el fin de linea
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

            //si se presiona la tecla para borrar, se "actualiza" el nombre y la contrasena, restandoles un elemento y moviendo el fin de linea
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
                //si se presiona enter y se esta ingresando el nombre
                if (letras > 0 && ingresoNombre) {
                    letras = 0;
                    existe = false;
                    
                    //recorre el archivo para ver si ya existe el nombre 
                    rewind(archJugadores);
                    while(fread(&temp, sizeof(Usuarios), 1, archJugadores)){
                    if(strcmp(temp.nombre, usuario->nombre) == 0){
                        existe = true;
                        break;
                    }
                }
                verificado = true;

                if(!existe){
                    //si no existe, ingreso nombre se marca como false para que se pueda pasar a escribir la contrasena
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


        //si se escribio la contrasena y se presiona enter, se guarda la informacion en el archivo
        if (!ingresoNombre && IsKeyPressed(KEY_ENTER) && letras > 0) {
            usuario->id = id++;
            fseek(archJugadores, 0, SEEK_END);
            fwrite(usuario, sizeof(Usuarios), 1, archJugadores);
            datosGuardados = true;
            idActual = usuario->id;
            terminado = true;   //marca que se termino el registro

        }



        if (mouseOnText) framesCounter++;
        else framesCounter = 0;

        //boton para volver
        Rectangle botonVolver = {screenWidth/2.0f - 100, 380, 150, 40};

            if(CheckCollisionPointRec(GetMousePosition(), botonVolver)){
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    fclose(archJugadores);
                    return;
                }
            }

        BeginDrawing();
            //dibujo del fondo y los mensajes
        dibujarFondoDegradado(BLUE,PURPLE);

        DrawText("REGISTRO", 300, 20, 40, RED);

        DrawText("COLOCA EL MOUSE EN LA CAJA!", 240, 90, 20, GRAY);

            if(ingresoNombre){
                DrawText("Nombre: ", 330, 135, 40, BLACK);
            }
            else{
                    DrawText("Contraseña: ", 280, 135, 40, BLACK);

            }

            //dibujo de la caja de texto y del texto
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
                DrawText("Este nombre ya está registrado. Intenta con otro!", 150, 350, 20, RED);
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
            terminado = true;
            sesionIniciada = true;
        }

        DrawRectangleRec(botonVolver, LIGHTGRAY);
        DrawRectangleLines(botonVolver.x, botonVolver.y, botonVolver.width, botonVolver.height, DARKGRAY);
        DrawText("Volver", botonVolver.x + 10, botonVolver.y + 10, 20, BLACK);


        EndDrawing();

    }

    //se cierra el archivo de texto
     fclose(archJugadores);

     if(datosGuardados){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Sesion Iniciada!", 200, 200, 40, GREEN);
        WaitTime(3.0);
        EndDrawing();
     }

}


//funcion para iniciar sesion
void inicioSesion(){
    FILE *archJugadores;

     Usuarios temp;
    char nomTemp[MAX+1];
    bool existe = false, verificado = false, nomExistente = false, usuarioExiste = false, terminado = false;

    //el archivo se abre en modo lectura
    archJugadores = fopen("jugadores.dat", "rb");

    if(archJugadores == NULL){
        //archJugadores = fopen("jugadores.dat", "wb+");
    }
    bool datosGuardados = false;


    char nomSesion[MAX+1] = {0};
    char contSesion[MAX+1] = {0};

    const int screenWidth = 800;
    const int screenHeight = 450;


    int letras = 0;

    Rectangle textBox = { screenWidth/2.0f - 100, 180, 250, 50 };
    bool mouseOnText = false;

    int framesCounter = 0;
    bool ingresoNombre = true;  //variable bool que indica si se esta ingresando el nombre o la contrasena
    bool error = false;


    //mientras no se haya terminado el inicio de sesion 
    while (!terminado){
        //si el ouse esta situado en la caja de texto
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (mouseOnText){
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0){
                //si se esta presionando una letra y no se ha alcanzado el maximo de caracteres, se ingresa el nombre o la contrasena
                if ((key >= 32) && (key <= 125) && (letras < MAX)){
                    if(ingresoNombre){
                        //en la posicion letras, se mete el caracter que se presiono y en la siguiente posicion se escribe el fin de linea
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

            //si se presiona la tecla para borrar, se "actualiza" el nombre o la contrasena, restandoles un elemento y moviendo el fin de linea 
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
                //si se presiona enter y se esta ingresando el nombre, ingresoNombre se cambia a falso para pasar a ingresar la contrasena
                if (letras > 0 && ingresoNombre) {
                    letras = 0;
                    ingresoNombre = false;
                }
                else{
                    if(letras > 0 && !ingresoNombre){
                        //si ya se ingreso el nombre busca en el archivo una coincidencia en nombre y contrasena
                        rewind(archJugadores);
                        while(fread(&temp, sizeof(Usuarios), 1, archJugadores)){
                            if(strcmp(temp.nombre, nomSesion) == 0 && strcmp(temp.password, contSesion) == 0){
                                usuarioExiste = true;   //marca si se encontro la coincidencia
                                break;
                            }
                        }
                    }
                    if (usuarioExiste) {
                        fclose(archJugadores);
                        strcpy(nom, nomSesion);
                        sesionIniciada = true;
                        terminado = true;   //marca que se termino el inicio de sesion exitosamente 
                    }
                    else {
                        error = true;   //marca que no se encontro la coincidencia
                        ingresoNombre = true;
                        letras = 0;
                     }
                }

            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText) framesCounter++;
        else framesCounter = 0;

        //boton para volver
        Rectangle botonVolver = {screenWidth/2.0f - 100, 380, 150, 40};

            if(CheckCollisionPointRec(GetMousePosition(), botonVolver)){
                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                    fclose(archJugadores);
                    return;
                }
            }

        BeginDrawing();

            //dibuja el fondo y los mensajes
            dibujarFondoDegradado(BLUE,PURPLE);

            DrawText("INICIAR SESION", 200, 20, 40, RED);

            DrawText("COLOCA EL MOUSE EN LA CAJA!", 240, 90, 20, GRAY);

            if(ingresoNombre){
                DrawText("Nombre: ", 330, 135, 40, BLACK);
            }
            else{
                    DrawText("Contraseña: ", 280, 135, 40, BLACK);

            }

            //dibuja el cuadro de texto
            DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

            if(ingresoNombre){
                DrawText(nomSesion, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
            }
            else{
                DrawText(contSesion, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);
            }

            if(error){      //si no se encontro la coincidencia, muestra el mensaje
                DrawText("Usuario o contraseña incorrecta!", 150, 350, 20, RED);
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

            DrawRectangleRec(botonVolver, LIGHTGRAY);
            DrawRectangleLines(botonVolver.x, botonVolver.y, botonVolver.width, botonVolver.height, DARKGRAY);
            DrawText("Volver", botonVolver.x + 10, botonVolver.y + 10, 20, BLACK);

        EndDrawing();


    }

    //cierra el archivo
     fclose(archJugadores);

     if(sesionIniciada){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Sesion Iniciada!", 200, 200, 40, GREEN);
        WaitTime(3.0);
        EndDrawing();
     }

}

//funcion para mostrar el ranking
void dibujarRanking() {
     FILE* archJugadores = fopen("jugadores.dat", "rb");

    if(archJugadores == NULL){
        DrawText("No hay jugadores registrados", 400, 200, 60, BLACK);
    }
    
    int cont=0, totalUsuarios=0, freno=0;
    Usuarios rankUsuarios[5], mayor, usuario;
    
    for(int i=0; i<5; i++){
    	rankUsuarios[i].puntos = -1;
    	strcpy(rankUsuarios[i].nombre, " ");
	}
    
    //cuenta el total de usuarios guardados
    while(fread(&usuario, sizeof(Usuarios), 1, archJugadores)){
    	totalUsuarios++;
	}
	
    //variable que indica cuando dejar de mostrar usuarios
	if(totalUsuarios < 5){
		freno = totalUsuarios;
	}else{
		freno = 5;
	}
	
    //muestra los usuarios 
    while(cont < freno){
    	rewind(archJugadores);
    	mayor.puntos = 0;
    	while(fread(&usuario, sizeof(Usuarios), 1, archJugadores)){
    		if(usuario.puntos >= mayor.puntos && !rankGuardado(rankUsuarios, usuario)){
    			mayor = usuario;
			}	
		}	
		rankUsuarios[cont] = mayor;
		cont++;
	}

    DrawText("RANKING",500,40,60,RED);

    int y=0;
    char puntosChar[100];

    
    for(int i=0; i<freno; i++){
        sprintf(puntosChar, "%d", rankUsuarios[i].puntos);

        y += 100;
        DrawText(rankUsuarios[i].nombre, 400, y, 70, BLACK);
        DrawText(".............", 600, y, 55, BLACK);
        DrawText(puntosChar, 800, y, 70, BLACK);
	}
    
    fclose(archJugadores);
}
 void dibujarInstrucciones() {
    int anchoRectangulo = SCREEN_WIDTH / 2.5f;    // Un poco más ancho que 1/3
    int altoRectangulo = SCREEN_HEIGHT / 2;
    int posXRectangulo = (SCREEN_WIDTH - anchoRectangulo) / 2;
    int posYRectangulo = (SCREEN_HEIGHT - altoRectangulo) / 2;
    int tamanoLetra = SCREEN_HEIGHT / 35;         // Un poco más pequeño

    // Dibujar rectángulo centrado
    DrawRectangle(posXRectangulo, posYRectangulo, anchoRectangulo, altoRectangulo, BLUE);

    // Margenes y espaciado (10% de margen interno)
    float margenInterno = anchoRectangulo * 0.1f;
    float posXTexto = posXRectangulo + margenInterno;
    float posYTexto = posYRectangulo + margenInterno;
    float espacioLineas = tamanoLetra * 1.5f;

    // Textos ajustados para que quepan
    DrawText("Moverse con flechas o WASD", posXTexto, posYTexto, tamanoLetra, BLACK);
    DrawText("Enter: voltear carta", posXTexto, posYTexto + espacioLineas, tamanoLetra, BLACK);
    DrawText("Backspace: salir del juego", posXTexto, posYTexto + espacioLineas * 2, tamanoLetra, BLACK);
    DrawText("Tecla I: cerrar esta ventana", posXTexto, posYTexto + espacioLineas * 4, tamanoLetra, BLACK);
    DrawText("Abajo indica de quien es el turno", posXTexto, posYTexto + espacioLineas * 3, tamanoLetra, BLACK);
 }


//funcion para evaluar que el nombre todavia no se haya mostrado en el ranking
bool rankGuardado(Usuarios rankUsuarios[], Usuarios usuario){
	for(int i=0; i<5; i++){
		if(strcmp(rankUsuarios[i].nombre, usuario.nombre) == 0){
			return true;
		}
	}
	return false;	
}


//Funciones para el tablero de memorama--------------------------------

//función para reiniciar las variables y otras cosas para poder iniciar un juego desde 0
void reiniciarPtsTab(CartasEnTablero*cartas, Juego tablero[][COLUMNAS],int &x, int&y, int &volteadas,bool&terminado) {
    srand(time(nullptr));
    inicializarCartas(cartas);
    llenarTab(tablero, cartas);
    puntuacion=0;
    puntuacionMac=0;
    pares=0;
    machine=false;
    volteadas=0;
    x=0;
    y=0;
    terminado=false;
}
//Función para guardar en un vector los id a usar de las cartas y su disponibilidad para la hora de asignar en tablero
void inicializarCartas(CartasEnTablero*cartas) {
    for (int i = 0; i < (CANT_PARES); i++) {
        cartas[i].id = i;
        cartas[i].disponible = 2;
    }
}
//Función que retorna un numero aleatorio(id), pero que sea un id valido(que exista y que aun este disponible)
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
//llena el tablero con el id de la carta y la coloca boca abajo
void llenarTab(Juego tablero[][COLUMNAS], CartasEnTablero cartas[CANT_PARES]) {
    int k = 1;
    for (int i = 0; i <FILAS; i++) {
        for (int j = 0; j <COLUMNAS; j++) {
            int id = IdCarta(cartas);
            semilla[k]=id;
            tablero[i][j].id_Carta =id;
            tablero[i][j].estado = false;//Volteado
            k++;
        }
    }
}
//función para dibujar el tablero en pantalla
void mostrarMat(Juego tablero[][COLUMNAS],int x,int y) {
    // Para que sea proporcional
    float espacio_util = 0.85; //porcentaje de pantalla a usar
    float separacion = 0.05f;   // porcentaje de separación entre cartas
    float escala_seleccion = 1.08f; // porcentaje del tamaño de la carta al seleccionarla

    // Tamaños
    float ancho_carta = (GetScreenWidth() * espacio_util) / COLUMNAS;
    float alto_carta = (GetScreenHeight() * espacio_util) / FILAS;
    float tamano_base;

    if (ancho_carta < alto_carta) {
        tamano_base = ancho_carta * (1.0f - separacion);
    } else {
        tamano_base = alto_carta * (1.0f - separacion);
    }

    //Tamaños finales
    float espacio_entre = tamano_base * separacion;
    float total_ancho = COLUMNAS * tamano_base + (COLUMNAS - 1) * espacio_entre;
    float total_alto = FILAS * tamano_base + (FILAS - 1) * espacio_entre;
    float inicio_x = (GetScreenWidth() - total_ancho) / 2;
    float inicio_y = (GetScreenHeight() - total_alto) / 2;

    DrawText(TextFormat("Pares encontrados: %d/%d",pares,CANT_PARES), inicio_x, inicio_y - 40, 30, BLACK);
    DrawText("Presiona I por ayuda", inicio_x+650, inicio_y - 40, 30, BLACK);
    DrawText(TextFormat("Tu: %d",puntuacion),inicio_x,inicio_y-90,50,BLACK);
    DrawText(TextFormat("Maquina: %d",puntuacionMac),inicio_x+900,inicio_y-90,50,BLACK);
    if(machine) {
        DrawText(TextFormat("MAQUINA",puntuacionMac),GetScreenWidth()/2,GetScreenHeight()-55,50,BLACK);
    } else {
        DrawText(TextFormat("TU",puntuacionMac),GetScreenWidth()/2,GetScreenHeight()-55,50,BLACK);
    }



    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {

            // Posición
            float pos_x = inicio_x + col * (tamano_base + espacio_entre);
            float pos_y = inicio_y + fila * (tamano_base + espacio_entre);
            float tamano_actual = tamano_base;

            //Si es la seleccionada
            if (fila == y && col == x) {
                tamano_actual = tamano_base * escala_seleccion;
                pos_x -= (tamano_actual - tamano_base) / 2;
                pos_y -= (tamano_actual - tamano_base) / 2;
                if (machine) {
                    DrawRectangle(pos_x-2,pos_y-2,tamano_actual+6,tamano_actual+6,GREEN);
                }
                else {
                    DrawRectangle(pos_x-2,pos_y-2,tamano_actual+6,tamano_actual+6,YELLOW);
                }
            }

            // mostrar la imagen correspondiente
            int textura;
            if (tablero[fila][col].estado == false) {
                textura = 0;
            } else {
                textura = tablero[fila][col].id_Carta + 1;
            }

            // Dibujar
            DrawTexturePro(imagenes[textura],(Rectangle){0, 0, TAM_IMG, TAM_IMG},
                (Rectangle){pos_x, pos_y, tamano_actual, tamano_actual},
                (Vector2){0, 0},0,WHITE);
        }
    }

}

//Función para voltear la carta(cambia su estado)
void voltearCarta(Juego tablero[FILAS][COLUMNAS], int x, int y, int &volteadas, int cordArriba1[2]) {
    if (tablero[y][x].estado == false && volteadas < 2) {
        PlaySound(sonido[1]);
        tablero[y][x].estado = true;
        insertEnd(tablero[y][x].id_Carta,x,y);//Cada vez que se voleta una carta se alamcena en una lista para que la maquina la usa
        if (volteadas == 0) {
            cordArriba1[0] = x;//almacenar las anteriores por si al voltear la siguiente es necesario ponerlas boca abajo
            cordArriba1[1] = y;
            volteadas = 1;
        }
        else {
            volteadas = 2;
        }
    }
}
//función para saber si las cartas volteadas son par o no
void evaluarPar(Juego tablero[FILAS][COLUMNAS], int cordsArriba1[2], int &volteadas,int &pares,int x,int y) {
    if (volteadas==2) {
        mostrarMat(tablero, x, y);
        WaitTime(.5);
        if (tablero[y][x].id_Carta==tablero[cordsArriba1[1]][cordsArriba1[0]].id_Carta) {
            PlaySound(sonido[2]);
            pares++;
            if (!machine){
                puntuacion+=10;
            } else {
                puntuacionMac+=10;
            }
        }
        else {
            if (!machine) {
                puntuacion-=2;
                if (puntuacion<0) puntuacion = 0;
            } else {
                puntuacionMac-=2;
                if (puntuacionMac<0) puntuacionMac = 0;
            }
            PlaySound(sonido[0]);
            WaitTime(0.3);
            tablero[y][x].estado=false;
            tablero[cordsArriba1[1]][cordsArriba1[0]].estado=false;
        }
        volteadas=0;
        mostrarMat(tablero, x, y);
    }
}

// =================================================================
// Funciones para el uso de listas

Nodo *createNode(type data, Nodo *left, Nodo *right, type x, type y) { // Funciona para crear un nuevo nodo para la lista
  Nodo *nuevo= NULL;
  nuevo = (Nodo *)malloc(sizeof(Nodo));
  nuevo->data = data;
    nuevo->x=x;
    nuevo->y=y;
  nuevo->left = left;
  nuevo->right = right;
  return nuevo;
}

bool isEmpty() { // funcion para saber si la lista esta vacia
  return ini == NULL;
}

bool tieneUnElemento() { // funciona para saber si la lista solamente tiene un elemento
    return ini == end;
}

void displayListLTR() { //Funcion para mostrar la lista de izuierda a derecha
  Nodo *aux = NULL;
  aux = ini;
  while (aux != NULL) {
      printf("%d (%d,%d)", aux->data,aux->x,aux->y);
    aux = aux->right;
  }
}

void displayListRTL() { //Funcion para mostrar la lista de derecha a izuquierda
  Nodo *aux = NULL;
  aux = end;
  while (aux != NULL) {
    printf("%d (%d,%d)", aux->data,aux->x,aux->y);
    aux = aux->left;
  }
}

void insertIni(type data, type x, type y) { //Funcion para insertar un elemento en lista al inicio
  Nodo *nuevo = createNode(data, NULL, NULL,x,y);
  if (isEmpty()) {
    ini = nuevo;
    end = nuevo;
  } else {
    nuevo->right = ini;
    ini->left = nuevo;
    ini = nuevo;
  }
}

void insertEnd(type data,type x, type y) { //Funcion para insertar un elemento en lista al final
  Nodo *nuevo = createNode(data, NULL, NULL,x,y);
  if (isEmpty()) {
    ini = nuevo;
    end = nuevo;
  } else {
    nuevo->left = end;
    end->right = nuevo;
    end = nuevo;
  }
}

bool buscar(Juego tablero[FILAS][COLUMNAS], int &volteadas, int cordArriba1[2], int &pares) {
    Nodo *auxI = NULL, *auxE = NULL;
    auxI = ini;

    while (auxI!=NULL) {
        auxE = end;
        while (auxE != auxI) {
            if(auxI->data == auxE->data  && (auxI->x != auxE->x || auxI->y != auxE->y)) { //Lo que hace esta función es buscar dos cartas que tengan el mismo id pero difertene posicion
                //En caso de que si la haya encontrado en la lista de cartas voletadas, voltea esas dos cartas automaticamente.
                voltearCarta(tablero, auxI->x, auxI->y, volteadas, cordArriba1);
                voltearCarta(tablero, auxE->x, auxE->y, volteadas, cordArriba1);
                evaluarPar(tablero,cordArriba1,volteadas,pares,auxE->x,auxE->y);
                freeList();//Despues libera toda la lista para volver a almacenar cartas nuevas, así evitando volver a voltear cartas que ya se hallan encontrado
                return true;
            }
            auxE=auxE->left;
        }
        auxI=auxI->right;
    }
    return false;
}

void freeList() { // Funcion para liberar todos los elementos de la lista
  if (!isEmpty()) {
    while(ini!=NULL) {
      Nodo *aux = ini;
      ini = ini->right;
      printf("Liberandp %d ", aux->data);
      free(aux);
    }
  }
  end = NULL;
}



// Funciones para el uso de listas
// =================================================================