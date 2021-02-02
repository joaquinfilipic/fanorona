typedef struct{
		int posX;
		int posY;
		int ** movimientos;
		int cantmovs;
			}tipoF;
/*Guarda la posición de la ficha encontrada al azar y las direcciones en las que puede moverse.
**Sirve únicamente para el juego contra la computadora*/
struct infoTablero{
		char **Mapa;
		int Fil;
		int Col;
			};
/*Guarda la información del tablero*/
struct infoRecorrido{
		int **recorrido;
		int dimrecorrido;
			};
/*Guarda la información del recorrido que realiza una ficha en un movimiento de capturas sucesivas*/
typedef struct infoTablero * tipoT;
typedef struct infoRecorrido * tipoR;

int guardarPartida(int modojuego, int turno, tipoT tablero, char * nombre);
/*Recibe como parámetros de entrada:
** -modojuego: indica si juegan 2 jugadores o 1 jugador contra la Pc
** -turno: indica a quién le toca mover
** -tablero.
** -nombre: es el nombre con el que se va a guardar el archivo
Devuelve en su nombre 1 si hubo error y 0 si se pudo guardar correctamente*/

int recuperarPartida(tipoT tablero, int * modojuego, int * turno, int fichas[], char * nom);
/*Recibe nom: el nombre del archivo que contiene la informacion de una partida que fue guardada.
**Devuelve en parámetros de salida, la información del tablero, modojuego, turno y la cantidad de fichas de cada jugador.
**Devuleve en su nombre 1 si hubo error y 0 si se cargo correctamente*/

void generarTablero( tipoT tablero );
/*Distribuye las fichas en un mapa de dimensiones ya establecidas*/

void aumentaRecorrido( tipoR rec, int orig[], int dest[], int *error );
/*Recibe: -Un recorrido, que contiene las posiciones por las que pasó una ficha en su movimiento de captura.
** -Un vector con la posición de origen, para el caso de que sea la primer captura del turno.
** -Un vector con la posición de destino, que se agrega al final del recorrido.
** -Un flag para indicar si hubo algun error en la función.*/

void disminuyeRecorrido(tipoR rec);	/*Para el caso del UNDO*/
/*Remueve del final del recorrido, la última posición*/

void liberaRecorrido( tipoR rec );
/*Usada para liberar el espacio reservado para el recorrido, una vez finalizado el turno.*/

int controlaRecorrido( const tipoR rec, int orig[], int dest[], int chequeaorig );
/*Todos sus parámetros son de entrada.
**Verifica que la posición de destino de una fichas no esté en su recorrido de capturas de ese turno.
**Chequeaorig indica si se debe controlar, o no, que la posición de origen sea igual a la última posición de destino del movimiento
**Devuelve en su nombre 1 si el movimiento es válido, y 0, si no lo es*/

void elegirFichaAzar(int orig[], int dest[], const tipoT tablero, const tipoR rec, int *errormemoria);
/*Recibe el tablero y el recorrido.
**Devuelve en orig y dest, el movimiento a realizar elegido al azar, luego de verificar que es válido*/

int esEmpate( const tipoT tablero, char turno, const tipoR rec );
/*Verifica si se da la situación de empate. Los 3 parámetros son de entrada.
**Devuelve en su nombre 1 si hay empate, o 0 en caso de que no haya*/

void backup( char ** Mapa, int Fil, int Col, char** Aux );
/*Fil y Col son parámetros de entrada e indican las dimensiones del tablero.
**Mapa es de entrada y Aux de salida.
**Guarda un auxiliar del mapa en aux. En el caso de realizar Undo, se invierten Mapa y Aux al invocar.*/

int movida( const tipoT tablero, char* turno, int orig[], int dest[], char WoA, int fichas[], const tipoR rec, int * fichascomidas );
/*Son todos parámetros de entrada menos: -fichascomidas, que indica la cantidad de fichas que fueron capturadas en el movimiento.
** -Fichas: guarda la cantidad de fichas restantes de cada jugador.
**tablero, turno, rec, orig, dest, son los parámetros necesarios para realizar el movimiento.
**WoA indica si se quiere capturar por acercamiento o alejamiento. En el caso de no especificar, vale \0.
**Devuelve en su nombre el resultado del movimiento, que puede ser:
** -Sigueturno: Tiene capturas disponible con esa ficha.
** -Terminoturno: Ya no puede realizar mas capturas.
** -Terminojuego: algún jugador se quedo sin fichas.
** -Jugadainvalida: hubo algún error en el movimiento elegido.*/
