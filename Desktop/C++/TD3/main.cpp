#include "cell.h"
#include "vibes.h"
#include "path.h"
#include <fstream>
using namespace std;
 
struct Maze
{
	Cell *c0 = NULL;
	Cell *cf = NULL;
};

/**
* \brief 	Affichage d'un mur entre deux cellules non voisines
* \param 	*n1	La première cellule
* \param  	*n2 La seconde cellule
* \return 	Rien
*/
void draw_gate(const Cell *n1, const Cell *n2)
{
	vibes::drawBox(
		min(n1->m_x, n2->m_x) + 0.1, max(n1->m_x, n2->m_x) + 0.9,
		min(n1->m_y, n2->m_y) + 0.1, max(n1->m_y, n2->m_y) + 0.9,
		"lightGray[lightGray]");
}


/**
* \brief 	Affichage récursif d'une cellule et de ses cellules voisines
* \param 	*cell	La cellule à afficher
* \return 	Rien
*/
void display_cell(Cell *cell)
{
	vibes::drawBox(cell->m_x, cell->m_x + 1, cell->m_y, cell->m_y + 1, "[lightGray]");
	
	cell->m_displayed = true;
	
	for(int i=0; i<cell->m_nb_neighb; i++)
	{
		if(!cell->m_neighb[i]->m_displayed)
			display_cell(cell->m_neighb[i]);
		draw_gate(cell, cell->m_neighb[i]);			
	}
}

/**
* \brief 	Affiche un labyrinthe par affichage récursif de ses cellules
* \details	La fonction met également en évidence les cellules de départ et d'arrivée pour un meilleur visuel
* \param 	*n1	La première cellule
* \param  	maze 	Le labyrinthe à afficher 
* \return 	Ne retourne rien
*/
void display(Maze maze)
{
	//Affichage le labyrinthe par récursivité sur la cellule initiale
	Cell* c0 = maze.c0;
	display_cell(c0);


	//Affichage de la cellule initiale en vert
	vibes::drawBox(
		min(c0->m_x, c0->m_x) + 0.1, max(c0->m_x, c0->m_x) + 0.9,
		min(c0->m_y, c0->m_y) + 0.1, max(c0->m_y, c0->m_y) + 0.9,
		"Green[Green]");

	//Affichage de la cellule d'arrivée en rouge
	Cell* cf = maze.cf;
	vibes::drawBox(
		min(cf->m_x, cf->m_x) + 0.1, max(cf->m_x, cf->m_x) + 0.9,
		min(cf->m_y, cf->m_y) + 0.1, max(cf->m_y, cf->m_y) + 0.9,
		"Red[Red]");
}

/**
* \brief 	Crée un labyrinthe en initialisant toutes les cellules et leurs voisines
* \return 	maze 	Le labyrinthe créé
*/
Maze create_maze()
{
	//Création des cellules
	Cell *c00 = new Cell(0,0);
	Cell *c10 = new Cell(1,0);
	Cell *c20 = new Cell(2,0);
	Cell *c30 = new Cell(3,0);

	Cell *c01 = new Cell(0,1);
	Cell *c11 = new Cell(1,1);
	Cell *c21 = new Cell(2,1);
	Cell *c31 = new Cell(3,1);

	Cell *c02 = new Cell(0,2);
	Cell *c12 = new Cell(1,2);
	Cell *c22 = new Cell(2,2);
	Cell *c32 = new Cell(3,2);

	Cell *c03 = new Cell(0,3);
	Cell *c13 = new Cell(1,3);
	Cell *c23 = new Cell(2,3);
	Cell *c33 = new Cell(3,3);

	//Définition des voisins de chaque cellule
	c00->add_neighb(c10);
	c10->add_neighb(c20);
	c20->add_neighb(c30);
	c30->add_neighb(c31);
	c31->add_neighb(c32);
	c32->add_neighb(c33);
	c33->add_neighb(c23);
	c23->add_neighb(c22);
	c22->add_neighb(c12);
	c12->add_neighb(c11, c13);
	c11->add_neighb(c21);
	c13->add_neighb(c03);
	c03->add_neighb(c02);
	c02->add_neighb(c01);
	
	//Création du labyrinthe
	Maze maze;
	maze.c0 = c00;
	maze.cf = c01;
	return maze;
}


bool find_path(Cell *c, Cell *cf, Path *path, bool &over)
{	
	c->m_pflag = true; 

	if (c == cf)
	{
		over = true;
		return true;
	}

	for(int i=0; i<c->m_nb_neighb; i++) 
	{
		if (!c->m_neighb[i]->m_pflag)
		{
			if (over == false)
			{
				path->add_to_path(c->m_neighb[i]);
				find_path(c->m_neighb[i], cf, path->m_next, over);
			}
			else
			{
				return true;
			}
		}
	}
}


