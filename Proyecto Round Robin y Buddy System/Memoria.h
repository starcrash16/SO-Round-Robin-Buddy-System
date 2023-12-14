#include <iostream>
#include <windows.h>
#include <time.h>
#ifndef MEMORIA_H
#define MEMORIA_H
#define TAMMIN 32
using namespace std;

typedef int tipo;					//Definimos tipo = int para una mayor facilidad

typedef struct Elemento { 			//Estructura para los procesos
	tipo dato; 						// Tamaño de cada nodo del sistema de memoria
	bool ocupado;					// Evalua si es que hay un proceso en el nodo
	tipo frag;						// Hueco del proceso
	tipo quantum;					// Quantum del proceso
	tipo idPro;						// ID del proceso (Numero del proceso)
	Elemento* izq; 					// Puntero que apunta a la izquierda del nodo
	Elemento* der; 					// Puntero que apunta a la derecha del nodo
	int ordenLlegada;
} Nodo;


class Memoria { 					// Clase para lista
public:

	Nodo* inicio; 					// Puntero al inicio de la lista
	Nodo* fin; 						// Puntero al final de la lista
	Nodo* actual;
	// Constructor

	Memoria(void); 					// Constructor por defecto
	Memoria(tipo); 					// Constructor con parametros

	// Metodos
	Nodo* crearNodo(tipo x, Nodo* i, Nodo* d);	// Metodo para crear nodo
	
	//Metodos
	void mostrarProceso(int, int, int);

	// Metodos para insertar elemento
	void insertaAntesDe(tipo x, Nodo*);

	// Mostrar Info
	void mostrarLista(int); 			//Metodo mostrar de infoMemoria a fin
	void mostrarProcesos();
	void mostrarProcesosAlReves(Nodo*);
	void buscarProceso(int, int, int);

	// Metodos para insertar Nodos
	void insertaInicio(tipo x);
	void insertaFinal(tipo x);
	void insertaDespuesDe(tipo x, tipo y);

	// Cuestion de memoria
	void eliminaActual(Nodo*);
	void buscaEspacio(int, int, int, int);
	void partir(Nodo*, int, int, int, int);
	void guardarInfo(Nodo*, int, int, int, int);
	void unirMemoria();
	void sacarProceso(int id);
	
	// INFORMACION PROCESOS
	float porcentajeMemoria(int);
	bool siEntra(int);
	bool esVacia();
	void elimina(tipo x);

	~Memoria();						// Destructor
};


// Constructor por defecto
Memoria::Memoria(void) {
	fin = inicio = actual = NULL;
}

// Constructor por general con parametro
Memoria::Memoria(tipo dato) {
	fin = inicio = actual = crearNodo(dato, NULL, NULL);
}

bool Memoria::esVacia() {
	if (inicio == NULL) {
		return true;
	}
	return false;
}

// Función para crear un nuevo nodo con el valor especificado y punteros izquierdo y derecho dados
Nodo* Memoria::crearNodo(tipo x, Nodo* i, Nodo* d) {
	Nodo* nuevo;  // Crea un nuevo nodo
	nuevo = new(Nodo);  // Asigna memoria para el nuevo nodo
	nuevo->dato = x;  // Asigna el valor al nuevo nodo
	nuevo->ocupado = false;  // Inicializa el estado de ocupación como falso
	nuevo->idPro = -1;  // Inicializa el ID del proceso como -1
	nuevo->izq = i;  // Asigna el puntero izquierdo al proporcionado
	nuevo->der = d;  // Asigna el puntero derecho al proporcionado
	nuevo->ordenLlegada = -1;  // Inicializa el orden de llegada como -1
	return nuevo;  // Devuelve el nuevo nodo creado
}


// Función para mostrar información de un proceso específico
void Memoria::mostrarProceso(int quantumSis, int idProc, int tamProc) {
	int k = 0;
	Nodo* aux = inicio;

	// Mueve el puntero al inicio de la lista
	while (aux->izq != nullptr) {
		aux = aux->izq;
	}

	// Busca y muestra la información del proceso con el ID y tamaño dados
	buscarProceso(idProc, quantumSis, tamProc);
}



void Memoria::mostrarProcesosAlReves(Nodo* q) {
	if (q == NULL) {
		return; // Llegamos al final de la lista
	}

	// Llamada recursiva para avanzar al siguiente nodo
	mostrarProcesosAlReves(q->der);

	// Imprimir la información del nodo actual
	if (q->ocupado) {
		cout << "[" << q->idPro << "," << q->frag << "(" << q->dato << ")" << "," << q->quantum << "] ";
	}
}

void Memoria::mostrarProcesos() {
	Nodo* q = inicio;
	while (q->izq != nullptr) {
		q = q->izq;
	}
	mostrarProcesosAlReves(q);
	if (q == nullptr) {
		cout << "Cola de procesos vacia." << endl;

	}
}


