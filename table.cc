#include "table.h"

table::table() {

}

table::table(int filas, int columnas, int tamaño) {
  filas_ = filas;
  columnas_ = columnas;
  obstacles_.resize(tamaño);
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
    }
  }
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

bool table::isOpen(list<node*> openList, node node1) {
  list<node*>::iterator it;
  /* 
  it = find(openList.begin(), openList.end(), node1);
  if (it != openList.end())
    return true;
  return false;
  */
  for (it = openList.begin(); it != openList.end(); it++) {
    if (((*it)->x == node1.x) && ((*it)->y == node1.y)) {
      return true;         
    }
  }
  return false;
  
}

// Calculo de H
float table::calculeH_manhattan(int fila, int columna) {
  float H = abs(fila - destination_[0]) + abs(columna - destination_[1]);
  return H;
} 

float table::calculeH_euclidean(int fila, int columna) {
  float H = sqrt(pow((abs(destination_[0] - fila)), 2) + pow((abs(destination_[1] - columna)), 2));
  return H;
} 

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

void table::aStar(int show_details, int funcion) {
  show_details_ = show_details;
  vector<node*> neighs(4);
  vector<node> track;
  list<node*> openList;
  vector<node> closeList; // Hacer bool en el node para saber si esta en close
  if (funcion == 1) {
    //tablero_[car_[0]][car_[1]].h = calculeH_manhattan(car_[0], car_[1]);
  } else {
    tablero_[car_[0]][car_[1]].h = calculeH_euclidean(car_[0], car_[1]);
  }
  tablero_[car_[0]][car_[1]].g = 0;
  tablero_[car_[0]][car_[1]].f = 0;
  openList.push_back(&tablero_[car_[0]][car_[1]]); // Posicion inicial
  bool finish = false;
  node actual;
  list<node*>::iterator winner, it;
  
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
        closeList.push_back(tablero_[actual.x][actual.y]);
        neighboring(neighs, actual); // Coge bien los vecinos
        for (int i = 0; i < neighs.size(); i++) { // Revisar porque recorre varias veces los mismos vecinos
          if (neighs[i] != NULL) {
            // hacer for para rescorre todo el close y comprobar que no esta
            float tempG = tablero_[actual.x][actual.y].g + 1;
            if (!isClose(closeList, *neighs[i])) { // No esta en CLOSE
              if (tempG < neighs[i]->g) { // Esta en OPEN
                neighs[i]->g = tempG;
                if (funcion == 1) {
                  //neighs[i]->h = calculeH_manhattan(neighs[i]->x, neighs[i]->y);
                } else {
                  neighs[i]->h = calculeH_euclidean(neighs[i]->x, neighs[i]->y);
                }
                neighs[i]->f = neighs[i]->g + neighs[i]->h;
                neighs[i]->parent = &tablero_[actual.x][actual.y];
              } 
              if (!isOpen(openList, *neighs[i])) {
                openList.push_back(&tablero_[neighs[i]->x][neighs[i]->y]);
              } else {
                neighs[i]->g = tempG;
                neighs[i]->f = neighs[i]->g + neighs[i]->h;
              }
            } 
            // Si esta en CLOSE por lo que se continua 
          }
        }
      }
    }
  
  if (!finish) {
    cout << "No se ha encontrado el camino";
  } else {
    if (show_details == 1){
      track_.resize(track.size());
      while (!track.empty()) {    
        car_[0] = track[track.size() - 1].x;
        car_[1] = track[track.size() - 1].y;
        track_.push_back(track[track.size()]);
        track.pop_back();
        cout << *this;
        cout << endl;
      }
    } else {
      car_[0] = destination_[0];
      car_[1] = destination_[1];
      track_ = track;
      cout << *this;
      cout << endl;
    }
  }
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
            if (tablero.show_details_ == 1) {
              if ((tablero.track_[k].x == i) && (tablero.track_[k].y == j) && !tablero.isDestination(i,j) && !track) { // Poner !track si se quiere mostrar individual
                os << "X";
                track = true;
              }
            } else {
              if ((tablero.track_[k].x == i) && (tablero.track_[k].y == j) && !tablero.isDestination(i,j)) { // Poner !track si se quiere mostrar individual
                os << "X";
                track = true;
              }
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