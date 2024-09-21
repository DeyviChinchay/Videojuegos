//INTEGRANTES
//Cavero Winchez, Oneyda
//Luis Perez, Anthony Joel


#include<stdio.h>
#include<windows.h>
#include<iostream> 
#include<conio.h>
#include<stdlib.h>
#include<list>


#include <MMsystem.h> //audio

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
using namespace std;

//para subir de nivel
int nivel = 1;
int vidas_ast = 2; //1
bool condicion = false;

void gotoxy(int x, int y){

	
	HANDLE hCon;
	hCon=GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X=x;
	dwPos.Y=y;
	SetConsoleCursorPosition(hCon,dwPos);
}

void OcultarCursor(){
	
	HANDLE hCon;
	hCon=GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;	
	cci.dwSize=2;
	cci.bVisible=FALSE;
	
	
	SetConsoleCursorInfo(hCon,&cci);
	
}

void pintar_limites(){
	for(int i=2;i<78;i++){
	gotoxy(i,3); printf("%c",205);
	gotoxy(i,30);printf("%c",205);
	}
	
	for(int i=4;i<30;i++){
	gotoxy(2,i); printf("%c",186);
	gotoxy(77,i);printf("%c",186);
	}
	
	gotoxy(2,3); printf("%c",201);
	gotoxy(2,30); printf("%c",200);
	gotoxy(77,3); printf("%c",187);
	gotoxy(77,30); printf("%c",188);


}


class NAVE{
	int x,y;
	int corazones;
	int vidas;
public:
	NAVE(int _x,int _y, int _corazones, int _vidas):x(_x),y(_y),corazones(_corazones),vidas(_vidas){	}
	int X(){ return x;}
	int Y(){ return y;}
	int VID(){ return vidas;
	}
	void COR(){corazones--;	}
    void pintar();
	void borrar();
	void mover();	
	void pintar_corazones();
	void morir();
};



void NAVE::pintar(){
	gotoxy(x,y);   printf("   *    ",30);
	gotoxy(x,y+1); printf("  ***   ",40,207,41);
	gotoxy(x,y+2); printf(" *****  ",30,190,190,30);


}

void NAVE::borrar(){
	gotoxy(x,y); 	printf("        ");
	gotoxy(x,y+1); 	printf("        ");
	gotoxy(x,y+2); 	printf("        ");
}

void NAVE::mover(){
 
 if(kbhit()){
			char tecla=getch();
			borrar();
			if(tecla==IZQUIERDA && x>3) x--;
			if(tecla== DERECHA && x+6<75) x++;
			if(tecla=='e') corazones--;
			pintar();
			pintar_corazones();
		}
}

void NAVE::pintar_corazones(){
	gotoxy(50,2); printf("Vidas %d", vidas);
	gotoxy(64,2); printf("Salud");
	gotoxy(70,2); printf("      ");
	for(int i=0;i<corazones;i++){
		gotoxy(70+i,2); printf("%c",3);
	}
}

void NAVE::morir(){
	if(corazones==0){
	
		PlaySound(TEXT("audio\\perdite.wav"),NULL, SND_ASYNC); // Reproduce un sonido cuando un asteroide toca a la nave   
		borrar();    
		gotoxy(x,y);   printf("   **   ");
		gotoxy(x,y+1); printf("  ****  ");
		gotoxy(x,y+2); printf("   **   ");
		Sleep(200);
		borrar();
		gotoxy(x,y);   printf(" * ** *");
		gotoxy(x,y+1); printf("  **** ");
		gotoxy(x,y+2); printf(" * ** *");
		Sleep(200);
		borrar();
		vidas--;
		corazones=3;
		pintar_corazones();
		pintar();
		
	}
}

class AST{
	int x,y;
	int vida;
	int velocidad;// 
    int contador;// 
public:
	
    //AST(int _x, int _y): x(_x), y(_y), vida(2), velocidad(3), contador(0) {} //  vida inicial de los asteroides sea 2 + vidaAdicionalAsteroides 
    AST(int _x, int _y, int _vida) : x(_x), y(_y), vida(_vida), velocidad(3), contador(0) {}
	int X() { return x; }
    int Y() { return y; }
    void pintar();
    void mover();
    void choque(class NAVE &N);
    void impacto();
    void incrementatVida(){vida++;} //Nuevo método para obtener la vida
    bool destruir() { return vida == 0; }
};


void AST::pintar(){
	
	gotoxy(x,y); printf("%c",184);
	
}

