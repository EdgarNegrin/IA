/* Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Inteligencia Artificial
 *
 * @author Edgar Negrin Gonzalez
 * @email alu0101210964@ull.edu.es
 * @date 9 nov 2020
 * @brief Algoritmo de busqueda de camino minimo
 *
 * Ejecucion: makefile
 * table.h Cabecera de la clase table
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <list>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <fstream>
#include <ctime>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

using namespace std;

// Estructura de los nodos
struct node
{
    int y;
    int x;
    node* parent;
    float g;
    float h; 
    float f;
    bool close;
    bool open;
};

class table {
  private:
    int filas_;
    int columnas_;
    int n_obstacles_;
    int show_details_;
    int n_close_;
    vector<int> destination_;
    vector<int> car_;
    vector<vector<int> > obstacles_;
    vector<node> track_;
    vector<vector<node> > tablero_;
    
  public:
    table(int, int, int);
    ~table(){};

    void position_car(int, int);
    void position_destination(int, int);
    void position_obstacle(int, int, int);

    bool isValid(int, int);
    bool isDestination(int, int);
    bool isCar(int, int);
    void details();

    float calculeH_manhattan(int, int);
    float calculeH_euclidea(int, int);

    void neighboring(vector<node*>&, node);
    void aStar(int, int);

    friend ostream& operator <<(ostream&, table);
};