#include "W_Cell.h"

void W_Cell::set_x(int x)
{
	this-> x = x; 
}
void W_Cell::set_y(int y) {

	this->y = y;

}
void W_Cell::set_layer(int layer) {
	this->layer = layer;
}
void W_Cell::set_pathcost(int pathcost) {
	this->pathcost = pathcost;
}
void W_Cell::set_pred(char pred) {
	this->pred = pred;
}

int W_Cell::get_x()
{
	return x;
}
int W_Cell::get_y()
{
	return y;
}
int W_Cell::get_layer()
{
	return layer;
}
int W_Cell::get__pathcost() {
	return pathcost;
}
char W_Cell::get_pred() {
	return pred;
}
