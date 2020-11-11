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

int main(){
  table tablero(20,20,5); // pasar el numero de obstaculos
  tablero.position_car(0,0);
  tablero.position_destination(10,10);
  tablero.position_obstacle(0,6,0);
  tablero.position_obstacle(1,5,1);
  tablero.position_obstacle(1,4,2);
  tablero.position_obstacle(1,3,3);
  tablero.position_obstacle(1,6,4);
  //cout << tablero;
  tablero.aStar();
}