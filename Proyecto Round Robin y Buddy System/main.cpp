/*
   -------------------------------------------------
   |			 SIMULADOR DE TECNICAS				|
   |		    DE COLOCACION DE MEMORIA			|
   |		   PLANIFICACION DE PROCESOS			|
   |		   BUDDY SYSTEM & ROUND ROBIN			|
   |												|
   |	REALIZADO POR:								|
   |	* DE ANDA MEDINA RENE ROSENDO		260866  |
   |	* ESPARZA TORRES MIGUEL ANGEL		272437	|
   |	* GARCÍA QUESADA GIBRAN ALEXANDER	281614	|
   |	* MUÑOZ LÓPEZ BRUNO SANTIAGO		280023	|
   |												|
   |	 UNIVERSIDAD AUTONOMA DE AGUASCALIENTES		|
   |				 3° ISC C						|
   |		MATERIA: SISTEMAS OPERATIVOS			|
   |					12/2023						|
   --------------------------------------------------

   Git de Proyecto:
*/

#include <iostream>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#include <thread>
#include <chrono>
#include "Memoria.h"

#define TAMMIN 32					
#define PRO 100

using namespace std;				// Se utiliza el espacio de nombres estándar para poder acceder a las funciones y clases estándar de C++
using namespace std::chrono;		// Se vuelve a utilizar el espacio de nombres chrono 
using namespace std::this_thread;	// Se vuelve a utilizar el espacio de nombres this_thread 


struct Proceso {
	int tam;						// Tamaño de proceso
	int quantum;					// Quantum de proceso
	int ordenllegada;				// 
};

struct Cola {						// Estructura de la lista Cola
	Cola* der;						// Nodo siguiente
	Proceso* datos;					// Información de cada proceso
};

struct Tamanios {
	int tamMM,						// Tamaño de memoria para la simulación del programa
		tamMP,						// Tamaño máximo aleatorio para los procesos
		numQuan,					// Valor máximo aleatorio para los procesos
		tiempo,						// Tiempo de espera de ejecución
		quantum_Sistema;			// Valor de Quantum del Sistema
}sistema;

// DECLARACION DE FUNCIONES:
Cola* crearCola();
void mostrarCola(Cola* p);
Proceso crearProceso(int, int);
Cola* insertaFinal(Cola*, Proceso);
Cola* insertaInicio(Cola*, Proceso);
Cola* infoProceso(Cola*, Proceso);
Proceso sacarProcesoCola(Cola*);
Cola* insertaProcesos(Cola*, Proceso);
Cola* eliminarCabeceraCola(Cola*);
void quita(Cola*);
void seleccionTam(Tamanios&);
void menuParametros(Tamanios&);
void gotoxy(int, int);
void centrarTexto(string, int);
void recuadro(int, int, int, int);
void mostrarParametros(Tamanios&);
void menuPrincipal();
int seleccionVelocidad();
int seleccionModo();
int cajaResp(int modo);
void gotoxy(int x, int y);
void menuFinal();

//Variables globales para el control del programa
int numeroP = 0;
int velocidad = 1; 						// Indica la velocidad a la que se lleva la simulacion
int modo = 1; 							// Selecciona entre dos modos: llevar la simulacion completa hasta terminar, o terminar la somulacion cuando el usuario lo indique
int inicio = 1; 						// hacer la simulacion desde el inicio o continuar con los mismos valores
int repetir = 1; 						// Repetir la simulacion o no
int proAten = 0;						// Variable para estadísticas, nos permitirá saber el total de procesos atendidos por cada ciclo
float ocup = 0;							// Variable para estadísticas, nos permitirá saber el porcentaje total de memoria ocupada por cada ciclo
float auxPorcentaje = 0;				// Auxiliar de ocup

// Declaracion de HANDLE global			Se obtiene el identificador del manipulador de la consola estándar. Almacenandolo en la variable hConsole de tipo Handle
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Declaracion de colores 
int colorMargen = 12;
int colorTitulo = 3;
int colorTexto = 14;
int colorPregunta = 10;
int colorError = 4;

// Variables de tiempo, marcando el tiempo por el valor int
seconds duracion(3);
seconds duracion2(2);
seconds duracion1(1);
seconds duracion8(8);
seconds duracion5(5);


