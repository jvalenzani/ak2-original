#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include "ak2gfx.inc"

#define TITLE "A^k2"
#define VER "1.0ã"
#define AUTHOR "VA.MO."
#define F1 '\x3b'
#define F2 '\x3c'
#define ESC 27
#define MAXLARGO 50	// Largo Maximo de la palabra a adivinar
#define TOPEMAX 20 	// Maximo de lineas por archivo y de letras de un nombre
#define ERRORMAX 6      // Cantidad Maxima de errores
#define MAXPARTIDAS 5   // Maxima cantidad de Partidas que se pueden jugar

int exist(char *);
void mayusculas(char *);
int valido_letra(int);
void usage(void);
void pongo_rayas(char *);
void jugar(char *,char , char * , char *, char *,int);
int gano(char *);
int leo_linea(char *, int);
int cargo_peli(char *,char *[]);
int salir(int);
void helpig(void);    //ayuda durante el juego
void helpmain(void);  //ayuda del menu principal
int pido_letra(char *);
void muestro_main_texto(char *,int,int, int, int);
int puntos_error(char);
char otro_juego(void);
int arriesgar(char *);
int menu(void);
void play(char *, int, int);
int top10(char *,int,int);
void highscore(int);
void opciones(int *, int *);
void fin(void);
void creditz(void);
/*----------------------------(Fin Prototipos)----------------------------*/


void creditz()  //muestra el ansi de creditos y espera una tecla
{
	clrscr();
	puttext(1,1,CREDITZ_WIDTH,CREDITZ_DEPTH,CREDITZ);
	getch();
	return;
}

void main(int argc, char *argv[])
{
	int opcion;
	int rep=1;
	int fondo=0;
	if (argc<2)
		usage();
	if ((exist(argv[1]))==0){
		printf("\007 El archivo %s no existe",argv[1]);
		exit(0);
	}
	clrscr();
	opcion=menu();
	while(1)  //while sin fin para que solo salga si elije salir
  {
		switch (opcion){
			case 0:play(argv[1], rep, fondo);break;
			case 1:opciones(&rep, &fondo);break;
			case 2:highscore(0);break;
			case 3:helpmain();break;
			case 4:creditz();break;
			case 5:fin();
		}
		opcion=menu();
	}

}

/*------------------------------(Fin Main)--------------------------------*/

/* Funcion FIN()

	Limpia la pantalla y luego muestra un ANSi colocando el cursor al
	final del mismo de forma de que el prompt quede debajo del ANSi*/



void fin(void)
{
	window(1,1,80,25);
	clrscr();
	puttext(1,1,EXIT_WIDTH,EXIT_DEPTH,EXIT);
	gotoxy(1,EXIT_DEPTH+1);
	_setcursortype(_NORMALCURSOR);
	exit(0);
}

/*	Declaracion de la estructura Utilizada para el TOP 10 */

struct ranking{
		char nombre[TOPEMAX];
		int partidas;
		int puntos;
};

/* Funcion OPCIONES()

	Esta funcion muestra el menu de opciones donde se puede cambiar
	el ansi de fondo, borrar el historial y cambiar la opcion de
	repetir teclas*/

