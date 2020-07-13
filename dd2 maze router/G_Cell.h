#pragma once
class G_Cell // class for the grid cell object
{

private:
	int cost = 1; // cost of the cell (Higher if there is a via)
	char pred; // the direction if the cell through we reached this cell (N , S , W , E , U , D)
	bool reached = true; // we reached this cell and expanded by adding it to the wavefront and removing it or not
public:
	void set_cost(int x);
	void set_pred(char x);
	void set_reached(bool x);
	int get_cost();
	char get_pred();
	bool get_reached();

};

