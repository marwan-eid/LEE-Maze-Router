#pragma once
class Cell // class for the grid cell object
{
private:
	int cost = 1; // cost of the cell (Higher if there is a via)
	char pred; // the direction if the cell through we reached this cell (N , S , W , E , U , D)
	bool reached = true; // we reached this cell and expanded by adding it to the wavefront and removing it or not
	int x, y; // coordinates
	int layer; // layer where the cell resides on
	int pathcost; // sum of all costs upto c
	Cell* predecessor;

public:
	// setter functions
	void set_cost(int x);
	void set_pred(char x);
	void set_reached(bool x);
	void set_x(int x);
	void set_y(int y);
	void set_layer(int layer);
	void set_pathcost(int pathcost);
	// getter function
	int get_x();
	int get_y();
	int get_layer();
	int get_pathcost();
	int get_cost();
	char get_pred();
	bool get_reached();
	Cell* predecessor;

};
