#include "table.h"

table::table() {

}

table::table(int filas, int columnas, int tamaño) {
  filas_ = filas;
  columnas_ = columnas;
  obstacles_.resize(tamaño);
}

void table::position_car(int fila, int columna) {
  car_.push_back(fila);
  car_.push_back(columna);
}

void table::position_destination(int fila, int columna) {
  destination_.push_back(fila);
  destination_.push_back(columna);
}

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

bool table::isClose(vector<node> closeList, node node) {
  for (int j = 0; j < closeList.size(); j++) {
    if ((closeList[j].x == node.x) && (closeList[j].y == node.y)) {
      return true;         
    }
  }
  return false;
}

bool table::isOpen(list<node> openList, node node1) {
  list<node>::iterator it;
  for (it = openList.begin(); it != openList.end(); it++) {
    if ((it->x == node1.x) && (it->y == node1.y)) {
      return true;         
    }
  }
  return false;
}

// Calculo de H
int table::calculeH_manhattan(int fila, int columna) {
  int H = abs(fila - destination_[0]) + abs(columna - destination_[1]);
  return H;
} 

int table::calculeH_euclidean(int fila, int columna) {
  int H = sqrt(((fila - destination_[0]) * 2) + (columna - destination_[1]) * 2);
  return H;
} 

void table::neighboring(vector<node>& vector, node actual) {
  node temp;
  temp.x = -1;
  temp.y = -1;
  if (isValid(actual.x - 1, actual.y)) {
    temp.x = actual.x - 1;
    temp.y = actual.y;
  }
  vector[0] = temp;
  temp.x = -1;
  temp.y = -1;
  if (isValid(actual.x, actual.y - 1)) {
    temp.x = actual.x;
    temp.y = actual.y - 1;
  }
  vector[1] = temp;
  temp.x = -1;
  temp.y = -1;
  if (isValid(actual.x + 1, actual.y)) {
    temp.x = actual.x + 1;
    temp.y = actual.y;
  }
  vector[2] = temp;
  temp.x = -1;
  temp.y = -1;
  if (isValid(actual.x, actual.y + 1)) {
    temp.x = actual.x;
    temp.y = actual.y + 1;
  }
  vector[3] = temp;
}

void table::aStar() {

  vector<node> neighs;
  vector<vector<node> > tablero;
  tablero.resize(filas_);
  for (int i = 0; i < filas_; i++) {
    tablero[i].resize(columnas_);
  }
  for (int i = 0; i < filas_; i++) {
    for (int j = 0; j < columnas_; j++){
      tablero[i][j].x = i;
      tablero[i][j].y = j;
      tablero[i][j].h = 0;
      tablero[i][j].g = FLT_MAX;
      tablero[i][j].f = 0;
      tablero[i][j].parent = NULL;
    }
  }
  vector<node> track;
  list<node> openList;
  vector<node> closeList; // Hacer bool en el node para saber si esta en close
  tablero[car_[0]][car_[1]].h = calculeH_manhattan(car_[0], car_[1]);
  tablero[car_[0]][car_[1]].g = 0;
  tablero[car_[0]][car_[1]].f = 0;

  openList.push_back(tablero[car_[0]][car_[1]]); // Posicion inicial
  bool finish = false;
  node actual;
  list<node>::iterator winner, it;
  
    while (!openList.empty() && !finish) {
      // Calculamos el minimo coste del OPEN
      winner = openList.begin();
      for (it = openList.begin(); it != openList.end(); it++) {
        if (it->f < winner->f)
          winner = it;
      }
      actual = *winner;
      // Comprobamos que no es el destino
      if (isDestination(actual.x, actual.y)){
        node temp = tablero[actual.x][actual.y];
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
        closeList.push_back(tablero[actual.x][actual.y]);
        neighboring(neighs, actual); // Coge bien los vecinos
        for (int i = 0; i < neighs.size(); i++) { // Revisar porque recorre varias veces los mismos vecinos
          if ((neighs[i].x != -1) && (neighs[i].y != -1)) {
            // hacer for para rescorre todo el close y comprobar que no esta
            float tempG = actual.g + 1;
            if (!isClose(closeList, neighs[i])) { // No esta en CLOSE
              if (tempG < tablero[neighs[i].x][neighs[i].y].g) { // Esta en OPEN
                tablero[neighs[i].x][neighs[i].y].g = tempG;
                tablero[neighs[i].x][neighs[i].y].h = calculeH_manhattan(neighs[i].x, neighs[i].y);
                tablero[neighs[i].x][neighs[i].y].f = tablero[neighs[i].x][neighs[i].y].g + tablero[neighs[i].x][neighs[i].y].h;
                tablero[neighs[i].x][neighs[i].y].parent = &tablero[actual.x][actual.y];
              } 
              if (!isOpen(openList, neighs[i])) {
                openList.push_back(tablero[neighs[i].x][neighs[i].y]);
              } else {
                for (it = openList.begin(); it != openList.end(); it++) {
                  if ((it->x == neighs[i].x) && (it->y == neighs[i].y)) {
                    it->g = tablero[neighs[i].x][neighs[i].y].g;
                    it->h = tablero[neighs[i].x][neighs[i].y].h;
                    it->f = tablero[neighs[i].x][neighs[i].y].f;
                  }
                }
              }
            } 
            // Si esta en CLOSE por lo que se continua 
          }
        }
      }
    }

  // Mostrar todos los movimientos en individual
  /* 
  track_.resize(track.size());
  while (!track.empty()) {
    track_.push_back(track[track.size()]);
    car_[0] = track[track.size() - 1].x;
    car_[1] = track[track.size() - 1].y;
    track.pop_back();
    cout << *this;
    cout << endl;
  }
  */
  
  track_ = track;
  car_[0] = track[track.size() - 1].x;
  car_[1] = track[track.size() - 1].y;
  std::cout << *this;
}

ostream& operator <<(ostream& os, table tablero) {
  bool obstacle = false;
  bool track;
  for (int i = 0; i < tablero.filas_; i++) {
    for (int j = 0; j < tablero.columnas_; j++) {
      obstacle = false;
      track = false;
      if ((tablero.car_[0]) == i && (tablero.car_[1] == j)) {
        os << "©";
      } else {
        for (int k = 0; k < tablero.obstacles_.size(); k++) {
          if (tablero.obstacles_[k][0] == i && tablero.obstacles_[k][1] == j) {
            os << "▣";
            obstacle = true;
          }
        }
        if (!obstacle) {
          if (tablero.isDestination(i,j)) {
            os << "D";
          }  
          for (int k = 0; k < tablero.track_.size(); k++) {
            if ((tablero.track_[k].x == i) && (tablero.track_[k].y == j) && !tablero.isDestination(i,j)) { // Poner !track si se quiere mostrar individual
              os << "X";
              track = true;
            }
          }
          if (!track && !tablero.isDestination(i,j))
            os << "·";
        }
      }
    }
    os << endl;
  }
  return os;
}