#include <iostream>
#include <stdio.h>
#include <cstdlib>             //librerie
#include <windows.h>
#include <cstdio>
#include <time.h>
#include <conio.h>
#include <fstream>

#define Y_BASE 2          //y di partenza
#define Y_INCRE 4       //come incrementare la y per andare di quadrato in quadrato(verticale)
#define X_BASE 4         //x di partenza
#define X_INCRE 8         //la x per andare di quadrato in quadrato(orizzontale)
#define N 10                   //righe  matrice
#define M 10                //colonne matrice
#define sleep 50           //costante per lo sleep (in minuscolo)
#define verde_chiaro 10              //per non cercare sempre i numeri dei colori più usati (in minuscolo)
#define nero 0
#define giallo 14
#define bianco 15
#ifndef CURSORI_H_INCLUDED        //goto xy
#define CURSORI_H_INCLUDED       //goto xy
#define CONSOLE_FULLSCREEN_MODE 1  //schermo intero

using namespace std;
/*
0 BLACK
1 BLUE
2 GREEN
3 CYAN
4 RED
5 MAGENTA
6 BROWN                                           //codici dei colori
7 LIGHTGRAY
8 DARKGRAY
9 LIGHTBLUE
10 LIGHTGREEN
11 LIGHTCYAN
12 LIGHTRED
13 LIGHTMAGENTA
14 YELLOW
15 WHITE
*/
void schermoIntero();  //prototipo schermo intero
typedef BOOL (WINAPI *SetConsoleDisplayModeProc) (HANDLE, DWORD, PCOORD); //schermo intero
void gotoXY(int x, int y)    //sottoprogramma per i cursori x ed y
{
    COORD CursorPos = {x, y};
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, CursorPos);
}
void stampa_menu();   // prototipo menù
int scelta_menu(int scelta, int x, int y);
void riempi_cas(int mat[N][M], int ncaratteri);   //prototipo riempimento casuale
void stampa_tab(int x, int y,int mat[N][M]);   //prototipo tabellone di gioco
void controllo(int mat[N][M],int mat2[N][M]);  //prototipo controllo sulle matrici e copio
void regole();    // prototipo dello stampaggio delle regole
bool controlloMatriceVuota(int mat[N][M]); //prototipo controllo se la matrice è vuota
void SonoMortiTutti();        //prototipo stampo se la matrice è vuota
void stampa_personalizzazione(); //prototipo stampa personalizzazione
int frecciaPersonalizzazione(int scelta2,int x, int y);  //prototipo scelta personalizzazione
void setColor(int bg, int fg); //prototipo per impostare il colore sfondo e poi carattere
int richiesta_SalvaIniziale(int richiestaSalva,int x, int y);  //prototipo richiesta caricamento salvataggio
int Salva_inGioco(int salva,int x, int y,int generazione);  //prototipo richiesta salvataggio ogni 25 generazioni



