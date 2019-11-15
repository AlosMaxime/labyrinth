#include "cell.h"

Cell::Cell(int x, int y)
{
	m_x = x;
	m_y = y;
}

Cell::~Cell()
{

}

void Cell::add_neighb(Cell *c)
{
	// verifie si c n'est pas déjà dans le tableau des voisins
	bool has_c = false;
	for (int i=0; i<=this->m_nb_neighb; i++)
	{
		if(c==this->m_neighb[i])
		{
			has_c=true;
		}
	}

	//Si c n'est pas déjà un voisin, on l'ajoute et augmente le nombre de voisins par 1
	if (not(has_c))
	{
		m_nb_neighb++;
		c->add_neighb(this);
	}
}

void Cell::add_neighb(Cell *c1, Cell *c2)
{
	this->add_neighb(c1);
	this->add_neighb(c2);
}