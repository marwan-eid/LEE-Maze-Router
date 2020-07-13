#pragma once
class W_Cell // class for the wavefront cell
{
private:
	int x, y; // coordinates
	int layer; // layer where the cell resides on
	int pathcost; // sum of all costs upto c
	char pred; // N , S , E , W , U , D

public:
	// setter functions
	void set_x(int x);
	void set_y(int y);
	void set_layer(int layer);
	void set_pathcost(int pathcost);
	void set_pred(char pred);
	// getter functions
	int get_x();
	int get_y();
	int get_layer();
	int get__pathcost();
	char get_pred();


};

