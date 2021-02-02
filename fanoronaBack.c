#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define J1 1
#define J2 2
#define MAQUINA 3
#define JUGADAINVALIDA 9
#define SIGUETURNO 10
#define TERMINOTURNO 11
#define TERMINOJUEGO 12
#define NECESITOWOA 13
typedef struct{
		int posX;
		int posY;
		int **movimientos;
		int cantmovs;
			}tipoF;
struct infoTablero{
		char **Mapa;
		int Fil;
		int Col;
			};
struct infoRecorrido{
		int **recorrido;
		int dimrecorrido;
			};
typedef struct infoTablero * tipoT;
typedef struct infoRecorrido * tipoR;

int recuperarPartida(tipoT tablero, int * modojuego, int * turno, int fichas[], char * nom)
{
	FILE * archivocargado;
	char c;
	int i, j;
	archivocargado = fopen( nom , "rb" );
	if(archivocargado==NULL)
		return 1;
	fread( modojuego, sizeof(int), 1, archivocargado);
	fread( turno, sizeof(int), 1, archivocargado);
	if(*modojuego==0)
		*modojuego=MAQUINA;
	else
		*modojuego=J2;
	if(*turno==2&&*modojuego==MAQUINA)
		*turno=MAQUINA;
	fread( &(tablero->Fil), sizeof(int), 1, archivocargado);
	fread( &(tablero->Col), sizeof(int), 1 , archivocargado);
	tablero->Mapa=malloc(tablero->Fil*sizeof(char*));
	if(tablero->Mapa==NULL)
		return 1;
	fichas[0]=0;
	fichas[1]=0;
	for(i=0;i<tablero->Fil;i++)
	{
		tablero->Mapa[i]=malloc(tablero->Col*sizeof(char));
		if( tablero->Mapa[i] == NULL)
		{
			free(tablero->Mapa);
			return 1;
		}
		for(j=0;j<tablero->Col;j++)
		{
			fread(&c, sizeof(char), 1, archivocargado);
			switch(c)
			{
				case '0':
					tablero->Mapa[i][j]=' ';
					break;
				case '#':
					tablero->Mapa[i][j]='X';
					(fichas[1])++;
					break;
				case '&':
					tablero->Mapa[i][j]='O';
					(fichas[0])++;
					break;
			}
		}
	}
	fclose(archivocargado);
	return 0;
}
int guardarPartida(int modojuego, int turno, tipoT tablero, char * nombre)
{
	int tipojuego=0,i,j;
	char c,*ruta;
	FILE * archivoguardado;
	ruta=malloc((3+strlen(nombre))*sizeof(char));
	if(ruta==NULL)
		return 1;
	sprintf(ruta,"./%s",nombre);
	archivoguardado = fopen(ruta , "wb" );
	if(archivoguardado==NULL)
		return 1;
	if(modojuego==J2)
		tipojuego=1;
	fwrite(&tipojuego,sizeof(int),1, archivoguardado);
	fwrite(&(turno), sizeof(int), 1, archivoguardado);
	fwrite(&(tablero->Fil), sizeof(int), 1, archivoguardado);
	fwrite(&(tablero->Col), sizeof(int), 1, archivoguardado);
	for(i=0;i<tablero->Fil;i++)
	{
		for(j=0;j<tablero->Col;j++)
		{
			switch(tablero->Mapa[i][j])
			{
				case ' ':
					c='0';
					break;
				case 'X':
					c='#';
					break;
				case 'O':
					c='&';
					break;
			}
			fputc(c, archivoguardado);
		}
	}
	fclose(archivoguardado);
	return 0;
}
void generarTablero( tipoT tablero )	
{	
	int i, j;
	for (i=0; i<tablero->Fil; i++)
	{
		for (j=0; j<tablero->Col; j++)
		{
			if (i<((tablero->Fil-1)/2))
				tablero->Mapa[i][j]='X';
			if (i>((tablero->Fil-1)/2))
				tablero->Mapa[i][j]='O';
		}
	}
	i=(tablero->Fil-1)/2;
	for (j=0; j<((tablero->Col-1)/2); j+=2)
		tablero->Mapa[i][j]='X';
	for (j=1; j<((tablero->Col-1)/2); j+=2)
		tablero->Mapa[i][j]='O';
	for (j=(tablero->Col-1); j>((tablero->Col-1)/2); j-=2)
		tablero->Mapa[i][j]='O';
	for (j=(tablero->Col-1)-1; j>((tablero->Col-1)/2); j-=2)
		tablero->Mapa[i][j]='X';
	tablero->Mapa[((tablero->Fil-1)/2)][((tablero->Col-1)/2)]=' '; 
}
int controlaRecorrido( const tipoR rec, int orig[], int dest[], int chequeaorig )	/*Ve si no se vuelve a un lugar donde ya estuvo*/
{
	int i, salida=1;
	if ( rec->dimrecorrido>0 )
	{
		if (chequeaorig)
		{
			if ( (orig[0]!=rec->recorrido[(rec->dimrecorrido)-1][0]) || (orig[1]!=rec->recorrido[(rec->dimrecorrido)-1][1]) )
				salida=0;
		}
		for (i=0; i<rec->dimrecorrido; i++)
		{
			if ( (dest[0]==rec->recorrido[i][0]) && (dest[1]==rec->recorrido[i][1]) )
				salida=0;
		}
	}
	return salida;
}
void backup( char ** Mapa, int Fil, int Col, char** Aux )	/*Guarda un auxiliar del tablero, intercambia cuando se hace undo*/
{
	int i, j;
	for( i=0; i<Fil; i++ )
	{
		for( j=0; j<Col; j++ )
			Aux[i][j]=Mapa[i][j];
	}
}
static void intercambia( char* a, char* b )		/*Cambia la ficha de lugar*/
{
	char aux;
	aux=*a;
	*a=*b;
	*b=aux;
}
static int controlaOrigen(  char** Mapa, const int orig[], char turno )
{
	int valida=1;
	if ( Mapa[orig[0]][orig[1]]!=turno)
		valida=0;
	return valida;
}
static int controlaDistancia(  int dirX, int dirY )
{
	int valida=1;
	if ( (dirX<-1)||(dirX>1)||(dirY<-1)||(dirY>1) )
		valida=0;
	return valida;
}
static int controlaDestino(  char** Mapa, const int dest[] )
{
	int valida=1;
	if ( Mapa[dest[0]][dest[1]]!=' ' )
		valida=0;
	return valida;
}
static int noSePasa( int X, int Y, int Fil, int Col )	/*Por las dimensiones del tablero*/
{
	int sepasa=1;
	if ((X<0)||(X>=Fil)||(Y<0)||(Y>=Col))
		sepasa=0;
	return sepasa;
}
static int hayCapturaA( const tipoT tablero, const int dest[], int dirX, int dirY, char enemigo )	/*Por approach*/
{
	int haycaptura=0;
	if ( noSePasa( dest[0]+dirX, dest[1]+dirY, tablero->Fil, tablero->Col ) )
	{
		if ( tablero->Mapa[dest[0]+dirX][dest[1]+dirY]==enemigo )
			haycaptura=1;
	}
	return haycaptura;
} 
static int hayCapturaW( const tipoT tablero, const int orig[], int dirX, int dirY, char enemigo )	/*Por withdraw*/
{
	int haycaptura=0;
	if ( noSePasa( orig[0]-dirX, orig[1]-dirY, tablero->Fil, tablero->Col ) )
	{
		if ( tablero->Mapa[orig[0]-dirX][orig[1]-dirY]==enemigo )
			haycaptura=1;
	}
	return haycaptura;
}
static int capturaW( tipoT tablero, int X1, int Y1, int dirX, int dirY, char enemigo )
{
	int i, termino=0, eliminadas=0;
	for (i=1; (termino==0); i++)
	{
		if ( noSePasa((X1-(i*dirX)), (Y1-(i*dirY)), tablero->Fil, tablero->Col) && (tablero->Mapa[X1-(i*dirX)][Y1-(i*dirY)]==enemigo))
		{
			tablero->Mapa[X1-(i*dirX)][Y1-(i*dirY)]=' ';
			eliminadas++;
		}
		else
			termino=1;
	}
	return eliminadas;
}
static int capturaA( tipoT tablero, int X2, int Y2, int dirX, int dirY, char enemigo )
{
	int i, termino=0, eliminadas=0;
	for (i=1; (termino==0); i++)
	{
		if ( noSePasa( (X2+(i*dirX)), (Y2+(i*dirY)), tablero->Fil, tablero->Col )&&(tablero->Mapa[X2+(i*dirX)][Y2+(i*dirY)]==enemigo ))
		{
			tablero->Mapa[X2+(i*dirX)][Y2+(i*dirY)]=' ';
			eliminadas++;
		}
		else
			termino=1;
	}
	return eliminadas;
}
static int controlaPipes( const int orig[], const int dest[] )
{
        int indiceorigen, indicedestino, valido=1;
        indiceorigen=(orig[0]+orig[1]);
        indicedestino=(dest[0]+dest[1]);
        if (((indiceorigen%2)==1)&&((indicedestino%2==1)))
                valido=0;
        return valido;
}
static int capturasDisponibles( const tipoT tablero, int orig[], int auxorig[], char enemigo, const tipoR rec, int chequeaorig )
{
	int i, j, encontro=0, auxdest[2], auxdirX, auxdirY, diranteriorX, diranteriorY;
	if (rec->dimrecorrido>1)
	{
		diranteriorX=auxorig[0]-rec->recorrido[(rec->dimrecorrido)-1][0];
		diranteriorY=auxorig[1]-rec->recorrido[(rec->dimrecorrido)-1][1];
	}
	for (i=-1; (i<2)&&(encontro==0); i++)
	{
		for (j=-1; (j<2)&&(encontro==0); j++)
		{
			if ( noSePasa( auxorig[0]+i, auxorig[1]+j, tablero->Fil, tablero->Col ) )
			{
				auxdest[0]=auxorig[0]+i;
				auxdest[1]=auxorig[1]+j;
				if ( controlaDestino( tablero->Mapa, auxdest ) && controlaPipes( auxorig, auxdest ) )
				{
					auxdirX=auxdest[0]-auxorig[0];
					auxdirY=auxdest[1]-auxorig[1];
					if ( (rec->dimrecorrido==0) || (diranteriorX!=auxdirX || diranteriorY!=auxdirY) )
					{
						if ( controlaRecorrido( rec, auxorig, auxdest, chequeaorig ) && !(orig[0]==auxdest[0] && orig[1]==auxdest[1]) )
						{
							if ( (hayCapturaA( tablero, auxdest, auxdirX, auxdirY, enemigo ))||(hayCapturaW( tablero, auxorig, auxdirX, auxdirY, enemigo)) )
								encontro=1;
						}
					}
				}
			}
		}
	}
	return encontro;
}
static int encuentraFicha( const tipoT tablero, int auxorig[], char turno, const tipoR rec )	
{
	int i, j, encontro=0, chequeaorig=1, null[2]={0,0};
	char enemigo;
	if ( turno=='X' )
	{
		enemigo='O';
		for (i=0; (i<tablero->Fil)&&(encontro==0); i++)
		{
			for (j=0; (j<tablero->Col)&&(encontro==0); j++)
			{
				if( tablero->Mapa[i][j]=='X' )
				{
					auxorig[0]=i;
					auxorig[1]=j;
					encontro=capturasDisponibles( tablero, null, auxorig, enemigo, rec, chequeaorig );
				}
			}
		}
	}
	else
	{
		enemigo='X';
		for (i=(tablero->Fil-1); (i>=0)&&(encontro==0); i--)
		{
			for (j=(tablero->Col-1); (j>=0)&&(encontro==0); j--)
			{
				if( tablero->Mapa[i][j]=='O' )
				{
					auxorig[0]=i;
					auxorig[1]=j;
					encontro=capturasDisponibles( tablero, null, auxorig, enemigo, rec, chequeaorig );
				}
			}
		}
	}
	return encontro;
}
static tipoF **encuentraFichaPC( const tipoT tablero, const tipoR rec , int*totfichas, int capturar, int *error )
{
	tipoF **fichas=NULL, **auxfichas;
	int i, j, k=0, ii, jj, kk=0, auxdirX, auxdirY, auxdest[2], auxorig[2], diranteriorX, diranteriorY, chequeaorig=1;
	char enemigo='O';
	if (rec->dimrecorrido>1)
	{
		diranteriorX=rec->recorrido[(rec->dimrecorrido)-1][0]-rec->recorrido[(rec->dimrecorrido)-2][0];
		diranteriorY=rec->recorrido[(rec->dimrecorrido)-1][1]-rec->recorrido[(rec->dimrecorrido)-2][1];
	}
	auxfichas=fichas;
	for (i=0; (i<tablero->Fil); i++)
	{
		for (j=0; (j<tablero->Col); j++)
		{
			if( tablero->Mapa[i][j]=='X' )
			{
				auxfichas=realloc(fichas, (k+1)*sizeof(*fichas) );	/*Aquí hay una ficha, falta ver si tiene capturas disponibles*/
				if (auxfichas==NULL)
				{
					free( fichas );
					*error=1;
					return fichas;
				}
				fichas=auxfichas;
				fichas[k]=malloc(sizeof(**fichas));
				if (fichas[k]==NULL)
				{
					*error=1;
					return fichas;
				}
				fichas[k]->posX=i;
				fichas[k]->posY=j;
				kk=0;
				fichas[k]->cantmovs=0;
				auxorig[0]=i;
				auxorig[1]=j;
				fichas[k]->movimientos = malloc ( sizeof( *( fichas[k]->movimientos ) )  );
				if (fichas[k]->movimientos==NULL)
				{
					*error=1;
					return fichas;
				}
				for (ii=-1; (ii<2); ii++)
				{
					for (jj=-1; (jj<2); jj++)	/*Chequea en todas las direcciones*/
					{
						if ( noSePasa( auxorig[0]+ii, auxorig[1]+jj, tablero->Fil, tablero->Col ) )
						{
							auxdest[0]=auxorig[0]+ii;
							auxdest[1]=auxorig[1]+jj;
							if ( controlaDestino( tablero->Mapa, auxdest )&&controlaPipes( auxorig, auxdest ) )
							{
								auxdirX=auxdest[0]-auxorig[0];
								auxdirY=auxdest[1]-auxorig[1];
								if ( (rec->dimrecorrido==0) || (diranteriorX!=auxdirX || diranteriorY!=auxdirY) )
								{
									if ( controlaRecorrido( rec, auxorig, auxdest, chequeaorig ) )
									{
										if(capturar==1)
										{	/*Guarda las direcciones en las que tiene capturas disponibles*/
											if( (hayCapturaA(tablero,auxdest,auxdirX,auxdirY,enemigo ))||(hayCapturaW(tablero,auxorig,auxdirX,auxdirY,enemigo)) )
											{
												fichas[k]->movimientos=realloc( fichas[k]->movimientos ,(kk+1)*sizeof( *( fichas[k]->movimientos ) ));
												if (fichas[k]->movimientos==NULL)
												{
													*error=1;
													return fichas;
												}
												fichas[k]->movimientos[kk]=malloc( 2*sizeof(** (fichas[k]->movimientos) ));
												if (fichas[k]->movimientos[kk]==NULL)
												{
													*error=1;
													return fichas;
												}
												fichas[k]->movimientos[kk][0]=auxdest[0];
												fichas[k]->movimientos[kk][1]=auxdest[1];
												kk++;
												(fichas[k]->cantmovs)++;
											}
										}
										else
										{	/*Guarda las direcciones en las que se puede mover, cuando no tenga capturas disponibles*/
											if(!( (hayCapturaA(tablero,auxdest,auxdirX,auxdirY,enemigo ))||(hayCapturaW(tablero,auxorig,auxdirX,auxdirY,enemigo)) ))
											{
												fichas[k]->movimientos=realloc( fichas[k]->movimientos ,(kk+1)*sizeof( *( fichas[k]->movimientos ) ));
												if (fichas[k]->movimientos==NULL)
												{
													*error=1;
													return fichas;
												}
												fichas[k]->movimientos[kk]=malloc( 2*sizeof(** (fichas[k]->movimientos) ));
												if (fichas[k]->movimientos[kk]==NULL)
												{
													*error=1;
													return fichas;
												}
												fichas[k]->movimientos[kk][0]=auxdest[0];
												fichas[k]->movimientos[kk][1]=auxdest[1];
												kk++;
												(fichas[k]->cantmovs)++;
											}
										}						
									}
								}
							}
						}
					}
				}
				if(kk>0)
				k++;
			}
		}
	}        
	*totfichas=k;
	return fichas;
}
void elegirFichaAzar(int orig[], int dest[], const tipoT tablero, const tipoR rec, int *errormemoria)
{
	int n, m, totfichas, i, j, capturar=1, error=0;
	tipoF **fichas;
	fichas=encuentraFichaPC (tablero, rec, &totfichas, capturar, &error);	/*Busca fichas y capturas disponibles*/
	if (error)
	{
		*errormemoria=1;
		return;
	}
	if(totfichas!=0)
	{
		n=rand()%totfichas;
		m=rand()%(  fichas[n]->cantmovs  );
	}
	else
	{
		capturar=0;
		fichas= encuentraFichaPC (tablero, rec, &totfichas, capturar, &error);	/*Busca fichas que se muevan sin capturar*/
		if (error)
		{
			*errormemoria=1;
			return;
		}
		n=rand()%totfichas;
		m=rand()%(  fichas[n]->cantmovs  );
	}
	orig[0]=fichas[n]->posX;
	orig[1]=fichas[n]->posY;
	dest[0]=fichas[n]->movimientos[m][0];
	dest[1]=fichas[n]->movimientos[m][1];
	for(i=0;i<totfichas;i++)
	{
		for(j=0;j< (fichas[i]->cantmovs);j++)
			free( fichas[i]->movimientos[j] );
		free( fichas[i]->movimientos );
		free( fichas[i]);
	}
	free(fichas);
	return;
}
void aumentaRecorrido( tipoR rec, int orig[], int dest[], int *error )
{
	tipoR auxrec;
	auxrec=rec;
	if (rec->dimrecorrido>0)
        	(rec->dimrecorrido)++;
	else 
		(rec->dimrecorrido)+=2;
	auxrec->recorrido=realloc( rec->recorrido, rec->dimrecorrido*sizeof(*(rec->recorrido)) );
        if (auxrec->recorrido==NULL)
        {
                free( rec->recorrido );
                *error=1;
                return;
        }
	rec->recorrido=auxrec->recorrido;
	if(rec->dimrecorrido==2)
	{
		rec->recorrido[(rec->dimrecorrido)-2]=malloc( 2*sizeof(int) );
                if (rec->recorrido[(rec->dimrecorrido)-2]==NULL)
                {
                        *error=1;
                        return;
                }
                rec->recorrido[(rec->dimrecorrido)-2][0]=orig[0];
                rec->recorrido[(rec->dimrecorrido)-2][1]=orig[1];
	}
	rec->recorrido[(rec->dimrecorrido)-1]=malloc( 2*sizeof(int) );
        if (rec->recorrido[(rec->dimrecorrido)-1]==NULL)
        {
                *error=1;
                return;
        }
        rec->recorrido[(rec->dimrecorrido)-1][0]=dest[0];
        rec->recorrido[(rec->dimrecorrido)-1][1]=dest[1];
	return;
}
void liberaRecorrido(tipoR rec)
{
	int i;
	for(i=0; i<(rec->dimrecorrido) ;i++)
		free(rec->recorrido[i]);
	free(rec->recorrido);
	rec->dimrecorrido=0;
	rec->recorrido=NULL;
	return;
}
void disminuyeRecorrido(tipoR rec)
{
	if(rec->dimrecorrido==2)
	{
		free(rec->recorrido[1]);
		free(rec->recorrido[0]);
		free(rec->recorrido);
		rec->dimrecorrido=0;
		rec->recorrido=malloc( sizeof(*(rec->recorrido)) );
	}
	else
	{
		free(rec->recorrido[(rec->dimrecorrido)-1]);
		(rec->dimrecorrido)--;
	}
	return;
}
static int mePuedeComerA( const tipoT tablero, int dest[], char enemigo )
{
	int i, j, puede=0;
	for (i=-1; (i<2) && (puede==0); i++)
	{
		for (j=-1; (j<2) && (puede==0); j++)
		{
			if ( noSePasa( dest[0]+2*i, dest[1]+2*j, tablero->Fil, tablero->Col ) )
			{
				if ( tablero->Mapa[dest[0]+2*i][dest[1]+2*j]==enemigo )
				{
					if ( tablero->Mapa[dest[0]+i][dest[1]+j]==' ' )
						puede=1;
				}
			}
		}
	}
	return puede;
}
static int mePuedeComerW( const tipoT tablero, int dest[], char enemigo )
{
	int i, j, encontro=0, auxdest[2];
	for (i=-1; (i<2) &&( encontro==0); i++)
	{
		for (j=-1; (j<2) && ( encontro==0 ); j++)
		{
			if ( noSePasa( dest[0]+i, dest[1]+j, tablero->Fil, tablero->Col ) )
			{
				auxdest[0]=dest[0]+i;
				auxdest[1]=dest[1]+j;
				if ( controlaPipes( dest, auxdest ) )
				{
					if ( tablero->Mapa[auxdest[0]][auxdest[1]]==enemigo )
					{
						if ( noSePasa( auxdest[0]+i, auxdest[1]+j, tablero->Fil, tablero->Col ) )
						{
							if ( tablero->Mapa[auxdest[0]+i][auxdest[1]+j]==' ' )
								encontro=1;
						}
					}
				}
			}
		}
	}
	return encontro;
}
static int meCapturan( const tipoT tablero, int orig[], char enemigo )
{
	int i, j, encontro=0, auxdest[2];
	for ( i=-1; (i<2)&&(encontro==0); i++ )
	{
		for (j=-1; (j<2)&&(encontro==0); j++)
		{
			if ( noSePasa( orig[0]+i, orig[1]+j, tablero->Fil, tablero->Col ) )
			{
				auxdest[0]=orig[0]+i;
				auxdest[1]=orig[1]+j;
				if ( controlaDestino( tablero->Mapa, auxdest ) )
				{
					if ( controlaPipes( orig, auxdest ) )
					{
						if ( !(mePuedeComerA( tablero, auxdest, enemigo )||mePuedeComerW( tablero, auxdest, enemigo )) )
							encontro=1;
					}
				}
			}
		}
	}
	return encontro;
}
int esEmpate( const tipoT tablero, char turno, tipoR rec )
{
	int i, j, encontro=0, orig[2], auxorig[2];
	char enemigo;
	if ( encuentraFicha( tablero, auxorig, turno, rec ) )
		return 0;
	if ( turno=='X' )
		enemigo='O';
	else
		enemigo='X';
	for ( i=0; (i<tablero->Fil) && (encontro==0); i++ )
	{
		for ( j=0; (j<tablero->Col) && (encontro==0); j++ )
		{
			if ( tablero->Mapa[i][j]==turno )
			{
				orig[0]=i;
				orig[1]=j;
				encontro=meCapturan( tablero, orig, enemigo );
			}
		}
	}
	if ( encontro )
		return 0;
	else
		return 1;
}
int movida( tipoT tablero, char* turno, int orig[], int dest[], char WoA, int fichas[], const tipoR rec, int * fichascomidas )
{
	int dirX, dirY, haycapW=0, haycapA=0, captA=0, captW=0, capturo=0, termino=0, auxorig[2];
	int diranteriorX, diranteriorY, chequeaorig=1;
	char enemigo;
	if (rec->dimrecorrido>1)
	{
		diranteriorX=rec->recorrido[(rec->dimrecorrido)-1][0]-rec->recorrido[(rec->dimrecorrido)-2][0];
		diranteriorY=rec->recorrido[(rec->dimrecorrido)-1][1]-rec->recorrido[(rec->dimrecorrido)-2][1];
	}
	if (*turno=='O')
		enemigo='X';
	else
		enemigo='O';
	if ( !controlaOrigen( tablero->Mapa, orig, *turno ) )
		return JUGADAINVALIDA;
	if ( !noSePasa( dest[0], dest[1], tablero->Fil, tablero->Col ))
		return JUGADAINVALIDA;
	if ( !controlaDestino( tablero->Mapa, dest ) )
		return JUGADAINVALIDA;
	dirX=(dest[0]-orig[0]);
	dirY=(dest[1]-orig[1]);
	if ( !controlaDistancia( dirX, dirY ) )
		return JUGADAINVALIDA;
	if ( !(rec->dimrecorrido==0 || diranteriorX!=dirX || diranteriorY!=dirY)  )
		return JUGADAINVALIDA;
	if ( !controlaPipes( orig, dest ))
		return JUGADAINVALIDA;
	haycapA=hayCapturaA( tablero, dest, dirX, dirY, enemigo );
	haycapW=hayCapturaW( tablero, orig, dirX, dirY, enemigo );
	if ( haycapA && haycapW )
	{
		if ( WoA=='A' )
			captA=1;
		else if ( WoA=='W' )
			captW=1;
		else
			return NECESITOWOA;
	}
	else if ( haycapA && !haycapW )
		captA=1;
	else if ( haycapW && !haycapA )
		captW=1;
	else
	{
		if( encuentraFicha( tablero, auxorig, *turno, rec ) )
			return JUGADAINVALIDA;
	}
	if ( captA==1 )
	{
		if ( *turno=='O' )
		{
			*fichascomidas=capturaA( tablero, dest[0], dest[1], dirX, dirY, enemigo );
			fichas[1]=fichas[1]-*fichascomidas;
		}
		else
			fichas[0]=fichas[0]-capturaA( tablero, dest[0], dest[1], dirX, dirY, enemigo );
		capturo=1;
	}
	else if( captW==1 )
	{
		if ( *turno=='O' )
		{	
			*fichascomidas=capturaW( tablero, orig[0], orig[1], dirX, dirY, enemigo );	
			fichas[1]=fichas[1]-*fichascomidas;
		}
		else
			fichas[0]=fichas[0]-capturaW( tablero, orig[0], orig[1], dirX, dirY, enemigo );
		capturo=1;
	}
	intercambia( &(tablero->Mapa[orig[0]][orig[1]]), &(tablero->Mapa[dest[0]][dest[1]]) );
	if ( (fichas[0]==0)||(fichas[1]==0) )
                return TERMINOJUEGO;
	if ( !capturo )
		return TERMINOTURNO;
	chequeaorig=0;
	if (!capturasDisponibles( tablero, orig, dest, enemigo, rec, chequeaorig ))
		return TERMINOTURNO;
	return SIGUETURNO;
}