int main(int argc, char** argv) {
	int op = 0, auxtam = 0, seguir = 1;
	int cont = 0, tamPrimer = 0;
	int tiempo;
	bool continua = false;
	char seguirResp = 0;
	Tamanios sistema;
	HWND hwnd = GetConsoleWindow();  // Esto asume que estás utilizando una ventana de consola, ajusta según tu caso

	// Coordenadas de la ventana (posición x, posición y, ancho, alto)
	int x = 320;
	int y = 100;
	int width = 600;
	int height = 550;

	// Ajusta la posición y el tamaño de la ventana
	SetWindowPos(hwnd, 0, x, y, width, height, SWP_NOZORDER | SWP_NOSIZE);

	menuPrincipal();
	sleep_for(duracion2);
	system("cls");


	// Menu para seleccionar el tamanio de la memoria	
	seleccionTam(sistema);

	//Crear objeto dinamico de la clase Memoria (Declaracion de clase en encabezado: Memoria.h)
	Memoria* lista = new Memoria(sistema.tamMM);				
	// Menu para la seleccion los parametros del programas
	menuParametros(sistema);

	/*Creamos dos Estructuras de tipo : Lista Simple Ligada de tipo Cola (FIFO) .
		1. colaProcesos: Una que almacenara todos los procesos que se crearan aleatoriamente
		2. colaNoTerminados: Fungira para los procesos NO terminados, en caso que la cola de Procesos ya no pueda recibir mas procesos (memoria llena)
	*/

	Cola* colaProcesos = nullptr;
	Cola* colaNoTerminados = nullptr;

	system("cls");

	srand(time(NULL));
	Proceso aux;

	//Ciclo encargado de generar procesos aleatorios en base a los parametros proporcionados
	for (int k = 0; k < PRO; k++) {
		aux = crearProceso(sistema.tamMP, sistema.numQuan);
		colaProcesos = insertaFinal(colaProcesos, aux);			// 1. Cola encargada de los procesos

	}

	system("cls");
	SetConsoleTextAttribute(hConsole, colorMargen);
	recuadro(0, 0, 118, 29);

	// Selecciona la velocidad de ejecución del programa
	tiempo = seleccionVelocidad();

	// Crea una duración en segundos basada en la velocidad seleccionada
	seconds duracionPrograma(tiempo);

	// Configura el color del texto en la consola para el título
	SetConsoleTextAttribute(hConsole, colorTitulo);
	centrarTexto("Tiempo de pausa: ", 27);
	SetConsoleTextAttribute(hConsole, colorTexto);
	cout << tiempo << " seg";

	// Pausa la ejecución del programa según el tiempo que seleccionó el usuario
	sleep_for(duracionPrograma);

	modo = seleccionModo();
	system("cls");

	// CICLO DE PROGRAMA
	do {
		system("cls");
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(0, 0, 118, 29);
		SetConsoleTextAttribute(hConsole, colorTitulo);
		centrarTexto("Modo Simulacion", 2);
		SetConsoleTextAttribute(hConsole, colorTitulo);
		centrarTexto("Quantum de procesamiento del sistema: ", 3);
		SetConsoleTextAttribute(hConsole, colorTexto);
		cout << sistema.quantum_Sistema;
		gotoxy(42, 4); cout << "Proceso Entrando: " << "[" << colaProcesos->datos->ordenllegada << "," << colaProcesos->datos->tam << "," << colaProcesos->datos->quantum << "]" << endl;
		sleep_for(duracionPrograma);
		tamPrimer = colaProcesos->datos->tam;
		// Se verifica si la cola no esta llena o si el tamaño de la memoria
		continua = lista->siEntra(tamPrimer);
		// Caso en que la cola todavia no esta llena
		if (continua) {
			int impresionLista = 0;
			proAten++;
			SetConsoleTextAttribute(hConsole, colorTexto);
			gotoxy(1, 6); cout << "El proceso si puede entrar" << endl;
			gotoxy(1, 7), SetConsoleTextAttribute(hConsole, colorTitulo); cout << "Lista de los Procesos ";
			gotoxy(1, 8); lista->mostrarProcesos();
			// Buscaremos el proceso 1 en cola, FIFO
			Proceso a = sacarProcesoCola(colaProcesos);				
			colaProcesos = eliminarCabeceraCola(colaProcesos);
			
			SetConsoleTextAttribute(hConsole, colorPregunta);
			lista->buscaEspacio(a.tam, sistema.tamMM, a.ordenllegada, a.quantum);
			sleep_for(duracionPrograma);

			SetConsoleTextAttribute(hConsole, colorTitulo);
			SetConsoleTextAttribute(hConsole, colorTexto);
			sleep_for(duracionPrograma);
			SetConsoleTextAttribute(hConsole, colorTitulo);
			
			gotoxy(1, 11); cout << "Memoria: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			gotoxy(1, 12); lista->mostrarLista(++impresionLista);	// Función para mostrar la memoria
			sleep_for(duracionPrograma);

			SetConsoleTextAttribute(hConsole, colorTitulo);
			gotoxy(1, 14); cout << "Cola de procesos: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			gotoxy(1, 15); lista->mostrarProcesos();				// Mostramos Cola de Procesos

			a.quantum -= sistema.quantum_Sistema; 					// Quantum del proceso - quantum del sistema


			if (a.quantum <= 0) {
				a.quantum = 0;
			}

			SetConsoleTextAttribute(hConsole, colorTitulo);
			gotoxy(1, 18); cout << "Atendiendo proceso: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			lista->mostrarProceso(sistema.quantum_Sistema, a.ordenllegada, a.tam);
			sleep_for(duracionPrograma);
			SetConsoleTextAttribute(hConsole, colorTitulo);
			gotoxy(1, 19); cout << "Quantums restantes del proceso: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			cout << a.quantum;

			if (a.quantum > 0) {
				SetConsoleTextAttribute(hConsole, colorTitulo);
				gotoxy(1, 21); cout << "Proceso a la cola de espera" << endl;
				gotoxy(1, 22); cout << "Memoria Final: ";
				SetConsoleTextAttribute(hConsole, colorTexto);
				lista->unirMemoria();
				gotoxy(1, 23); lista->mostrarLista(++impresionLista);
				colaNoTerminados = insertaFinal(colaNoTerminados, a);
				sleep_for(duracionPrograma);
			}
			else {
				SetConsoleTextAttribute(hConsole, colorPregunta);
				gotoxy(1, 21); cout << "El proceso termino su ejecucion";
				lista->sacarProceso(a.ordenllegada);
				lista->unirMemoria();
				SetConsoleTextAttribute(hConsole, colorTitulo);
				gotoxy(1, 22); cout << "Memoria Final: ";
				SetConsoleTextAttribute(hConsole, colorTexto);
				gotoxy(1, 23); lista->mostrarLista(++impresionLista);
				sleep_for(duracionPrograma);
			}

			SetConsoleTextAttribute(hConsole, colorMargen);
			recuadro(1, 25, 59, 28);
			SetConsoleTextAttribute(hConsole, colorTitulo);
			gotoxy(2, 26); cout << "Total de procesos atendidos: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			cout << proAten << endl;
			SetConsoleTextAttribute(hConsole, colorTitulo);
			ocup = lista->porcentajeMemoria(sistema.tamMM);
			if (ocup > 100) {
				gotoxy(2, 27); cout << "Porcentaje de memoria ocupado en el proceso: ";
				SetConsoleTextAttribute(hConsole, colorTexto);
				SetConsoleTextAttribute(hConsole, colorMargen);
				cout << auxPorcentaje << " % " << endl;
			}
			else {
				auxPorcentaje = ocup;
				gotoxy(2, 27); cout << "Porcentaje de memoria ocupado en el proceso: ";
				SetConsoleTextAttribute(hConsole, colorTexto);
				SetConsoleTextAttribute(hConsole, colorMargen);
				cout << ocup << " % " << endl;
			}
			seguir = cajaResp(modo);
			system("cls");
		}
		// Caso en que la cola este llena
		else {
			system("cls");
			SetConsoleTextAttribute(hConsole, colorMargen);
			recuadro(0, 0, 118, 29);
			SetConsoleTextAttribute(hConsole, colorTitulo);
			centrarTexto("Modo Simulacion", 2);
			centrarTexto("Quantum de procesamiento del sistema: ", 3);
			SetConsoleTextAttribute(hConsole, colorTexto);
			cout << sistema.quantum_Sistema;
			// Proceso que no pudo entrar
			gotoxy(42, 4); cout << "Proceso Entrando: " << "[" << colaProcesos->datos->ordenllegada << "," << colaProcesos->datos->tam << "," << colaProcesos->datos->quantum << "]" << endl;
			sleep_for(duracionPrograma);
			SetConsoleTextAttribute(hConsole, colorError);
			centrarTexto("El Proceso no puede entrar", 5);
			sleep_for(duracionPrograma);
			sleep_for(duracionPrograma);
			int impresionLista = 0;
			SetConsoleTextAttribute(hConsole, colorTexto);
			centrarTexto("Debe continuar con la cola de los procesos en memoria", 7);
			
			// Sacamos proceso de la cola No Terminados
			Proceso b = sacarProcesoCola(colaNoTerminados);
			colaNoTerminados = eliminarCabeceraCola(colaNoTerminados);

			SetConsoleTextAttribute(hConsole, colorTitulo);
			gotoxy(1, 9); cout << "Cola de Procesos: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			gotoxy(1, 10); lista->mostrarProcesos();
			sleep_for(duracionPrograma);

			SetConsoleTextAttribute(hConsole, colorTitulo);
			gotoxy(1, 12); cout << "Memoria: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			gotoxy(1, 13); lista->mostrarLista(++impresionLista);
			sleep_for(duracionPrograma);

			SetConsoleTextAttribute(hConsole, colorTitulo);
			gotoxy(1, 15); cout << "Atendiendo proceso: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			lista->mostrarProceso(sistema.quantum_Sistema, b.ordenllegada, b.tam);
			//lista->buscarProceso(b.ordenllegada);
			sleep_for(duracionPrograma);

			b.quantum -= sistema.quantum_Sistema;

			if (b.quantum <= 0) {
				b.quantum = 0;
			}
			SetConsoleTextAttribute(hConsole, colorTitulo);
			gotoxy(1, 17); cout << "Quantum restantes del proceso: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			cout << b.quantum;
			sleep_for(duracionPrograma);

			if (b.quantum > 0) {				//En dado caso que hayan restado quantums, que no haya terminado el proceso
				SetConsoleTextAttribute(hConsole, colorTexto);
				gotoxy(1, 20); cout << "El proceso tiene " << b.quantum << " quantum de tiempo";
				gotoxy(1, 21); cout << "Regresa a la cola de listos" << endl;
				SetConsoleTextAttribute(hConsole, colorTitulo);
				gotoxy(1, 22); cout << "Memoria Final: ";
				SetConsoleTextAttribute(hConsole, colorTexto);
				gotoxy(1, 23); lista->mostrarLista(++impresionLista);
				sleep_for(duracionPrograma);
				colaNoTerminados = insertaFinal(colaNoTerminados, b);
			}
			else {
				SetConsoleTextAttribute(hConsole, colorTexto);
				gotoxy(1, 21); cout << "El proceso termino su ejecucion";
				lista->sacarProceso(b.ordenllegada);
				lista->unirMemoria();
				SetConsoleTextAttribute(hConsole, colorTitulo);
				gotoxy(1, 22); cout << "Memoria Final: ";
				SetConsoleTextAttribute(hConsole, colorTexto);
				lista->unirMemoria();
				gotoxy(1, 23); lista->mostrarLista(++impresionLista);
				sleep_for(duracionPrograma);
			}
			// Estadística del ciclo, numero de procesos y porcentaje de memoria
			SetConsoleTextAttribute(hConsole, colorMargen);
			recuadro(1, 25, 59, 28);
			SetConsoleTextAttribute(hConsole, colorTitulo);
			gotoxy(2, 26); cout << "Total de procesos atendidos: ";
			SetConsoleTextAttribute(hConsole, colorTexto);
			cout << proAten << endl;

			SetConsoleTextAttribute(hConsole, colorTitulo);
			ocup = lista->porcentajeMemoria(sistema.tamMM);
			if (ocup > 100) {
				gotoxy(2, 27); cout << "Porcentaje de memoria ocupado en el proceso: ";
				SetConsoleTextAttribute(hConsole, colorTexto);
				SetConsoleTextAttribute(hConsole, colorMargen);
				cout << auxPorcentaje << " % " << endl;
			}
			else {
				auxPorcentaje = ocup;
				gotoxy(2, 27); cout << "Porcentaje de memoria ocupado en el proceso: ";
				SetConsoleTextAttribute(hConsole, colorTexto);
				SetConsoleTextAttribute(hConsole, colorMargen);
				cout << ocup << " % " << endl;
			}
			seguir = cajaResp(modo);
			SetConsoleTextAttribute(hConsole, colorTexto);
			system("cls");
		}
		// El proceso no pudo entrar a la cola, esto debido a que la memoria esta llena
		continua = false;
	} while (seguir == 1);
	// Menu final fin de programa
	menuFinal();
	// Liberamos la memoria
	delete(lista);
	delete(colaProcesos);
	delete(colaNoTerminados);
	return 0;
}