// Función para buscar un proceso en la memoria por su número de proceso
void Memoria::buscarProceso(int numeroProceso, int quantumSis, int tamProc) {
	int k;
	Nodo* q = inicio;

	// Recorre la lista hasta el final
	for (k = 0; q; q = q->der) {
		if (q->idPro == numeroProceso) {
			// Muestra la información del proceso
			cout << "[" << q->idPro << "," << q->frag << "(" << q->dato << ")" << "," << q->quantum << "] ";

			// Actualiza el quantum del proceso
			q->quantum = q->quantum - quantumSis;

			return;
		}
		k++;
	}
}


// Función para mostrar la lista enlazada que representa la memoria
void Memoria::mostrarLista(int impresion) {
	int k;  // Variable para contar la posición del nodo en la lista
	Nodo* q = inicio;  // Puntero para recorrer la lista de nodos

	// Verifica si la cola de procesos está vacía
	if (q->izq == nullptr && q->der == NULL) {
		// Cola de procesos vacía, no se imprime nada
	}

	// Recorre la lista e imprime la información de cada nodo
	for (k = 0; q; q = q->der) {
		// Si se supera la décima posición y la impresión es del tipo 1 o 2, no se realiza la impresión
		if (k > 9 && (impresion == 1 || impresion == 2)) {
			// No se realiza la impresión
		}
		else if (k > 9 && impresion == 2) {
			// No se realiza la impresión
		}

		// Verifica si el nodo está ocupado y muestra la información correspondiente
		if (q->ocupado) {
			cout << "[" << q->idPro << "," << q->frag << "(" << q->dato << ")" << "," << q->quantum << "] ";
		}
		else {
			cout << "[" << "0" << "," << q->dato << "," << "0" << "] ";  // Nodo no ocupado, muestra información predeterminada
		}

		k++;  // Incrementa la posición del nodo en la lista
	}
}


// Función para insertar un nuevo nodo al inicio de la lista enlazada que representa la memoria
void Memoria::insertaInicio(tipo tam) {
	Nodo* nuevo = crearNodo(tam, NULL, NULL);  // Crea un nuevo nodo con el tamaño especificado
	nuevo->der = inicio;  // El nuevo nodo apunta al inicio por la derecha
	inicio->izq = nuevo;  // El nodo actualmente en el inicio apunta al nuevo nodo por la izquierda
	inicio = nuevo;  // Se actualiza el inicio para que apunte al nuevo nodo
}


// Función para insertar un nuevo nodo al final de la lista enlazada que representa la memoria
void Memoria::insertaFinal(tipo x) {
	Nodo* nuevo = crearNodo(x, NULL, NULL);  // Crea un nuevo nodo con el valor especificado
	nuevo->izq = fin;  // El nuevo nodo apunta al nodo final por la izquierda
	fin->der = nuevo;  // El nodo final actual apunta al nuevo nodo por la derecha
	fin = nuevo;  // Se actualiza el nodo final para que apunte al nuevo nodo
}


void Memoria::elimina(tipo x) {
	Nodo* q = inicio; 							//Puntero al inicio
	if (inicio->dato == x && !q->ocupado) { 		//Si el dato a eliminar esta al inicio
		inicio = inicio->der; 					//Inicio se recorre hacia la derecha
	}
	else {
		while (q->der != NULL) { 					//Recorre q mientras q->der no sea NULL
			q = q->der;
			if (q->dato == x && !q->ocupado) { 	//si encuentra el dato
				if (q->der == NULL) {
					q = q->izq; 					//regresa q a la izquierda
					fin = q; 						//se recorre el final hacia q
					q->der = NULL; 				//q->der apunta a NULL
				}
				else {
					q->der->izq = q->izq; 		//el puntero de q a la derecha apunta a q->izq
					q->izq->der = q->der; 		//el puntero de q a la izquierda apunta a q->der
					q = q->der; 					//q se recorre a la derecha
				}
				break; 							//se rompe el ciclo
			}
		}
	}
}



// Función para insertar un nuevo nodo después de un nodo con un valor específico en la lista enlazada
void Memoria::insertaDespuesDe(tipo x, tipo y) {
	Nodo* nuevo = crearNodo(x, NULL, NULL);  // Crea un nuevo nodo con el valor especificado
	Nodo* q = inicio;  // Puntero al inicio

	while (q->der != NULL) {  // Recorre la lista mientras el nodo actual tenga un siguiente nodo
		if (q->dato == y) {  // Si el valor del nodo actual coincide con el valor específico
			nuevo->izq = q;  // El nuevo nodo por la izquierda apunta al nodo actual
			q->der->izq = nuevo;  // El nodo a la derecha del nodo actual apunta al nuevo nodo por la izquierda
			nuevo->der = q->der;  // El nuevo nodo por la derecha apunta a donde apunta el nodo actual por la derecha
			q->der = nuevo;  // El nodo actual apunta al nuevo nodo por la derecha
			break;
		}
		q = q->der;  // Se mueve al siguiente nodo en la lista
	}

	if (q->dato == y && q->der == NULL) {  // Si el valor está al final de la lista
		insertaFinal(x);  // Llama a la función "insertaFinal" para agregar el nuevo nodo al final
	}
}