void opciones(int *rep, int *fondo){
	const tope_sup=17;
	const cant_opciones=4;
	const col_ini=9;
	char c,borrar;
	int i;
	char *opciones[4]={"BORRAR hiStORiAl", "REPEtiR ERRORES ","tEMA             ","VOlVER          "};

	puttext (7,16,39,21,OPTION);
	window(1,1,80,25);
	_setcursortype(_NOCURSOR);
	textcolor(LIGHTGRAY);
	for (i=0 ; i<cant_opciones; i++){
		gotoxy(col_ini,tope_sup+i);
		cprintf("%s",opciones[i]);
	}
	gotoxy(col_ini+20,tope_sup+1);
	if(*rep==1)
		printf("ON ");
	else
		printf("Off");
	gotoxy(col_ini+20,tope_sup+2);
	switch(*fondo){
		case 0:printf("Ice    ");  break;
		case 1:printf("Natura ");  break;
		case 2:printf("Inferno");  break;
	}
	i=tope_sup;
	gotoxy(col_ini,i);
	textcolor(WHITE);
	textbackground(LIGHTBLUE);
	cprintf("> %s",opciones[0]);
	for(;;){
		while((c=getch())!='\r'){
			if(c==ESC){
				textcolor(LIGHTGRAY);
				textbackground(BLACK);
				return;
			}
			if(c=='\x48' ||  c=='\x50' ){
				gotoxy(col_ini,i);
				textcolor(LIGHTGRAY);
				textbackground(BLACK);
				cprintf("%s  ",opciones[i-tope_sup]);
				if (c=='\x48'){  // flecha hacia arriba
					if(i==tope_sup)
						i=tope_sup+cant_opciones-1;
					else
						i--;
				}
				if (c=='\x50'){ //flecha hacia abajo
					if(i==tope_sup+cant_opciones-1)
						i=tope_sup;
					else
						i++;
				}
				gotoxy(col_ini,i);
				textcolor(WHITE);
				textbackground(LIGHTBLUE);
				cprintf("> %s",opciones[i-tope_sup]);
			}
		}
		textcolor(LIGHTGRAY);
		textbackground(BLACK);
		switch(i-tope_sup){
			case 0: gotoxy(col_ini+20,tope_sup);
				cprintf("BORRO?");
				borrar=toupper(getch());
				if (borrar=='S'|| borrar==13){
					fopen("ranking.dat","w");
					gotoxy(col_ini+20,tope_sup);
					printf("VACiO ");
					getch();
					gotoxy(col_ini+20,tope_sup);
					printf("      ");
					break;}
				else {
					gotoxy(col_ini+20,tope_sup);
					printf("      ");
					break;
				}
			case 1: *rep=*rep*(-1);  //cambio rep entre 1 y -1
				gotoxy(col_ini+20,tope_sup+1);
				if(*rep==1)
					printf("ON ");
				else
					printf("Off");
				break;
			case 2: if(*fondo==2)
					*fondo=0;
				else
					(*fondo)++;
				gotoxy(col_ini+20,tope_sup+2);
				switch(*fondo){
					case 0:printf("Ice    ");  break;
					case 1:printf("Natura ");  break;
					case 2:printf("Inferno");  break;
				}
				break;
			case 3: return;
		}
	}

}

/* Funcion HIGHSCORE()

	Esta funcion muestra un ANSi y a continuacion los 10 mejores puntajes
	los cuales estan guardados en un archivo llamado "TOP10.DAT". Este
	archivo es generado por la funcion TOP10().
	El parametro es un nro entero de 1 a 10 que marca una posicion en
	la tabla para que lo muestre con otros atributos.*/

void highscore(int posicion){
	const xntop=21;
	const ytop=13;
	const xptop=47;
	const xpntop=59;
	int i,part,ptos,resp;
	char nombre[TOPEMAX];
	FILE *f=fopen("top10.dat","r");

	window(1,1,80,25);
	clrscr();
	puttext (1,1,TOPTEN_WIDTH,TOPTEN_DEPTH,TOPTEN);
	i=0;
	resp=fscanf(f,"%s\t%i\t%i",nombre,&part,&ptos);
	while((resp!=0)&&(i<10)&&(resp!=EOF)){

		if (i==posicion){
			textcolor(LIGHTCYAN+128);
			gotoxy(xntop,ytop+i);
			cprintf("%s",nombre);
			textcolor(GREEN+128);
			gotoxy(xptop, ytop+i);
			cprintf("%i", part);
			textcolor(LIGHTGREEN+128);
			gotoxy(xpntop, ytop+i);
			cprintf("%3i", ptos);
			resp=fscanf(f,"%s\t%i\t%i\t",nombre,&part,&ptos);
			i++;
		}
		else{
			textcolor(LIGHTCYAN);
			gotoxy(xntop,ytop+i);
			cprintf("%s",nombre);
			textcolor(GREEN);
			gotoxy(xptop, ytop+i);
			cprintf("%i", part);
			textcolor(LIGHTGREEN);
			gotoxy(xpntop, ytop+i);
			cprintf("%3i", ptos);
			resp=fscanf(f,"%s\t%i\t%i\t",nombre,&part,&ptos);
			i++;
		}
	}
	fclose(f);
	while(i<10){
		gotoxy(xntop,ytop+i);
		printf("Lugar libre\n");
		i++;
	}
	getch();
	clrscr();
}