void AST::mover(){
  gotoxy(x,y); printf(" ");
  //y++;
   contador++;
   if(contador >= velocidad) {
        y++;
        contador = 0;
    }
  if(y>29){
  	x=rand()%71+4;
  	y=4;
  	vida=2; 
  }
  pintar();
  
  }	
  
  void AST:: choque(class NAVE &N){
  	if (x>=N.X()&& x<N.X()+6 && y>=N.Y()&& y<N.Y()+2)
  	{ N.COR();
  	  N.borrar();
  	  N.pintar();
  	  N.pintar_corazones();
  	  x=rand()%71+4;
      y=4;
      vida= 2; //vida asteroide
  	}
  }
  
  
  void AST::impacto(){
    vida--;
    
    
    if(destruir() ){
        gotoxy(x, y); printf(" ");
        x = rand() % 71 + 4;
        y = 4;
        vida = 2; // Resetear la vida del asteroide
        PlaySound(TEXT("audio\\eliminado.wav"),NULL, SND_ASYNC); // asteroide eliminado  
    }
   
   
}


  
  class BALA{
  	int x,y;
  public:
  	BALA(int _x,int _y): x(_x),y(_y){	}
  	int X(){return x;}
  	int Y(){return y;}
  	void mover();
  	bool fuera();
  };
  
  void BALA::mover(){
  	gotoxy(x,y); printf(" ");
    y--;
  	gotoxy(x,y); printf("*");
  }

bool BALA::fuera(){
	if(y==4) return true;
	return false;
}

void mostrarMensajeNivel() {
    gotoxy(35, 14); printf("Nivel %d alcanzado!",nivel);
    Sleep(2000); // Esperar 2 segundos
    gotoxy(35, 14); printf("                      "); // Borrar el mensaje
}


int main(){
	OcultarCursor();
	pintar_limites();
	NAVE N(37,27,3,3);
	N.pintar();
	N.pintar_corazones();
	
	list<AST*> A;
	list<AST*>::iterator itA;
   

	for(int i=0;i<5;i++){
		//A.push_back(new AST(rand()%75+3,rand()%5+4));
	      A.push_back(new AST(rand() % 75 + 3, rand() % 5 + 4, vidas_ast));
	}
	list<BALA*> B;
	list<BALA*>::iterator it;
	
	bool gameover = false;
    int puntos = 0;
    int puntosTotales = 0;
    while(!gameover){
        gotoxy(4, 2); printf("Puntos %d", puntos);
        if(kbhit()){
            char tecla = getch();
            if(tecla = IZQUIERDA){
            	B.push_back(new BALA(N.X() + 2, N.Y() - 1));
				PlaySound(TEXT("audio\\disparo.wav"),NULL, SND_ASYNC);
			}
                
        }

        for(it = B.begin(); it != B.end(); it++){
            (*it)->mover();
            if((*it)->fuera()){
                gotoxy((*it)->X(), (*it)->Y()); printf(" ");
                delete(*it);
                it = B.erase(it);
            }
        }

        for(itA = A.begin(); itA != A.end(); itA++){
            (*itA)->mover();
            (*itA)->choque(N);
        }

        for(itA = A.begin(); itA != A.end(); itA++){
            for(it = B.begin(); it != B.end(); it++){
                if((*itA)->X() == (*it)->X() && ((*itA)->Y() + 1 == (*it)->Y() || (*itA)->Y() == (*it)->Y())){
                    gotoxy((*it)->X(), (*it)->Y()); printf(" ");
                    delete(*it);
                    it = B.erase(it);
                    (*itA)->impacto();
                    if((*itA)->destruir()){
                        gotoxy((*itA)->X(), (*itA)->Y()); printf(" ");
                        delete(*itA);
                        itA = A.erase(itA);     
                     
                    }
                    puntos += 5;
                    puntosTotales +=5;
                    //cambio de nivel
					if(!condicion){
						condicion = true;
					}
	
					if(puntos == 50){
						nivel++;
						vidas_ast++;
						gotoxy(35,2); printf("NIVEL %i",nivel);
						puntos = 0;
					
						mostrarMensajeNivel(); //Mensaje
						for (int i = 0; i < 5; i++) {
			                A.push_back(new AST(rand() % 75 + 3, rand() % 5 + 4, vidas_ast));
			            }
					
					
					}
					if(nivel == 5){
						cout<<PlaySound(TEXT("audio\\levelCompleted.wav"),NULL, SND_SYNC);
						gotoxy(35,155);printf("GANASTE");
						gotoxy(35,157); printf("Puntaje Final %d", puntosTotales);
						gameover = true;
						Sleep(2000);
					}
                    break;
                    
                }
            }
        }

        if(N.VID() == 0) {
		gameover = true;
                
                cout<<PlaySound(TEXT("audio\\perdite.wav"),NULL, SND_SYNC); // Reproduce un sonido        
                gotoxy(35,215);printf("Game Over");// Mostrar mensaje en pantalla
                gotoxy(35,217); printf("Puntaje Final %d", puntosTotales);
                Sleep(2000); // Esperar 2 segundos antes de salir
                break;
        }
        N.morir();
        N.mover();
        Sleep(30);
    }

    return 0;
}