void menuFinal() {
	SetConsoleTextAttribute(hConsole, colorMargen);
	recuadro(0, 0, 118, 29);
	SetConsoleTextAttribute(hConsole, colorMargen);
	recuadro(40, 12, 73, 17);
	SetConsoleTextAttribute(hConsole, colorTitulo);
	centrarTexto("SIMULADOR DE TECNICAS", 13);
	centrarTexto("DE COLOCACION DE MEMORIA", 14);
	centrarTexto("PLANIFICACION DE PROCESOS", 15);
	centrarTexto("BUDDY SYSTEM & ROUND ROBIN", 16);
	SetConsoleTextAttribute(hConsole, colorTexto);
	centrarTexto("REALIZADO POR :", 19);
	centrarTexto("DE ANDA MEDINA RENE ROSENDO", 20);
	centrarTexto("ESPARZA TORRES MIGUEL ANGEL", 21);
	centrarTexto("GARCIA QUESADA GIBRAN ALEXANDER", 22);
	centrarTexto("MUNIOZ LOPEZ BRUNO SANTIAGO", 23);
}

void mostrarCola(Cola* p) {
	Cola* q = p;
	while (q != nullptr) {
		cout << "  [" << q->datos->tam << "]  ";
		q = q->der;
	}
}

Proceso crearProceso(int tamMP, int numQuan) {
	Proceso aux;
	aux.tam = rand() % tamMP;				//Valor de Memoria Aleatorio
	if (aux.tam > tamMP) {
		aux.tam = tamMP;					//Si el tamaño es mayor, ese será su propio tamaño
	}
	aux.quantum = rand() % numQuan;			//Valor de quantum aleatorio
	if (aux.quantum == 0) {					//Unico caso especial
		aux.quantum = 1;
	}
	aux.ordenllegada = numeroP;				//Numero de proceso a partir de la variable global
	numeroP++;
	return aux;
}


