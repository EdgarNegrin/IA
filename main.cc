#include "table.h"

// tamaño maximo?
// poner colores en el camino
// se puede pedir el numero de obstaculos
// obstaculos aleatorios
// obstaculos a traves de un txt
// comprobacion de que no se repitan posiciones
// funciones euristicas de distancia manjatan, distancia euclidea.
// para el informe, los operadores son los posibles 4 movimientos. Nodos expandidos es todos los nodos evaluados.

// A* ? puede servir de funcion de busqueda. para usarlo hace falta una funcion de evaluacion de dos componenetes
// posibles movimientos(4) que no sean obstaculos y sin salirnos del recinto. fijarse en la distancia para llegar al final con la funcion
// euristica (distancia de manhatan)¿sin contar los obstaculos?, desde cada una de los posibles movimientos, eligiendo el menor numero de movimientos.
// Posivilidad de volver atras solo si queremos desacer un camino para buscar otra ruta.

// tiene solucion? el algoritmo tiene que detectarlo
// minimo numero de estructura de datos necesario

// 2 min ejecucion programa. 2 min aspectos fundamentales del algoritmo. 2 min aspectos de informe. 2 min preguntas.

bool check_random(vector<int> vector1, vector<int> vector2, int num1, int num2) {
  for (int i = 0; i < vector1.size(); i++) {
    for (int j = 0; j < vector2.size(); j++) {
      if ((vector1[i] == num1) && (vector2[j] == num2)) 
        return false;
    }
  }
  return true;
}

int main(int argc, char** argv){
  int filas, columnas, n_obstaculos, type_obj, show_details, funcion, fila, columna, T0, T1;
  vector<int> v_fila, v_columna;
  if (argc == 2){
    ifstream file(argv[1]);

    file >> filas;
    file >> columnas;    
    file >> n_obstaculos;
    file >> type_obj;
    table tablero(filas, columnas, n_obstaculos); // pasar el numero de obstaculos
    file >> fila;
    file >> columna;
    tablero.position_car(fila, columna);
    file >> fila;
    file >> columna;
    tablero.position_destination(fila, columna);
    if (type_obj == 2) {
      srand(time(NULL));
      for (int i = 0; i < n_obstaculos; i++) {
        fila = rand()%filas;
        v_fila.push_back(fila);
        columna = rand()%columnas;
        v_columna.push_back(columna);
        while (!check_random(v_fila, v_columna, fila, columna)) { // Hacer que no pueda coincidir con el inicio ni con el destino
          fila = rand()%filas;
          columna = rand()%columnas;
        }
        tablero.position_obstacle(fila, columna, i);
      }
    } else {
      for (int i = 0; i < n_obstaculos; i++) {
        file >> fila;
        file >> columna;
        tablero.position_obstacle(fila, columna, i);
      }
    }
    file >> funcion;
    file >> show_details;
    T0 = clock();
    tablero.aStar(show_details, funcion);
    T1 = clock();
  } else {
    cout << "Numero de filas: ";
    cin >> filas;
    cout << "Numero de columnas: ";
    cin >> columnas;

    cout << endl << "1.Obstaculos por posicion";
    cout << endl << "2.Obstaculos aleatorios" << endl;
    cin >> type_obj;

    cout << endl << "Numero de obstaculos: ";
    cin >> n_obstaculos;
    table tablero(filas, columnas, n_obstaculos); // pasar el numero de obstaculos

    cout << endl << "Posicion coche fila: ";
    cin >> fila;
    cout << "Posicion coche columna: ";
    cin >> columna;
    tablero.position_car(fila, columna);

    cout << endl << "Posicion destino fila: ";
    cin >> fila;
    cout << "Posicion destino columna: ";
    cin >> columna;
    tablero.position_destination(fila, columna);

    if (type_obj == 1) { // Obstaculos por consola
      for (int i = 0; i < n_obstaculos; i++) {
        cout << "Obstaculo " << i + 1;
        cout << endl << "Posicion de fila: ";
        cin >> fila;
        cout << "Posicion de columna: ";
        cin >> columna;
        tablero.position_obstacle(fila, columna, i);
      }
    } else { // Obstaculos aleatorios
      if (type_obj == 2) {
        srand(time(NULL));
        for (int i = 0; i < n_obstaculos; i++) {
          fila = rand()%filas;
          v_fila.push_back(fila);
          columna = rand()%columnas;
          v_columna.push_back(columna);
          while (!check_random(v_fila, v_columna, fila, columna)) {
            fila = rand()%filas;
            columna = rand()%columnas;
          }
          tablero.position_obstacle(fila, columna, i);
        }
      }
    }
    cout << endl << "1.Manhattan";
    cout << endl << "2.Euclidean" << endl;
    cin >> funcion;

    cout << endl << "1.Mostrar proceso detallado";
    cout << endl << "2.Mostrar solucion" << endl;
    cin >> show_details;
    cout << endl;
    tablero.aStar(show_details, funcion);
  }
  double time = (double(T1-T0)/CLOCKS_PER_SEC);
  cout << "Tiempo de ejecucion: " << time << endl;
}