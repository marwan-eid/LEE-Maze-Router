#include "G_Cell.h"

void G_Cell::set_cost(int x)
{
	cost = x; 
}

void G_Cell::set_pred(char x)
{
	pred = x; 
}
void G_Cell::set_reached(bool x)
{
	reached = x; 
}
int G_Cell::get_cost() {
	return cost;
}
char G_Cell::get_pred()
{
	return pred;
}
bool G_Cell::get_reached()
{
	return reached;
}