// Función para insertar un nuevo nodo antes de un nodo específico en la lista enlazada
void Memoria::insertaAntesDe(tipo tam, Nodo* aux) {
	Nodo* nuevo = crearNodo(tam, NULL, NULL);  // Crea un nuevo nodo con el tamaño especificado
	Nodo* q = inicio;  // Puntero para recorrer la lista de nodos

	// Verifica si el nodo a insertar antes es el primer nodo (inicio)
	if (q == aux) {
		insertaInicio(tam);  // Llama a la función insertaInicio en caso de ser el primer nodo
	}
	else {
		// Recorre la lista hasta encontrar el nodo antes del cual se insertará el nuevo nodo
		while (q->der != nullptr) {
			q = q->der;					// Se mueve al siguiente nodo en la lista
			if (q == aux) {				// Si encuentra el nodo antes del cual se insertará
				nuevo->der = q;			// El nuevo nodo apunta al nodo actual (q) por la derecha
				q->izq->der = nuevo;	// El nodo anterior al actual (q) apunta al nuevo nodo por la derecha
				nuevo->izq = q->izq;	// El nuevo nodo apunta al nodo anterior al actual (q) por la izquierda
				q->izq = nuevo;			// El nodo actual (q) apunta al nuevo nodo por la izquierda
				break;					// Sale del bucle una vez que se inserta el nuevo nodo
			}
		}
	}
}


// Función para eliminar un nodo específico de la lista enlazada
void Memoria::eliminaActual(Nodo* aux) {
	Nodo* q = inicio;  // Puntero para recorrer la lista de nodos

	// Verifica si el nodo a eliminar está al inicio de la lista
	if (inicio == aux) {
		inicio = inicio->der;  // El inicio se recorre hacia la derecha
	}
	else {
		// Recorre la lista hasta encontrar el nodo a eliminar
		while (q->der != NULL) {
			q = q->der;
			if (q == aux) {
				// Verifica si el nodo a eliminar está al final de la lista
				if (q->der == NULL) {
					q = q->izq;
					fin = q;  // Actualiza el puntero fin al nodo anterior
					q->der = NULL;  // Desconecta el nodo a eliminar
				}
				else {
					q->der->izq = q->izq;
					q->izq->der = q->der;  // Conecta los nodos anterior y siguiente al nodo a eliminar
					q = q->der;  // Avanza al siguiente nodo
				}
				break;  // Sale del bucle una vez que se elimina el nodo
			}
		}
	}
}


// Función para buscar un espacio en la memoria para almacenar un proceso
void Memoria::buscaEspacio(int tamProceso, int tamMM, int id, int quantum) {
	Nodo* aux = inicio;  // Puntero para recorrer la lista de nodos

	while (aux != nullptr) {
		// Verifica si la partición actual es lo suficientemente grande y no está ocupada
		if (aux->dato >= tamProceso && !aux->ocupado) {
			// Verifica si se puede dividir la partición actual para acomodar el proceso
			if ((aux->dato / 2) >= tamProceso) {
				partir(aux, aux->dato, tamProceso, id, quantum);  // Divide la partición actual
				break;
			}
			else {
				guardarInfo(aux, tamProceso, tamMM, id, quantum);  // Almacena el proceso en la partición actual
				break;
			}
		}
		aux = aux->der;  // Avanza al siguiente nodo en la lista
	}
}


// Función para dividir una partición de memoria
void Memoria::partir(Nodo* aux, int tamEsp, int tamProceso, int id, int quantum) {
	int tam = tamEsp / 2;  // Calcula la mitad del tamaño de la partición

	// Si el tamaño del proceso es mayor que la mitad del espacio
	if (tamProceso > tam) {
		if (tam < TAMMIN) {
			guardarInfo(aux, tamProceso, 0, id, quantum);  // Guarda la información del proceso en la partición
			actual->dato = aux->dato = TAMMIN;  // Establece el tamaño de la partición como el tamaño mínimo permitido
			return;
		}
		else {
			guardarInfo(aux, tamProceso, 0, id, quantum);  // Guarda la información del proceso en la partición
			actual->dato = aux->dato;  // Copia el tamaño de la partición actual a la partición nueva
			return;
		}
	}
	// Si el tamaño del proceso es menor o igual que el espacio de la partición
	else if (tamProceso <= tamEsp) {
		insertaAntesDe(tam, aux);  // Inserta una nueva partición antes de la actual
		insertaAntesDe(tam, aux);  // Inserta en el nuevo nodo con el mismo tamaño pero vacio, al inicio del puntero
		aux = aux->izq->izq;  // Mueve el puntero auxiliar dos niveles hacia la izquierda
		eliminaActual(aux->der->der);  // Elimina la partición original (derecha)
		partir(aux, tam, tamProceso, id, quantum);  // Llamada recursiva para continuar dividiendo
	}
}