/* Funcion TOP10()

	Esta funcion genera el archivo TOP10.DAT en donde se guardan los 10
	mejores puntajes, los parametros que recibe son los datos del ultimo
	jugador los cuales son evaluados y si corresponde son agregados en
	la posicion. Devuelve la posicion donde se agrego-2 */

int top10(char *nom,int part,int pto)
{
	int i,j,k, punto, partida;
	char linea[27];
	struct ranking t10[10];
	FILE *f=fopen("top10.dat","r");

	for(i=0;i<10;i++)
	       t10[i].nombre[0]='\0';
	i=0;
	while((fscanf(f,"%s\t%i\t%i",t10[i].nombre,&(t10[i].partidas),&(t10[i].puntos)))!=EOF)
		i++;
	fclose(f);
	j=i-1;
	while((pto>=(t10[j].puntos))&&(j>=0))
		j--;
	for(k=8;k>j;k--){
		strcpy(t10[k+1].nombre,t10[k].nombre);
		t10[k+1].partidas=t10[k].partidas;
		t10[k+1].puntos=t10[k].puntos;
	}
	if(j<9){
		strcpy(t10[k+1].nombre,nom);
		t10[k+1].partidas=part;
		t10[k+1].puntos=pto;
	}
	f=fopen("top10.dat","w");
	i=0;
	while(t10[i].nombre[0]!='\0' && i<10){
		fprintf(f,"%s\t%i\t%i\n",t10[i].nombre,t10[i].partidas,t10[i].puntos);
		i++;
	}
	fclose(f);
	return j;
}

/* Funcion PLAY()

	Esta es la funcion principal, se le pasa el nombre del
	archivo que fue ingresado en la linea de comandos, un entero rep que
	se utiliza para saber si se va a jugar repitiendo letras o no, y un
	entero fondo que se usa para las distintas pantallas*/

