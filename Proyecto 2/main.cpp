#include <iostream>
#include "fstream"
#include <cstdlib>
#include <sstream>
#include <string>
#include <stack>

using namespace std;


/**<
    Se inicio el 18/05/2014 y se finalizo 11/06/2014
    Por: Brian y Jonathan
 */


//******************** DECLARACION DE METODOS ********************************

void menuPrincipal();

void rutasGUI();
void insertarRutaGUI();
void imprimirRutasGUI();

void transportesGUI();
void insertarTransporteGUI();
void imprimirTransporteGUI();

void horariosGUI();
void insertarHorarioGUI();
void imprimirHorariosGUI();

void reservacionesGUI();
void insertarReservacionGUI();
void imprimirReservacionesGUI();

void consultasGUI();
void consultaUnoGUI();
void consultaRutaBarataGUI();
void consultaRutaCortaGUI();
void consultaSinTransXGUI();
void consultaDosGUI();
void consultaTres();


//*************************************** GRAFO ********************************************

//Estructura del arco Rutas del Grafo
struct arco {

    struct vertice *destino;
    int id;
    bool dirigido;
    struct arco *sigA;
};

//Estructura del vertice Ciudad del Grafo
struct vertice{

    string ciudad;
    struct vertice *sigV;
    struct arco *sigA;
    bool visitado;

    int cantTicos = 0;

}*grafo;


struct pila{
    string C;
    struct pila *sig;
}*primero, *primero1;

int minmin = 1000000;
int cantTicos = 3000;

void Push(string C){
    struct pila *nn = new pila();
    nn->C = C;
    nn->sig=primero;
    primero =nn;
}

void pop(){
    if(primero != NULL)
        primero = primero->sig;

}

void Push1(string C){
    struct pila *nn = new pila();
    nn->C = C;
    nn->sig = primero1;
    primero1 = nn;
}

void pop1(){
    if(primero1 != NULL)
        primero1 = primero1->sig;
}


//Imprimir la pila
void imprimirPila(struct pila *primero){
    if(primero==NULL)
        return;
    imprimirPila(primero->sig);
    cout<<primero->C<<" ";
}

void imprimirPila1(struct pila *primero1){
    struct pila *temp = primero1;
    if(temp == NULL)
        return;
    imprimirPila(temp->sig);
    cout<<temp->C<<" ";
}


//******************************** Estructuras ARCHIVOS ************************************

//Estructura de Rutas del Archivo Rutas
struct Ruta{

public:

    int id;
    bool dirigido;
    char origen[15];
    char destino[15];


}registroRuta;

//Estructura de Transportes del Archivo Transportes
struct Transporte{

public:

    int idRuta;
    int idTransp;
    char tipo[10];
    char duracion[10];
    int costo;
    int capacidad;
    int reservaciones;

    long izq, der;

    Transporte(){

        izq = -1;
        der = -1;
    }

}registroTransporte, registroTransporteAux;

//Estructura de Horario del Archivo Horarios
struct Horario{

public:

    int idRuta;
    int idTransp;
    int idHorario;
    int horaSalida;
    int horaLlegada;
    int capacidad;
    int reservados;

    long izq, der;

    Horario(){
        izq = -1;
        der = -1;
    }
}registroHorario;


//Estructura de Reservaciones del Archivo Reservaciones
struct Reservaciones{

public:

    int idRuta;
    int idTransporte;
    int idReservacion;
    int numReservaciones;
    char origen[15];
    char destino[15];
    char nacionalidad[15];

}registroReservaciones;

//Estructura de Partidos del Archivo Partidos
struct Partido{

public:

    int dia;
    int mes;
    char equi1[15];
    char equi2[15];

    int nacion1 = 0;
    int nacion2 = 0;


}registroPartidos;

//------------------------------ CREACION DE LOS ARCHIVOS ----------------------------------

fstream ArchivoRuta("ArchivoRuta.dat", ios::in | ios::out |ios::binary |ios::trunc );
fstream ArchivoRutaAux("ArchivoRuta.dat", ios::in | ios::out |ios::binary );

fstream ArchivoTransporte("ArchivoTransporte.dat", ios::in | ios::out |ios::binary |ios::trunc );
fstream ArchivoTransporteAux("ArchivoTransporte.dat", ios::in | ios::out |ios::binary );

fstream ArchivoHorario("ArchivoHorario.dat", ios::in | ios::out |ios::binary |ios::trunc );
fstream ArchivoHorarioAux("ArchivoHorario.dat", ios::in | ios::out |ios::binary );

fstream ArchivoReservaciones("ArchivoReservaciones.dat", ios::in | ios::out |ios::binary |ios::trunc );
fstream ArchivoReservacionesAux("ArchivoReservaciones.dat", ios::in | ios::out |ios::binary );

fstream ArchivoPartidos("ArchivoPartidos.dat", ios::in | ios::out |ios::binary |ios::trunc );
fstream ArchivoPartidosAux("ArchivoPartidos.dat", ios::in | ios::out |ios::binary );

//Obtiene el ultimo del Archivo Ruta
long finArchivoRuta(){
    long ultimo;
    ArchivoRuta.seekg(0, ios::end);
    ultimo = ArchivoRuta.tellg();
    ultimo = (ultimo/sizeof(registroRuta))-1;

    return ultimo;
}

//Obtiene el ultimo del Archivo Transporte
long finArchivoTransporte(){
    long ultimo;
    ArchivoTransporte.seekg(0, ios::end);
    ultimo = ArchivoTransporte.tellg();
    ultimo = (ultimo/sizeof(registroTransporte))-1;

    return ultimo;
}

//Obtiene el ultimo del Archivo Horario
long finArchivoHorario(){
    long ultimo;
    ArchivoHorario.seekg(0, ios::end);
    ultimo = ArchivoHorario.tellg();
    ultimo = (ultimo/sizeof(registroHorario))-1;

    return ultimo;
}

//Obtiene el ultimo del Archivo Reservaciones
long finArchivoReservaciones(){
    long ultimo;
    ArchivoReservaciones.seekg(0, ios::end);
    ultimo = ArchivoReservaciones.tellg();
    ultimo = (ultimo/sizeof(registroReservaciones))-1;

    return ultimo;
}

//Obtiene el ultimo del Archivo Partidos
long finArchivoPartidos(){
    long ultimo;
    ArchivoPartidos.seekg(0, ios::end);
    ultimo = ArchivoPartidos.tellg();
    ultimo = (ultimo/sizeof(registroPartidos))-1;

    return ultimo;
}

//Inserta una Ruta al Archivo Ruta
bool insertarRuta(int id, bool dirigido, char *origen, char *destino){

    int index = 0;

    if(grafo != NULL){

        struct vertice *tempV = grafo;
        while(tempV != NULL){
            struct arco *tempA = tempV->sigA;
            while(tempA != NULL){
                if(tempA->id == id){
                    cout<<endl<<endl<<"************* LA RUTA YA EXISTE ****************"<<endl;
                    return false;
                }
                tempA = tempA->sigA;
            }
            tempV = tempV->sigV;
        }
    }

    if(!ArchivoRuta){
        cout<<endl<<"-----ERROR EN LA APERTURA DEL ARCHIVO-----"<<endl;
        return false;
    }

    for(int i = 0; i<15; i++){
        registroRuta.origen[i] = origen[i];
    }
    for(int j = 0; j<15; j++){
        registroRuta.destino[j] = destino[j];
    }

    registroRuta.id = id;
    registroRuta.dirigido = dirigido;

    ArchivoRuta.seekg(0,ios::end);
    ArchivoRuta.write(reinterpret_cast<char *> (&registroRuta), sizeof(registroRuta));
    return true;
}

//Global que genera el id a reservaciones
int generarId = 0;

//Inserta una Reservacion al Archivo Reservaciones
bool insertarReservacion(int idRuta, int idTrans, string origen, string destino, int cantidadReserv, char *nacion){

    if(!ArchivoReservaciones){
        cout<<endl<<"-----ERROR EN LA APERTURA DEL ARCHIVO-----"<<endl;
        return false;
    }

    for(int i=0; i<15; i++){
        registroReservaciones.origen[i] = origen[i];
    }
    for(int i=0; i<15; i++){
        registroReservaciones.destino[i] = destino[i];
    }
    for(int i=0; i<15; i++){
        registroReservaciones.nacionalidad[i] = nacion[i];
    }
    generarId++;

    registroReservaciones.idRuta = idRuta;
    registroReservaciones.idTransporte = idTrans;
    registroReservaciones.idReservacion = generarId;
    registroReservaciones.numReservaciones = cantidadReserv;

    ArchivoReservaciones.seekg(0,ios::end);
    ArchivoReservaciones.write(reinterpret_cast<char *> (&registroReservaciones), sizeof(registroReservaciones));
    return true;
}

