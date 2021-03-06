#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "Funciones_Suffrag.h"





/*FUNCION PARA LAS JUGADAS HECHAS POR LA COMPUTADORA*/
void Juega_Computadora(){
	int x,y,x2,y2,pieza,m;
	do{
		x = (rand() % (16-0+1));
		y = (rand() % (15-2+1))+2;
	}while(!Primer_Click(bandos[turno-1],x,y));
	g_print("\n x = %d \n\n y = %d \n",x,y);
	g_print("\n coor[0] = %d\n coor[1] = %d \n",coor[0],coor[1]);
	pieza = Tablero[x][y];
	do{
		do{
			x2 = (rand() % ((coor[0]+1)-(coor[0]-1)+1))+(coor[0]-1);
			y2 = (rand() % ((coor[1]+1)-(coor[1]-1)+1))+(coor[1]-1);
		}while(!limite_movimiento(x2,y2) || !verificar_adyacente(x,y,x2,y2) || !Limitacion_Bases(x2,y2,pieza));
		m = Movimiento(x,y,x2,y2,pieza);
	}while(m==0);
	bandera = 0;
	g_print("\n x2 = %d \n\n y2 = %d \n",x2,y2);

}





/*Imprime las Instrucciones del uso del programa*/
void instrucciones(){
	printf("          ----------INSTRUCCIONES PARA USO DEL PROGRAMA---------\n\n");
	printf(" -El usuario puede determinar quién inicia el juego, quién será suffragetto y quién será policía. \n  O bien los turnos y bandos se definen de forma aleatoria.\n\n -Para mover las piezas se deben introducir las coordenadas de la pieza a mover y las de la posicion a moverse.\n\n -Las coordendas del tablero van desde (0) hasta (16) para las filas y las columnas.\n\n -El programa se encargara de advertirle de errores o movimientos invalidos.\n\n -Cada movimiento valido que usted realice, podra ser corroborado en el tablero de juego.\n\n -Al finalizar el juego, el programa indicara cual es el bando ganador y perdedor.\n\n ");

}




/*Funcion que recibe dos contadores. Uno para la carcel, otro para el hospital.
 * Esta funcion se utiliza para saber si el numero de fichas habilitadas de ambos bandos es menor o igual a 5
 * puesto que si algun equipo tiene solo 5 fichas, no podra conquistar las seis casillas de la base enemiga.
 * Se alguno de estos dos contadores resulta ser mayor o igual a 16, algun equipo no podra ganar. Luego, su oponente es el ganador.*/
void rev_carcel_hosp(int *num3, int *num4){
	int i,j;

	/*Contar el numero de policias y/o Inspectores en el hospital*/
	for(i=0; i<17; i++){
		j = 0;
		if(Tablero[i][j]==1 || Tablero[i][j]==2){
			(*num3)++;
		}
	}
	/*Contar el numero de suffragettes y/o Lideres en la carcel*/
	for(i=0; i<17; i++){
		j = 16;
		if(Tablero[i][j]==3 || Tablero[i][j]==4){
			(*num4)++;
		}
	}

}


/*Limitacion para evitar que los bando entren en sus propias bases*/
int Limitacion_Bases(int x, int y, int pieza){
	int aux = pieza,m;
	if(aux==3 || aux==4){
		m = limitacion_Suffrag(x,y);
	}
	if(aux==1 || aux==2){
		m = limitacion_Polic(x,y);
	}
	return m;
}


/*Limitacion para evitar que las suffragettes entren en su propia base*/
int limitacion_Suffrag(int x, int y){
	gchar *text;
	if((x==3 || x==4) && (y>=7 && y<10)){
		printf("\nNo puede ingresar a esta casilla. \n");
		text = g_strdup_printf("\nError. No puede entrar en esta casilla. \nIntente con una casilla diferente. ");
		gtk_label_set_text(GTK_LABEL(label_estado), text);
		g_free(text);
		return 0;
	}else{
		return 1;
	}
}