// Función para guardar información de un proceso en una partición de memoria
void Memoria::guardarInfo(Nodo* aux, int tamProceso, int tamMM, int id, int quantum) {
	// Asigna la información del proceso a las propiedades de la partición actual y la partición dada
	actual->frag = aux->frag = tamProceso;				// Tamaño del fragmento de memoria ocupado por el proceso
	actual->idPro = aux->idPro = id;					// ID del proceso asignado a la partición
	actual->quantum = aux->quantum = quantum;			// Valor del quantum asociado al proceso
	actual->ocupado = aux->ocupado = true;				// Marca la partición como ocupada
	actual->ordenLlegada = aux->ordenLlegada = 0;		// Inicializa el orden de llegada del proceso
}




// Función para unir bloques de memoria contiguos
void Memoria::unirMemoria() {
	Nodo* q = inicio;  // Puntero para recorrer la lista de nodos

	while (q->der != NULL) {
		// Verifica si hay dos bloques de memoria contiguos con el mismo tamaño
		if (q->dato == q->der->dato) {
			// Verifica que ambos bloques contiguos no estén ocupados
			if (!q->ocupado && !q->der->ocupado) {
				// Une los bloques contiguos duplicando su tamaño
				insertaAntesDe(q->dato * 2, q);
				elimina(q->dato);
				elimina(q->dato);
				// Llama recursivamente a la función para continuar buscando y uniendo bloques
				unirMemoria();
			}
		}
		q = q->der;  // Avanza al siguiente nodo en la lista
	}

	return;  // Retorna al finalizar la función
}


// Función para verificar si un bloque de memoria del tamaño dado puede ser alojado en la memoria
bool Memoria::siEntra(int aTam) {
	Nodo* aux = inicio;  // Puntero para recorrer la lista de nodos

	// Recorre la lista hasta encontrar un bloque de memoria que sea lo suficientemente grande y no esté ocupado
	while (aux != NULL) {
		if (aux->dato >= aTam && !aux->ocupado) {
			return true;  // Retorna verdadero si encuentra un bloque adecuado
		}
		aux = aux->der;  // Avanza al siguiente nodo en la lista
	}
	return false;  // Retorna falso si no encuentra un bloque adecuado
}


// Función para retirar un proceso de la memoria
void Memoria::sacarProceso(int id) {
	Nodo* q = inicio;  // Puntero para recorrer la lista de nodos

	// Recorre la lista hasta encontrar el nodo con el ID del proceso
	while (q != NULL) {
		if (q->idPro == id) {
			q->ocupado = false;  // Marca la partición de memoria como no ocupada
			q->idPro = -1;  // Restablece el ID del proceso a un valor no válido
			break;  // Sale del bucle una vez que se encuentra el nodo
		}
		q = q->der;  // Avanza al siguiente nodo en la lista
	}
}



// Función para calcular el porcentaje de memoria ocupada
float Memoria::porcentajeMemoria(int tamMM) {
	float valor = 100;  // Valor base para calcular el porcentaje
	float sub = 0, total = 0;  // Variables para almacenar resultados parciales
	Nodo* q = inicio;  // Puntero para recorrer la lista de nodos

	valor /= tamMM;  // Calcula el valor porcentual de cada unidad de memoria

	while (q != NULL) {
		if (q->ocupado) {  // Si la partición de memoria está ocupada
			sub = valor * q->dato;  // Calcula el porcentaje ocupado por la partición actual
			total += sub;  // Agrega el porcentaje parcial al total acumulado
		}
		q = q->der;  // Avanza al siguiente nodo en la lista
	}

	return total;  // Retorna el porcentaje total de memoria ocupada
}



// Destructor de la clase Memoria
Memoria::~Memoria() {
	if (!esVacia()) {  // Verifica si la lista no está vacía
		while (inicio) {  // Mientras exista un nodo en el inicio de la lista
			Nodo* temp = inicio;  // Crea una variable temporal apuntando al inicio actual
			inicio = inicio->der;  // Desplaza el inicio al siguiente nodo a la derecha
			delete temp;  // Libera la memoria del nodo eliminado
		}
	}
}


#endif
