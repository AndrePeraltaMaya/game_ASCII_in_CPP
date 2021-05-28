#include<stdio.h>
#include<windows.h>/**ventana y caracteres en ella**/
#include<conio.h> /**teclas**/
#include<stdlib.h>

using namespace std;



#define UP 72  /**DEFINE SIRVE PARA DECLARAR CONSTANTES**/
#define LEFT 75
#define RIGHT 77 /**ASCII **/
#define DOWN 80


/**------------  VENTANA ---------------**/
void gotoxy(int x,int y){
/**Posicionar un caracter en una posición de la pantalla**/

	HANDLE window; /**la ventana**/
	/**tipo de control que vamos a tener en la ventana (salida)**/
	window = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coordenada; /**creamos un objeto COORD y asignamos las coordenadas**/
	coordenada.X = x;
	coordenada.Y = y;

	/**dar una posicion en el cursor (|) y en dicha posición escribir lo que queramos**/
	/**dos parametros el HANDLE (nombre de la ventana) y las coordenadas**/
	SetConsoleCursorPosition(window,coordenada);

}
/**----------------------------**/
/**--------------  CURSOR  --------------**/
void cursor_oculto(){
/**ocultar el cursor en la ventana**/
    HANDLE window;
    window = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cur;
    cur.dwSize = 50;/**1-100 tamano del cursor**/
    cur.bVisible = FALSE;/**ocultar el cursor**/

    SetConsoleCursorInfo(window,&cur);
}
/**----------------------------**/
/**------------ LIMITES   ----------------**/
void pintar_limites(){
/**pinta caracteres para simular paredes**/
    for(int i = 2; i<78 ; i++){/**horizontal**/
        gotoxy(i,3); printf("%c",205);
        gotoxy(i,58); printf("%c",205);
    }

    for(int i = 4; i< 58 ; i++){/**vertical**/
        gotoxy(2,i); printf("%c",186);
        gotoxy(77,i); printf("%c",186);
    }
    gotoxy(2,3); printf("%c",201); //esquina superior izquierda
    gotoxy(77,3); printf("%c",187); //esquina superior derecha
    gotoxy(2,58); printf("%c",200); //esquina inferior izquierda
    gotoxy(77,58); printf("%c",188); //esquina inferior derecha
}
/**----------------------------**/
/**------------- SUPER CLASE SHIP---------------**/
class ship{
protected:

    int x,y; /**posición de la nave**/
    int health; /**corazones de la nave**/
    int lifes; /**cantidad de vidas**/

public:
        ship(int _x, int _y,int _health,int _lifes): x(_x),y(_y),health(_health),lifes(_lifes){}

        virtual void pintar() = 0;/**muestra la nave**/
        void borrar();/**borra la nave (movimieto)**/
        virtual void mover() = 0;/**Mueve la nave**/
        virtual void death() = 0;/**aimacion de muerte**/


        void corazonMenos(){
            /**resta un corazón**/
            health--;
        }
        int getLifes(){
            return lifes;
        }

        int getX(){
            /**regresa el coordenada x de la nave**/
            return x;
        }
        int getY(){
            /**regresa el coordenada y de la nave**/
            return y;
        }

        const int n_health = health; /**para restaurar los corazones en una nueva vida**/

};

/** :: nos sirve para acceder a los metodos de una clase **/
void ship::borrar(){
/**elimina la nave para darle el efecto de movimiento**/
    gotoxy(x-1,y);  printf("       ");
    gotoxy(x-1,y+1);printf("       ");
    gotoxy(x-1,y+2);printf("       ");
}

/**---------------------------------------**/
/**------------- SUB CLASE NAVE---------------**/
class nave:public ship{

    public:
        // Inherit all constructors from Base
        using ship::ship;

        void mover();
        void pintar();
        void mostrar_salud();/**muestra la salud en pantalla**/
        void mostrar_vidas();/****/
        void death();/**aimacion de muerte**/
};

void nave::mover(){
/**permite que la nave se mueva**/

    if(kbhit()){
        char tecla = getch();/**sirve para obtener la tecla que se está presionando**/
        borrar();
        if((tecla == 'a' || tecla == LEFT) && x > 4 )x--;
        else if((tecla == 's' || tecla == DOWN) && y+3 < 58 )y++;
        else if((tecla == 'w' || tecla == UP) && y > 4)y--;
        else if((tecla == 'd' || tecla == RIGHT) && x+6 < 77)x++;
        if(tecla == 'q' ){
            health--;
            mostrar_salud();
        }
        gotoxy(x,y);pintar();
    }
}
void nave::pintar(){
/**muestra la nave**/
    gotoxy(x,y);   printf("  %c",30);
    gotoxy(x,y+1); printf(" %c%c%c",40,207,41);
    gotoxy(x,y+2); printf("%c%c %c%c",30,190,190,30);
}
void nave::mostrar_salud(){
/**muestra los corazones **/
    gotoxy(64,2); printf("Health");
    gotoxy(72,2); printf("      ");

    for(int i = 0;i<health;i++){
        gotoxy(72+i,2); printf("%c",3);

    }
}
void nave::mostrar_vidas(){
/**muestra el número de vidas **/
    gotoxy(50,2); printf("Lifes %d",lifes);

}