/*Limitacion para evitar que los policias entren en su propia base*/
int limitacion_Polic(int x, int y){
	gchar *text;
	if((x==12 || x==13) && (y>=7 && y<10)){
		printf("\nNo puede ingresar a esta casilla. \n");
		text = g_strdup_printf("\nError. No puede entrar en esta casilla. \nIntente con una casilla diferente. ");
		gtk_label_set_text(GTK_LABEL(label_estado), text);
		g_free(text);
		return 0;
	}else{
		return 1;
	}
}

/*Combinacion con limite_tablero para impedir que las piezas entren en la carcel
 * u hospital*/
int limite_movimiento(int x, int y){
	gchar *text;
	int aux = limite_tablero(x,y);
	switch(aux){
		case 0:
			return 0;
		case 1:
			if((y==1 && x<4) || (y==15 && x>12)){
				printf("\nError. No puede entrar en esta casilla. \n");
				text = g_strdup_printf("\nError. No puede entrar en esta casilla. \nIntente con una casilla diferente. ");
				gtk_label_set_text(GTK_LABEL(label_estado), text);
				g_free(text);
				return 0;
			}else{
				return 1;
			}
	}
}

/*Verifica que las coordenadas de movimiento sean las de una casilla adyacentes
 * a la casilla seleccionada*/
int verificar_adyacente(int x, int y, int x2, int y2){
	gchar *text;
	if((x2==x) && (y2==y)){		//para evitar la misma casilla
		printf("\nCoordenadas inicial y final coinciden. Vuelva a ingresar las coordenadas. ");
		text = g_strdup_printf("\nCoordenadas inicial y final coinciden. \nIntente con una casilla diferente. ");
		gtk_label_set_text(GTK_LABEL(label_estado), text);
		g_free(text);
		return 0;
	}
	if(abs(x2-x)<=1 && abs(y2-y)<=1){	//abs es una funcion de stdlib.h
		return 1;
	}else{
		printf("\nCasilla fuera de la adyacencia. \n");
		text = g_strdup_printf("\nCasilla fuera de la adyacencia. \nIntente con una casilla diferente. ");
		gtk_label_set_text(GTK_LABEL(label_estado), text);
		g_free(text);
		return 0;
	}
}


/*Rotar jugadores*/
void rotar_jugador(){
	gchar *temp =  g_strdup_printf("   ");
	if(turno==1){		//si antes era turno = 1, ahora sera turno = 2
		turno = 2;
	}else{
		turno = 1;		//turno era = 2, luego pasa a ser 1.
	}
	if(bandera==1){
		bandera = 0;
	}
	gtk_label_set_text(GTK_LABEL(label_estado), temp);
	g_free(temp);
	g_print("\n turno = %d \n",turno);
}

/*Mostrar turnos. Imprime un mensaje sobre a quien le toca jugar */
void mostrar_turno(){
	if(turno == 1){
		gtk_label_set_text(GTK_LABEL(label_turno), nombre1);
	}else{
		gtk_label_set_text(GTK_LABEL(label_turno), nombre2);
	}
}


/*Imprime el Mensaje de fin del juego de acuerdo a las condiciones de fin del juego*/
void fin_del_juego(int num1, int num2, int num3, int num4){
	if(num3>=16){
		if(bandos[0]==1){
			printf("\nEl equipo Policias no tiene suficientes fichas para conquistar la base enemiga.\n\n\t\tVICTORIA DE LAS SUFFRAGETTES! \n");
			printf("\nEl Usuario ha ganado.\n");
		}else{
			printf("\nEl equipo Policias no tiene suficientes fichas para conquistar la base enemiga.\n\n\t\tVICTORIA DE LAS SUFFRAGETTES! \n");
			printf("\nLa Computadora ha ganado.\n");
		}
	}
	if(num4>=16){
		if(bandos[1]==3){
			printf("\nEl equipo Suffragetto no tiene suficientes fichas para conquistar la base enemiga.\n\n\t\tVICTORIA DE LOS POLICIAS! \n");
			printf("\nEl Usuario ha ganado.\n");
		}else{
			printf("\nEl equipo Suffragetto no tiene suficientes fichas para conquistar la base enemiga.\n\n\t\tVICTORIA DE LOS POLICIAS! \n");
			printf("\nLa Computadora ha ganado.\n");
		}
	}
	if(num1==6){
		printf("\n\n\t\t\tALBERT HALL CONQUISTADO! \n\n\t\t\tVICTORIA DE LOS POLICIAS! \n\n");
		if(bandos[0]==1){
			printf("\n\n\n\tLa Computadora ha ganado. ");
		}else{
			printf("\n\n\n\tEl Usuario ha ganado. ");
		}
	}
	if(num2==6){
		printf("\n\n\t\t\tHOUSE OF COMMONERS CONQUISTADO! \n\n\t\t\tVICTORIA DE LAS SUFFRAGETTES! \n\n");
		if(bandos[0]==3){
			printf("\n\n\n\tLa Computadora ha ganado. ");
		}else{
			printf("\n\n\n\tEl Usuario ha ganado. ");
		}
	}
	printf("\n\n\n\n\t\tFIN DEL JUEGO\n");
}





