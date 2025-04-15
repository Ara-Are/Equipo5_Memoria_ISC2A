//Datos Usuario

#include <iostream>
#include <string.h>
using namespace std;

int top=0;	//tope 

struct Usuario{		//Datos de cada usuario
	char nombre[30];
	int pts;
};

void registro(Usuario*);
void ranking(Usuario*);


int main(){
	int opc;
	Usuario usuario[100];
	do{
		cout << "\nRegistro...1";
		cout << "\nRanking....2";
		cout << "\nSalir......3";
		cout << "\nElige una opcion: ";
		cin >> opc;
		switch(opc){
			case 1: registro(usuario);break;
			case 2: ranking (usuario);break;
			case 3: cout << "Saliendo...";break;
			default: cout << "\nOpcion no valida!\n"; break;
		}
	}while(opc!=3);

}

void registro(Usuario usuario[100]){	//Registrar un nuevo usuario mientras no se llegue a un limite de usuarios
	if(top<1000){
		cout << "\nIntroduce tu nombre: ";
		cin >> usuario[top].nombre;
		for(int i=0; i<top; i++){
			if(strcmp(usuario[top].nombre, usuario[i].nombre) == 0){	//Compara para que no hayan dos nombres iguales
				cout << "Ese nombre ya esta registrado, intenta con otro!" << endl;
				registro(usuario);
				return;
			}
		}
		usuario[top].pts=0;	//Los puntos de cada nuevo jugador inician en 0
		top++;	//Se le suma 1 al tope
		cout << "\nJugador registrado!\n";
	}else{
		cout << "\nNo hay espacio para otro jugador!\n";
		return;
	}
	
}

void ranking(Usuario usuario[100]){	//Mostrar el ranking, con nombre del jugador y su puntaje
	cout << "\nRANKING\n";
	if(top > 0){
		cout << endl;
		for(int i=0; i<top; i++){
			cout << usuario[i].nombre << "...... " << usuario[i].pts << " pts" << endl << endl;
		}
	}else{
		cout << "\nNo hay usuarios registrados!" << endl;
	}
	
}


