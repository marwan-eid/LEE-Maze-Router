#include "Cell.h"

void Cell::set_cost(int x)
{
	cost = x; 
}

void Cell::set_pred(char x)
{
	pred = x; 
}
void Cell::set_reached(bool x)
{
	reached = x; 
}
int Cell::get_cost()
{
	return cost;
}
char Cell::get_pred()
{
	return pred;
}
bool Cell::get_reached()
{
	return reached;
}

void Cell::set_x(int x)
{
	this->x = x;
}
void Cell::set_y(int y)
{
	this->y = y;
}
void Cell::set_layer(int layer)
{
	this->layer = layer;
}
void Cell::set_pathcost(int pathcost)
{
	this->pathcost = pathcost;
}
void Cell::set_pred(char pred)
{
	this->pred = pred;
}

int Cell::get_x()
{
	return x;
}
int Cell::get_y()
{
	return y;
}
int Cell::get_layer()
{
	return layer;
}
int Cell::get_pathcost()
{
	return pathcost;
}
char Cell::get_pred()
{
	return pred;
}