Cola* crearCola() {
	Cola* q = new Cola;
	q->datos = new Proceso; // Asignar memoria para la estructura Proceso
	return q;
}

Cola* infoProceso(Cola* nodo, Proceso _proc) {
	// Copiar la cadena de caracteres desde _proc.nombre a nodo->datos.nombre
	nodo->datos->tam = _proc.tam;
	nodo->datos->quantum = _proc.quantum;
	nodo->datos->ordenllegada = _proc.ordenllegada;
	return nodo;
}

// Función para insertar un nuevo nodo al inicio de una cola
Cola* insertaInicio(Cola* p, Proceso x) {
	Cola* q = nullptr;  // Puntero auxiliar

	// Manejo especial si la cola está vacía
	if (p == NULL) {
		p = crearCola();  // Crea un nuevo nodo cola
		p = infoProceso(p, x);  // Asigna la información del proceso al nuevo nodo
		p->der = NULL;  // Establece el puntero derecho del nuevo nodo como nullptr
	}
	else {
		q = crearCola();  // Crea un nuevo nodo cola
		q = infoProceso(q, x);  // Asigna la información del proceso al nuevo nodo
		q->der = p;  // Establece el puntero derecho del nuevo nodo para que apunte al nodo original
		p = q;  // Actualiza el inicio de la cola al nuevo nodo
	}

	return p;  // Retorna el inicio de la cola actualizada
}