void nave::death(){
/**Animacion de muerte**/
    if(health == 0){
        borrar();
        gotoxy(x,y);   printf("  **  ");
        gotoxy(x,y+1); printf(" **** ");
        gotoxy(x,y+2); printf("  **  ");
        Sleep(200);
        borrar();

        gotoxy(x,y);   printf("* ** *");
        gotoxy(x,y+1); printf(" **** ");
        gotoxy(x,y+2); printf("* ** *");
        Sleep(200);
        borrar();

        gotoxy(x,y);   printf("  ** ");
        gotoxy(x,y+1); printf(" ****");
        gotoxy(x,y+2); printf("  ** ");
        Sleep(200);
        borrar();

        lifes--;
        mostrar_vidas();
        health = n_health;
        mostrar_salud();
        pintar();
    }
}

/**------------- SUB CLASE NAVE ---------------**/
/**------------- SUB CLASE ENEMY---------------**/
class enemy:public ship{
    protected:

        bool movLeft = TRUE;
        bool movRigh = FALSE;

        int velocidad = 2;
        int cont = velocidad;

        bool lifeIsTrue = TRUE;

    public:
        // Inherit all constructors from Base
        using ship::ship;

        void mover();
        void pintar();
        void death();/**aimacion de muerte**/
        bool getLife(){
            return lifeIsTrue;
        }

        void setVelocidad(int v){
            velocidad = v;
        }

};


void enemy::mover(){


    if(cont%velocidad == 0){
        borrar();
        gotoxy(x,y);pintar();

        if(x == 6){
            movLeft = FALSE;
            movRigh = TRUE;
        }
        if(x+6 == 76 ){
            movLeft = TRUE;
            movRigh = FALSE;
        }

        if(movLeft )x--;
        if(movRigh )x++;

        cont++;
    }else{
        cont++;
    }

}

void enemy::pintar(){
/**muestra la nave**/
    gotoxy(x,y); printf("%c%c%c%c",47,77,77,92);
    gotoxy(x,y+1); printf("%c%c%c%c",92,126,126,47);
}

void enemy::death(){
/**Animacion de muerte**/
    if(health == 0){
        borrar();
        gotoxy(x,y);   printf("  **  ");
        gotoxy(x,y+1); printf(" **** ");
        gotoxy(x,y+2); printf("  **  ");
        Sleep(200);
        borrar();

        gotoxy(x,y);   printf("* ** *");
        gotoxy(x,y+1); printf(" **** ");
        gotoxy(x,y+2); printf("* ** *");
        Sleep(200);
        borrar();

        gotoxy(x,y);   printf("  ** ");
        gotoxy(x,y+1); printf(" ****");
        gotoxy(x,y+2); printf("  ** ");
        Sleep(200);
        borrar();

        lifes--;
        health = n_health;
        pintar();
        if(lifes < 0){
            lifeIsTrue = FALSE;
        }
    }

}
/**-------------- SUB CLASE ENEMY --------------**/
/**-------------- SUB CLASE BOSS --------------**/
class Boss: public enemy{
    public:
        // Inherit all constructors from Base
        using enemy::enemy;

        void pintar();
        void borrar();
};
void Boss::pintar(){
/**muestra la nave**/

    gotoxy(x,y); printf("%c%c %c%c",30,190,190,30);
    gotoxy(x,y+1); printf(" %c%c%c",40,207,41);
    gotoxy(x,y+2);   printf("  %c",30);


}



/**-------------- SUB CLASE BOSS --------------**/
/**-------------- CLASE ASTEROIDE --------------**/

class asteroid{


    int x,y;

    public:
        asteroid(int _x, int _y):x(_x),y(_y){}
        void show();
        void mover();
        void choque(class nave &N);
        void choque(class enemy &N);
};

void asteroid::show(){
/**mostrar un asteroide**/
    gotoxy(x,y); printf("|");

}
void asteroid::mover(){
/**mover el asteroide**/
    gotoxy(x,y); printf(" ");
    y++;
    if(y > 57){

        x = rand()%71 + 4;
        y = 4;
    }
    show();

}
void asteroid::choque(class nave &N){
    if(x >= N.getX() && x < N.getX()+6 && y >= N.getY() && y <= N.getY()+2){

        N.corazonMenos();

        N.borrar();
        N.pintar();
        N.mostrar_salud();

        x = rand()%71 + 4;
        y = 4;

    }
}
void asteroid::choque(class enemy &N){
    if(x >= N.getX() && x < N.getX()+6 && y >= N.getY() && y <= N.getY()+2){

        N.corazonMenos();

        N.borrar();
        N.pintar();

        x = rand()%71 + 4;
        y = 4;

    }
}

/**--------------- CLASE ASTEROIDE -------------**/
/**------------------  MAIN  -----------------**/
int main(){

    cursor_oculto();
    pintar_limites();


    nave player(35,55,4,4);
    player.pintar();
    player.mostrar_salud();
    player.mostrar_vidas();

    enemy one(15,15,2,2);
    one.pintar();

    Boss two(15,5,8,10);
    two.pintar();
    two.setVelocidad(5);


    /**--------animacion del fondo ---------**
    asteroid ast(10,4);
    asteroid ast2(4,8);
    asteroid ast3(15,10);
    asteroid ast4(8,10);
    /**--------animacion del fondo ---------**/



    bool game_over = FALSE;

	while(!game_over){


 /**--------animacion del fondo ---------**
        ast.mover(); ast.choque(player);  ast.choque(one);
        ast2.mover();ast2.choque(player); ast2.choque(one);
        ast3.mover();ast3.choque(player); ast3.choque(one);
        ast4.mover();ast4.choque(player); ast4.choque(one);

/**--------animacion del fondo ---------**/

        player.death();
        player.mover();



        if(player.getLifes() < 0){

            game_over = TRUE;

        }
        if(one.getLife()){
            one.mover();
            one.death();
        }else{
            one.borrar();
        }
        two.mover();
        two.death();


        Sleep(20);

    }

	return 0;

}




