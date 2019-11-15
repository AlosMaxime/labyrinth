#ifndef __ROAD_H__
#define __ROAD_H__

#include <cstddef>

class Cell
{
	public:
		Cell(int x, int y);
		~Cell();
		void add_neighb(Cell *c);
		void add_neighb(Cell *c1, Cell *c2);

		int m_x;
		int m_y;
		int m_nb_neighb = 0;
		Cell **m_neighb = NULL;
		bool m_displayed = false;

	private:
};


#endif