// Función para insertar un nuevo nodo al final de una cola
Cola* insertaFinal(Cola* p, Proceso proc) {
	Cola* q = p, * t = nullptr;  // Punteros auxiliares

	// Manejo especial si la cola está vacía
	if (p == nullptr) {
		q = crearCola();  // Crea un nuevo nodo cola
		q = infoProceso(q, proc);  // Asigna la información del proceso al nuevo nodo
		q->der = nullptr;  // Establece el puntero derecho del nuevo nodo como nullptr
		return q;  // Retorna el nuevo nodo como el inicio de la cola
	}

	t = p;  // Inicializa el puntero t para recorrer la cola

	// Recorre la cola hasta llegar al último nodo
	while (t->der != nullptr) {
		t = t->der;
	}

	q = crearCola();  // Crea un nuevo nodo cola
	q = infoProceso(q, proc);  // Asigna la información del proceso al nuevo nodo
	t->der = q;  // Establece el puntero derecho del último nodo para que apunte al nuevo nodo
	q->der = nullptr;  // Establece el puntero derecho del nuevo nodo como nullptr, ya que es el último nodo

	return p;  // Retorna el inicio de la cola actualizada
}

Proceso sacarProcesoCola(Cola* p) {
	if (p == nullptr || p->datos == nullptr) {
		SetConsoleTextAttribute(hConsole, colorPregunta);
		centrarTexto("Ya no hay procesos en cola!", 20);
		system("pause");
		Proceso proceso_vacio;  // Devuelve un Proceso vacío o algún valor predeterminado
		return proceso_vacio;
	}

	Proceso proceso_aux;
	proceso_aux.tam = p->datos->tam;
	proceso_aux.ordenllegada = p->datos->ordenllegada;
	proceso_aux.quantum = p->datos->quantum;

	return proceso_aux;
}

// Función para eliminar el nodo de la cabecera de una cola
Cola* eliminarCabeceraCola(Cola* p) {
	if (p == nullptr) {
		return nullptr;  // No hay nada que eliminar en una cola vacía
	}

	Cola* aux = p;  // Se crea un puntero auxiliar para apuntar al nodo que se va a eliminar

	if (aux->der != nullptr) {
		p = p->der;  // Avanza la cabecera de la cola al siguiente nodo si existe
	}
	else {
		p = nullptr;  // Si no hay más nodos, se establece la cola como vacía
	}

	delete aux;  // Libera la memoria del nodo eliminado
	return p;  // Devuelve la nueva cola después de la eliminación
}


void quita(Cola* aux) {
	if (aux->datos != nullptr) {
		delete aux->datos;  // Liberar la memoria de la estructura Proceso
	}
	delete aux;  // Liberar la memoria del nodo
}

int tamProceso(Cola* q) {
	// Manejo de error o valor predeterminado si la cola está vacía o el nodo siguiente es nulo
	return q->datos->tam; // O cualquier otro valor predeterminado que tenga sentido en tu aplicación
}


// Función para permitir al usuario seleccionar la velocidad del programa
int seleccionVelocidad() {
	int op;  // Variable para almacenar la opción seleccionada por el usuario
	int velocidad = 0;  // Variable que almacenará la velocidad correspondiente a la opción seleccionada

	// Bucle para mostrar el menú hasta que se seleccione una opción válida
	do {
		system("cls");  // Limpia la pantalla
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(0, 0, 118, 29);  // Dibuja un recuadro en la pantalla
		SetConsoleTextAttribute(hConsole, colorTitulo);
		centrarTexto("Seleccione la velocidad del programa: ", 8);
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(25, 12, 90, 22);
		SetConsoleTextAttribute(hConsole, colorTexto);
		centrarTexto("1) Lento....................... (5 seg cada proceso)", 14);
		centrarTexto("2) Normal.......................(3 seg cada proceso)", 17);
		centrarTexto("3) Rapido..............(se ejecutan todas las tareas)", 20);
		SetConsoleTextAttribute(hConsole, colorPregunta);
		gotoxy(45, 25); cout << "Selecciona una opcion : ";
		cin >> op;  // Lee la opción seleccionada por el usuario

		// Evalúa la opción seleccionada y asigna la velocidad correspondiente
		switch (op) {
		case 1: {
			velocidad = 5;  // Velocidad lenta
			break;
		}
		case 2: {
			velocidad = 3;  // Velocidad normal
			break;
		}
		case 3: {
			velocidad = 0;  // Velocidad rápida (sin espera)
			break;
		}
		default: {
			centrarTexto("                             ", 25);
			SetConsoleTextAttribute(hConsole, colorError);
			centrarTexto("Opcion Invalida. Favor de seleccionar una de las opciones correctas.", 25);
			sleep_for(duracion);  // Espera antes de limpiar el mensaje de error
			break;
		}
		}
	} while (op >= 4 && op > 0);  // Continúa el bucle si la opción no es válida

	return velocidad;  // Devuelve la velocidad seleccionada
}