void play(char *argv, int rep, int fondo){
	char *palabra=NULL, h_antes[4096],*p;
	FILE *arch;
	int posicion,letra,i,ultimo,puntos,j, tope,tope_max;
	char linea[MAXLARGO],nombre[TOPEMAX], error;
	char *pelis[TOPEMAX],letras[26];

	if (exist("top10.dat")==0)
		fopen("top10.dat","w");
	clrscr();
	puttext(1,1,80,25,HORCA);
	gotoxy(13,13);
	printf("Ingrese su nombre : ");
	letra=leo_linea(nombre,TOPEMAX);
	p=nombre;
	while(*p!='\0'){    //cambio los espacios por _ para que funcione
		if (*p==' ')        //la funcion fscanf
			*p='_';
		p++;
	}
	if(*nombre=='\0')
		strcpy(nombre,"jugador");  //nombre por defecto
	if (letra==0)    //presiono ESC
	       return;
	tope_max=cargo_peli(argv,pelis);  //guardo cant lineas del archivo
	puntos=0;
	clrscr();
	j=0;
	while((j<MAXPARTIDAS)&&(j<tope_max)){
		for(i=0;i<4096;i++)        //matriz que contendra la imagen
			h_antes[i]=0;      //de la horca
		for(i=0;i<26;i++)
			letras[i]='1';     //inicializo el vector letras que
		window(1,1,80,25);         //determina si las letras ya
		error=0;                   //fueron jugadas
		randomize();
		tope=rand() % tope_max;
		while(pelis[tope][0]=='\0')  //si la pelicula ya fue jugada
			tope=rand()%tope_max;
		palabra=(char *)malloc(strlen(pelis[tope])); //palabra es un vector del largo de la
		for(i=0;i<strlen(pelis[tope])-1;i++) // pelicula con un 0 donde falta adivinar una
			palabra[i]='1';             //letra y un 1 donde ya se adivino
		palabra[strlen(pelis[tope])-1]='\0';
		muestro_main_texto(nombre,j,puntos, fondo,1);
		pongo_rayas(pelis[tope]);

		for (i=0;i<(strlen(pelis[tope])-1);i++){
			if ((pelis[tope][i]!=' ') && (pelis[tope][i]!='\n'))
				palabra[i]='0';
		}

		while(error<ERRORMAX && gano(palabra)==0){
			letra=pido_letra(letras);
			if(letra==ESC){
				letra=salir(fondo);
				if (letra==0){  //presiono ESC
					j++;
					break;
				}
				continue;
			}
			if(letra==F1){  //Ayuda durante el juego
				helpig();
				continue;
			}
			if((letra=='#')||(letra==F2)){
				letra=arriesgar(pelis[tope]);
				switch(letra){
					case 0: letra=1;   //acerto
						p=palabra;
						while(*p!='\0'){ //lleno palabra de 1
							*p='1'; //para que gane la partida
							p++;
						}
					break;
					case 1:	jugar(h_antes,'0',pelis[tope],palabra,&error, fondo); //erro
						break; //juego el 0 para sumar un error
					case 2: clrscr();  //presiono ESC
						window(1,1,80,25);   //vuelvo
						break;
				}

			}
			else
				if ((rep==1)||(letras[letra-'A']!='2'))  //si la letra estaba jugada y estaba configurado para
					jugar(h_antes,letra, pelis[tope],palabra, &error, fondo); //no repetir errores no la juega
		}
		window(1,1,80,25);
		gotoxy(4,15);
		printf("%s", pelis[tope]); //muestra la pelicula
		pelis[tope][0]='\0';//la deja en 0 para que se sepa que
		free(palabra);      //esta jugada

		if (letra!=0)  //si no presiono ESC actualizo los puntos
			puntos=puntos+puntos_error(error);
		muestro_main_texto(nombre,j,puntos, fondo,0);
		if (gano(palabra)==0){   //perdio
			window(2,21,79,23);
			textcolor(LIGHTRED+128);
			cprintf(" PERdiO!!!\r\n");
			textcolor(LIGHTGRAY);
			cprintf(" JUEGO tERMiNAdO, PRESiONE UNA tEClA PARA CONtiNUAR...");
			getch();
			puttext(1,1,80,24,DEAD);
		}
		else{       //gano
			window(2,21,79,23);
			textcolor(LIGHTGREEN+128);
			cprintf(" GANO!!!\r\n");
			textcolor(LIGHTGRAY);
			cprintf(" JUEGO tERMiNAdO, PRESiONE UNA tEClA PARA CONtiNUAR...");
			getch();
			window(1,1,80,25);
			clrscr();
			puttext(1,1,WIN_WIDTH,WIN_DEPTH,WIN);
		}
		window(2,21,79,23);
		if ((j<tope_max-1) && (letra!=0) && (j<MAXPARTIDAS-1)){
			letra=otro_juego();
			if (letra=='N'){
				j++;
				break;}
		}
		if(letra==0)
			break;
		clrscr();
		j++;
	}
	arch=fopen("ranking.dat","a");
	fprintf(arch,"%s:%i:%i\n",nombre,j,puntos);
	fclose(arch);
	if ((posicion=top10(nombre,j,puntos))<9){
		window(20,12,58,13);
		textbackground(BLUE);
		clrscr();
		gotoxy(21,22);
		textcolor(YELLOW+128);
		cprintf(" Felicitaciones, su puntaje esta entre\r\n            los 10 mejores");
		normvideo();
		getch();
		window(1,1,80,25);
		clrscr();
		highscore(posicion+1);
        }
	else
		if (letra==0)
			getch();
	_setcursortype(_NORMALCURSOR);
	for(i=0;i<tope_max;i++)
		free (pelis[i]);
}


