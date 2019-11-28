#ifndef __CELL_H__
#define __CELL_H__

#include <iostream>

/**
* \class 	Cell
* \author 	Maxime ALOS Ulrich CAMILLE
* \brief 	Classe représenatnt une cellule
* \date 	28 nov 2019
*/
class Cell
{
	public:
		Cell(int x, int y);
		~Cell();
		void set_neighb(Cell *c);
		void add_neighb(Cell *c);
		void add_neighb(Cell *c1, Cell *c2);
		int m_x;
		int m_y;
		int m_nb_neighb = 0;
		Cell **m_neighb = NULL; //tableau de pointeurs vers les voisins
		Cell **m_backup = NULL; //copie intermédiaire des voisins
		bool m_displayed = false; //flag utile pour l'affichage du labyrinthe
		bool m_pflag = false; //flag pour savoir si la cellule a été traité dans l'algorithme de découverte du chemin
		bool m_saved = false; //flag pour sauvegarder les labyrinthes dans des fichiers
		bool m_corrupted = false; //flag pour charger des labyrinthes depuis des fichiers
};

std::ostream &operator<<(std::ostream &out, const Cell &c); //surcharge le flux d'écriture
std::istream &operator>>(std::istream &in, Cell &c); //surcharge le flux de lecture
bool operator==(const Cell &c1, const Cell &c2); //surcharge l'opération d'égalité entre deux cellules, non utilisée dans le programme

#endif
