#include "cell.h"

Cell::Cell(int x, int y)
{
	m_x = x;
	m_y = y;
}

Cell::~Cell()
{
	delete[] m_neighb;
	delete[] m_backup;
}


void Cell::set_neighb(Cell *c)
{
	delete[] m_backup;
	m_backup = new Cell*[m_nb_neighb];
	if (m_nb_neighb > 0)
	{

		bool is_c_in_neighb = false;
		for (int i=0; i<m_nb_neighb; i++)
		{
			if (m_neighb[i] == c)
			{
				is_c_in_neighb = true;
			}
		}

		if (!is_c_in_neighb)
		{
			//Copie temporairement les voisins de la cellule	
			for (int i=0; i<m_nb_neighb; i++)
			{
				m_backup[i] = m_neighb[i];
			}
			
			//Reconstitue le tableau des cellules voisines avec le nouveau vosin en plus
			m_nb_neighb++;
			delete[] m_neighb;
			m_neighb = new Cell*[m_nb_neighb];
			for (int i=0; i<m_nb_neighb-1; i++)
			{
				m_neighb[i] = m_backup[i];
			}
			m_neighb[m_nb_neighb-1] = c;
		}
	}

	else
	{
		m_nb_neighb++;
		m_neighb = new Cell*[m_nb_neighb];
		m_neighb[m_nb_neighb-1] = c;
	}
}


void Cell::add_neighb(Cell *c)
{
	this->set_neighb(c);
	c->set_neighb(this);
}


void Cell::add_neighb(Cell *c1, Cell *c2)
{
	this->add_neighb(c1);
	this->add_neighb(c2);
}



std::ostream &operator<<(std::ostream &out, const Cell &c)
{
	out << "(" << c.m_x << "," << c.m_y << ")";
	return out;
}

std::istream &operator>>(std::istream &in, Cell &c)
{
	std::string chain;
	in >> chain;

	//Vérifie si la dernière ligne est vide
	if (chain == "")
	{
		c.m_corrupted = true;
		return in;
	}

	chain = chain.substr(1, chain.size()-2);
	c.m_x = std::stoi(chain.substr(0, chain.find(',')));
	c.m_y = std::stoi(chain.substr(chain.find(',')+1, chain.size()));
	return in;
}

bool operator==(const Cell &c1, const Cell &c2)
{
	return (c1.m_x == c2.m_x and c1.m_y == c2.m_y);
}