/*Maximo y Minimo valores posibles que pueden tomar las coordenadas x e y*/
int limite_tablero(int x, int y){
	gchar *text;
	if(x>=0 && x<17 && y>=1 && y<16){
		return 1;
	}else{
		g_print("\nCasilla fuera de limites. \n");
		text = g_strdup_printf("\nCasilla fuera de limites. \nIntente con una casilla diferente. ");
		gtk_label_set_text(GTK_LABEL(label_estado), text);
		g_free(text);
		return 0;
	}
}



/*Para verificar si alguna de las bases ha sido conquistada
 * Se utilizan num1 y num2(o mejor dicho sus direcciones), ambos se van incrmentando.
 * Si alguno de estos dos llega a valer seis, quiere decir que una de las bases
 * enemigas ha sido completamente invadida*/
void verificar_conquista(int *num1, int *num2){
	int i,j;
	/*Base de las Suffragettes. ALBERT HALL*/
	for(i=3; i<5; i++){
		for(j=7; j<10; j++){
			if(Tablero[i][j]==1 || Tablero[i][j]==2){
				(*num1)++;
			}
		}
	}
	/*Base de los Policias. COMMONERS HALL*/
	for(i=12; i<14; i++){
		for(j=7; j<10; j++){
			if(Tablero[i][j]==3 || Tablero[i][j]==4){
				(*num2)++;
			}
		}
	}
}




/*Verifica si la casilla seleccionada esta vacia o no*/
int verificar_vacia(int x, int y){
	gchar *text;
	if(Tablero[x][y]==0){
		return 1;
	}
	if(Tablero[x][y]==5 && bandos[turno-1]==3){
		return 1;
	}
	if(Tablero[x][y]==6 && bandos[turno-1]==1){
		return 1;
	}
	if(Tablero[x][y]>0 && bandera==0){
		printf("\nCasilla Ocupada o NO habilitada para su equipo. ");
		text = g_strdup_printf("\nCasilla Ocupada o NO habilitada para su Equipo. \nIntente con una casilla diferente. \n");
		gtk_label_set_text(GTK_LABEL(label_estado), text);
		g_free(text);
		return 0;
	}else if(Tablero[x][y]>0 && bandera==1){
		return 0;
	}
}



/*Revisa los alrededores de una ficha en busca de otras que esten en su adyacencia
 * Auxiliar para saltos en cadena */
int revisar_vecinos(int x, int y){
	B = 0;
	int i,j;
	int aux=0;

	for(i=x-1; i<x+2; i++){
		if(i<0)	// Para el caso en que la ficha este en la fila 0
			i = i+1;
		if(i>16)	// Para el caso en que la ficha este en la fila 16
			break;
		for(j=y-1; j<y+2; j++){
			if(j<1)	// Para el caso en que la ficha este en la columna 1
				j = j+1;
			if(j>15)	// Para el caso en que la ficha este en la columna 15
				break;
			if(!verificar_vacia(i,j)){
				aux = aux +1;
			}
		}
	}
	if(!verificar_vacia(x,y)){
		aux = aux-1;
	}

	return aux;		//Retorna aux, que es una cantidad que indica que existe al menos una o ninguna ficha en las cercanias
					//Esta funcion es mas que nada para ayudar en la accion de saltos multiples
}



/*Verifica si la pieza que se encuentra en la casilla seleccionada pertenece al bando
 * propio o al bando enemigo*/