/* Funcion MENU()

	Muestra el menu principal y devuelve un entero con el nuemro de
	opcion seleccionado*/

int menu(void){
	const tope_sup=15;
	const cant_opciones=6;
	const col_ini=41;
	char c;
	int i;
	char *opciones[6]={"  NUEVO JUEGO ", "  OPCiONES    ", "  hiGh SCORES " , "  AYUdA       " , "  CREditOS    " , "  SAliR A O.S."};

	window(1,1,80,25);
	clrscr();
	puttext (1,1,MAINMENU_WIDTH,MAINMENU_DEPTH,MAINMENU);
	_setcursortype(_NOCURSOR);
	for (i=0 ; i<cant_opciones; i++){
		gotoxy(col_ini,tope_sup+i);
		printf("%s",opciones[i]);
	}
	i=tope_sup;
	gotoxy(col_ini,i);
	textcolor(WHITE);
	textbackground(LIGHTBLUE);
	cprintf("> %s",opciones[0]);
	while((c=getch())!='\r'){
		if(c=='\x48' ||  c=='\x50' ){
			gotoxy(col_ini,i);
			textcolor(LIGHTGRAY);
			textbackground(BLACK);
			cprintf("%s  ",opciones[i-tope_sup]);
			if (c=='\x48'){
				if(i==tope_sup)
					i=tope_sup+cant_opciones-1;
				else
					i--;
			}
			if (c=='\x50'){
				if(i==tope_sup+cant_opciones-1)
					i=tope_sup;
				else
					i++;
			}
			gotoxy(col_ini,i);
			textcolor(WHITE);
			textbackground(LIGHTBLUE);
			cprintf("> %s",opciones[i-tope_sup]);
		}
	}
	textcolor(LIGHTGRAY);
	textbackground(BLACK);
	return i-tope_sup;
}

/* Funcion ARRIESGAR()

	Recibe un puntero al nombre de la frase que hay que adivinar y
	devuelve 0 si arriesgo correctamente, 1 si es incorrecto y 2 si
	se presiono ESC*/

int arriesgar(char *p){
	char linea[MAXLARGO];

	window(2,21,79,23);
	textcolor(LIGHTGRAY);
	cprintf(" arriesgue: ");
	if(leo_linea(linea,MAXLARGO)==0)
		return 2;    //Esc
	mayusculas(linea);
	strcat(linea,"\n");
	clrscr();
	window(1,1,80,25);
	if (strcmp(linea,p)==0)
	    return 0;     // Iguales
	else
	    return 1;    // Distintas
}

/* Funcion OTRO_JUEGO()

	Devuelve S o N*/

char otro_juego()
{
	int letra=0;

	window(2,21,79,23);
	//gotoxy(1,25);
	cprintf(" ¨Desea jugar otra vez?(s/n)");
	while(letra!='S' && letra!= 'N')
		letra=toupper(getch());
	window(1,1,80,25);
	return letra;
}

/* Funcion PUNTOS_ERROR()

	Devuelve la cantidad de puntos correspondientes segun la cantidad de
	errores*/

int puntos_error(char error)
{
	switch (error){
		case 0:return 100;
		case 1:return 80;
		case 2:return 60;
		case 3:return 40;
		case 4:return 20;
		case 5:return 10;
		default: return 0;
	}
}

/* Funcion MUESTRO_MAIN_TEXTO()

	Muestra la pantalla principal del juego. Esta funcion recibe como
	parametros el nombre del jugador, el numero de partidas, los puntos y
	el fondo. Ademas hay un parametro adicional que imprime o no el ANSi*/

void muestro_main_texto(char *nombre,int j,int puntos, int fondo, int ansi)
{
	int jug_x, jug_y;

	switch(fondo){
		case 0:	if (ansi==1) //ansi determina si se debe volver a
				     //imprimir el ansi
				puttext (1,1,80,25,ICE);
			jug_x=ICE_JUG_X;
			jug_y=ICE_JUG_Y;
			break;
		case 1: if (ansi==1)
				puttext (1,1,80,25,NATURE);
			jug_x=NATURE_JUG_X;
			jug_y=NATURE_JUG_Y;
			break;
		case 2: if (ansi==1)
				puttext (1,1,80,25,INFERNO);
			jug_x=INFERNO_JUG_X;
			jug_y=INFERNO_JUG_Y;
			break;
	}
	gotoxy(jug_x,jug_y);
	printf("%s",nombre);
	gotoxy(jug_x,jug_y+1);
	printf("%i",j+1);
	gotoxy(jug_x,jug_y+2);
	printf("%i",puntos);
}

