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
 * table.cc Metodos de la clase table.h
 */

#include "../include/table.h"

// Constructor por parametros
table::table(int filas, int columnas, int tam) {
  filas_ = filas;
  columnas_ = columnas;
  obstacles_.resize(tam);
  n_obstacles_ = 0;
  tablero_.resize(filas_);
  for (int i = 0; i < filas_; i++) {
    tablero_[i].resize(columnas_);
  }
  for (int i = 0; i < filas_; i++) {
    for (int j = 0; j < columnas_; j++){
      tablero_[i][j].x = i;
      tablero_[i][j].y = j;
      tablero_[i][j].h = 0;
      tablero_[i][j].g = FLT_MAX;
      tablero_[i][j].f = 0;
      tablero_[i][j].parent = NULL;
      tablero_[i][j].close = false;
      tablero_[i][j].open = false;
    }
  }
}

// Set de la posicion del coche
void table::position_car(int fila, int columna) {
  car_.push_back(fila);
  car_.push_back(columna);
}

// Set de la posicion destino
void table::position_destination(int fila, int columna) {
  destination_.push_back(fila);
  destination_.push_back(columna);
}

// Set de la posicion del obstaculo
void table::position_obstacle(int fila, int columna, int posicion) {
  obstacles_[posicion].push_back(fila);
  obstacles_[posicion].push_back(columna);
}

// Comprobar que la posicion no es un obstaculo o esta fuera de rango
bool table::isValid(int fila, int columna) {
  for (int i = 0; i < obstacles_.size(); i++) {
    if ((fila == obstacles_[i][0]) && (columna == obstacles_[i][1]))
      return false; 
  }
  if ((fila < 0) || (fila >= filas_) || (columna < 0) || (columna >= columnas_)) 
    return false;
  return true;
}

// Comprobacion si la posicion es el destino
bool table::isDestination(int fila, int columna) {
  if (fila == destination_[0] && columna == destination_[1])
    return true;
  return false;
}

// Comprobacion si la posicion es el coche
bool table::isCar(int fila, int columna) {
  if (fila == car_[0] && columna == car_[1])
    return true;
  return false;
}

// Muestra el camino minimo y el numero de nodos expandidos
void table::details() {
  // Restamos una unidad porque el coste de la primera posicion es 0
  cout << "Camino minimo: " << track_.size() - 1 << endl; 
  cout << "Numero de nodos expandido: " << n_close_ << endl;
}

// Calculo de la funcion manhattan
float table::calculeH_manhattan(int fila, int columna) {
  float H = abs(fila - destination_[0]) + abs(columna - destination_[1]);
  return H;
} 

// Calculo de la funcion euclidea
float table::calculeH_euclidea(int fila, int columna) {
  float H = sqrt(pow((abs(destination_[0] - fila)), 2) + pow((abs(destination_[1] - columna)), 2));
  return H;
} 

// Calculo de los nodos vecinos del nodo actual
void table::neighboring(vector<node*>& vector, node actual) {
  vector[0] = NULL;
  vector[1] = NULL;
  vector[2] = NULL;
  vector[3] = NULL;
  if (isValid(actual.x - 1, actual.y))
    vector[0] = &tablero_[actual.x - 1][actual.y];
  if (isValid(actual.x, actual.y - 1))
    vector[1] = &tablero_[actual.x][actual.y - 1];
  if (isValid(actual.x + 1, actual.y)) 
    vector[2] = &tablero_[actual.x + 1][actual.y];
  if (isValid(actual.x, actual.y + 1))
    vector[3] = &tablero_[actual.x][actual.y + 1];
}