int verificar_bando(int x, int y, int bando){
	gchar *text;
	if(Tablero[x][y]==bando || Tablero[x][y]==bando+1){
		return 1;
	}
	if(Tablero[x][y]!=bando){
		printf("\nCoordenada no valida. Pieza del bando opuesto o casilla vacia. \n");
		text = g_strdup_printf("\n\nCoordenada no valida. \nPieza del bando opuesto o casilla vacia. \n");
		gtk_label_set_text(GTK_LABEL(label_estado), text);
		g_free(text);
		return 0;
	}
}

/*Para verificar que la ficha enemiga este dentro de la arena de combate
 * de asi serlo, podra ser incapacitada por sus oponentes a la hora de ser saltada*/
int verificar_arena(int x, int y){
	gchar *text;
	if(x>=0 && x<17 && y>=4 && y<13){
		return 1;
	}else{
		printf("\nFuera de la arena de combate. \nNo puede incapacitar a esta pieza. \n");
		text = g_strdup_printf("\n\nFuera de la arena de combate. \nNo puede incapacitar a esta pieza. \n");
		gtk_label_set_text(GTK_LABEL(label_estado), text);
		g_free(text);
		return 0;
	}
}



/*Funcion encargada de cargar en la carcel/hostpital a las piezas derrotadas*/
void Pieza_Derrotada(int derrotado){
	int i,j;
	int aux = derrotado;
	if(aux==1 || aux==2){		//Inspector(1) o policia(2)
		for(j=0; j<2; j++){
			if(aux==0){
				break;
			}
			for(i=0; i<17; i++){
				if(Tablero[i][j]==0){
					Tablero[i][j] = aux;
					gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),j,i)), imagenes[aux]);
					aux = 0;
					break;
				}else{
					Tablero[i][j] = Tablero[i][j] +0;
				}
			}
		}
	}
	if(aux==3 || aux==4){		//Lider Suffragetto(3) o suffragette(4)
		for(j=16; j>14; j--){
			if(aux==0){
				break;
			}
			for(i=16; i>-1; i--){
				if(Tablero[i][j]==0){
					Tablero[i][j] = aux;
					gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),j,i)), imagenes[aux]);
					aux = 0;
					break;
				}else{
					Tablero[i][j] = Tablero[i][j] +0;
				}
			}
		}
	}
}


/*Auxiliar para la funcion Mov_Salto. Recoge las direcciones del posible salto*/
int aux_salto(int x, int y, int x2, int y2){
	if(x2>x && y2==y){		//Direccion vertical, descendiente
		return 1;
	}
	if(x2>x && y2<y){		//Diagonal derecha descendiente
		return 2;
	}
	if(x2>x && y2>y){		//Diagonal izquierda descendiente
		return 3;
	}
	if(x2==x && y2<y){		//Horizontal, de derecha a izquierda
		return 4;
	}
	if(x2==x && y2>y){		//Horizontal, de izquierda a derecha
		return 5;
	}
	if(x2<x && y2<y){		//Diagonal derecha ascendiente
		return 6;
	}
	if(x2<x && y2>y){		//Diagonal izquierda ascendiente
		return 7;
	}
	if(x2<x && y2==y){		//Vertical ascendiente
		return 8;
	}
}



/*Recoge y verifica las coordenadas de los posibles saltos*/

