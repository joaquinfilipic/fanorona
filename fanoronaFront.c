#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include "fanoronaBack.h"
#include "getnum.h"
#define J1 1 
#define J2 2
#define MAQUINA 3
#define SAVE 4
#define UNDO 5
#define QUIT 6
#define ERRORLECT 7
#define MOVER 8
#define JUGADAINVALIDA 9
#define SIGUETURNO 10
#define TERMINOTURNO 11
#define TERMINOJUEGO 12
#define NECESITOWOA 13
#define ERRORMEMORIA 14
#define BLOQUE 30

void fanorona()
{
	printf("\n\n");
	printf("  █████▒▄▄▄       ███▄    █  ▒█████   ██▀███   ▒█████   ███▄    █  ▄▄▄      \n");
	printf("▓██   ▒▒████▄     ██ ▀█   █ ▒██▒  ██▒▓██ ▒ ██▒▒██▒  ██▒ ██ ▀█   █ ▒████▄    \n");
	printf("▒████ ░▒██  ▀█▄  ▓██  ▀█ ██▒▒██░  ██▒▓██ ░▄█ ▒▒██░  ██▒▓██  ▀█ ██▒▒██  ▀█▄  \n");
	printf("░▓█▒  ░░██▄▄▄▄██ ▓██▒  ▐▌██▒▒██   ██░▒██▀▀█▄  ▒██   ██░▓██▒  ▐▌██▒░██▄▄▄▄██ \n");
	printf("░▒█░    ▓█   ▓██▒▒██░   ▓██░░ ████▓▒░░██▓ ▒██▒░ ████▓▒░▒██░   ▓██░ ▓█   ▓██▒\n");
	printf(" ▒ ░    ▒▒   ▓▒█░░ ▒░   ▒ ▒ ░ ▒░▒░▒░ ░ ▒▓ ░▒▓░░ ▒░▒░▒░ ░ ▒░   ▒ ▒  ▒▒   ▓▒█░\n");
	printf(" ░       ▒   ▒▒ ░░ ░░   ░ ▒░  ░ ▒ ▒░   ░▒ ░ ▒░  ░ ▒ ▒░ ░ ░░   ░ ▒░  ▒   ▒▒ ░\n");
}
void mostrarTablero( tipoT tablero )	/*En la matriz se guardan las posiciones, al mostrar el tablero se incluyen los pipes*/
{
	int i, j, ultimafila=0;
	printf("\n");
	printf("    ");
	for (i=0; i<tablero->Col; i++)
	{
		if (i<10)
			printf("%d   ", i);
		else
			printf("%d  ", i);
	}
	printf("\n\n");
	for (i=0; i<tablero->Fil; i++)
	{
		if( i<10 )
			printf("%d   ", i);
		else
			printf("%d  ", i);
		if( i==(tablero->Fil-1))
			ultimafila=1;
		for (j=0; j<tablero->Col; j++)
		{
			if (j==((tablero->Col)-1))
				printf("%c\n", tablero->Mapa[i][j]);
			else
				printf("%c - ", tablero->Mapa[i][j]);
		}
		for (j=0; (j<tablero->Col)&&(ultimafila==0); j++)
		{
			if(j==((tablero->Col)-1))
				printf("|\n");
			else
			{
				if( j==0 )
					printf("    | ");
				else
					printf("| ");
				if (((i+j)%2)==0)
					printf("\\ ");
				else
					printf("/ ");
			}
		}
	}
	printf("\n");
}
int leerWoA()
{
	int n;
	do
	{
		n=getint("Hay ambigüedad.\n1:Approach.\n2:Withdraw.\n");
	}while(n!=1 && n!=2 );
	return n;
}
int Leer(int orig[], int dest[], char * ambigua, char ** archivo)
{
	enum estado {COMIENZO=0, IMOVIMIENTO, MOVIMIENTONUM, MOVIMIENTOA, GUARDAR, DESHACER, SALIR};
	int salida=0, estado=COMIENZO, aux, flagmov=0, c, i=0;
	*ambigua='\0';
	while( salida!=ERRORLECT && (c=getchar())!='\n' )
	{
		switch(estado)
		{
			case COMIENZO:
						if(c=='M')
						estado=IMOVIMIENTO;
						else if(c=='S')
							estado=GUARDAR;
						else if(c=='U')
							estado=DESHACER;
						else if(c=='Q')
							estado=SALIR;
						else
							salida=ERRORLECT; /*comando inválido*/
						break;
			case IMOVIMIENTO:
						if((c=='[')&&(flagmov!=7))
						{
							estado=MOVIMIENTONUM;
							aux=0;
						}
						else if((c=='[')&&(flagmov==7))
						{
							estado=MOVIMIENTOA;
							salida=0;
						}
						else
							salida=ERRORLECT; /* comando inválido, debió ser [*/
						break;
			case MOVIMIENTONUM:
						if(isdigit(c))
						{
							if((flagmov==0)||(flagmov==1))
							{
								aux=aux*10+c-'0';
								flagmov=1;
							}
							else if((flagmov==2)||(flagmov==3))
							{
								aux = aux*10+c-'0';
								flagmov=3;
							}
							else if((flagmov==4)||(flagmov==5))
							{
								aux = aux*10+c-'0';
								flagmov=5;
							}
							else if((flagmov==6||flagmov==7))
							{
								aux = aux*10+c-'0';
								flagmov=7;
							}
						}
						else if(c==',')
						{
							if(flagmov==1)
							{
								orig[0] = aux;
								flagmov=2;
								aux=0;
							}
							else if(flagmov==5)
							{
								dest[0] = aux;
								flagmov=6;
								aux=0;
							}
							else if((flagmov==3)||(flagmov==7))
								salida=ERRORLECT; /*comando inválido, debió ser número o ]*/
							else
								salida=ERRORLECT; /*com inv, ningún num isertado*/
						}
						else if(c==']')
						{
							if(flagmov==3)
							{
								estado=IMOVIMIENTO;
								orig[1] = aux;
								flagmov=4;
							}
							else if(flagmov==7)
							{
								estado=IMOVIMIENTO;
								dest[1] = aux;
								salida=MOVER;
							}
							else if((flagmov==1)||(flagmov==5))
								salida=ERRORLECT; /*com inv, debió ser num o ,*/
							else
								salida=ERRORLECT; /*com inv, ningún num insertado*/
						}
						else
						{
							if ((flagmov==3)||(flagmov==7))
								salida=ERRORLECT; /*com inv, debió ser num o ]*/
							else if((flagmov==1)||(flagmov==5))
								salida=ERRORLECT; /*com, inv, debió ser num o ,*/
							else
								salida=ERRORLECT; /*com inv, debió ser num*/
						}
						break;
			case MOVIMIENTOA:
						if((c=='W')||(c=='A'))
						{
							*ambigua = c;
							flagmov=0;
						}
						else if(c==']')
						{
							if(flagmov==0)
								salida=MOVER;
							else
								salida=ERRORLECT; /*no insertó A o W*/
						}
						else
						{
							if(flagmov==7)
								salida=ERRORLECT; /*com inv, debe ser A o W*/
							else
								salida=ERRORLECT; /*debe terminar en ]*/
						}
						break;
			case GUARDAR:
						if(c=='A'&&flagmov==0)
							flagmov=1;
						else if(c=='V'&&flagmov==1)
							flagmov=2;
						else if(c=='E'&&flagmov==2)
							flagmov=3;
						else if(c==' '&&flagmov==3)
							flagmov=4;
						else if(flagmov==4)
						{
							salida=SAVE;
							if(i%BLOQUE==0)
							{
								*archivo=realloc(*archivo,(i+BLOQUE)*sizeof(char));
								if(*archivo==NULL)
									return ERRORMEMORIA;
							}
							(*archivo)[i]=c;
							i++;
						}
						else
							salida=ERRORLECT; /* SAVE?*/
						break;
			case DESHACER:
						if(c=='N'&&flagmov==0)
							flagmov=1;
						else if(c=='D'&&flagmov==1)
							flagmov=2;
						else if(c=='O'&&flagmov==2)
							salida=UNDO;
						else
							salida=ERRORLECT; /*UNDO?*/
						break;
			case SALIR:
						if(c=='U'&&flagmov==0)
							flagmov=1;
						else if(c=='I'&&flagmov==1)
							flagmov=2;
						else if(c=='T'&&flagmov==2)
							salida=QUIT;
						else
							salida=ERRORLECT; /*QUIT?*/
						break;
		}
	}
	if(salida==ERRORLECT)
		printf("Comando inválido\n");
	else if (salida==0)
		printf("Comando incompleto\n");
	while(  (salida==ERRORLECT) && getchar()!='\n'  )
		;
	(*archivo)[i]='\0';
	return salida;
}
int jugar(int turno1, int turno2, tipoT tablero, int partidarecuperada, int fichas[], int tocaturno)
{
	int TURNO=tocaturno, primerTJ1=1, primerTJ2=1, estadojuego=1, primeravez1=1, habilitarundo1=0, SoN;
	int i, lectura, lectura2, huboambiguedad=0, retornomovida, tamarchivo, errormemoria=0;
	int orig[2], dest[2], chequeaorig=1, fichascomidas, dimarch;
	char  **Aux, WoA='\0', cturno='O', * archivo=NULL, c;
	tipoR rec;
	archivo=malloc(sizeof(char));
	if(archivo==NULL)
		return ERRORMEMORIA;
	rec=malloc(sizeof(struct infoRecorrido));
	rec->recorrido=NULL;
	if(rec==NULL)
		return ERRORMEMORIA;
	if(tocaturno==2)
		cturno='X';
	if(partidarecuperada==0)
	{
		do
		{
			tablero->Fil=getint("\nIngrese cantidad de filas del tablero (impares, entre 3 y 19): ");
		}while(  tablero->Fil<3 ||  tablero->Fil>19 || (tablero->Fil%2!=1) );
		do
		{
			tablero->Col=getint("Ingrese cantidad de columnas del tablero (impares, entre 3 y 19): ");
		}while( tablero->Col<3 || tablero->Col>19 || (tablero->Col%2!=1) );
		fichas[0]=( tablero->Col*((tablero->Fil-1)/2) ) + ( (tablero->Col-1)/2);
		fichas[1]=fichas[0];
		tablero->Mapa=malloc(tablero->Fil*sizeof(char*));
		if(tablero->Mapa==NULL)
			return ERRORMEMORIA;
		for(i=0;i<tablero->Fil;i++)
		{
			tablero->Mapa[i]=malloc(tablero->Col*sizeof(char));
			if(tablero->Mapa[i]==NULL)
			{
				free(tablero->Mapa);
				return ERRORMEMORIA;
			}
		}
		generarTablero(tablero);
	}
	if(fichas[1]<( tablero->Col*((tablero->Fil-1)/2) ) + ( (tablero->Col-1)/2) )
		primerTJ1=0;
	if(fichas[0]<( tablero->Col*((tablero->Fil-1)/2) ) + ( (tablero->Col-1)/2) )
		primerTJ2=0;
	Aux=malloc(tablero->Fil*sizeof(char*));
	if(Aux==NULL)
		return ERRORMEMORIA;
	for(i=0;i<tablero->Fil;i++)
	{
		Aux[i]=malloc(tablero->Col*sizeof(char));
		if(Aux[i]==NULL)
		{
			free(tablero->Mapa);
			return ERRORMEMORIA;
		}
	}
	mostrarTablero(tablero);
	rec->dimrecorrido=0;	
	while(estadojuego)	/*Se sigue entrando al while hasta que se termine el juego, sea empate o gane alguien*/
	{
		if(huboambiguedad==0)
		{
			if (TURNO==MAQUINA)
			{
				printf("Turno PC (negras)\n");
				mostrarTablero(tablero);
				elegirFichaAzar(orig,dest, tablero, rec, &errormemoria);	/*Cpu elige ficha y movimiento al azar*/
				if (errormemoria)
					return ERRORMEMORIA;
				printf("Se mueve de [%d,%d] a [%d,%d]\n",orig[0],orig[1],dest[0],dest[1]);
				lectura=MOVER;
			}
			else
			{	
				printf("Turno Jugador %d ",TURNO);
				if (TURNO==J1)
					printf("(blancas)\n");
				else
					printf("(negras)\n");
				mostrarTablero(tablero);
				printf("Opciones:\n");
				printf("->M[Xi,Yi][Xf,Yf]([A/W])\n");
				if(turno2==MAQUINA)
					printf("->UNDO\n");
				printf("->SAVE archivo\n");
				printf("->QUIT\n\n");
				do
				{
					lectura=Leer(orig,dest,&WoA,&archivo);	/*Se pide que se intoduzca qué se desea hacer*/
					if(lectura==ERRORMEMORIA)
						return ERRORMEMORIA;
				}while(lectura==ERRORLECT || lectura==0);
			}
		}
		switch(lectura)
		{
			case MOVER:
					backup( tablero->Mapa,tablero->Fil,tablero->Col, Aux);	/*Hace backup para el undo*/
					if (   (rec->dimrecorrido>0)  &&  (!controlaRecorrido( rec, orig, dest, chequeaorig ))  )
						retornomovida=JUGADAINVALIDA;
					else
					{	
						retornomovida=movida( tablero, &cturno, orig, dest, WoA, fichas, rec, &fichascomidas);
						if( (retornomovida==SIGUETURNO) )
						{
							aumentaRecorrido( rec, orig, dest, &errormemoria );
							if (errormemoria)
								return ERRORMEMORIA;
						}
					}
					huboambiguedad=0;
					switch(retornomovida)
					{
						case SIGUETURNO:
									if (TURNO==J1)	/*Se capturó y sigue el mismo jugador, a menos que sea el primer turno*/
									{
										if(primerTJ1)
										{
											TURNO=turno2;
											cturno='X';
											primerTJ1=0;
											liberaRecorrido( rec );
										}
										habilitarundo1=1;
									}
									else if (TURNO==J2||TURNO==MAQUINA)
									{
										if(primerTJ2)
										{
											TURNO=turno1;
											cturno='O';
											primeravez1=1;
											primerTJ2=0;
											liberaRecorrido( rec );
										}
									}
									break;
						case TERMINOTURNO:
									if (TURNO==J1)	/*No puede capturar más, se cambia de turno*/
									{
										TURNO=turno2;
										cturno='X';
										if(primerTJ1)
											primerTJ1=0;
									}
									else if (TURNO==J2||TURNO==MAQUINA)
									{
										TURNO=turno1;
										cturno='O';
										primeravez1=1;
										if(primerTJ2)
											primerTJ2=0;
									}
									liberaRecorrido( rec );
									if ( esEmpate( tablero, cturno, rec ) )
									{
										printf("FIN\nEs empate\n");
										estadojuego=0;
									}
									break;
						case NECESITOWOA:
									if (TURNO==MAQUINA)	/*Hay ambigüedad, se necesita saber Approach o Withdraw*/
										lectura2=rand()%2;
									else
										lectura2=leerWoA();
									if(lectura2==1)
										WoA='A';
									else 
										WoA='W';
									huboambiguedad=1;		
									break;
						case TERMINOJUEGO:	estadojuego=0;
									break;
						case JUGADAINVALIDA:	printf("Jugada inválida.\n");
									break;
					}
					break;
			case UNDO:	
					if (TURNO==J1)	/*Sirve sólo para el jugador 1 si juega contra la PC*/
					{
						if(turno2!=MAQUINA)
						{
							printf("Opción no disponible en este modo de juego\n");
							break;
						}
						if(primerTJ1)
							printf("Opción no disponible en primer turno.\n");
						else
						{
							if(primeravez1==1 && habilitarundo1==1)
							{	
								backup(Aux,tablero->Fil,tablero->Col,tablero->Mapa);	/*Pone Aux en Mapa*/
								primeravez1=0;
								habilitarundo1=0;
								disminuyeRecorrido(rec);
								fichas[1]=fichas[1]+fichascomidas;
							}
							else 
								printf("No es posible realizar UNDO.\n");
						}
					}
					else 
						printf("Opción no disponible en este modo de juego\n");
					break;
			case QUIT:
					do
					{
						SoN=getint("¿Desea guardar partida? (1:Sí, 2:No): ");
					}while( SoN!=1 && SoN!=2 );
					if( SoN==2)
					{
						estadojuego=0;
						break;
					}
					dimarch=0;
					printf("Ingrese el nombre del archivo: ");
					while( (c=getchar())!='\n' )
					{
						if(dimarch%BLOQUE==0)
						{
							archivo=realloc(archivo,(dimarch+BLOQUE)*sizeof(char));
							if(archivo==NULL)
								return ERRORMEMORIA;
						}
						archivo[dimarch]=c;		
						dimarch++;
					}
					archivo[dimarch]='\0';
			case SAVE:
					if( guardarPartida( turno2, TURNO, tablero, archivo ) ==1)
					{
						printf("Error al guardar partida\n");
						break;
					}
					printf("Nombre del archivo: %s\n",archivo);
					free(archivo);
					archivo=malloc(sizeof(char));
					if(archivo==NULL)
                				return ERRORMEMORIA;
					if(lectura==QUIT)
						estadojuego=0;
					break;
		}
	}
	mostrarTablero( tablero );
	if(fichas[0]==0)
		printf("FIN\nGanaron las negras\n");
	else if(fichas[1]==0)
		printf("FIN\nGanaron las blancas\n");	
	return 0;
}
int main()
{
	int elegirmenu,elegirmenu2,fin=0, hayerror;
	int tocaturno, modojuego, fichas[2], dim;
	char c, * nom;
	tipoT tablero;
	srand(time(0));
	tablero=malloc(sizeof(struct infoTablero));
	if(tablero==NULL)
	{
		printf("Ocurrió un error en memoria\n");
		return 1;
	}
	do
	{
		fanorona();
		printf("\n\nBienvenido al FANORONA.\n");
		printf("Desplácese por el menú eligiendo cada opción con su número\n");
		printf("1. Juego Nuevo.\n");
		printf("2. Recuperar Juego Grabado.\n");
		printf("3. Terminar.\n");
		do
		{
			elegirmenu=getint("");
		}while(elegirmenu<1 || elegirmenu>3);
		switch (elegirmenu)
		{
			case 1:
				printf("\n1. Juego individual contra la computadora.\n");
				printf("2. Juego de 2 jugadores.\n");
				printf("3. Volver.\n");
				do
				{
					elegirmenu2=getint("");
				}while(elegirmenu2<1 || elegirmenu2>3);
				if(elegirmenu2==1)
				{
					hayerror=jugar(J1,MAQUINA,tablero,0,fichas,J1);
					if (hayerror)
						printf("Ocurrió un error en memoria\n");
				}
				else if(elegirmenu2==2)
				{
					hayerror=jugar(J1,J2,tablero,0,fichas,J1);
					if (hayerror)
						printf("Ocurrió un error en memoria\n");
				}
				break;
			case 2:
				dim=0;
				nom=malloc(BLOQUE*sizeof(char));
				if(nom==NULL)
				{
					printf("Ocurrió un error en memoria\n");
					break;
				}
				nom[dim++]='.';
				nom[dim++]='/';
				printf("Ingrese nombre del archivo a cargar: ");
				while( (c=getchar())!='\n' )
				{
					if(dim%BLOQUE==0)
					{
						nom=realloc(nom,(dim+BLOQUE)*sizeof(char));
						if(nom==NULL)
							return 1;
					}
					nom[dim++]=c;
				}
				nom=realloc(nom, dim*sizeof(char));
				nom[dim]='\0';
				if( recuperarPartida(tablero, &modojuego, &tocaturno, fichas, nom) == 1 )
				{
					printf("Error al cargar archivo\n");
					break;                                
				}
				free(nom);
				hayerror=jugar(J1,modojuego,tablero,1,fichas,tocaturno);
				if (hayerror)
					printf("Ocurrió un error en memoria\n");
				break;
			case 3:
				fin=1;
				break;
		}
	}while(fin==0);
	return 0;
}
