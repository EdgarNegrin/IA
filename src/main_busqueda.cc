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
 * main_busqueda.cc Main principal para el calculo del camino minimo
 */

#include "../include/table.h"

int main(int argc, char** argv){
  int filas, columnas, n_obstaculos, type_obj, show_details, funcion, fila, columna, T0, T1;
  vector<pair<int, int> > v_obstacles;
  srand(time(NULL));

  // Si hay dos parametros se extraen los datos desde el fichero
  if (argc == 2){
    ifstream file(argv[1]);
    file >> filas;
    file >> columnas;    
    file >> n_obstaculos;
    file >> type_obj;
    table tablero(filas, columnas, n_obstaculos); 
    // Establecemos la posicion del coche
    file >> fila;
    file >> columna;
    tablero.position_car(fila, columna);
    // Establecemos la posicion de destino
    file >> fila;
    file >> columna;
    tablero.position_destination(fila, columna);

    if (type_obj == 2) { // Obstaculos aleatorios
      pair<int, int> temp;
      int list_obstacle;
      // Guardamos todos los posibles obstaculos
      for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
          if (!tablero.isCar(i, j) && !tablero.isDestination(i, j)){
            temp.first = i;
            temp.second = j;
            v_obstacles.push_back(temp);
          }
        }
      }
      // Extraemos un obstaculo aleatorio y lo introducimos en el tablero
      for (int i = 0; i < n_obstaculos; i++) {
        list_obstacle = rand() % v_obstacles.size();
        temp = v_obstacles[list_obstacle];
        v_obstacles.erase(v_obstacles.begin() + list_obstacle);
        tablero.position_obstacle(temp.first, temp.second, i);
      }
    
    } else { // Obstaculos por posicion
      for (int i = 0; i < n_obstaculos; i++) {
        file >> fila;
        file >> columna;
        tablero.position_obstacle(fila, columna, i);
      }
    }
    file >> funcion;
    file >> show_details;
    if (show_details == 1) { // Mostrar cada cambio de estado
      T0 = clock();
      tablero.aStar(show_details, funcion);
      T1 = clock();
      tablero.details();
    } else { // Muestra el inicio y el fin
      cout << tablero;
      T0 = clock();
      tablero.aStar(show_details, funcion);
      T1 = clock();
      cout << tablero;
      tablero.details();
    }
    file.close();
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
    while (n_obstaculos > (filas * columnas - 2)){
      cout << "Demasiados obstaculos, pruebe otra vez" << endl;
      cin >> n_obstaculos;
    }
    table tablero(filas, columnas, n_obstaculos); // pasar el numero de obstaculos
    // Establecemos la posicion del coche
    cout << endl << "Posicion coche fila: ";
    cin >> fila;
    while (fila >= filas || fila < 0){
      cout << "Fuera de rango, pruebe otra vez" << endl;
      cin >> fila;
    }
    cout << "Posicion coche columna: ";
    cin >> columna;
    while (columna >= columnas || columna < 0){
      cout << "Fuera de rango, pruebe otra vez" << endl;
      cin >> fila;
    }
    tablero.position_car(fila, columna);
    // Establecemos la posicion del destino
    cout << endl << "Posicion destino fila: ";
    cin >> fila;
    while (fila >= filas || fila < 0){
      cout << "Fuera de rango, pruebe otra vez" << endl;
      cin >> fila;
    }
    cout << "Posicion destino columna: ";
    cin >> columna;
    while (columna >= columnas || columna < 0){
      cout << "Fuera de rango, pruebe otra vez" << endl;
      cin >> columna;
    }
    tablero.position_destination(fila, columna);

    if (type_obj == 1) { // Obstaculos por consola
      for (int i = 0; i < n_obstaculos; i++) {
        do {
          cout << endl << "Obstaculo " << i + 1;
          cout << endl << "Posicion de fila: ";
          cin >> fila;
          while (fila >= filas || fila < 0){
            cout << "Fuera de rango, pruebe otra vez" << endl;
            cin >> fila;
          }
          cout << "Posicion de columna: ";
          cin >> columna;
          while (columna >= columnas || columna < 0){
            cout << "Fuera de rango, pruebe otra vez" << endl;
            cin >> columna;
          }
        } while (!tablero.isCar(fila, columna) && !tablero.isDestination(fila,columna));
        tablero.position_obstacle(fila, columna, i);
      }
    } else { // Obstaculos aleatorios
      if (type_obj == 2) {
        pair<int, int> temp;
        int list_obstacle;
        // Guardamos todos los posibles obstaculos
        for (int i = 0; i < filas; i++) {
          for (int j = 0; j < columnas; j++) {
            if (!tablero.isCar(i, j) && !tablero.isDestination(i, j)){
              temp.first = i;
              temp.second = j;
              v_obstacles.push_back(temp);
            }
          }
        }
        // Extraemos un obstaculo aleatorio y lo introducimos en el tablero
        for (int i = 0; i < n_obstaculos; i++) {
          list_obstacle = rand() % v_obstacles.size();
          temp = v_obstacles[list_obstacle];
          v_obstacles.erase(v_obstacles.begin() + list_obstacle);
          tablero.position_obstacle(temp.first, temp.second, i);
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
    if (show_details == 1) { // Mostrar cada cambio de estado
      T0 = clock();
      tablero.aStar(show_details, funcion);
      T1 = clock();
      tablero.details();
    } else { // Muestra el inicio y el fin
      cout << tablero;
      T0 = clock();
      tablero.aStar(show_details, funcion);
      T1 = clock();
      cout << tablero;
      tablero.details();
    }
  }
  double time = (double(T1-T0)/CLOCKS_PER_SEC);
  cout << "Tiempo de ejecucion: " << time << " segundos" << endl;
}