//Inserta un Horario al Archivo Horarios
bool insertarHorario(int idRuta, int idTransp, int idHorario, int horaSalida, int horaLlegada){

    int index = 0;
    int idRuTransHora1 = (idRuta*100) + (idTransp*10) + idHorario;
    int idRuTransHora2;

    if(!ArchivoTransporte){
        cout <<"-----ERROR EN LA APERTURA DEL ARCHIVO-----";
        return false;
    }

    ArchivoTransporte.seekg(0);

    while(ArchivoTransporte.eof() != true){
        ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
        if((registroTransporte.idRuta == idRuta) && (registroTransporte.idTransp == idTransp)){

            if(!ArchivoHorario){
                cout <<"-----ERROR EN LA APERTURA DEL ARCHIVO-----";
                return false;
            }

            registroHorario.idRuta = idRuta;
            registroHorario.idTransp = idTransp;
            registroHorario.idHorario = idHorario;
            registroHorario.horaSalida = horaSalida;
            registroHorario.horaLlegada = horaLlegada;
            registroHorario.capacidad = registroTransporte.capacidad;
            registroHorario.reservados = registroTransporte.reservaciones;

            registroHorario.der = -1;
            registroHorario.izq = -1;

            ArchivoHorario.seekg(0,ios::end);
            ArchivoHorario.write(reinterpret_cast<char *> (&registroHorario), sizeof(registroHorario));

            while(true){

                ArchivoHorario.seekg(index*sizeof(registroHorario));
                ArchivoHorario.read(reinterpret_cast<char *> (&registroHorario), sizeof(registroHorario));
                idRuTransHora2 = (registroHorario.idRuta*100) + (registroHorario.idTransp*10) + registroHorario.idHorario;

                if(idRuTransHora2 == idRuTransHora1){
                    //cout <<"------- EL HORARIO YA EXISTE -------"<<endl<<endl;
                    return false;
                }

                else if(idRuTransHora2 < idRuTransHora1){
                    if(registroHorario.der != -1){
                        index = registroHorario.der;
                    }
                    else{
                        registroHorario.der = finArchivoHorario();
                        ArchivoHorario.seekg(index*sizeof(registroHorario));
                        ArchivoHorario.write(reinterpret_cast<char *> (&registroHorario), sizeof(registroHorario));
                        return true;
                    }

                }
                else{
                    if(registroHorario.izq != -1){
                        index = registroHorario.izq;
                    }

                    else{
                        registroHorario.izq = finArchivoHorario();
                        ArchivoHorario.seekg(index*sizeof(registroHorario));
                        ArchivoHorario.write(reinterpret_cast<char *> (&registroHorario), sizeof(registroHorario));
                        return true;
                    }
                }
            }
        cout<<endl<<endl<<"---------------- EL HORARIO SE AGREGO CORRECTAMENTE ------------------"<<endl<<endl;
        }
    }
    ArchivoTransporte.close();
    ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
    return false;
}

//Inserta Transporte al Archivo Transporte
bool insertarTransporte(int idRuta, int idTransp, char *tipo, char *duracion, int costo, int capacidad, int reservaciones){

    int index = 0;
    int idRuTrans1;
    int idRuTrans2;

    if(!ArchivoTransporte){
        cout<<endl<<"----- ERROR EN LA APERTURA -----"<<endl;
        return false;
    }

    for(int i=0; i<15; i++){
        registroTransporte.tipo[i] = tipo[i];
    }

    for(int i=0; i<15; i++){
        registroTransporte.duracion[i] = duracion[i];
    }

    registroTransporte.idRuta = idRuta;
    registroTransporte.idTransp = idTransp;
    registroTransporte.costo = costo;
    registroTransporte.capacidad = capacidad;
    registroTransporte.reservaciones = reservaciones;

    registroTransporte.der = -1;
    registroTransporte.izq = -1;

    idRuTrans1 = (idRuta*10) + idTransp;
    ArchivoTransporte.seekg(0, ios::end);
    ArchivoTransporte.write(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));

    while(true){

        ArchivoTransporte.seekg(index*sizeof(registroTransporte));
        ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));

        idRuTrans2 = (registroTransporte.idRuta*10) + registroTransporte.idTransp;
        if(idRuTrans2 == idRuTrans1){
            //cout <<"------- EL TRANSPORTE YA EXISTE -------"<<endl<<endl;
            return false;
        }

        else if(idRuTrans2 < idRuTrans1){
            if(registroTransporte.der != -1){
                index = registroTransporte.der;
            }
            else{
                registroTransporte.der = finArchivoTransporte();
                ArchivoTransporte.seekg(index*sizeof(registroTransporte));
                ArchivoTransporte.write(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
                return true;
            }

        }
        else{
            if(registroTransporte.izq != -1){
                index = registroTransporte.izq;
            }
            else{
                registroTransporte.izq = finArchivoTransporte();
                ArchivoTransporte.seekg(index*sizeof(registroTransporte));
                ArchivoTransporte.write(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
                return true;
            }
        }
    }
    cout<<endl<<endl<<"---------------- EL TRANSPORTE SE AGREGO CORRECTAMENTE ------------------"<<endl<<endl;
    return false;
}


//Inserta un Partido al Archivo Partidos
void insertarArchivoPartidos(int dia, int mes, char *equi1, char *equi2){

    int index = 0;

    if(!ArchivoPartidos){
        cout<<endl<<"-----ERROR EN LA APERTURA DEL ARCHIVO-----"<<endl;
        return;
    }

    for(int i=0; i<15; i++){
        registroPartidos.equi1[i] = equi1[i];
    }
    for(int i=0; i<15; i++){
        registroPartidos.equi2[i] = equi2[i];
    }

    registroPartidos.dia = dia;
    registroPartidos.mes = mes;

    ArchivoPartidos.seekg(0,ios::end);
    ArchivoPartidos.write(reinterpret_cast<char *> (&registroPartidos), sizeof(registroPartidos));
}

//Busca un Transporte por medio del ID
//Retorna su posicion
int buscarIndex(int idTrans, int idRuta){

    int index = 0;
    int idGenerado;
    int idGenerado1;

    ArchivoTransporte.seekg(0);
    while(true){
        ArchivoTransporte.seekg(index*sizeof(registroTransporte));
        ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
        idGenerado = (registroTransporte.idRuta*10) + registroTransporte.idTransp;
        idGenerado1 = (idRuta*10) + idTrans;

        if(idGenerado == idGenerado1){
            return index;
        }
        else if(idGenerado < idGenerado1){//irse por el lado der
            if(registroTransporte.der != -1)
                index = registroTransporte.der;
            else{
                return false;
            }

        }
        else if(idGenerado > idGenerado1){//irse por el lado der
            if(registroTransporte.izq != -1)
                index = registroTransporte.izq;
            else{
                return false;
            }

        }
    }
    ArchivoTransporte.close();
    ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
}

//Imprime al Archivo Horario
void imprimirArchivoHorario(){

    ArchivoHorario.seekg(0);
    cout<< endl <<"--------------- ARCHIVO ARBOL BINARIO HORARIO ----------------"<<endl<<endl<<endl;
    cout<< "Reg     Izq   IdR IdT IdH   Hora Sali   Hora Lleg   Capac   Reserv  der"<<endl<<endl;

    int reg = 0;
    while(ArchivoHorario.eof() != true){
        ArchivoHorario.read(reinterpret_cast<char *> (&registroHorario), sizeof(registroHorario));
        if(ArchivoHorario.eof() != true){
            cout<<reg<<"\t"<<registroHorario.izq<<"\t"<<registroHorario.idRuta <<"  "<<registroHorario.idTransp<<"   ";
            cout<<registroHorario.idHorario <<"\t"<<registroHorario.horaSalida<<"\t\t"<<registroHorario.horaLlegada<<"\t"<<registroHorario.capacidad<<"\t";
            cout<<registroHorario.reservados<<"\t"<<registroHorario.der<<endl;
        }
        reg++;
    }
    ArchivoHorario.close();
    ArchivoHorario.open("ArchivoHorario.dat", ios::in | ios::out |ios::binary);
}

//Imprime al Archivo Transportes
void imprimirArchivoTransporte(){

    ArchivoTransporte.seekg(0);
    cout<< endl <<"--------ARCHIVO ARBOL BINARIO TRANSPORTE-----------"<<endl<<endl<<endl;
    cout<< "Reg     Izq   IdR IdT   Tipo    Durac   Costo   Capac   Reserv  der"<<endl<<endl;

    int reg = 0;
    while(ArchivoTransporte.eof() != true){
        ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
        if(ArchivoTransporte.eof() != true){
            cout<<reg<<"\t"<<registroTransporte.izq<<"\t"<<registroTransporte.idRuta <<"  " <<registroTransporte.idTransp<<"\t";
            cout<<registroTransporte.tipo<<"\t"<<registroTransporte.duracion<<"\t"<<registroTransporte.costo<<"\t";
            cout<<registroTransporte.capacidad<<"\t"<<registroTransporte.reservaciones<<"\t"<<registroTransporte.der<<endl;
        }
        reg++;
    }
    ArchivoTransporte.close();
    ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
}