void save_cells(Cell *cell, ofstream *f)
{
	*f << *cell << " " << cell->m_nb_neighb;
	cell->m_saved = true;

	for(int i=0; i<cell->m_nb_neighb; i++)
	{
		*f << " " << *(cell->m_neighb[i]);
	}
	
	for(int j=0; j<cell->m_nb_neighb; j++)
	{
		if(!cell->m_neighb[j]->m_saved)
		{
			*f << endl;
			save_cells(cell->m_neighb[j], f);
		}
	}
}


void save_maze(const Maze& maze, const string& file_name)
{
	ofstream f(file_name);
	if(!f.is_open())
	{
		cout << "Erreur d'ouverture du fichier : " << file_name << endl;
		return;
	}

	Cell *c0 = maze.c0;
	f << "# Start :" << endl << *c0 << endl;
	f << "# End:" << endl << *(maze.cf) << endl << "# Cells:" << endl; 
	save_cells(c0, &f);

	f.close();
}

pair<int,int> convert_coords(string coords)
{
	coords = coords.substr(1, coords.size()-2);
	int x = stoi(coords.substr(0, coords.find(',')));
	int y = stoi(coords.substr(coords.find(',')+1, coords.size()));
	return pair<int,int> (x, y);
}

Maze read_maze(const string& file_name)
{
	ifstream f(file_name);
	if(!f.is_open())
	{
		cout << "Erreur d'ouverture du fichier : " << file_name << endl;
		Maze maze;
		return maze;
	}

	string line = ""; //buffer de lecture, il sert de poubelle pour passer les lignes qui ne nous intéressent pas
	string startCoords = "";
	string stopCoords = "";

	getline(f, line);
	getline(f, startCoords); //récupère les coordonnées de départ
	getline(f, line);
	getline(f, stopCoords); //récupère les coordonnées d'arrivée
	getline(f, line);

	pair<int,int> c0Coords = convert_coords(startCoords); //coordonnées de départ
	pair<int,int> cfCoords = convert_coords(stopCoords); //coordonnées d'arrivée

	map<pair<int,int>, Cell*> cells; //dictionnaire des cellules, les clés étant les coordonnées

	while(!f.eof()) //tant qu'on arrive pas à la fin du fichier
	{
		Cell *cell = new Cell(0,0);
		int nb_neighb;
		f >> *cell >> nb_neighb; //lecture de la cellule courante et de son nombre de voisins
		if(cell->m_corrupted) //flag qui vérifie si la cellule doit être ignorée ou non, cf la surcharge >> de la classe Cell 
		{
			delete cell;
			break;
		}

		//Traitement de la cellule courante

		if(cells.find(pair<int,int> (cell->m_x, cell->m_y)) == cells.end()) //si la clé n'existe pas
		{
			cells[pair<int,int> (cell->m_x, cell->m_y)] = cell; //elle est crée
		}
		else //si elle existe déjà
		{
			int x = cell->m_x;
			int y = cell->m_y;
			delete cell;
			cell = cells.find(pair<int,int> (x, y))->second; //on utilise la cellule déjà créée pour la suite du traitements
		}

		//Traitement des voisins

		for (int i=0; i<nb_neighb; i++)
		{
			Cell *neighb = new Cell(0,0);
			f >> *neighb;
			if(cells.find(pair<int,int> (neighb->m_x, neighb->m_y)) == cells.end()) //si le voisin n'existe pas déjà dans le dictionnaire
			{
				cell->add_neighb(neighb);
				cells[pair<int,int> (neighb->m_x, neighb->m_y)] = neighb; //on l'ajoute au dictionnaire
			}
			else //s'il existe déjà
			{
				cell->add_neighb(cells.find(pair<int,int> (neighb->m_x, neighb->m_y))->second);
				delete neighb;
			}
		}
		getline(f, line); //permet de passer à la ligne suivante
	}

	Maze maze; //création du labyrinthe
	maze.c0 = cells.find(c0Coords)->second; //définition de la cellule de départ, ses coordonnées étant connues
	maze.cf = cells.find(cfCoords)->second; //définition de la cellule d'arrivée, ses coordonnées étant connues

	return maze;
}

// ------------------------------------------------------------------------------------------- //

int main()
{
	vibes::beginDrawing(); // initialisation de VIBes
	vibes::newFigure("Maze"); // création d’une figure
	vibes::axisLimits(-1., 9., -1., 9.); // dimensions de la vue graphique
	vibes::setFigureProperties(
		"Maze", 
		vibesParams("x", 100, "y", 100, "width", 400, "height", 400)); // propriétés de la figure


	string filepath = "maze3.txt";
	Maze maze = read_maze(filepath); 
	display(maze); //affichage du labyrinthe

	//Recherche et affichage d'un chemin pour résoudre le labyrinthe
	Path* path = new Path(maze.c0);
	bool over = false;
	find_path(maze.c0, maze.cf, path, over);
	path->display_path();

	delete path;

	return EXIT_SUCCESS;
}