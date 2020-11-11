#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <list>

using namespace std;

struct node
{
    int y;
    int x;
    node* parent;
    float g;
    float h; 
    float f;
};

class table {
  private:
    int filas_;
    int columnas_;
    vector<int> destination_;
    vector<int> car_;
    vector<vector<int> > obstacles_;
    vector<node> track_;
  public:
    table();
    table(int, int, int);
    ~table(){};

    void position_car(int, int);
    void position_destination(int, int);
    void position_obstacle(int, int, int);

    bool isValid(int fila, int columna);
    bool isDestination(int fila, int columna);
    bool isClose(vector<node> closeList, node node);
    bool isOpen(list<node> openList, node node);

    int calculeH_manhattan(int fila, int columna);
    int calculeH_euclidean(int fila, int columna);

    void neighboring(vector<node>&, node);
    void aStar();

    friend ostream& operator <<(ostream&, table);
};