// Función para permitir al usuario seleccionar el tamaño de la memoria
void seleccionTam(Tamanios& sistema) {
	int op;         // Variable para almacenar la opción seleccionada por el usuario
	int tamMM = 0;  // Variable que almacenará el tamaño de la memoria correspondiente a la opción seleccionada

	// Bucle para mostrar el menú hasta que se seleccione una opción válida
	do {
		system("cls");  // Limpia la pantalla
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(0, 0, 118, 29);  // Dibuja un recuadro en la pantalla
		SetConsoleTextAttribute(hConsole, colorTitulo);
		centrarTexto("TAMANIO DE LA MEMORIA", 8);
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(40, 12, 75, 22);
		SetConsoleTextAttribute(hConsole, colorTexto);
		centrarTexto("1 MB ................ 1", 14);
		centrarTexto("4 MB ................ 2", 17);
		centrarTexto("8 MB ................ 3", 20);

		SetConsoleTextAttribute(hConsole, colorPregunta);
		gotoxy(45, 25); cout << "Selecciona una opcion : ";
		cin >> op;  // Lee la opción seleccionada por el usuario

		// Evalúa la opción seleccionada y asigna el tamaño de la memoria correspondiente
		switch (op) {
		case 1: {
			tamMM = 1 * 1024;  // Tamaño de la memoria: 1 MB
			break;
		}
		case 2: {
			tamMM = 4 * 1024;  // Tamaño de la memoria: 4 MB
			break;
		}
		case 3: {
			tamMM = 8 * 1024;  // Tamaño de la memoria: 8 MB
			break;
		}
		default: {
			centrarTexto("                             ", 25);
			SetConsoleTextAttribute(hConsole, colorError);
			centrarTexto("Opcion Invalida. Favor de seleccionar una de las opciones correctas.", 25);
			sleep_for(duracion);  // Espera antes de limpiar el mensaje de error
			break;
		}
		}
	} while (op >= 4 && op > 0);  // Continúa el bucle si la opción no es válida

	sistema.tamMM = tamMM;  // Tamaño de la memoria seleccionado a la estructura 'sistema'
}


// Función para permitir al usuario ingresar parámetros relacionados con el tamaño de memoria y quantum
void menuParametros(Tamanios& sistema) {
	// Bucle para el tamaño máximo de memoria de los procesos
	do {
		system("cls");  // Limpia la pantalla
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(0, 0, 118, 29);  // Dibuja un recuadro en la pantalla
		SetConsoleTextAttribute(hConsole, colorTitulo);
		centrarTexto("DATOS DEL PROCESO", 10);
		SetConsoleTextAttribute(hConsole, colorTexto);
		centrarTexto("Tamanio maximo de memoria de los procesos (en kb): ", 13);
		cin >> sistema.tamMP;

		// Verifica que el tamaño de la partición del proceso sea válido
		if (sistema.tamMP < TAMMIN || sistema.tamMP >= sistema.tamMM) {
			system("cls");
			SetConsoleTextAttribute(hConsole, colorError);
			cout << endl << "\t\t\tTamano de particion: invalido \n\t\t\t";
			SetConsoleTextAttribute(hConsole, colorTexto);
			cout << "Ingresar nuevo valor : ";
			cin >> sistema.tamMP;
		}
	} while (sistema.tamMP < TAMMIN || sistema.tamMP > sistema.tamMM);

	// Bucle para el valor máximo del quantum de los procesos
	do {
		system("cls");  // Limpia la pantalla
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(0, 0, 118, 29);  // Dibuja un recuadro en la pantalla
		SetConsoleTextAttribute(hConsole, colorTitulo);
		centrarTexto("DATOS DEL PROCESO", 10);
		SetConsoleTextAttribute(hConsole, colorTexto);
		centrarTexto("Valor maximo para el quantum de los Procesos:  ", 13);
		cin >> sistema.numQuan;

		// Verifica que el valor del quantum sea válido
		if (sistema.numQuan <= 0) {
			SetConsoleTextAttribute(hConsole, colorError);
			cout << "Valor de quantum invalido,";
			SetConsoleTextAttribute(hConsole, colorTexto);
			cout << "ingresa nuevo valor" << endl;
		}
		cout << "\t\t\t\t\t";
	} while (sistema.numQuan <= 0);

	// Bucle para el valor del quantum del sistema
	do {
		system("cls");  // Limpia la pantalla
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(0, 0, 118, 29);  // Dibuja un recuadro en la pantalla
		SetConsoleTextAttribute(hConsole, colorTitulo);
		centrarTexto("DATOS DEL PROCESO", 10);
		SetConsoleTextAttribute(hConsole, colorTexto);
		centrarTexto("Valor para el quantum del Sistema:  ", 13);
		cin >> sistema.quantum_Sistema;

		// Verifica que el valor del quantum del sistema sea válido
		if (sistema.quantum_Sistema <= 0) {
			SetConsoleTextAttribute(hConsole, colorError);
			cout << "\t\tNumero de quantum invalido,";
			SetConsoleTextAttribute(hConsole, colorTexto);
			cout << "ingresa nuevo valor" << endl;
		}
		cout << "\t\t\t\t\t";
	} while (sistema.numQuan <= 0);

	mostrarParametros(sistema);  // Muestra los parámetros ingresados
}