//Imprime al Archivo Rutas
void imprimirArchivoRuta(){

    ArchivoRuta.seekg(0);
    cout<< endl <<"-------- ARCHIVO ARBOL BINARIO RUTA -----------"<<endl<<endl<<endl;
    cout<< "Reg     Id      Origen         Destino       Dirigido"<<endl<<endl;

    int reg = 0;
    while(ArchivoRuta.eof() != true){
        ArchivoRuta.read(reinterpret_cast<char *> (&registroRuta), sizeof(registroRuta));
        if(ArchivoRuta.eof() != true){
            cout<<reg<<"\t"<<registroRuta.id<<"\t"<<registroRuta.origen<<"\t";
            cout<<registroRuta.destino<<"\t"<<registroRuta.dirigido<<endl;
        }
        reg++;
    }
    ArchivoRuta.close();
    ArchivoRuta.open("ArchivoRuta.dat", ios::in | ios::out |ios::binary);
}

//Imprime al Archivo Reservaciones
void imprimirArchivoReservaciones(){

    ArchivoReservaciones.seekg(0);
    cout<< endl <<"-------- ARCHIVO ARBOL BINARIO RUTA -----------"<<endl<<endl<<endl;
    cout<< "Reg   IdRut  IdTrans IdReser    Origen   Destino    Cantidad    Naciones"<<endl<<endl;
    int reg = 0;
    while(ArchivoReservaciones.eof() != true){
        ArchivoReservaciones.read(reinterpret_cast<char *> (&registroReservaciones), sizeof(registroReservaciones));
        if(ArchivoReservaciones.eof() != true){
            cout<<reg<<"\t"<<registroReservaciones.idRuta<<"\t"<<registroReservaciones.idTransporte<<"\t";
            cout<<registroReservaciones.idReservacion<<"\t"<<registroReservaciones.origen<<"\t"<<registroReservaciones.destino;
            cout<<"\t"<<registroReservaciones.numReservaciones<<"\t"<<registroReservaciones.nacionalidad<<endl;
        }
        reg++;
    }
    ArchivoReservaciones.close();
    ArchivoReservaciones.open("ArchivoReservaciones.dat", ios::in | ios::out |ios::binary);
}

//Imprime al Archivo Partidos
void imprimirArchivoPartido(){

    ArchivoPartidos.seekg(0);
    cout<< endl <<"-------- ARCHIVO ARBOL BINARIO PARTIDO -----------"<<endl<<endl<<endl;
    cout<< "Reg     Dia     Mes     Casa            Visista"<<endl<<endl;

    int reg = 0;
    while(ArchivoPartidos.eof() != true){
        ArchivoPartidos.read(reinterpret_cast<char *> (&registroPartidos), sizeof(registroPartidos));
        if(ArchivoPartidos.eof() != true){
            cout<<reg<<"\t"<<registroPartidos.dia<<"\t"<<registroPartidos.mes<<"\t";
            cout<<registroPartidos.equi1<<"\t\t"<<registroPartidos.equi2<<endl;
        }
        reg++;
    }
    ArchivoPartidos.close();
    ArchivoPartidos.open("ArchivoPartidos.dat", ios::in | ios::out |ios::binary);
}

//Imprime los Transportes disponibles por el id Ruta
void transportesDisponibles(int idRuta){

    int idTrans;
    cout<<endl<<"---------------------- Transportes Disponibles ---------------------"<<endl<<endl;
    ArchivoTransporte.seekg(0);
    cout<<endl<<"IdR    IdT      Tipo    Durac   Costo   Capac   Reserv"<<endl<<endl;
    while(ArchivoTransporte.eof() != true){
        ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
        if((registroTransporte.idRuta == idRuta) && (registroTransporte.idTransp != idTrans)){
            idTrans = registroTransporte.idTransp;
            cout<<registroTransporte.idRuta <<"\t"<<registroTransporte.idTransp<<"\t";
            cout<<registroTransporte.tipo<<"\t"<<registroTransporte.duracion<<"\t"<<registroTransporte.costo<<"\t";
            cout<<registroTransporte.capacidad<<"\t"<<registroTransporte.reservaciones<<endl;
        }
    }
    ArchivoTransporte.close();
    ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
}



//-------------------------------------- Metodos Grafos ----------------------------------------

//Busca si existe la Ciudad
//Si la encuentra retorna el vertice o sino NULL
struct vertice *existeVertice(string ciudad){

    if(grafo == NULL){
        return NULL;
    }

    struct vertice *tempV = grafo;
    while(tempV != NULL){
        if(tempV->ciudad == ciudad){
            return tempV;
        }
        tempV = tempV->sigV;
    }
    return NULL;
}

//Inserta una Ciudad a un vertica del Grafo
void insertarVertice(string ciudad){

    if(existeVertice(ciudad) != NULL){
        //cout<<"Ciudad repetida"<<endl<<endl;
        return;
    }
    struct vertice *nnv = new vertice();

    nnv->ciudad = ciudad;
    nnv->sigA = NULL;

    nnv->sigV = grafo;
    grafo = nnv;

}

//Inserta una Ruta al arco del Grafo
void insertarArco(string origen, string destino, int id, bool dirigido){

    struct vertice *vO = existeVertice(origen);
    struct vertice *vD = existeVertice(destino);

    if(vO == NULL){
        cout<<"No existe el origen"<<endl<<endl;
        return;
    }
    if(vD == NULL){
        cout<<"No existe el destino"<<endl<<endl;
        return;
    }

    if(grafo != NULL){

        struct vertice *tempV = grafo;
        while(tempV != NULL){
            struct arco *tempA = tempV->sigA;
            while(tempA != NULL){
                if((tempA->id == id) and (tempV->ciudad != destino) and (tempA->destino->ciudad != origen)){
                    //cout<<endl<<endl<<"************* EL ARCO YA EXISTE ****************"<<endl;
                    return;
                }
                tempA = tempA->sigA;
            }
            tempV = tempV->sigV;
        }
    }

    struct arco *nnA = new arco();
    nnA->id = id;
    nnA->dirigido = dirigido;
    nnA->destino = vD;

    nnA->sigA = vO->sigA;
    vO->sigA = nnA;
}

//Vuelve false al visitado de los vertices
void desmarcar(){

    struct vertice *tempV = grafo;

    while(tempV!=NULL){
        tempV->visitado=false;
        tempV=tempV->sigV;
    }
}

//Imprime el Grafo en Amplitud
void amplitud(){

    system("cls");
    char x;

    cout<<endl<<endl<<"--------------------------- GRAFO AMPLITUD ------------------------------"<<endl<<endl;
    struct vertice *tempV = grafo;
    struct arco *tempA;
    while(tempV != NULL){
        cout<<endl<<endl<<"Visitando el vertice: "<<tempV->ciudad<<endl;
        tempA = tempV->sigA;
        while(tempA != NULL){
            cout<<tempV->ciudad<<"  ->  "<<tempA->destino->ciudad<<" ID: "<<tempA->id<<", ";
            cout<<endl<<"---------------------------------------------------------------"<<endl;
            tempA = tempA->sigA;
        }
        tempV = tempV->sigV;
    }
    cout <<endl<<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
    cin>>x;
    if(x == '1'){
        menuPrincipal();
    }
    else{
        menuPrincipal();
    }
}

//Imprime grafo en profundida
void profundidad(struct vertice *tempV){

    if(tempV == NULL){
        return;
    }
    if(tempV->visitado == true){
        return;
    }
    tempV->visitado = true;

    struct arco *tempA = tempV->sigA;

    while(tempA != NULL){
        cout<<"Origen: "<<tempV->ciudad<<" -> "<<" Destino: "<<tempA->destino->ciudad<<" ID: "<<tempA->id<<endl;
        cout<<endl<<"----------------------------------------------------------"<<endl<<endl;
        profundidad(tempA->destino);
        tempA = tempA->sigA;
    }
}



int buscarIndexHorarios(int idRuta, int idTrans, int idHorario){

    int idGenerado1 = (idRuta*100) + (idTrans*10) + idHorario;
    int idGenerado;

    int index = 0;

    ArchivoHorario.seekg(0);

    while(true){
        ArchivoHorario.seekg(index*sizeof(registroHorario));
        ArchivoHorario.read(reinterpret_cast<char *> (&registroHorario), sizeof(registroHorario));
        idGenerado = (registroHorario.idRuta*100) + (registroHorario.idTransp*10) + registroHorario.idHorario;

        if(idGenerado == idGenerado1){
            return index;
        }
        else if(idGenerado < idGenerado1){//irse por el lado der
            if(registroHorario.der != -1)
                index = registroHorario.der;
            else{
                return -1;
            }

        }
        else if(idGenerado > idGenerado1){//irse por el lado der
            if(registroHorario.izq != -1)
                index = registroHorario.izq;
            else{
                return -1;
            }
        }
    }
    ArchivoHorario.close();
    ArchivoHorario.open("ArchivoHorario.dat", ios::in | ios::out |ios::binary);
    return -1;
}

//Realiza Reservaciones en el horario
//Por medio del Id Ruta, Id Transporte y las reservaciones
void horariosDisponibles(int idRuta, int idTrans){

    int i = 0;
    ArchivoHorario.seekg(0);
    cout<< endl <<"-------- HORARIO DISPONIBLES -----------"<<endl<<endl<<endl;

    cout<<"IdR IdT IdH   Hora Sali   Hora Lleg   Capac   Reserv"<<endl<<endl;
    while(ArchivoHorario.eof() != true){
        ArchivoHorario.read(reinterpret_cast<char *> (&registroHorario), sizeof(registroHorario));
        if ((registroHorario.idRuta == idRuta) && (registroHorario.idTransp == idTrans)){
            cout<<registroHorario.idRuta <<"  "<<registroHorario.idTransp<<"   ";
            cout<<registroHorario.idHorario <<"\t"<<registroHorario.horaSalida<<"\t\t"<<registroHorario.horaLlegada<<"\t"<<registroHorario.capacidad<<"\t";
            cout<<registroHorario.reservados<<endl;
        }
    }
    ArchivoHorario.close();
    ArchivoHorario.open("ArchivoHorario.dat", ios::in | ios::out |ios::binary);
}