int main()
{
    int mat[N][M];     //prima matrice generazioni
    int mat2[N][M];   //seconda matrice per il controllo
    int x=X_BASE; //x gotoxy
    int y=Y_BASE;  //y gotoxy
    int generazione=0;
    int scelta=0;            //scelta del menu principale
    int scelta2=0;        //scelta della personalizzazione
    bool matriceVuota;   //guardo se la matrice è vuota
    int velocita=100;     //valore di default modificabile da utente
    int ncaratteri=50;  //valore di default modificabile da utente
    int maxGenerazione=1000; //Valore di default modificabole da utente
    int salva=0;              //richiesta se salvare
    int richiestaSalva=0;    //richiesta di prendere salvataggio
    fstream mioFile;

    schermoIntero();  //invocazione schermo intero
    do
    {   generazione=0;
        stampa_menu();//invocazione menù
        scelta=scelta_menu(scelta,x,y);  //invocazione funzione scelta
        switch(scelta)
        {
             case 1:                  //stampo regole
                        system("cls");
                        regole();
                        system("pause");
                        break;
             case 2:                                                                        //stampo gioco
                        richiestaSalva=richiesta_SalvaIniziale(richiestaSalva,x,y); //implementazione se prendere salvataggio
                        if(richiestaSalva==1)  //riprendo ultimo salvataggio
                        {
                            mioFile.open("mat.txt",ios::in);
                            while(!mioFile.eof())
                            {
                               for(int i=0;i<N;i++)
                                {
                                    for(int j=0;j<M;j++)
                                    {
                                        mioFile>>mat[i][j];
                                    }
                                }
                            }
                            mioFile.close();
                            system("cls");
                        }
                        else
                        {
                            system("cls");
                            riempi_cas(mat,ncaratteri);  //invocazione riempi cas
                            system("cls");
                        }

                        stampa_tab(x,y,mat); //invocazione tabellone di gioco
                        gotoXY(20,43);
                        system("pause");
                        system("cls");
                        do
                        {
                            controllo(mat,mat2);  //invocazione del controllo sulla matrice 1
                            matriceVuota=controlloMatriceVuota(mat); //invocazione controllo mat vuota
                            stampa_tab(x,y,mat);  // invocazione e stampo la generazione successiva
                            gotoXY(100,5);
                            generazione++;
                            cout<< "Generazione "<<generazione;
                            salva=Salva_inGioco(salva,x,y,generazione);
                            if(salva==1) //salvo la generazione
                            {
                                mioFile.open("mat.txt", ios :: out); //salvo matrice nel file
                                for(int i=0;i<N;i++)
                                {
                                    for(int j=0;j<M;j++)
                                    {
                                        mioFile<< mat[i][j];
                                        mioFile<<endl;
                                    }
                                mioFile<<endl;
                                }
                            mioFile.close();
                            gotoXY(20,42);
                            return 0;
                            }
                            Sleep(velocita);
                            system ("cls");
                             if(generazione==maxGenerazione)  //raggiunto limire impostato dall'utente
                             {
                                cout<<"\n Sei arrivato al limite impostato delle generazioni, verrai riportato al menu' iniziale"<<endl;
                                system("pause");
                             }
                        }while(matriceVuota==false&&generazione!=maxGenerazione);
                        if(matriceVuota==true)
                        {
                            cout<< "\nLa generazione "<<generazione<< " non e' sopravvissuta"<< endl;
                            SonoMortiTutti(); //invocazione stampa morte
                            system("pause");
                        }
                        break;
             case 3:                    //impostazione dei parametri selezionati nella personalizzazione
                        do
                        {
                        stampa_personalizzazione(); //invocazione stampa personalizzazione
                        scelta2=frecciaPersonalizzazione(scelta2,x,y); //invocazione scelta personalizzazione (quale opzione modificare)
                        switch(scelta2) //scelta delle personalizzazioni
                        {
                            case 1:
                                    system("cls");
                                    do
                                    {
                                        cout<< "Inserisci la velocita' di generazione desiderata (ms e maggiore di 0) --> ";
                                        cin>> velocita;
                                    }while(velocita<=0);
                                    break;
                            case 2:
                                    system("cls");
                                    do
                                    {
                                        cout<< "Inserisci il numero limite di caratteri che vuoi nella matrice (max 100 e min 1) --> ";
                                        cin>> ncaratteri;
                                    }while(ncaratteri>100||ncaratteri<1);
                                    break;
                            case 3:
                                    system("cls");
                                    do{
                                        cout<<"Scegli a quale generazione vuoi eventualmente fermarti --> ";
                                        cin>>maxGenerazione;
                                      }while(maxGenerazione<1);
                        }
                        }while(scelta2!=4);
                        break;


             case 4:                            //esco
                    system("cls");
                    return 0;
                    break;

        }
        system("cls");
    }while(1!=2); //infinito per tornare sempre al menù (tranne quando si vuole uscire)
    return 0;
}



