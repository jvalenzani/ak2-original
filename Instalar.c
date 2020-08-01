#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dir.h>
#include <dos.h>
#include <ctype.h>
#include <direct.h>
#include "instalar.inc"

#define DEF_DIR "ak2" 		// default dir
#define DEF_DEST 'C' 		// default dirve para instalar
#define ESPACIO 300  		// en kbytes
#define PRG_NAME "A^k2" 	// Nombre del programa
#define PRG_TYPE "JUEGO" 	// Tipo de programa
#define xini 27
#define yini 3

void main()
{

char i,c,inst_source=0,instalar[MAXPATH],source[2];
struct dfree free;
long avail;



inst_source=_getdrive(); // Drive de donde se ejecuta el install

switch(inst_source){             // Cambio el int segun el drive
	case 1: source[0]='A';break;   // y si no es disketera doy un
	case 2: source[0]='B';break;   // error.
	default: printf("\n\007Se debe ejecutar la instalacion de %s desde el diskette correspondiente\n",PRG_NAME);exit(1);
}
source[1]='\0';
instalar[0]='\0';

strcat(instalar,"copy ");   //
strcat(instalar,source);    //   Creo la cadena de instalacion
strcat(instalar,":\\*.* >null");  //

clrscr();
printf("AtENCiON: Se dispone a instalar el %s %s en %c:\\%s\\ \n",PRG_TYPE,PRG_NAME,DEF_DEST,DEF_DIR);
printf("\n Presione ESC para cancelar, ENTER para continuar");
c=getch();
while(c!=13 && c!=27){
	printf("AtENCiON: Se dispone a instalar el %s %s en %c\\%s\\ \n",PRG_TYPE,PRG_NAME,DEF_DEST,DEF_DIR);
	printf("\n Presione ESC para cancelar, ENTER para continuar");
	c=getch();
	}
if (c==27){
	printf("\n Instalacion Abortada\n");
	exit(0);
}

clrscr();
puttext(1,1,INSTALAR_WIDTH,INSTALAR_DEPTH,INSTALAR);
gotoxy(xini,yini);
printf("%c",DEF_DEST);
gotoxy(xini,yini+1);
printf("%s",DEF_DIR);
gotoxy(xini,yini+2);
printf("%s",PRG_NAME);
gotoxy(xini,yini+3);
printf("%s",PRG_TYPE);
gotoxy(xini,yini+4);
printf("%d kb aprox.",ESPACIO);

/* Veo si hay espacio libre */

getdfree(DEF_DEST-64, &free);
if (free.df_sclus == 0xFFFF)
{
	 printf("\007 Unidad inexistenete o No accesible \n");
	 exit(1);
}

avail =  (long) free.df_avail
				 * (long) free.df_bsec
				 * (long) free.df_sclus;


if ((avail/100) <= ESPACIO){
		printf("\n\007No hay espacio suficiente para instalar %s",PRG_NAME);
		exit(2);
}
/**/

/* Si ta todo bien, INSTALo.... */

if(_chdrive(DEF_DEST-64)==0){
	printf("\n");
	chdir("\\");
	if (mkdir(DEF_DIR)==0){
			chdir(DEF_DIR);
			system(instalar);
			gotoxy(4,10);
			textcolor(LIGHTBLUE);
			cprintf("Û");
			system("pkunzip ak2.zip -srecorcholis >null");
			textcolor(LIGHTGRAY);
			for(i=0;i<30;i++){
				gotoxy(4+i,10);
				textcolor(LIGHTBLUE);
				cprintf("Û");
				delay(100);
			    }
			textcolor(LIGHTGRAY);
			system("del *.zip >null");
			system("del instalar.exe >null");
			system("del pkunzip.exe >null");
			system("del null");
			for(i=30;i<40;i++){
				gotoxy(4+i,10);
				textcolor(LIGHTBLUE);
				cprintf("Û");
				delay(50);
			    }
			textcolor(LIGHTGRAY);

	 clrscr();
	 printf("Antes de jugar lea el archivo LEAME.TxT\n");
	 }
 else{
	printf("\007 Error al crear %s en %c:\n",DEF_DIR,c);
	exit(3);
 }
}

}