// Función para mostrar los parámetros establecidos y permitir al usuario confirmar o cambiar los valores
void mostrarParametros(Tamanios& sistema) {
	char resp = 0;  // Variable para almacenar la respuesta del usuario

	// Limpia la pantalla y dibuja un recuadro
	system("cls");
	SetConsoleTextAttribute(hConsole, colorMargen);
	recuadro(0, 0, 118, 29);

	// Muestra el título
	SetConsoleTextAttribute(hConsole, colorTitulo);
	centrarTexto("PARAMETROS ESTABLECIDOS", 10);

	// Muestra y obtiene la confirmación del tamaño máximo de memoria de los procesos
	SetConsoleTextAttribute(hConsole, colorTitulo);
	gotoxy(23, 13); cout << "Tamanio maximo de memoria de los procesos (en kb): ";
	SetConsoleTextAttribute(hConsole, colorTexto);
	cout << sistema.tamMP;

	// Muestra y obtiene la confirmación del valor del quantum de los procesos
	SetConsoleTextAttribute(hConsole, colorTitulo);
	gotoxy(23, 15); cout << "Valor para el quantum de los procesos: ";
	SetConsoleTextAttribute(hConsole, colorTexto);
	cout << sistema.numQuan;

	// Muestra y obtiene la confirmación del valor del quantum del sistema
	SetConsoleTextAttribute(hConsole, colorTitulo);
	gotoxy(23, 17); cout << "Valor para el quantum del Sistema:  ";
	SetConsoleTextAttribute(hConsole, colorTexto);
	cout << sistema.quantum_Sistema;

	// Solicita al usuario confirmar o cambiar los parámetros
	SetConsoleTextAttribute(hConsole, colorPregunta);
	centrarTexto("Continuar con estos parametros? [S/N]  ", 22);
	gotoxy(78, 22); cin >> resp;

	// Si el usuario confirma, la función simplemente retorna. Si no, se vuelve al menú de parámetros
	if (resp == 'S' || resp == 's') {
		return;
	}
	else {
		menuParametros(sistema);
	}
}


	//Función para imprimir el Menu Principal
void menuPrincipal() {
	SetConsoleTextAttribute(hConsole, colorTitulo);
	recuadro(0, 0, 118, 29);
	sleep_for(duracion2);
	SetConsoleTextAttribute(hConsole, colorTexto);
	recuadro(10, 3, 108, 26);
	sleep_for(duracion2);
	SetConsoleTextAttribute(hConsole, colorTitulo);
	recuadro(20, 6, 98, 23);
	sleep_for(duracion1);
	SetConsoleTextAttribute(hConsole, colorTexto);
	recuadro(30, 9, 88, 20);
	sleep_for(duracion1);
	SetConsoleTextAttribute(hConsole, colorTitulo);
	recuadro(40, 12, 73, 17);
	SetConsoleTextAttribute(hConsole, colorTexto);
	centrarTexto("SIMULADOR DE TECNICAS", 13);
	centrarTexto("DE COLOCACION DE MEMORIA", 14);
	centrarTexto("PLANIFICACION DE PROCESOS", 15);
	centrarTexto("BUDDY SYSTEM & ROUND ROBIN", 16);
}