void stampa_menu()       //implementazione dello stampa menù
{
    setColor(nero,verde_chiaro);cout<<"          ____________________________________________________________________________            "<<endl;
    cout<<" ________|                                                                            |________   "<<endl;
    cout<<" \\       |                                                                            |       /  "<<endl;
    cout<<"  \\      |       ___ _                       _      _ _               _ _             |      /   "<<endl;
    cout<<"   \\     |      / _ (_) ___   ___ ___     __| | ___| | | __ _  __   _(_| |_ __ _      |     /    "<<endl;
    cout<<"    \\    |     / /_\\| |/ _ \\ / __/ _ \\   / _` |/ _ | | |/ _` | \\ \\ / | | __/ _` |     |    /     "<<endl;
    cout<<"    /    |    / /_\\\\| | (_) | (_| (_) | | (_| |  __| | | (_| |  \\ V /| | || (_| |     |    \\    "<<endl;
    cout<<"   /     |    \\____/|_|\\___/ \\___\\___/   \\__,_|\\___|_|_|\\__,_|   \\_/ |_|\\__\\__,_|     |     \\   "<<endl;
    cout<<"  /      |____________________________________________________________________________|      \\  "<<endl;
    cout<<" /__________)                                                                      (__________\\ "<<endl;setColor(nero,bianco);
    setColor(nero,giallo);cout<<"    _____________________________ ";setColor(nero,bianco);  cout<<"     _____________________________________________________ "<<endl;
    setColor(nero,giallo);cout<<"   /        _____________        \\";setColor(nero,bianco);  cout<<"    |                                                     |"<<endl;
    setColor(nero,giallo);cout<<"   | == .  |             |     o |";setColor(nero,bianco); cout<<"    |                                                     |"<<endl;
    setColor(nero,giallo);cout<<"   |   _   | BENVENUTI!  |   B   |";setColor(nero,bianco); cout<<"    |                                                     |"<<endl;
    setColor(nero,giallo);cout<<"   |  / \\  |             | A  O  |";setColor(nero,bianco); cout<<"    |                       1 Regole                      |"<<endl;
    setColor(nero,giallo);cout<<"   | | O | |             |  O    |";setColor(nero,bianco); cout<<"    |                       2 Gioco                       |"<<endl;
    setColor(nero,giallo);cout<<"   |  \\_/  |             |       |";setColor(nero,bianco); cout<<"    |                       3 Personalizza                |"<<endl;
    setColor(nero,giallo);cout<<"   |       |             | . . . |";setColor(nero,bianco); cout<<"    |                       4 Esci                        |"<<endl;
    setColor(nero,giallo);cout<<"   |  :::  | ----------> | . . . |";setColor(nero,bianco); cout<<"    |                                                     |"<<endl;
    setColor(nero,giallo); cout<<"   |  :::  |_____________| . . . |";setColor(nero,bianco); cout<<"    |                                                     |"<<endl;
    setColor(nero,giallo);cout<<"   |           S N K             |";setColor(nero,bianco); cout<<"    |                                                     |"<<endl;
    setColor(nero,giallo);cout<<"   \\_____________________________/";setColor(nero,bianco);cout<<"    |_____________________________________________________|"<<endl;

}
int scelta_menu(int scelta,int x, int y)   //implementazione funzione scelta con getch
{
    scelta=0;
    x=52,y=14;
    gotoXY(x,y);
    cout<<"-->";
    do
    {
        switch(_getch())
        {
            case 80:                //freccia giu
                        gotoXY(x,y);
                        cout<<"   ";
                        if(y!=17)
                        {
                            y=y+1;
                        }
                        else
                        {
                            y=14;
                        }
                        gotoXY(x,y);
                        cout<<"-->";
                        break;
            case 72:                                //freccia su
                        gotoXY(x,y);
                        cout<<"   ";
                        if(y!=14)
                        {
                            y=y-1;
                        }
                        else
                        {
                            y=17;
                        }
                        gotoXY(x,y);
                        cout<<"-->";
                        break;
            case 13:                     //invio
                        switch(y)
                        {
                            case 14: scelta=1;     //la scelta varia a seconda della posizione
                                     break;
                            case 15: scelta=2;
                                     break;
                            case 16: scelta=3;
                                     break;
                            case 17: scelta=4;
                                     break;
                        }
                    break;
    }
    }while(scelta==0);

    return scelta;
}
void regole()   //implementazione stampaggio regole
{

    cout<<"                                                  _____                  _      "<<endl;
    cout<<"                                                 |  __ \\                | |     "<<endl;
    cout<<"                                                 | |__) |___  __ _  ___ | | ___ "<<endl;
    cout<<"                                                 |  _  // _ \\/ _` |/ _ \\| |/ _ \\"<<endl;
    cout<<"                                                 | | \\ \\  __/ (_| | (_) | |  __/"<<endl;
    cout<<"                                                 |_|  \\_\\___|\\__, |\\___/|_|\\___|"<<endl;
    cout<<"                                                              __/ |            "<<endl;
    cout<<"                                                             |___/            "<<endl;
    cout<<"-------------------------------------------------------------------------------------------------------------------------------"<<endl;
    cout<<"| 1) Un organismo sopravvive fino alla generazione successiva se ha 2 o 3 vicini.                                              |"<<endl;
    cout<<"| 2) Un organismo muore lasciando la cella vuota se ha piu' di 3 o meno di 2 vicini .                                          |"<<endl;
    cout<<"| 3) Ogni cella vuota con 3 vicini diventa una cella di nascita e alla generazione successiva viene occupata da un organismo.  |"<<endl;
    cout<<"| 4) Ogni 25 generazioni e' possibile salvare ed uscire dal gioco.                                                             |"<<endl;
    cout<<"| 5) Se non verranno specificate delle impostazioni alternative verranno usate quelle di default.                              |"<<endl;
    cout<<"| 6) Le impostazioni selezionate vengono mantenute fino alla totale chiusura del programma.                                    |"<<endl;
    cout<<"| 7) Quando viene ripreso un salvataggio la generazione da cui il gioco riprendera' sara' considerata come la prima.           |"<<endl;
    cout<<"-------------------------------------------------------------------------------------------------------------------------------"<<endl;
}
void riempi_cas(int mat[N][M],int ncaratteri)  //implementazione del primo riempimento casuale del tabellone
{
    int contatore=0;

    srand(time(NULL));
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<M;j++)               //riempio casualmente con numero pedine del giocatore
        {
            mat[i][j]=rand()%2;
            if(mat[i][j]==1)
            {
                contatore++;
            }
            if(contatore>ncaratteri)
            {
                mat[i][j]=0;
            }
        }
    }
}