//Realiza Reservaciones en el transporte
//Por medio del Id Ruta y Id Transporte
bool reservarTransporte(int idTrans, int idRuta, string origen, string destino, struct vertice *origenV, struct vertice *destinoV){

    int i = 0;
    int x = 0;
    string nacionTico;

    ArchivoTransporte.seekg(0);
    while(ArchivoTransporte.eof() != true){
        ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
        if ((registroTransporte.idRuta == idRuta) && (registroTransporte.idTransp == idTrans)){
            int reservacion;
            char nacionalidad[15];
            cout<<endl<<"INGRESE LA CANTIDAD DE RESERVACIONES: "<<endl<<endl;
            cin>>reservacion;
            cout<<endl<<"INGRESE SU NACION: "<<endl<<endl;
            cin>>nacionalidad;
            nacionTico = nacionalidad;
            if((( registroTransporte.capacidad - registroTransporte.reservaciones) >= reservacion) && (registroTransporte.capacidad > registroTransporte.reservaciones)){
                horariosDisponibles(registroTransporte.idRuta, registroTransporte.idTransp);
                int idHorario;
                cout<<endl<<"INGRESE EL ID DEL HORARIO: "<<endl<<endl;
                cin>>idHorario;
                x = buscarIndexHorarios(registroTransporte.idRuta, registroTransporte.idTransp, idHorario);
                if(x != -1){
                    if((nacionTico == "CostaRica") && (destino == destinoV->ciudad)){
                        cantTicos -= reservacion;
                        destinoV->cantTicos += reservacion;
                    }
                    ArchivoHorario.seekg(x*sizeof(registroHorario));
                    registroHorario.reservados += reservacion;
                    ArchivoHorario.write(reinterpret_cast<char *> (&registroHorario), sizeof(registroHorario));

                    insertarReservacion(registroTransporte.idRuta, registroTransporte.idTransp, origen, destino, reservacion, nacionalidad);
                    i = buscarIndex(idTrans, idRuta);
                    ArchivoTransporte.seekg(i*sizeof(registroTransporte));
                    registroTransporte.reservaciones += reservacion;
                    ArchivoTransporte.write(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
    }
    ArchivoTransporte.close();
    ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
    return false;
}





//Busca una Ruta y muestra los transportes disponibles de esta ruta
//Si exite retorna true o sino retorna false
bool ruta(string origen, string destino, string origenReal, string destinoReal){

    int idRutaAux;
    struct vertice *vO = existeVertice(origen);
    struct vertice *vD = existeVertice(destino);

    if((vO == NULL) || (vD== NULL)){
        return false;
    }

    if(vO->visitado == true){
        return false;
    }
    vO->visitado = true;
    struct arco *tempA = vO->sigA;


    while(tempA != NULL){
        if(tempA->destino == vD){
            ArchivoTransporte.seekg(0);
            while(ArchivoTransporte.eof() != true){
                ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
                if(registroTransporte.idRuta == tempA->id){
                    idRutaAux = tempA->id;
                    transportesDisponibles(tempA->id);
                    int idTrans;
                    cout<<endl<<"INGRESE EL ID DEL TRANSPORTE DE "<<origen<<" A "<<destino<<": "<<endl<<endl;
                    cin>>idTrans;
                    if(reservarTransporte(idTrans, idRutaAux, origenReal, destinoReal, vO, tempA->destino) == true){
                        return true;
                    }
                }
            }
            ArchivoTransporte.close();
            ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
        }
        tempA = tempA->sigA;
    }
    return false;
}




//Transfiere los datos de una pila a otra
void modificarPila1(){

    struct pila *temp1 = primero;

    if(primero == NULL){
        return;
    }
    while(temp1 != NULL){
        Push1(temp1->C);
        temp1 = temp1->sig;
    }
}


//Busca la ruta mas corta y la guarda en pila
//Recive como parametros los nombres de la ciudad origen y destino
bool rutaCorta(string O, string D, int min){

    struct vertice *oV = existeVertice(O);
    struct vertice *dV = existeVertice(D);

    if((oV == NULL) || (dV == NULL))
        return false;

    Push(oV->ciudad);

    if(oV->visitado == true)
        return false;

    if(min > minmin)
        return false;


    if (oV->ciudad == D){

        if(min < minmin){
            minmin = min;
            primero1 = NULL;
            modificarPila1();
            return true;
        }
    }

    oV->visitado = true;
    struct arco *tempA=oV->sigA;

    while(tempA!=NULL){
        rutaCorta(tempA->destino->ciudad,D,min+1);
        tempA=tempA->sigA;
        pop();
    }
    oV->visitado=false;
}


//Imprime las Rutas disponibles del Grafo
void imprimirRutasGrafo(){

    struct vertice *tempV = grafo;
    cout<<endl<<"---------------------- Rutas Disponibles ---------------------"<<endl<<endl<<endl;
    while(tempV != NULL){
        struct arco *tempA = tempV->sigA;

        while(tempA != NULL){
            cout<<endl<<"ID Ruta: "<<tempA->id<<"\t Origen: "<<tempV->ciudad<<"\t Destino: "<<tempA->destino->ciudad<<endl;
            tempA = tempA->sigA;
        }
        tempV = tempV->sigV;
    }
}

//Carga los vertices y arcos del Archivo Rutas
void cargarGrafo(){

    ArchivoRuta.seekg(0);

    while(ArchivoRuta.eof() != true){
        ArchivoRuta.read(reinterpret_cast<char *> (&registroRuta), sizeof(registroRuta));
        if(ArchivoRuta.eof() != true){
            insertarVertice(registroRuta.origen);
            insertarVertice(registroRuta.destino);
        }
    }
    ArchivoRuta.close();
    ArchivoRuta.open("ArchivoRuta.dat", ios::in | ios::out |ios::binary);

    ArchivoRuta.seekg(0);

    while(ArchivoRuta.eof() != true){
        ArchivoRuta.read(reinterpret_cast<char *> (&registroRuta), sizeof(registroRuta));
        if(ArchivoRuta.eof() != true){
            insertarArco(registroRuta.origen, registroRuta.destino, registroRuta.id, registroRuta.dirigido);
            if(registroRuta.dirigido == false){
                insertarArco(registroRuta.destino, registroRuta.origen, registroRuta.id, registroRuta.dirigido);
            }
        }
    }
    ArchivoRuta.close();
    ArchivoRuta.open("ArchivoRuta.dat", ios::in | ios::out |ios::binary);
}

//Busca el transporte mas barato de una ruta
int transportesBaratos(int idRuta){

    long index = 0;
    int i = 0;

    ArchivoTransporte.seekg(0);

    registroTransporteAux.costo = 10000000;
    cout<<endl<<"---------------------- Transportes Baratos ---------------------"<<endl<<endl;
    cout<<endl<<"IdR    IdT      Tipo    Durac   Costo   Capac   Reserv"<<endl<<endl;

    while(ArchivoTransporte.eof() != true){
        ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
        if(registroTransporte.idRuta == idRuta){
            if(registroTransporteAux.costo >= registroTransporte.costo){
                registroTransporteAux = registroTransporte;
            }
        }
    }
    cout<<registroTransporteAux.idRuta <<"\t"<<registroTransporteAux.idTransp<<"\t";
    cout<<registroTransporteAux.tipo<<"\t"<<registroTransporteAux.duracion<<"\t"<<registroTransporteAux.costo<<"\t";
    cout<<registroTransporteAux.capacidad<<"\t"<<registroTransporteAux.reservaciones<<endl;

    ArchivoTransporte.close();
    ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
    return registroTransporteAux.idTransp;
}


//Busca una Ruta y muestra los transportes mas baratos de una ruta
//Si exite retorna true o sino retorna false
bool rutasBaratas(string origen, string destino, string origenReal, string destinoReal){

    int idRutaAux;
    int idTransporte;
    struct vertice *vO = existeVertice(origen);
    struct vertice *vD = existeVertice(destino);

    if((vO == NULL) || (vD== NULL)){
        return false;
    }

    if(vO->visitado == true){
        return false;
    }
    vO->visitado = true;
    struct arco *tempA = vO->sigA;


    while(tempA != NULL){
        if(tempA->destino == vD){
            ArchivoTransporte.seekg(0);
            while(ArchivoTransporte.eof() != true){
                ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
                if(registroTransporte.idRuta == tempA->id){
                    idRutaAux = tempA->id;
                    idTransporte = transportesBaratos(tempA->id);
                    int idTrans;
                    cout<<endl<<"INGRESE EL ID DEL TRANSPORTE DE "<<origen<<" A "<<destino<<": "<<endl<<endl;
                    cin>>idTrans;
                    if(idTransporte == idTrans){
                        if(reservarTransporte(idTrans, idRutaAux, origenReal, destinoReal, vO, tempA->destino) == true){
                            return true;
                        }
                    }
                }
            }
            ArchivoTransporte.close();
            ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
        }
        tempA = tempA->sigA;
    }
    return false;
}

void transportesSinX(int idRuta, string nomTrans){

    long index = 0;
    int i = 0;

    ArchivoTransporte.seekg(0);

    cout<<endl<<"---------------------- Transportes Baratos ---------------------"<<endl<<endl;
    cout<<endl<<"IdR    IdT      Tipo    Durac   Costo   Capac   Reserv"<<endl<<endl;

    while(ArchivoTransporte.eof() != true){
        ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
        if(registroTransporte.idRuta == idRuta){
            if(registroTransporte.tipo != nomTrans){
                cout<<registroTransporte.idRuta <<"\t"<<registroTransporte.idTransp<<"\t";
                cout<<registroTransporte.tipo<<"\t"<<registroTransporte.duracion<<"\t"<<registroTransporte.costo<<"\t";
                cout<<registroTransporte.capacidad<<"\t"<<registroTransporte.reservaciones<<endl;
            }
        }
    }
    ArchivoTransporte.close();
    ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
}

//Busca una Ruta y muestra los transportes sin el transporte X de una ruta
//Si exite retorna true o sino retorna false
bool rutasSinTransX(string origen, string destino, string nomTrans, string origenReal, string destinoReal){

    int idRutaAux;
    int idTransporte;
    struct vertice *vO = existeVertice(origen);
    struct vertice *vD = existeVertice(destino);

    if((vO == NULL) || (vD== NULL)){
        return false;
    }

    if(vO->visitado == true){
        return false;
    }
    vO->visitado = true;
    struct arco *tempA = vO->sigA;

    while(tempA != NULL){
        if(tempA->destino == vD){
            ArchivoTransporte.seekg(0);
            while(ArchivoTransporte.eof() != true){
                ArchivoTransporte.read(reinterpret_cast<char *> (&registroTransporte), sizeof(registroTransporte));
                if(registroTransporte.idRuta == tempA->id){
                    idRutaAux = tempA->id;
                    transportesSinX(tempA->id, nomTrans);
                    int idTrans;
                    cout<<endl<<"INGRESE EL ID DEL TRANSPORTE DE "<<origen<<" A "<<destino<<": "<<endl<<endl;
                    cin>>idTrans;
                    if(reservarTransporte(idTrans, idRutaAux, origenReal, destinoReal, vO, tempA->destino) == true){
                        return true;
                    }
                }
            }
            ArchivoTransporte.close();
            ArchivoTransporte.open("ArchivoTransporte.dat", ios::in | ios::out |ios::binary);
        }
        tempA = tempA->sigA;
    }
    return false;
}


//-------------------------Metodos de Interfaz de Ruta---------------------------------


//Muestra la opciones relacionadas con Rutas al usuario desde la Interfaz
void rutasGUI(){

    system("cls");

    char x;
    cout <<endl<<"----------------------------- MENU RUTAS -----------------------------"<<endl;
    cout <<endl<<"ELIJA UNA OPCION:";
    cout <<endl<<"1. INSERTAR RUTA";
    cout <<endl<<"2. IMPRIMIR RUTAS";
    cout <<endl<<"3. REGRESAR";
    cout <<endl<<"OPCION: ";
    cin >> x;

    if (x == '1'){
        insertarRutaGUI();
    }
    else if (x == '2'){
        imprimirRutasGUI();
    }
    else if (x == '3'){
        menuPrincipal();
    }
    else{
        rutasGUI();
    }
}

//Inserta una Ruta por medio de Interfaz
void insertarRutaGUI(){

    system("cls");

    int idRuta;
    char origen[15];
    char destino[15];
    bool dirigido;

    string dirig;

    char x = '1';

    while(x=='1'){
        cout <<endl<<"-------------------------- AGREGAR RUTA -----------------------"<<endl;
        cout <<endl<<"DIGITE EL NOMBRE DE LA CIUDAD ORIGEN: ";
        cin >> origen;
        cout <<endl<<"DIGITE EL NOMBRE DE LA CIUDAD DESTINO: ";
        cin >> destino;
        cout <<endl<<"DIGITE UN ID PARA LA RUTA: ";
        cin >> idRuta;
        cout <<endl<<"LA RUTA ES DIRIGIDA S/N: ";
        cin >> dirig;

        if((dirig == "S") && (dirig == "s")){
            dirigido = true;
        }
        dirigido = false;

        if(insertarRuta(idRuta, dirigido, origen, destino) == true){
            cout <<endl<<"---> SE AGREGO CORRECTAMENTE!!"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cargarGrafo();
            cin>>x;
        }
        else{
            cout <<endl<<"---> NO SE AGREGO CORRECTAMENTE ***"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
    }
    rutasGUI();
}

//Imprime las Rutas desde la Interfaz
void imprimirRutasGUI(){
    system("cls");

    char x = '1';

    while(x=='1'){
        cout <<endl<<"----------------------------- RUTAS -----------------------------"<<endl;
        imprimirArchivoRuta();
        cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
        cin>>x;
    }
    rutasGUI();
}

//-------------------------Metodos de Interfaz de Transportes---------------------------------



//Muestra la opciones relacionadas con Transportes al usuario desde la Interfaz

void transportesGUI(){

    system("cls");

    char x;
    cout <<endl<<"----------------------------- MENU TRANSPORTES -----------------------------"<<endl;
    cout <<endl<<"ELIJA UNA OPCION:";
    cout <<endl<<"1. INSERTAR TRANSPORTE";
    cout <<endl<<"2. IMPRIMIR TRANSPORTES";
    cout <<endl<<"3. REGRESAR";
    cout <<endl<<"OPCION: ";
    cin >> x;

    if (x == '1'){
        insertarTransporteGUI();
    }
    else if (x == '2'){
        imprimirTransporteGUI();
    }
    else if (x == '3'){
        menuPrincipal();
    }
    else{
        transportesGUI();
    }
}

//Inserta un Transporte por medio de Interfaz
void insertarTransporteGUI(){

    system("cls");

    int idRuta;
    int idTransp;
    char tipo[10];
    char duracion[10];
    int costo;
    int capacidad;
    int reservaciones;

    char x = '1';

    while(x=='1'){
        cout <<endl<<"-------------------------- AGREGAR TRANSPORTE -----------------------"<<endl;
        cout <<endl<<"DIGITE EL ID DE LA RUTA: ";
        cin >> idRuta;
        cout <<endl<<"DIGITE UN ID PARA EL TRANSPORTE: ";
        cin >> idTransp;
        cout <<endl<<"DIGITE EL TIPO DE TRANSPORTE: ";
        cin >> tipo;
        cout <<endl<<"DIGITE LA DURACION DEL VIAJE: ";
        cin >> duracion;
        cout <<endl<<"DIGITE EL COSTO DEL VIAJE: ";
        cin >> costo;
        cout <<endl<<"DIGITE LA CAPACIDAD DEL TRANSPORTE: ";
        cin >> capacidad;
        cout <<endl<<"DIGITE LAS RESERVACIONES: ";
        cin >> reservaciones;

        if(insertarTransporte(idRuta,idTransp,tipo,duracion,costo,capacidad,reservaciones) == true){
            cout <<endl<<"---> SE AGREGO CORRECTAMENTE!!"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
        else{
            cout <<endl<<"---> NO SE AGREGO CORRECTAMENTE ***"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
    }
    transportesGUI();
}

//Imprime los Transportes desde la Interfaz
void imprimirTransporteGUI(){
    system("cls");

    char x = '1';

    while(x=='1'){
        cout <<endl<<"-----------------------------TRANSPORTES-----------------------------"<<endl;
        imprimirArchivoTransporte();
        cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
        cin>>x;
    }
    transportesGUI();
}

//------------------------- Metodos de Interfaz de Horarios ---------------------------------


//Muestra la opciones relacionadas con Horarios al usuario desde la Interfaz
void horariosGUI(){

    system("cls");
    char x;

    cout <<endl<<"----------------------------- MENU HORARIOS -----------------------------"<<endl;
    cout <<endl<<"ELIJA UNA OPCION:";
    cout <<endl<<"1. INSERTAR HORARIO";
    cout <<endl<<"2. IMPRIMIR HORARIOS";
    cout <<endl<<"3. REGRESAR";
    cout <<endl<<"OPCION: ";
    cin >> x;

    if (x == '1'){
        insertarHorarioGUI();
    }
    else if (x == '2'){
        imprimirHorariosGUI();
    }
    else if (x == '3'){
        menuPrincipal();
    }
    else{
        rutasGUI();
    }
}


//Inserta un Horario por medio de Interfaz
void insertarHorarioGUI(){

    system("cls");

    int idRuta;
    int idTransp;
    int idHorario;
    int horaSalida;
    int horaLlegada;
    int capacidad;
    int reservaciones;

    char x = '1';

    while(x=='1'){
        cout <<endl<<"-------------------------- AGREGAR HORARIO -----------------------"<<endl;
        cout <<endl<<"DIGITE EL ID DE LA RUTA: ";
        cin >> idRuta;
        cout <<endl<<"DIGITE UN ID PARA EL TRANSPORTE: ";
        cin >> idTransp;
        cout <<endl<<"DIGITE EL ID DEL HORARIO: ";
        cin >> idHorario;
        cout <<endl<<"DIGITE LA HORA DE SALIDA DEL VIAJE: ";
        cin >> horaSalida;
        cout <<endl<<"DIGITE LA HORA DE LLEGAGA DEL VIAJE: ";
        cin >> horaLlegada;

        if(insertarHorario(idRuta,idTransp,idHorario,horaSalida,horaLlegada) == true){
            cout <<endl<<"---> SE AGREGO CORRECTAMENTE!!"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
        else{
            cout <<endl<<"---> NO SE AGREGO CORRECTAMENTE ***"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
    }
    horariosGUI();
}


//Imprime los Horarios desde la Interfaz
void imprimirHorariosGUI(){

    system("cls");

    char x = '1';

    while(x=='1'){
        cout <<endl<<"---------------------- HORARIOS -----------------------------"<<endl;
        imprimirArchivoHorario();
        cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
        cin>>x;
    }
    horariosGUI();
}

//------------------------- Metodos de Interfaz de Reservaciones ---------------------------------


//Muestra la opciones relacionadas con Reservaciones al usuario desde la Interfaz
void reservacionesGUI(){

    system("cls");
    desmarcar();

    char x;
    cout <<endl<<"----------------------------- MENU RESERVACIONES -----------------------------"<<endl;
    cout <<endl<<"ELIJA UNA OPCION:";
    cout <<endl<<"1. REALIZAR UNA RESERVACION";
    cout <<endl<<"2. IMPRIMIR RESERVACIONES";
    cout <<endl<<"3. REGRESAR";
    cout <<endl<<"OPCION: ";
    cin >> x;

    if (x == '1'){
        insertarReservacionGUI();
    }
    else if (x == '2'){
        imprimirReservacionesGUI();
    }
    else if (x == '3'){
        menuPrincipal();
    }
    else{
        reservacionesGUI();
    }
}

//Inserta Reservaciones por medio de Interfaz
void insertarReservacionGUI(){

    desmarcar();
    system("cls");

    string origen;
    string destino;

    char x = '1';

    while(x=='1'){
        cout <<endl<<"---------------------- RESERVACIONES ----------------------"<<endl;
        cout<<endl<<"INGRESE LA CIUDAD DE SALIDA: "<<endl<<endl;
        cin>>origen;
        cout<<endl<<"INGRESE LA CIUDAD DE DESTINO: "<<endl<<endl;
        cin>>destino;
        minmin = 1000000;
        if(rutaCorta(origen, destino,0) == true){
            struct pila *temp1 = primero1;
            struct pila *temp2 = primero1->sig;
            while(temp2 != NULL){
                if(ruta(temp1->C, temp2->C, origen, destino) == true){
                    cout <<endl<<"---> SE AGREGO CORRECTAMENTE!!"<<endl;
                    cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
                    cin>>x;
                }
                temp1 = temp2;
                temp2 = temp2->sig;
            }
        }
        else{
            cout <<endl<<"---> NO SE EXISTE RUTA!!"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
    }
    reservacionesGUI();
}

//Imprime las Reservaciones desde la Interfaz
void imprimirReservacionesGUI(){

    system("cls");

    char x = '1';

    while(x=='1'){
        cout <<endl<<"----------------------------- RESERVACIONES -----------------------------"<<endl;
        imprimirArchivoReservaciones();
        cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
        cin>>x;
    }
    reservacionesGUI();
}

//----------------------------- Consultas --------------------------------------


//Muestra la opciones relacionadas con las consulta de busqueda personalizada al usuario desde la Interfaz
void consultasGUI(){

    system("cls");
    desmarcar();
    char x;
    cout <<endl<<"----------------------------- MENU CONSULTAS -----------------------------"<<endl;
    cout <<endl<<"ELIJA UNA OPCION:";
    cout <<endl<<"1. BUSQUEDA PERSONALIZADA";
    cout <<endl<<"2. REALIZAR UNA RESERVACION";
    cout <<endl<<"3. CONSULTAR RESERVACIONES";
    cout <<endl<<"4. REGRESAR";
    cout <<endl<<"OPCION: ";
    cin >> x;

    if (x == '1'){
        consultaUnoGUI();
    }
    else if (x == '2'){
        consultaDosGUI();
    }
    else if (x == '3'){
        consultaTres();
    }
    else if (x == '4'){
        menuPrincipal();
    }
    else{
        consultasGUI();
    }
}


//Muestra la opciones relacionadas con las consulta de busqueda personalizada al usuario desde la Interfaz
void consultaUnoGUI(){

    system("cls");
    desmarcar();
    char x;
    cout <<endl<<"----------------------------- BUSQUEDA PERSONALIZADA -----------------------------"<<endl;
    cout <<endl<<"ELIJA UNA OPCION:";
    cout <<endl<<"1. LA RUTA MAS BARATA";
    cout <<endl<<"2. LA RUTA MAS CORTA";
    cout <<endl<<"3. LAS QUE NO UTILIZEN UN TRANSPORTE X";
    cout <<endl<<"4. REGRESAR";
    cout <<endl<<"OPCION: ";
    cin >> x;

    if (x == '1'){
        consultaRutaBarataGUI();
    }
    else if (x == '2'){
        consultaRutaCortaGUI();
    }
    else if (x == '3'){
        consultaSinTransXGUI();
    }
    else if (x == '4'){
        consultasGUI();
    }
    else{
        consultaUnoGUI();
    }
}


//Reserva la ruta mas barata por medio de Interfaz
void consultaRutaBarataGUI(){

    system("cls");

    string origen;
    string destino;

    char x = '1';

    while(x=='1'){
        cout <<endl<<"---------------------- RESERVAR LA RUTA MAS BARATA ----------------------"<<endl;
        cout<<endl<<"INGRESE LA CIUDAD DE SALIDA: "<<endl<<endl;
        cin>>origen;
        cout<<endl<<"INGRESE LA CIUDAD DE DESTINO: "<<endl<<endl;
        cin>>destino;
        minmin = 1000000;
        if(rutaCorta(origen, destino,0) == true){
            struct pila *temp1 = primero1;
            struct pila *temp2 = primero1->sig;
            while(temp2 != NULL){
                desmarcar();
                if(rutasBaratas(temp1->C, temp2->C, origen, destino) == true){
                    desmarcar();
                    cout <<endl<<"---> SE AGREGO CORRECTAMENTE!!"<<endl;
                    cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
                    cin>>x;
                }
                temp1 = temp2;
                temp2 = temp2->sig;
            }
        }
        else{
            cout <<endl<<"---> NO SE EXISTE RUTA!!"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
    }
   consultasGUI();
}


//Reserva la ruta mas corta por medio de Interfaz
void consultaRutaCortaGUI(){

    system("cls");

    string origen;
    string destino;

    char x = '1';

    while(x=='1'){
        cout <<endl<<"---------------------- RESERVAR RUTA CORTA ----------------------"<<endl;
        cout<<endl<<"INGRESE LA CIUDAD DE SALIDA: "<<endl<<endl;
        cin>>origen;
        cout<<endl<<"INGRESE LA CIUDAD DE DESTINO: "<<endl<<endl;
        cin>>destino;
        minmin = 1000000;
        if(rutaCorta(origen, destino,0) == true){
            struct pila *temp1 = primero1;
            struct pila *temp2 = primero1->sig;
            while(temp2 != NULL){
                desmarcar();
                if(ruta(temp1->C, temp2->C, origen, destino) == true){
                    desmarcar();
                    cout <<endl<<"---> SE AGREGO CORRECTAMENTE!!"<<endl;
                    cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
                    cin>>x;
                }
                temp1 = temp2;
                temp2 = temp2->sig;
            }
        }
        else{
            cout <<endl<<"---> NO SE EXISTE RUTA!!"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
    }
    consultasGUI();
}


//Reserva la ruta sin el transporte X;
void consultaSinTransXGUI(){

    system("cls");

    string origen;
    string destino;
    string nomTrans;

    char x = '1';

    while(x=='1'){
        cout <<endl<<"---------------------- RESERVAR SIN TRANSPORTE X ----------------------"<<endl;
        cout<<endl<<"INGRESE LA CIUDAD DE SALIDA: "<<endl<<endl;
        cin>>origen;
        cout<<endl<<"INGRESE LA CIUDAD DE DESTINO: "<<endl<<endl;
        cin>>destino;
        cout<<endl<<"INGRESE EL TRANSPORTE QUE NO DESEA: "<<endl<<endl;
        cin>>nomTrans;
        minmin = 1000000;
        if(rutaCorta(origen, destino,0) == true){
            struct pila *temp1 = primero1;
            struct pila *temp2 = primero1->sig;
            while(temp2 != NULL){
                desmarcar();
                if(rutasSinTransX(temp1->C, temp2->C, nomTrans, origen, destino) == true){
                    desmarcar();
                    cout <<endl<<"---> SE AGREGO CORRECTAMENTE!!"<<endl;
                    cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
                    cin>>x;
                }
                temp1 = temp2;
                temp2 = temp2->sig;
            }
        }
        else{
            cout <<endl<<"---> NO SE EXISTE RUTA!!"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
    }
    consultasGUI();
}

//Inserta Reservaciones por medio de Interfaz
void consultaDosGUI(){

    system("cls");

    string origen;
    string destino;

    char x = '1';

    while(x=='1'){
        cout <<endl<<"---------------------- CONSULTA 2 ----------------------"<<endl;
        cout<<endl<<"INGRESE LA CIUDAD DE SALIDA: "<<endl<<endl;
        cin>>origen;
        cout<<endl<<"INGRESE LA CIUDAD DE DESTINO: "<<endl<<endl;
        cin>>destino;
        minmin = 1000000;
        if(rutaCorta(origen, destino,0) == true){
            struct pila *temp1 = primero1;
            struct pila *temp2 = primero1->sig;
            while(temp2 != NULL){
                desmarcar();
                if(ruta(temp1->C, temp2->C, origen, destino) == true){
                    desmarcar();
                    cout <<endl<<"---> SE AGREGO CORRECTAMENTE!!"<<endl;
                    cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
                    cin>>x;
                }
                temp1 = temp2;
                temp2 = temp2->sig;
            }
        }
        else{
            cout <<endl<<"---> NO SE EXISTE RUTA!!"<<endl;
            cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
            cin>>x;
        }
    }
    consultasGUI();
}

//Muestra las reservaciones de una ciudad a otra por medio de la interfaz
void consultaTres(){

    system("cls");

    char x = '1';
    string origen;
    string destino;

    while(x=='1'){
        cout <<endl<<"---------------------- CONSULTA TRES ----------------------"<<endl;

        cout<<endl<<"INGRESE LA CIUDAD DE SALIDA: "<<endl<<endl;
        cin>>origen;
        cout<<endl<<"INGRESE LA CIUDAD DE DESTINO: "<<endl<<endl;
        cin>>destino;

        ArchivoReservaciones.seekg(0);
        cout<<endl<<endl<<"LA CANTIDAD DE RESERVACIONES DE "<<origen<<" A "<<destino<<" : "<<endl<<endl;
        cout<<"Origen    Destino     ID      Reservaciones"<<endl<<endl;

        while(ArchivoReservaciones.eof() != true){
            ArchivoReservaciones.read(reinterpret_cast<char *> (&registroReservaciones), sizeof(registroReservaciones));
            if(ArchivoReservaciones.eof() != true){
                if((registroReservaciones.origen == origen) && (registroReservaciones.destino == destino)){
                    cout<<registroReservaciones.origen <<"\t" <<registroReservaciones.destino<<"\t"<<registroReservaciones.idReservacion<<"\t"<<registroReservaciones.numReservaciones<<endl;
                }
            }
        }
        ArchivoReservaciones.close();
        ArchivoReservaciones.open("ArchivoReservaciones.dat", ios::in | ios::out |ios::binary);

        cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
        cin>>x;
    }
    consultasGUI();

}


//------------------------------- Reportes ------------------------------------------

//Imprime el grafo por profundidad desde la Interfaz
void profundidadGUI(){

    system("cls");

    char x = '1';

    while(x=='1'){
        cout <<endl<<"----------------------------- GRAFO PROFUNDIDAD -----------------------------"<<endl<<endl<<endl;
        profundidad(grafo);
        cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
        cin>>x;
    }
    menuPrincipal();
}

//Muestra la presencia de ticos de cada ciudad
void mostrarTicosGUI(){

    system("cls");
    char x = '1';

    cout<<endl<<endl<<"--------------------------- PRESENCIA DE TICOS ------------------------------"<<endl<<endl;

    while(x=='1'){
        struct vertice *tempV = grafo;
        struct arco *tempA;
        while(tempV != NULL){
            cout<<endl<<endl<<"Ciudad: "<<tempV->ciudad;
            cout<<" Cantidad de ticos: "<<tempV->cantTicos<<endl<<endl;
            tempV = tempV->sigV;
        }
        cout <<endl<<"---> PRESIONE UNA TECLA Y LUEGO ENTER PARA VOLVER <---"<<endl;
        cin>>x;
    }
    menuPrincipal();
}


//-------------------------------Menu Principal Interfaz-----------------------------------

//Muestra todas las opciones que puede realizar el Usuario desde la Interfaz
void menuPrincipal(){
    system("cls");

    desmarcar();
    char x;
    cout <<endl<<"-----------------------------MENU PRINCIPAL-----------------------------"<<endl;
    cout <<endl<<"ELIJA UNA OPCION:";
    cout <<endl<<"1. TODO RELACIONADO A RUTAS";
    cout <<endl<<"2. TODO RELACIONADO A TANSPORTES";
    cout <<endl<<"3. TODO RELACIONADO A HORARIOS";
    cout <<endl<<"4. TODO RELACIONADO A RESERVACIONES";
    cout <<endl<<"5. IMPRIMIR AMPLITUD";
    cout <<endl<<"6. IMPRIMIR PROFUNDIDAD";
    cout <<endl<<"7. MOSTRAR PRESENCIA DE TICOS";
    cout <<endl<<"8. CONSULTAS";
    cout <<endl<<"OPCION: ";
    cin >> x;

    if (x == '1'){
        rutasGUI();
    }
    else if (x == '2'){
        transportesGUI();
    }
    else if (x == '3'){
        horariosGUI();
    }
    else if (x == '4'){
        reservacionesGUI();
    }
    else if (x == '5'){
        amplitud();
    }
    else if (x == '6'){
        profundidadGUI();
    }
    else if (x == '7'){
        mostrarTicosGUI();
    }
    else if (x == '8'){
        consultasGUI();
    }
    else{
        menuPrincipal();
    }
}

int main()
{

    //menuPrincipal();
    insertarTransporte(1,1,"Bus","4horas", 500,60,0);
    insertarTransporte(2,1,"Avion","2 horas", 20000,250,0);
    insertarTransporte(3,1,"Tren","3horas", 1000,150,0);
    insertarTransporte(4,1,"Taxi","2 horas", 1000,7,0);

    insertarTransporte(5,1,"Bus","4horas", 500,60,0);
    insertarTransporte(6,1,"Avion","2 horas", 20000,250,0);
    insertarTransporte(7,1,"Tren","3horas", 1000,150,0);
    insertarTransporte(8,1,"Taxi","2 horas", 1000,7,0);

    insertarTransporte(9,1,"Bus","4horas", 500,60,0);
    insertarTransporte(10,1,"Avion","2 horas", 20000,250,0);
    insertarTransporte(11,1,"Tren","3horas", 1000,150,0);
    insertarTransporte(12,1,"Taxi","2 horas", 1000,7,0);
    //**************************************************


    insertarTransporte(1,2,"Avion","2 horas", 25000,200,0);
    insertarTransporte(2,2,"Bus","4horas", 350,60,0);
    insertarTransporte(3,2,"Tren","3horas", 1500,100,0);
    insertarTransporte(4,2,"Taxi","2 horas", 900,4,0);

    insertarTransporte(6,2,"Avion","2 horas", 25000,200,0);
    insertarTransporte(5,2,"Bus","4horas", 350,60,0);
    insertarTransporte(7,2,"Tren","3horas", 1500,100,0);
    insertarTransporte(8,2,"Taxi","2 horas", 900,4,0);

    insertarTransporte(9,2,"Avion","2 horas", 25000,200,0);
    insertarTransporte(10,2,"Bus","4horas", 350,60,0);
    insertarTransporte(11,2,"Tren","3horas", 1500,100,0);
    insertarTransporte(12,2,"Taxi","2 horas", 900,4,0);
    //*****************************************************

    insertarTransporte(1,3,"Tren","3horas", 1750,175,0);
    insertarTransporte(2,3,"Bus","4horas", 750,70,0);
    insertarTransporte(3,3,"Avion","2 horas", 32500,300,0);
    insertarTransporte(4,3,"Taxi","2 horas", 1000,3,0);

    insertarTransporte(5,3,"Tren","3horas", 1750,175,0);
    insertarTransporte(6,3,"Bus","4horas", 750,70,0);
    insertarTransporte(7,3,"Avion","2 horas", 32500,300,0);
    insertarTransporte(8,3,"Taxi","2 horas", 1000,3,0);

    insertarTransporte(9,3,"Tren","3horas", 1750,175,0);
    insertarTransporte(10,3,"Bus","4horas", 750,70,0);
    insertarTransporte(11,3,"Avion","2 horas", 32500,300,0);
    insertarTransporte(12,3,"Taxi","2 horas", 1000,3,0);
    //******************************************************

    insertarTransporte(1,4,"Taxi","2 horas", 700,5,0);
    insertarTransporte(2,4,"Bus","4horas", 250,40,0);
    insertarTransporte(3,4,"Avion","2 horas", 15000,200,0);
    insertarTransporte(4,4,"Tren","3horas", 800,100,0);

    insertarTransporte(5,4,"Taxi","2 horas", 700,5,0);
    insertarTransporte(6,4,"Bus","4horas", 250,40,0);
    insertarTransporte(7,4,"Avion","2 horas", 15000,200,0);
    insertarTransporte(8,4,"Tren","3horas", 800,100,0);

    insertarTransporte(9,4,"Taxi","2 horas", 700,5,0);
    insertarTransporte(10,4,"Bus","4horas", 250,40,0);
    insertarTransporte(11,4,"Avion","2 horas", 15000,200,0);
    insertarTransporte(12,4,"Tren","3horas", 800,100,0);
    //**************************************************************

    insertarRuta(12, false, "RiodeJaneiro", "Curitiba");
    insertarRuta(11, true, "SaoPaulo", "RiodeJaneiro");
    insertarRuta(10, false, "BeloHorizonte", "Salvador");
    insertarRuta(9, true, "Cuiaba", "BeloHorizonte");
    insertarRuta(8, false, "Brasilia", "Cuiaba");
    insertarRuta(7, true, "Salvador", "Brasilia");
    insertarRuta(6, false, "Natal", "Recife");
    insertarRuta(5, true, "Cuiaba", "Fortaleza");
    insertarRuta(4, true, "Fortaleza", "Brasilia");
    insertarRuta(3, false, "Fortaleza", "Natal");
    insertarRuta(2, true, "Manaos", "Cuiaba");
    insertarRuta(1, true, "Manaos", "Fortaleza");


    //******** Horario Transporte 1 ***********
    insertarHorario(1,1,1,10,2);
    insertarHorario(2,1,1,10,2);
    insertarHorario(3,1,1,18,21);
    insertarHorario(4,1,1,13,15);

    insertarHorario(5,1,1,10,2);
    insertarHorario(6,1,1,10,2);
    insertarHorario(7,1,1,18,21);
    insertarHorario(8,1,1,13,15);

    insertarHorario(9,1,1,10,2);
    insertarHorario(10,1,1,10,2);
    insertarHorario(11,1,1,18,21);
    insertarHorario(12,1,1,13,15);

    //*********** Horario Transporte 2 *******************
    insertarHorario(1,2,1,10,2);
    insertarHorario(2,2,1,10,2);
    insertarHorario(3,2,1,18,21);
    insertarHorario(4,2,1,13,15);

    insertarHorario(5,2,1,10,2);
    insertarHorario(6,2,1,10,2);
    insertarHorario(7,2,1,18,21);
    insertarHorario(8,2,1,13,15);

    insertarHorario(9,2,1,10,2);
    insertarHorario(10,2,1,10,2);
    insertarHorario(11,2,1,18,21);
    insertarHorario(12,2,1,13,15);


    //************* Horario Transporte 3 ******************
    insertarHorario(1,3,1,10,2);
    insertarHorario(2,3,1,10,2);
    insertarHorario(3,3,1,18,21);
    insertarHorario(4,3,1,13,15);

    insertarHorario(5,3,1,10,2);
    insertarHorario(6,3,1,10,2);
    insertarHorario(7,3,1,18,21);
    insertarHorario(8,3,1,13,15);

    insertarHorario(9,3,1,10,2);
    insertarHorario(10,3,1,10,2);
    insertarHorario(11,3,1,18,21);
    insertarHorario(12,3,1,13,15);

    //************** Horario Transporte 4 *****************
    insertarHorario(1,4,1,10,2);
    insertarHorario(2,4,1,10,2);
    insertarHorario(3,4,1,18,21);
    insertarHorario(4,4,1,13,15);

    insertarHorario(5,4,1,10,2);
    insertarHorario(6,4,1,10,2);
    insertarHorario(7,4,1,18,21);
    insertarHorario(8,4,1,13,15);

    insertarHorario(9,4,1,10,2);
    insertarHorario(10,4,1,10,2);
    insertarHorario(11,4,1,18,21);
    insertarHorario(12,4,1,13,15);

    //************* Horario Transporte 5 *****************
    insertarHorario(1,5,1,10,2);
    insertarHorario(2,5,1,10,2);
    insertarHorario(3,5,1,18,21);
    insertarHorario(4,5,1,13,15);

    insertarHorario(5,5,1,10,2);
    insertarHorario(6,5,1,10,2);
    insertarHorario(7,5,1,18,21);
    insertarHorario(8,5,1,13,15);

    insertarHorario(9,5,1,10,2);
    insertarHorario(10,5,1,10,2);
    insertarHorario(11,5,1,18,21);
    insertarHorario(12,5,1,13,15);

    //************* Horario Transporte 6 *****************
    insertarHorario(1,6,1,10,2);
    insertarHorario(2,6,1,10,2);
    insertarHorario(3,6,1,18,21);
    insertarHorario(4,6,1,13,15);

    insertarHorario(5,6,1,10,2);
    insertarHorario(6,6,1,10,2);
    insertarHorario(7,6,1,18,21);
    insertarHorario(8,6,1,13,15);

    insertarHorario(9,6,1,10,2);
    insertarHorario(10,6,1,10,2);
    insertarHorario(11,6,1,18,21);
    insertarHorario(12,6,1,13,15);

    //************* Horario Transporte 7 *****************
    insertarHorario(1,6,1,10,2);
    insertarHorario(2,6,1,10,2);
    insertarHorario(3,6,1,18,21);
    insertarHorario(4,6,1,13,15);

    insertarHorario(5,6,1,10,2);
    insertarHorario(6,6,1,10,2);
    insertarHorario(7,6,1,18,21);
    insertarHorario(8,6,1,13,15);

    insertarHorario(9,6,1,10,2);
    insertarHorario(10,6,1,10,2);
    insertarHorario(11,6,1,18,21);
    insertarHorario(12,6,1,13,15);

    //************* Horario Transporte 8 *****************
    insertarHorario(1,8,1,10,2);
    insertarHorario(2,8,1,10,2);
    insertarHorario(3,8,1,18,21);
    insertarHorario(4,8,1,13,15);

    insertarHorario(5,8,1,10,2);
    insertarHorario(6,8,1,10,2);
    insertarHorario(7,8,1,18,21);
    insertarHorario(8,8,1,13,15);

    insertarHorario(9,8,1,10,2);
    insertarHorario(10,8,1,10,2);
    insertarHorario(11,8,1,18,21);
    insertarHorario(12,8,1,13,15);

    //************* Horario Transporte 9 *****************
    insertarHorario(1,9,1,10,2);
    insertarHorario(2,9,1,10,2);
    insertarHorario(3,9,1,18,21);
    insertarHorario(4,9,1,13,15);

    insertarHorario(5,9,1,10,2);
    insertarHorario(6,9,1,10,2);
    insertarHorario(7,9,1,18,21);
    insertarHorario(8,9,1,13,15);

    insertarHorario(9,9,1,10,2);
    insertarHorario(10,9,1,10,2);
    insertarHorario(11,9,1,18,21);
    insertarHorario(12,9,1,13,15);

    //************* Horario Transporte 10 *****************
    insertarHorario(1,10,1,10,2);
    insertarHorario(2,10,1,10,2);
    insertarHorario(3,10,1,18,21);
    insertarHorario(4,10,1,13,15);

    insertarHorario(5,10,1,10,2);
    insertarHorario(6,10,1,10,2);
    insertarHorario(7,10,1,18,21);
    insertarHorario(8,10,1,13,15);

    insertarHorario(9,10,1,10,2);
    insertarHorario(10,10,1,10,2);
    insertarHorario(11,10,1,18,21);
    insertarHorario(12,10,1,13,15);

    //************* Horario Transporte 11 *****************
    insertarHorario(1,11,1,10,2);
    insertarHorario(2,11,1,10,2);
    insertarHorario(3,11,1,18,21);
    insertarHorario(4,11,1,13,15);

    insertarHorario(5,11,1,10,2);
    insertarHorario(6,11,1,10,2);
    insertarHorario(7,11,1,18,21);
    insertarHorario(8,11,1,13,15);

    insertarHorario(9,11,1,10,2);
    insertarHorario(10,11,1,10,2);
    insertarHorario(11,11,1,18,21);
    insertarHorario(12,11,1,13,15);

    //************* Horario Transporte 12 *****************
    insertarHorario(1,12,1,10,2);
    insertarHorario(2,12,1,10,2);
    insertarHorario(3,12,1,18,21);
    insertarHorario(4,12,1,13,15);

    insertarHorario(5,12,1,10,2);
    insertarHorario(6,12,1,10,2);
    insertarHorario(7,12,1,18,21);
    insertarHorario(8,12,1,13,15);

    insertarHorario(9,12,1,10,2);
    insertarHorario(10,12,1,10,2);
    insertarHorario(11,12,1,18,21);
    insertarHorario(12,12,1,13,15);

    insertarArchivoPartidos(14,06,"Uruguay","CostaRica");
    insertarArchivoPartidos(16,06,"Italia","Inglaterra");
    insertarArchivoPartidos(15,06,"Japon","Grecia");

    cargarGrafo();

    menuPrincipal();


    //imprimirRutasGrafo();
    //imprimirArchivoRuta();
    //imprimirArchivoTransporte();
    //imprimirArchivoHorario();
    //imprimirArchivoPartido();

    return 0;
}