/* Funcion PIDO_LETRA()

	Se le pasa como parametro el arreglo letras que indica si las letras
	fueron jugadas o no. Cada posicion de este arreglo puede tomar 3
	valores diferentes: 1 indica que la letra no fue jugada, 0 indica
	que la letra ya se jugo una vez y 2 si la letra se repitio.
	Devuelve el valor de la letra.*/

int pido_letra(char *letras){
	int letra;

	letra=valido_letra(getch());
	while (letra==0){
		sound(250);
		delay(300);
		nosound();
		letra=valido_letra(getch());
	}
	letra=toupper(letra);
	if (letras[letra-'A']=='0')
		letras[letra-'A']='2';
	else
		if (letras[letra-'A']=='1')
			letras[letra-'A']='0';
	return letra;
}


int cargo_peli(char *argv,char *pelis[]){
	char linea[MAXLARGO];
	int i;
	FILE *arch=fopen(argv,"r");
	for(i=0;i<TOPEMAX;i++)
		pelis[i]=NULL;
	i=0;
	while(((fgets(linea,MAXLARGO-1,arch))!=NULL)&&(i<TOPEMAX)){
		if (strcmp(linea,"\n")!=0){
			mayusculas(linea);
			pelis[i]=(char *)malloc(strlen(linea)+1);
			strcpy(pelis[i],linea);
			i++;
		}
	}
	fclose(arch);
	return i; //devuelve la cantidad de lineas del txt para luego
		  //hacer el random hasta esa linea
}


void jugar(char *h_antes, char c, char *s, char *p, char *error, int fondo){
	int i, head_x, head_y;
	int esta=0;
	char pantalla[4096];

	for(i=0;i<4096;i++)
		pantalla[i]=0;
	i=1;
	switch(fondo){   // fondo determina el tema de la pantalla
		case 0: head_x=ICE_HEAD_X;
			head_y=ICE_HEAD_Y;
			break;
		case 1: head_x=NATURE_HEAD_X;
			head_y=NATURE_HEAD_Y;
			break;
		case 2: head_x=INFERNO_HEAD_X;
			head_y=INFERNO_HEAD_Y;
			break;
	}
	while (*s != '\n' ){
		if (c==*s){
			textcolor(WHITE);
			gotoxy(((c-64)*2)+1,19);
			cprintf("%c",c);
			gotoxy(3+i,15);
			printf("%c", c);
			esta=1;
			*(p+i-1)='1';
		}
		s++;
		i++;
	}
	gettext(1,1,80,25,pantalla);
	if (esta==0 ){
		if (c!='0'){
			textcolor(WHITE);
			gotoxy(((c-64)*2)+1,19);
			cprintf("%c",c);
		}
		(*error)++;
		switch (*error){
		case 1:	gotoxy(head_x,head_y);
			printf("O");
			gettext(1,1,80,25, pantalla);
			puttext(1,1,80,25,HORCA);
			puttext(17 , 6, 28, 14,HEAD);
			break;
		case 2:	gotoxy(head_x,head_y+1);
			printf("|");
			gettext(1,1,80,25, pantalla);
			puttext(1,1,80,25,h_antes);
			puttext(13,15,34,19,BODY);
			break;
		case 3: gotoxy(head_x+1,head_y+1);
			printf("\\");
			gettext(1,1,80,25, pantalla);
			puttext(1,1,80,25,h_antes);
			puttext(35,14,44,16,BRAZODER);
			break;
		case 4: gotoxy(head_x-1,head_y+1);
			printf("/");
			gettext(1,1,80,25, pantalla);
			puttext(1,1,80,25,h_antes);
			puttext(3,14,12,16,BRAZOIZQ);
			break;
		case 5: gotoxy(head_x+1,head_y+2);
			printf("\\");
			gettext(1,1,80,25, pantalla);
			puttext(1,1,80,25,h_antes);
			puttext(18,20,30,25,PDERECHA);
			break;
		case 6: gotoxy(head_x-1,head_y+2);
			printf("/");
			gettext(1,1,80,25, pantalla);
			puttext(1,1,80,25,h_antes);
			puttext(16,23,22,25,PIZQUIER);
			delay(1000);
			break;
	      }
	      gettext(1,1,80,25,h_antes);
	      delay(1000);
	      clrscr();
	 }
	 puttext(1,1,80,25,pantalla);
}