// Algoritmo A*
void table::aStar(int show_details, int funcion) {
  show_details_ = show_details;
  vector<node*> neighs(4);
  vector<node> track;
  list<node*> openList;
  vector<node> closeList;
  bool finish = false;
  node actual;
  list<node*>::iterator winner, it;

  if (funcion == 1) {
    tablero_[car_[0]][car_[1]].h = calculeH_manhattan(car_[0], car_[1]);
  } else {
    tablero_[car_[0]][car_[1]].h = calculeH_euclidea(car_[0], car_[1]);
  }
  tablero_[car_[0]][car_[1]].g = 0;
  tablero_[car_[0]][car_[1]].f = 0;
  openList.push_front(&tablero_[car_[0]][car_[1]]); // Posicion inicial
  tablero_[car_[0]][car_[1]].open = true;
  
    while (!openList.empty() && !finish) {
      // Calculamos el minimo coste del OPEN
      winner = openList.begin();
      for (it = openList.begin(); it != openList.end(); it++) {
        if ((*it)->f < (*winner)->f)
          winner = it;
      }
      actual = **winner;
      // Comprobamos que no es el destino
      if (isDestination(actual.x, actual.y)){
        node temp = tablero_[actual.x][actual.y];
        track.push_back(temp);
        while (temp.parent != NULL) {
          temp = *temp.parent;
          track.push_back(temp);
        }
        finish = true;
        std::cout << "Camino encontrado" << endl;
      } else {
        // eliminar de OPEN al nodo actual y lo metemos en CLOSE
        openList.erase(winner);
        tablero_[actual.x][actual.y].open = false;
        closeList.push_back(tablero_[actual.x][actual.y]);
        tablero_[actual.x][actual.y].close = true;
        neighboring(neighs, actual); 
        for (int i = 0; i < neighs.size(); i++) { // Recorremos los vecino
          if (neighs[i] != NULL) {
            float tempG = tablero_[actual.x][actual.y].g + 1;
            if (!neighs[i]->close) { // Si el vecino NO esta en CLOSE
              if (tempG < neighs[i]->g) { 
                neighs[i]->g = tempG;
                if (funcion == 1) {
                  neighs[i]->h = calculeH_manhattan(neighs[i]->x, neighs[i]->y);
                } else {
                  neighs[i]->h = calculeH_euclidea(neighs[i]->x, neighs[i]->y);
                }
                neighs[i]->f = neighs[i]->g + neighs[i]->h;
                neighs[i]->parent = &tablero_[actual.x][actual.y];
              } 
              if (!neighs[i]->open) { // Si el vecino NO esta en OPEN
                openList.push_front(&tablero_[neighs[i]->x][neighs[i]->y]);
                tablero_[neighs[i]->x][neighs[i]->y].open = true;
              } else { // Si el vecino esta en OPEN
                neighs[i]->g = tempG;
                neighs[i]->f = neighs[i]->g + neighs[i]->h;
              }
            } 
            // Si esta en CLOSE se continua 
          }
        }
      }
    }
  
  if (!finish) {
    cout << "No se ha encontrado el camino" << endl;
  } else {
    if (show_details == 1){
      track_.resize(track.size());
      while (!track.empty()) {  
        // Ponemos al coche en cada uno de las posiciones del camino 
        car_[0] = track[track.size() - 1].x;
        car_[1] = track[track.size() - 1].y;
        track_.push_back(track[track.size()]);
        track.pop_back();
        cout << *this;
        cout << endl;
      }
    } else {
      // Ponemos al coche en el destino
      car_[0] = destination_[0];
      car_[1] = destination_[1];
      track_ = track;
    }
    n_close_ = closeList.size(); // Guardamos el tamaño de closList
  }
}

// Salida del tablero
ostream& operator <<(ostream& os, table tablero) {
  bool obstacle = false;
  bool track;
  for (int i = 0; i < tablero.filas_; i++) {
    for (int j = 0; j < tablero.columnas_; j++) {
      obstacle = false;
      track = false;
      if ((tablero.car_[0]) == i && (tablero.car_[1] == j)) {
        os << RED << " ©" << RESET;
      } else {
        for (int k = 0; k < tablero.obstacles_.size(); k++) {
          if (tablero.obstacles_[k][0] == i && tablero.obstacles_[k][1] == j) {
            os << " ▣";
            obstacle = true;
          }
        }
        if (!obstacle) {
          if (tablero.isDestination(i,j)) {
            os << RED << " D" << RESET;
          }  
          for (int k = 0; k < tablero.track_.size(); k++) {
            if (tablero.show_details_ == 1) {
              if ((tablero.track_[k].x == i) && (tablero.track_[k].y == j) && !tablero.isDestination(i,j) && !track) { // Poner !track si se quiere mostrar individual
                os << GREEN << " X" << RESET;
                track = true;
              }
            } else {
              if ((tablero.track_[k].x == i) && (tablero.track_[k].y == j) && !tablero.isDestination(i,j)) { 
                os << GREEN << " X" << RESET;
                track = true;
              }
            }
          }
          if (!track && !tablero.isDestination(i,j))
            os << " ·";
        }
      }
    }
    os << endl;
  }
  return os;
}