int Mov_Salto(int x, int y, int x2, int y2, int pieza){
	gchar *text;
	int x3,y3,m;
	B = 0;
	if(cont>0 && cont<3){
		m = verificar_vacia(x2,y2);
		if(m==1 || cont>=3){
			B = 0;
			cont = 4;
			return 9;
		}
	}
	int mov = aux_salto(x,y,x2,y2);		//Entra en accion la funcion aux_salto
	if(pieza==1 || pieza==3){	//1 y 3 son piezas especiales. Inspector y Lider suffragetto respectivamente
		switch(mov){			//pueden saltar en 8 direcciones
			case 1:
				x3 = x2 +1;
				y3 = y2 +0;
				break;
			case 2:
				x3 = x2 +1;
				y3 = y2 -1;
				break;
			case 3:
				x3 = x2 +1;
				y3 = y2 +1;
				break;
			case 4:
				x3 = x2 +0;
				y3 = y2 -1;
				break;
			case 5:
				x3 = x2 +0;
				y3 = y2 +1;
				break;
			case 6:
				x3 = x2 -1;
				y3 = y2 -1;
				break;
			case 7:
				x3 = x2 -1;
				y3 = y2 +1;
				break;
			case 8:
				x3 = x2 -1;
				y3 = y2 +0;
				break;
		}
	}
	if(pieza==2 || pieza==4){	//2 y 4 son piezas basicas. Policia y suffragette respectivamente
		switch(mov){			//solo pueden saltar en diagonal
			case 1:
				if(cont>0){
					printf("\nNo puede saltar en vertical u horizontal con esta ficha. \n");
					text = g_strdup_printf("\n\nNo puede saltar en vertical u horizontal con esta ficha. \n");
					gtk_label_set_text(GTK_LABEL(label_estado), text);
					g_free(text);
					cont = 4;
					return 9;
				}else{
					printf("\nNo puede saltar en vertical u horizontal con esta ficha. \nIntente con otras coordenadas. \n");
					text = g_strdup_printf("\n\nNo puede saltar en vertical u horizontal con esta ficha. \nIntente con una casilla distinta. \n");
					gtk_label_set_text(GTK_LABEL(label_estado), text);
					g_free(text);
					return 0;
				}
			case 2:
				x3 = x2 +1;
				y3 = y2 -1;
				break;
			case 3:
				x3 = x2 +1;
				y3 = y2 +1;
				break;
			case 4:
				if(cont>0){
					printf("\nNo puede saltar en vertical u horizontal con esta ficha. \n");
					text = g_strdup_printf("\n\nNo puede saltar en vertical u horizontal con esta ficha. \n");
					gtk_label_set_text(GTK_LABEL(label_estado), text);
					g_free(text);
					cont = 4;
					return 9;
				}else{
					printf("\nNo puede saltar en vertical u horizontal con esta ficha. \nIntente con otras coordenadas. /n");
					text = g_strdup_printf("\n\nNo puede saltar en vertical u horizontal con esta ficha. \nIntente con una casilla distinta. \n");
					gtk_label_set_text(GTK_LABEL(label_estado), text);
					g_free(text);
					return 0;
				}
			case 5:
				if(cont>0){
					printf("\nNo puede saltar en vertical u horizontal con esta ficha. \n");
					text = g_strdup_printf("\n\nNo puede saltar en vertical u horizontal con esta ficha. \n");
					gtk_label_set_text(GTK_LABEL(label_estado), text);
					g_free(text);
					cont = 4;
					return 9;
				}else{
					printf("\nNo puede saltar en vertical u horizontal con esta ficha. \nIntente con otras coordenadas. \n");
					text = g_strdup_printf("\n\nNo puede saltar en vertical u horizontal con esta ficha. \nIntente con una casilla distinta. \n");
					gtk_label_set_text(GTK_LABEL(label_estado), text);
					g_free(text);
					return 0;
				}
			case 6:
				x3 = x2 -1;
				y3 = y2 -1;
				break;
			case 7:
				x3 = x2 -1;
				y3 = y2 +1;
				break;
			case 8:
				if(cont>0){
					printf("\nNo puede saltar en vertical u horizontal con esta ficha. \n");
					text = g_strdup_printf("\n\nNo puede saltar en vertical u horizontal con esta ficha. \n");
					gtk_label_set_text(GTK_LABEL(label_estado), text);
					g_free(text);
					cont = 4;
					return 9;
				}else{
					printf("\nNo puede saltar en vertical u horizontal con esta ficha. \nIntente con otras coordenadas. \n");
					text = g_strdup_printf("\n\nNo puede saltar en vertical u horizontal con esta ficha. \nIntente con una casilla distinta. \n");
					gtk_label_set_text(GTK_LABEL(label_estado), text);
					g_free(text);
					return 0;
				}
		}
	}
	//Para verificar que las coordenadas del posible salto esten sean validas
	if(!limite_movimiento(x3,y3) || !verificar_vacia(x3,y3)){
		printf("\n\nLa casilla X2 = %d , Y2 = %d y su adyacente estan ocupadas. \nNo es posible saltar en esa direccion. ",x2,y2);
		text = g_strdup_printf("\nLa casilla X2 = %d , Y2 = %d y su adyacente estan ocupadas. \nNo es posible saltar en esa direccion. \n",x2,y2);
		gtk_label_set_text(GTK_LABEL(label_estado), text);
		g_free(text);
		return 0;
	}
	g_print("\n\n x2 = %d\n y2 = %d\n x3 = %d\n y3 = %d \n\n",x2,y2,x3,y3);
	/*Si la funcion llego hasta aqui, quiere decir que es posible realizar el salto.
	 * Por lo tanto se pregunta si la ficha a ser saltada pertenece al bando propio, sino, es un enemigo y puede ser incapacitada
	 * Se utilizan el vector bandos con turno como subindice*/
	if(Tablero[x2][y2] != bandos[turno-1] && Tablero[x2][y2] != (bandos[turno-1] +1) && verificar_arena(x2,y2)){
		Pieza_Derrotada(Tablero[x2][y2]);	//Entra en accion la funcion Pieza_Derrotada
		Tablero[x2][y2] = 0;
		gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),y2,x2)), imagenes[0]);
	}
	Movimiento(x,y,x3,y3,pieza);
	B = revisar_vecinos(x3,y3);
	/*Se guardan las coordenadas finales despues de la accion salto
	 * esto es por si es posible volver a saltar*/
	coor[0] = x3;
	coor[1] = y3;

	return 1;
}



