#ifndef __PATH_H__
#define __PATH_H__

#include "cell.h"
#include "vibes.h"

/**
* \class 	Path
* \author 	Maxime ALOS Ulrich CAMILLE
* \brief 	Classe représentant un chemin permettant de traverser le labyrinthe
* \date 	28 nov 2019
*/
class Path
{
	public:
		Path(const Cell *c);
		~Path();
		void add_to_path(const Cell *c);
		void display_path();
		const Cell *m_c;
		Path *m_next = NULL;
};

#endif