// Función para mostrar una caja de respuesta en la pantalla y obtener la decisión del usuario
int cajaResp(int modo) {
	SetConsoleTextAttribute(hConsole, colorMargen);

	// Dibuja una caja en la pantalla
	recuadro(60, 25, 116, 28);

	char seguirResp;
	int seguir = 0;

	SetConsoleTextAttribute(hConsole, colorPregunta);

	// Modo 1: Proceso siguiente
	if (modo == 1) {
		gotoxy(75, 27); cout << "Proceso siguiente";
		sleep_for(duracion1);

		// Muestra puntos adicionales si hay más de un proceso
		if (numeroP > 1) {
			gotoxy(93, 27); cout << ".";
			sleep_for(duracion1);
			gotoxy(95, 27); cout << ".";
			sleep_for(duracion1);
			gotoxy(97, 27); cout << ".";
		}
		else {
			sleep_for(duracion2);
			gotoxy(93, 27); cout << ".";
			sleep_for(duracion1);
			gotoxy(95, 27); cout << ".";
			sleep_for(duracion1);
			gotoxy(97, 27); cout << ".";
		}

		seguir = 1;
	}
	// Modo 2: Pregunta al usuario si desea seguir con la ejecución
	else {
		gotoxy(75, 27); cout << "Desea seguir con la ejecucion? S/N: ";
		fflush(stdin);
		cin >> seguirResp;

		// Verifica la respuesta del usuario
		if (seguirResp == 'S' || seguirResp == 's') {
			seguir = 1;  // Si el usuario elige 'S' o 's', la ejecución continúa
		}
		else {
			seguir = 0;  // Si elige otra opción, la ejecución se detiene
		}
	}

	return seguir;  // Retorna la decisión del usuario
}


// Función para permitir al usuario seleccionar el modo de ejecución por los procesos
int seleccionModo() {
	int op;    // Variable para almacenar la opción seleccionada por el usuario
	int modo = 0;  // Variable que almacenará el modo de ejecución correspondiente a la opción seleccionada

	// Bucle para mostrar el menú hasta que se seleccione una opción válida
	do {
		system("cls");  // Limpia la pantalla
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(0, 0, 118, 29);  // Dibuja un recuadro en la pantalla
		SetConsoleTextAttribute(hConsole, colorTitulo);
		centrarTexto("Seleccione el modo de ejecucion por los procesos: ", 8);
		SetConsoleTextAttribute(hConsole, colorMargen);
		recuadro(25, 12, 90, 22);
		SetConsoleTextAttribute(hConsole, colorTexto);
		centrarTexto("Procesamiento Automatico............................... (1)", 14);
		centrarTexto("Pausa por cada ciclo de proceso........................ (2)", 17);

		SetConsoleTextAttribute(hConsole, colorPregunta);
		gotoxy(45, 25); cout << "Selecciona una opcion : ";
		cin >> op;  // Lee la opción seleccionada por el usuario

		// Evalúa la opción seleccionada y asigna el modo de ejecución correspondiente
		switch (op) {
		case 1: {
			modo = 1;  // Modo de procesamiento automático
			break;
		}
		case 2: {
			modo = 2;  // Modo de pausa por cada ciclo de proceso
			break;
		}
		default: {
			centrarTexto("                             ", 25);
			SetConsoleTextAttribute(hConsole, colorError);
			centrarTexto("Opcion Invalida. Favor de seleccionar una de las opciones correctas.", 25);
			sleep_for(duracion);  // Espera antes de limpiar el mensaje de error
			break;
		}
		}
	} while (op >= 3 && op > 0);  // Continúa el bucle si la opción no es válida

	return modo;  // Devuelve el modo de ejecución seleccionado
}


// Función para imprimir un texto centrado en una posición específica en la pantalla
void centrarTexto(string texto, int y) {
	int x = (115 - texto.length()) / 2;  // Calcula la posición x para centrar el texto
	gotoxy(x, y);  // Llama a la función gotoxy para posicionar el cursor en la posición calculada
	cout << texto;  // Imprime el texto centrado en la pantalla
}


// Función para posicionar el cursor en una ubicación específica en la pantalla
void gotoxy(int x, int y) {
	HANDLE hCon;  // Identificador del manipulador de la consola
	COORD dwPos;  // Estructura que representa una posición en la consola

	dwPos.X = x;  // Establece la posición x en la estructura COORD
	dwPos.Y = y;  // Establece la posición y en la estructura COORD

	hCon = GetStdHandle(STD_OUTPUT_HANDLE);  // Obtiene el manipulador de la consola estándar
	SetConsoleCursorPosition(hCon, dwPos);  // Mueve el cursor a la posición especificada en la consola
}


// Función para dibujar un recuadro en la pantalla con esquinas y bordes
void recuadro(int x1, int y1, int x2, int y2) {
	// Dibuja los bordes horizontales superior e inferior del recuadro
	for (int i = x1; i <= x2; i++) {
		gotoxy(i, y1);
		cout << char(205);  // Caracter de línea horizontal
		gotoxy(i, y2);
		cout << char(205);  // Caracter de línea horizontal
	}

	// Dibuja los bordes verticales izquierdo y derecho del recuadro
	for (int i = y1; i <= y2; i++) {
		gotoxy(x1, i);
		cout << char(186);  // Caracter de línea vertical
		gotoxy(x2, i);
		cout << char(186);  // Caracter de línea vertical
	}

	// Dibuja las esquinas del recuadro
	gotoxy(x1, y1);
	cout << char(201);  // Caracter de esquina superior izquierda
	gotoxy(x1, y2);
	cout << char(200);  // Caracter de esquina inferior izquierda
	gotoxy(x2, y1);
	cout << char(187);  // Caracter de esquina superior derecha
	gotoxy(x2, y2);
	cout << char(188);  // Caracter de esquina inferior derecha
}