void stampa_tab(int x, int y,int mat[N][M])  //implementazione del tabellone di gioco
{
    cout<<" ------- ------- ------- ------- ------- ------- ------- ------- ------- ------- "<<endl;
    for(int i=0;i<N;i++)
    {
        cout<<"|       |       |       |       |       |       |       |       |       |       |"<<endl;
        cout<<"|       |       |       |       |       |       |       |       |       |       |"<<endl;
        cout<<"|       |       |       |       |       |       |       |       |       |       |"<<endl;                //stampo il tabellone
        cout<<" ------- ------- ------- ------- ------- ------- ------- ------- ------- ------- "<<endl;
    }
    for(int i=0;i<N;i++)   //controllo tutta la matrice
    {
        for(int j=0;j<M;j++)
        {
            x=X_BASE+(X_INCRE*i);
            y=Y_BASE+(Y_INCRE*j);             //stampo le X quando trovo un 1
            if(mat[j][i]==1)              //inverto righe e colonne perchè se no me le stampa invertite
            {
                gotoXY(x,y);
                cout<<"X";
            }
        }
    }
}
void controllo(int mat[N][M],int mat2[N][M])    //implementazione controllo sulla matrice per generazioni
{
    int presenze=0;

    for(int j=0;j<N;j++)
    {
        for(int i=0;i<M;i++)
        {
            if(i==0&&j==0)   //controllo angolo alto sx
            {
                if(mat[i][j+1]==1)   //dx
                    presenze++;
                if(mat[i+1][j]==1)  //sotto
                    presenze++;
                if(mat[i+1][j+1]==1) //sotto dx
                    presenze++;
                if(presenze==3)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==1)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==0)
                    mat2[i][j]=0;
                if(presenze<2||presenze>3)                 //salvo i risultati dei controlli nella matrice di appoggio
                    mat2[i][j]=0;
                presenze=0;
            }
            if(i==0&&j==9)       //controllo angolo alto dx
            {
                if(mat[i][j-1]==1)    //sx
                    presenze++;
                if(mat[i+1][j]==1)  //sotto
                    presenze++;
                if(mat[i+1][j-1]==1) //sotto sx
                    presenze++;
                if(presenze==3)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==1)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==0)
                    mat2[i][j]=0;
                if(presenze<2||presenze>3)                 //salvo i risultati dei controlli nella matrice di appoggio
                    mat2[i][j]=0;
                presenze=0;
            }
            if(i==9&&j==0)          //controllo angolo basso sx
            {
                if(mat[i-1][j]==1)  //sopra
                    presenze++;
                if(mat[i][j+1]==1)   //dx
                    presenze++;
                if(mat[i-1][j+1]==1)  //sopra dx
                    presenze++;
                if(presenze==3)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==1)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==0)
                    mat2[i][j]=0;
                if(presenze<2||presenze>3)                 //salvo i risultati dei controlli nella matrice di appoggio
                    mat2[i][j]=0;
                presenze=0;
            }
            if(i==9&&j==9)          //controllo angolo basso dx
            {
                if(mat[i-1][j]==1)  //sopra
                    presenze++;
                if(mat[i][j-1]==1)    //sx
                    presenze++;
                if(mat[i-1][j-1]==1) //sopra sx
                    presenze++;
                if(presenze==3)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==1)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==0)
                    mat2[i][j]=0;
                if(presenze<2||presenze>3)                 //salvo i risultati dei controlli nella matrice di appoggio
                    mat2[i][j]=0;
                presenze=0;
            }
            if(i==0&&j!=0&&j!=9)  //controllo bordo di sopra
            {
                if(mat[i+1][j]==1)  //sotto
                    presenze++;
                if(mat[i][j-1]==1)    //sx
                    presenze++;
                if(mat[i][j+1]==1)   //dx
                    presenze++;
                if(mat[i+1][j-1]==1) //sotto sx
                    presenze++;
                if(mat[i+1][j+1]==1) //sotto dx
                    presenze++;
                if(presenze==3)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==1)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==0)
                    mat2[i][j]=0;
                if(presenze<2||presenze>3)                 //salvo i risultati dei controlli nella matrice di appoggio
                    mat2[i][j]=0;
                presenze=0;
            }
            if(i==9&&j!=0&&j!=9)        //controllo bordo sotto
            {
                if(mat[i-1][j]==1)  //sopra
                    presenze++;
                if(mat[i][j-1]==1)    //sx
                    presenze++;
                if(mat[i][j+1]==1)   //dx
                    presenze++;
                if(mat[i-1][j-1]==1) //sopra sx
                    presenze++;
                if(mat[i-1][j+1]==1)  //sopra dx
                    presenze++;
                if(presenze==3)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==1)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==0)
                    mat2[i][j]=0;
                if(presenze<2||presenze>3)                 //salvo i risultati dei controlli nella matrice di appoggio
                    mat2[i][j]=0;
                presenze=0;
            }
            if(j==0&&i!=0&&i!=9)        //controllo bordo sinistro
            {
                if(mat[i-1][j]==1)  //sopra
                    presenze++;
                if(mat[i][j+1]==1)   //dx
                    presenze++;
                if(mat[i-1][j+1]==1)  //sopra dx
                    presenze++;
                if(mat[i+1][j]==1)  //sotto
                    presenze++;
                if(mat[i+1][j+1]==1) //sotto dx
                    presenze++;
                if(presenze==3)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==1)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==0)
                    mat2[i][j]=0;
                if(presenze<2||presenze>3)                 //salvo i risultati dei controlli nella matrice di appoggio
                    mat2[i][j]=0;
                presenze=0;
            }
            if(j==9&&i!=0&&i!=9)  //controllo bordo destro
            {
                if(mat[i-1][j]==1)  //sopra
                    presenze++;
                if(mat[i+1][j]==1)  //sotto
                    presenze++;
                if(mat[i][j-1]==1)    //sx
                    presenze++;
                if(mat[i-1][j-1]==1) //sopra sx
                    presenze++;
                if(mat[i+1][j-1]==1) //sotto sx
                    presenze++;
                if(presenze==3)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==1)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==0)
                    mat2[i][j]=0;
                if(presenze<2||presenze>3)                 //salvo i risultati dei controlli nella matrice di appoggio
                    mat2[i][j]=0;
                presenze=0;
            }
             if(i!=0&&i!=9&&j!=0&&j!=9)   //controllo il centro
            {
                if(mat[i][j-1]==1)    //sx
                    presenze++;
                if(mat[i][j+1]==1)   //dx
                    presenze++;
                if(mat[i+1][j]==1)  //sotto
                    presenze++;
                if(mat[i-1][j]==1)  //sopra
                    presenze++;
                if(mat[i+1][j-1]==1) //sotto sx
                    presenze++;
                if(mat[i+1][j+1]==1) //sotto dx
                    presenze++;
                if(mat[i-1][j-1]==1) //sopra sx
                    presenze++;
                if(mat[i-1][j+1]==1)  //sopra dx
                    presenze++;

                if(presenze==3)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==1)
                    mat2[i][j]=1;
                if(presenze==2&&mat[i][j]==0)
                    mat2[i][j]=0;
                if(presenze<2||presenze>3)                 //salvo i risultati dei controlli nella matrice di appoggio
                    mat2[i][j]=0;
                presenze=0;
            }
        }
    }
    for(int i=0;i<N;i++)       //copio la matrice di appoggio nella prima così dopo posso stampare
    {
        for(int j=0;j<M;j++)
        {
            mat[i][j]=mat2[i][j];
        }
    }
}
void schermoIntero() //implementazione schermo intero
{
  COORD coord;
  HMODULE dll = LoadLibrary("kernel32.dll");
  SetConsoleDisplayModeProc SetConsoleDisplayMode = (SetConsoleDisplayModeProc)GetProcAddress(dll, "SetConsoleDisplayMode");
  HANDLE hConsole = GetStdHandle( STD_OUTPUT_HANDLE );
  SetConsoleDisplayMode( hConsole, CONSOLE_FULLSCREEN_MODE, &coord);
}
void setColor(int bg, int fg)            //implementazione per settare il colore
{
    int val;
    if (bg<0)
    bg=0;
    if (fg>15)
    fg=15;
    val=bg*16+fg;
    SetConsoleTextAttribute(
    GetStdHandle(STD_OUTPUT_HANDLE),
    val);
}
bool controlloMatriceVuota(int mat[N][M]) //implementazione controllo matrice vuota
{
    bool matriceVuota;
    int contatore=0;

    for(int i=0;i<N;i++)   //controllo se la matrice è vuota
    {
        for(int j=0;j<M;j++)
        {
            if(mat[i][j]==0)
            {
                contatore++;
            }
            if(contatore==(N*M))
            {
                matriceVuota=true;
            }
        }
    }
    return matriceVuota;
}
void SonoMortiTutti()  //implementazione stampa della morte
{
    cout<<endl;
    cout<<"            `/shmmmmhs/`"<<endl;Sleep(sleep);
    cout<<"          :yNMMMMMMMMMMNh:"<<endl;Sleep(sleep);
    cout<<"         hMMMMMMMMMMMMMMMMh`"<<endl;Sleep(sleep);
    cout<<"        +MMMMMMMMMMMMMMMMMM+"<<endl;Sleep(sleep);
    cout<<"        hMmMMMMMMMMMMMMMMmMh"<<endl;Sleep(sleep);
    cout<<"        yMdMMMMMMMMMMMMMMdMh"<<endl;Sleep(sleep);
    cout<<"        :hNs/::sNMMNs::/sNh:"<<endl;Sleep(sleep);
    cout<<"        `yh     oMMo     yy`"<<endl;Sleep(sleep);
    cout<<"        :Mm-`./ym++my/.`-mN:"<<endl;Sleep(sleep);
    cout<<"         /mMMMMM-::.MMMMMm+"<<endl;Sleep(sleep);
    cout<<"           do:MMddmdMM/od"<<endl;Sleep(sleep);
    cout<<"  `yho`   `NNsymdmmdmysNN`   `ohy`"<<endl;Sleep(sleep);
    cout<<" :dMMMm:   .NMNhhdhhhNMN.   :mMMMd:"<<endl;Sleep(sleep);
    cout<<" dMMMMMMd+` -mMMNmmNMMm- `/dMMMMMMd"<<endl;Sleep(sleep);
    cout<<" `osssydMMNh+.:yddddy:.+hNNMdyssso`"<<endl;Sleep(sleep);
    cout<<"        `:sdNMdo-  -odMMds:"<<endl;Sleep(sleep);
    cout<<"            .+hNMmdddh+."<<endl;Sleep(sleep);
    cout<<"           `/yhdhmMNy/`"<<endl;Sleep(sleep);
    cout<<"        `:sdMMms-  -omMMds:"<<endl;Sleep(sleep);
    cout<<"  `ossyhmMMMms-        -omMMMmhysso`"<<endl;Sleep(sleep);
    cout<<" /MMMMMMNs-              -sNMMMMMM/"<<endl;Sleep(sleep);
    cout<<" -MMMN+                    /NMMM-"<<endl;Sleep(sleep);
    cout<<"   -/:`                      `:/-"<<endl;Sleep(sleep);
}
void stampa_personalizzazione() //implementazione stampa menù di personalizzazione
{
    system("cls");
    cout<<"          _____                                _ _                  _"<<endl;
    cout<<"         |  __ \\                              | (_)                (_)"<<endl;
    cout<<"         | |__) |__ _ __ ___  ___  _ __   __ _| |_ __________ _ _____  ___  _ __   ___"<<endl;
    cout<<"         |  ___/ _ \\ '__/ __|/ _ \\| '_ \\ / _` | | |_  /_  / _` |_  / |/ _ \\| '_ \\ / _ \\"<<endl;
    cout<<"         | |  |  __/ |  \\__ \\ (_) | | | | (_| | | |/ / / / (_| |/ /| | (_) | | | |  __/"<<endl;
    cout<<"         |_|   \\___|_|  |___/\\___/|_| |_|\\__,_|_|_/___/___\\__,_/___|_|\\___/|_| |_|\\___|"<<endl;
    cout<<endl;
    cout<<endl;
    cout<<"       Quale voce vuoi modificare?"<<endl;
    cout<<endl;
    cout<<"\n      1) Velocita' di generazione"<<endl;
    cout<<"      2) Numero massimo di caratteri nella matrice"<<endl;
    cout<<"      3) Limite Generazione"<<endl;
    cout<<"      4) Torna al menu'"<<endl;
}
int frecciaPersonalizzazione(int scelta2, int x, int y)  //implementazione scelta della personalizzazione
{
    scelta2=0;
    x=0,y=11;
    gotoXY(x,y);
    cout<<"-->";
    do
    {
        switch(_getch())
        {
            case 80:                //freccia giu
                    gotoXY(x,y);
                    cout<<"   ";
                    if(y!=14)
                    {
                        y=y+1;
                    }
                    else
                    {
                        y=11;
                    }
                    gotoXY(x,y);
                    cout<<"-->";
                    break;
            case 72:                                //freccia su
                    gotoXY(x,y);
                    cout<<"   ";
                    if(y!=11)
                    {
                        y=y-1;
                    }
                    else
                    {
                        y=14;
                    }
                    gotoXY(x,y);
                    cout<<"-->";
                    break;
            case 13:                     //invio
                    switch(y)
                    {
                        case 11:
                            scelta2=1;
                            break;
                        case 12:
                            scelta2=2;
                            break;
                        case 13:
                            scelta2=3;
                            break;
                        case 14:
                            scelta2=4;
                            break;
                     }
                    break;
        }
    }while(scelta2==0);
    return scelta2;

}
int richiesta_SalvaIniziale(int richiestaSalva,int x, int y)  //implementazione richiesta caricamento salvataggio
{
    richiestaSalva=0;
    system("cls");
    cout<<"Vuoi riprendere dall'ultimo salvataggio?"<<endl;
    gotoXY(10,5);
    cout<<"1) SI"<<endl;
    gotoXY(10,6);
    cout<<"2) NO"<<endl;
    x=6;
    y=5;
    gotoXY(x,y);
    cout<<"-->";
    do
    {
        switch(getch())
        {
            case 80:                     //freccia giu
                gotoXY(x,y);
                cout<<"   ";
                if(y!=6)
                    y++;
                else
                    y=5;
                gotoXY(x,y);
                cout<<"-->";
                break;
            case 72:                   //freccia su
                gotoXY(x,y);
                cout<<"   ";
                if(y!=5)
                    y--;
                else
                    y=6;
                gotoXY(x,y);
                cout<<"-->";
                break;
            case 13:                   //invio
                switch(y)
                {
                    case 5:
                        richiestaSalva=1;
                        break;
                    case 6:
                        richiestaSalva=2;
                        break;
                }
        }
    }while(richiestaSalva==0);
    return richiestaSalva;
}
int Salva_inGioco(int salva,int x, int y,int generazione)  //implementazione richiesta salvataggio ogni 25 generazioni
{
    salva=0;
    if(generazione%25==0)
    {
        x=100,y=10;
        gotoXY(x,y);
        cout<<"Sei arrivato ad un check point!";
        y++;
        gotoXY(x,y);
        cout<<"Vuoi salvare la generazione seguente ed interrompere il gioco?";
        y=12;
        gotoXY(x,y);
        cout<<"1) SI"<<endl;
        y++;
        gotoXY(x,y);
        cout<<"2) NO"<<endl;
        x=96;
        y=12;
        gotoXY(x,y);
        cout<<"-->";
        do
        {
            switch(getch())
            {
                case 80:             //freccia giu
                    gotoXY(x,y);
                    cout<<"   ";
                    if(y!=13)
                        y++;
                    else
                        y=12;
                    gotoXY(x,y);
                    cout<<"-->";
                    break;
                case 72:                  //freccia su
                    gotoXY(x,y);
                    cout<<"   ";
                    if(y!=12)
                        y--;
                    else
                        y=13;
                    gotoXY(x,y);
                    cout<<"-->";
                    break;
                case 13:                   //invio
                    switch(y)
                    {
                        case 12:
                        salva=1;
                        break;
                        case 13:
                        salva=2;
                        break;
                    }
            }
        }while(salva==0);
    }
    return salva;
}
#endif // CURSORI_H_INCLUDED