/* Desplazamiento de piezas.
 * Si la casilla de destino esta vacia, se realiza un movimiento simple */
int Movimiento(int x, int y, int x2, int y2, int pieza){
	int p=0;
	if(verificar_vacia(x2,y2)){		//verificar_vacia() retorna verdadero (1)
		//La casilla del tablero de las coordenadas de destino se llena con la pieza
		Tablero[x2][y2] = pieza;
		gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),y2,x2)), imagenes[pieza]);

		//La casilla del tablero con las coordenadas iniciales se vacia
		Tablero[x][y] = 0;
		gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),y,x)), imagenes[0]);
		return 1;
	}
	/*Si no esta vacia, puede que sea posible saltar */
	if(!verificar_vacia(x2,y2)){	//verificar_vacia() retorna falso (0)
		/*Como puede que sea posible saltar, entra en accion la funcion Mov_Salto*/
		p = Mov_Salto(x,y,x2,y2,pieza);
		cont = cont+1;	//cont se incrementa en 1. Cont se puede incrementar hasta 3, que es el numero maximo de saltos consecutivos.
		return p;
	}

}




void Click_Dos(int pieza, int x, int y, int x2, int y2){
	int m=0;
	gchar *temp2 = g_strdup_printf("Presiono la casilla coordenada [%d,%d]", x2,y2);
	gtk_label_set_text(GTK_LABEL(click_dos), temp2);
	g_free(temp2);
	if(limite_movimiento(x2,y2) && verificar_adyacente(x,y,x2,y2) && Limitacion_Bases(x2,y2,pieza)){
		if(cont==0){
			m = Movimiento(x,y,x2,y2,pieza);
		}
		if(cont>0 && cont<3){
			m = Mov_Salto(x,y,x2,y2,pieza);
		}
	}
	g_print("\n\n B = %d\n cont = %d\n m = %d \n\n",B,cont,m);
	if(B==0 || cont>=3){
		B = 0;
		cont = 0;
		if(m>0){
			rotar_jugador();
		}else{
			g_print("\nLa jugada no es valida. Intente una jugada distinta. \n");
		}
	}
}



/*Inicio. Se selecciona la pieza a mover y se guardan sus coordenadas.*/
int Primer_Click(int bando, int x, int y){
	gchar *temp = g_strdup_printf("Presiono la casilla coordenada [%d,%d]", x,y);
	gtk_label_set_text(GTK_LABEL(click_uno), temp);
	g_free(temp);
    if(verificar_bando(x,y,bando) && limite_movimiento(x,y) && limitacion_Polic(x,y) && limitacion_Suffrag(x,y)){
    	bandera = 1;
    	coor[0] = x;
    	coor[1] = y;
    	g_print("\n OK! \n");
    	return 1;
    }else{
		g_print("\n NOT OK!! \n");
		return 0;
	}
}