void helpig()
{
	char pantalla[4096];

	gettext(1,1,80,25,pantalla);
	clrscr();
	puttext(1,1,HELPIG_WIDTH,HELPIG_DEPTH,HELPIG);
	getch();
	clrscr();
	puttext(1,1,80,25,pantalla);
	return;
}

void helpmain()
{
	clrscr();
	puttext(1,1,HELPMAIN_WIDTH,HELPMAIN_DEPTH,HELPMAIN);
	getch();
	return;
}


int salir(int fondo){
	char letra=0;

	window(2,21,79,23);
	switch(fondo){

		case 0:	textcolor(LIGHTBLUE+128);break;
		case 1: textcolor(LIGHTGRAY+128);break;
		case 2: textcolor(LIGHTRED+128);break;
	}
	cprintf("\r\n AbANdONAR lA PARtidA? (s/n)");
	while ((letra!='S') && (letra!='N'))
		letra=toupper(getch());
	normvideo();
	clrscr();
	if(letra=='S')
		return 0; //quiere salir
	window(1,1,80,25);
	return 1;  //no quiere salir
}

int gano(char *s){
	int i;
	while(*s!='\0'){
		if (*s=='0')
			return 0;  //Perdio
		s++;
	}
	return 1; // Gano
}

void usage(){
	printf("þ %s Ver.%s COPilEft(c) 2k bY: %s þ\n\n",TITLE,VER,AUTHOR);
	printf("Modo de empleo:\n");
	printf("                 \"Ak2.eXe <archivo.ext>\"\n\n");
	printf("<archivo.ext> es el archivo de donde se extraeran las frases\n");
	exit(0);
}

int leo_linea(char *p, int tope){   //utilizamos tope para cortar la cantidad
				    //de caracteres a leer.
				    //para el nombre 20 para arriesgar 80
	char c, *s;
	s=p;
	c=getche();
	while(c!='\r'){
		if(c==8){
			if(s-p>=0){
				s=s-1;
				putchar(8);
				printf(" ");
                                putchar(8);
			}
			else{
				gotoxy(33,13);
				s=p-1;
			}
		}
		else{
			if (c==27)
				return 0;
			*s=c;
		}
		c=getch();
		if((s-p<tope-1)&&(c!=27)&&(c!=8)){
			putchar(c);
			s++;
		}
	}
	*s='\0';
	return 1;
}

int valido_letra(int letra){
	letra=toupper(letra);
	if(letra==0){
		letra=getch();
		if (letra==59 || letra==60 )
			return letra=(256+letra);
		else
			return 0;  // la letra no es valida
	}

	if ((letra>=65) && (letra<=90) || letra=='#' || letra==ESC)
		return letra;
	else
		return 0;
}

void pongo_rayas(char *p){
	gotoxy(4,15);
	while(*p != '\0'){
		if (*p==' ')
			printf("/");
		else
			if(*p!='\n')
				printf("_");
		p++;
	}
}

void mayusculas(char *p){
	while (*p!='\0'){
		*p=toupper(*p);
		p++;
	}
}

int exist(char *archivo){
   FILE *arch=fopen(archivo,"r");
   int existe;
   if (arch==NULL)
       existe=0;
   else
       existe=1;
   fclose(arch);
   return existe;
}