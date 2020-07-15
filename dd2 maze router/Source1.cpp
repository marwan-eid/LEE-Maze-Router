#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <stack>

#include "Cell.h"
using namespace std;

#define bend_cost 10
#define via_cost 20

bool is_dest(Cell c1, Cell c2)
{
	return (c1.get_x() == c2.get_x() && c1.get_y() == c2.get_y() && c1.get_layer() == c2.get_layer());
}
class Comparator
{
public:
	int operator() (Cell& w1, Cell& w2)
	{
		return w1.get_pathcost() > w2.get_pathcost();
	}
};

struct layer{
	Cell grid[1000][1000];
	int index ; // to know the layer's index (1-10)
};

// Pins data
struct pins {
	int layer, x_coordinate, y_coordinate;
	float distance_to_center;
};

float calc_distance_to_center(pins pin)
{
	return sqrt(pow(abs(pin.x_coordinate - 500), 2) + pow(abs(pin.y_coordinate - 500), 2));
}

void read_line(string input_line, vector<pins>& pins_vector)
{
	pins pin;
	int comma_substr, temp_pos, temp;
	do {
		temp_pos = input_line.find("(") + 1;
		comma_substr = input_line.find(",");
		pin.layer = atoi(input_line.substr(temp_pos, comma_substr).c_str());
		input_line = input_line.substr(comma_substr + 2);
		comma_substr = input_line.find(",");
		pin.x_coordinate = atoi(input_line.substr(0, comma_substr).c_str());
		input_line = input_line.substr(comma_substr + 2);
		comma_substr = input_line.find(")");
		pin.y_coordinate = atoi(input_line.substr(0, comma_substr).c_str());
		pin.distance_to_center = calc_distance_to_center(pin);
		pins_vector.push_back(pin);
	} while (input_line.find("(") != -1);
}

bool route(priority_queue<Cell, vector<Cell>, Comparator>& wavefront, vector <pins>& pins_vector, string & path_str)
{
/*
	1 10 20		2 30 50		1 5 100 
	2 100 200	1 300 50
	1 100 50	2 300 150	2 50 50		1 2 2
*/
	layer layer, upper_layer, lower_layer; // assuming odd layers are vertical and even layers are horzintal

	Cell source, target;
	layer.grid[source.get_x()][source.get_y()] = source;
	layer.grid[target.get_x()][target.get_y()] = target;
	Cell c;
	vector<Cell> sources;
	wavefront.push(source);
	stack<Cell> path;
	while (!is_dest(wavefront.top(),target))
	{
		if (wavefront.empty())
		{
			return false; // Quit and no path is found
		}
		c = wavefront.top();
		Cell temp;
		if (is_dest(c, target))
		{
			temp = c;
			while (!is_dest(temp, source))
			{
				path.push(temp);
				temp = *(temp.predecessor);

			}
			path.push(source); // we have path from source to target
			// return path here
			path_str.clear();
			while (!path.empty())
			{
				Cell tmp;
				string substr;
				tmp = path.top();
				path.pop();
				substr = "(" + to_string(tmp.get_layer()) + " " + to_string(tmp.get_x()) + " " + to_string(tmp.get_y()) + ")";
				path_str += substr;


			}
			return; // path got
			
		}
			// set neighbours cells to reached and calculate the costs.
			if (!(c.get_x() - 1 < 0 || c.get_x() + 1 == 1000))
			{
				// set neighbours cells to reached and calculate the costs.
				layer.grid[c.get_x() - 1][c.get_y()].set_reached(true);
				
				layer.grid[c.get_x() - 1][c.get_y()].predecessor = &(c);



				
				layer.grid[c.get_x() + 1][c.get_y()].predecessor = &(c);
				layer.grid[c.get_x() + 1][c.get_y()].set_reached(true);
				if (c.get_layer() % 2 == 0) {
					layer.grid[c.get_x() - 1][c.get_y()].set_cost(layer.grid[c.get_x() - 1][c.get_y()].get_cost() + c.get_cost()+ via_cost + bend_cost); // via + bend cost ( even layer and moving horizontal) 
					layer.grid[c.get_x() + 1][c.get_y()].set_cost(layer.grid[c.get_x() + 1][c.get_y()].get_cost() + c.get_cost() + via_cost + bend_cost);
					layer.grid[c.get_x() - 1][c.get_y()].set_pred('D');
					layer.grid[c.get_x() + 1][c.get_y()].set_pred('D');

				}
				else {
					layer.grid[c.get_x() - 1][c.get_y()].set_cost(layer.grid[c.get_x() - 1][c.get_y()].get_cost() + c.get_cost());
					layer.grid[c.get_x() + 1][c.get_y()].set_cost(layer.grid[c.get_x() + 1][c.get_y()].get_cost() + c.get_cost());
					layer.grid[c.get_x() - 1][c.get_y()].set_pred('E');
					layer.grid[c.get_x() + 1][c.get_y()].set_pred('W');
				}
				
				

			}
			if (!(c.get_y() - 1 < 0 || c.get_y() + 1 == 1000))
			{


				layer.grid[c.get_x()][c.get_y() + 1].set_reached(true);
				layer.grid[c.get_x()][c.get_y() - 1].set_reached(true);
				layer.grid[c.get_x()][c.get_y() + 1].predecessor = &(c);
				layer.grid[c.get_x()][c.get_y() - 1].predecessor = &(c);
				if (source.get_layer() % 2 != 0) {
					layer.grid[c.get_x()][c.get_y() + 1].set_cost(layer.grid[c.get_x()][c.get_y() + 1].get_cost() + c.get_cost() + via_cost + bend_cost); //  via + bend cost ( odd layer and moving vertical ) + Routing between a horizontal lower and a vertical upper only.
					layer.grid[c.get_x()][c.get_y() - 1].set_cost(layer.grid[c.get_x()][c.get_y() - 1].get_cost() + c.get_cost() + via_cost + bend_cost);
					layer.grid[c.get_x()][c.get_y() + 1].set_pred('U'); // Do we need to change or indicate that the layer changed in code?
					layer.grid[c.get_x()][c.get_y() - 1].set_pred('U');
				}
				else {
					layer.grid[c.get_x()][c.get_y() + 1].set_cost(layer.grid[c.get_x()][c.get_y() + 1].get_cost() + c.get_cost());
					layer.grid[c.get_x()][c.get_y() - 1].set_cost(layer.grid[c.get_x()][c.get_y() - 1].get_cost() + c.get_cost());
					layer.grid[c.get_x()][c.get_y() + 1].set_pred('N');
					layer.grid[c.get_x()][c.get_y() - 1].set_pred('S');
				}
			}


				wavefront.pop();
				wavefront.push(layer.grid[c.get_x()][c.get_y() + 1]);
				wavefront.push(layer.grid[c.get_x()][c.get_y() - 1]);
				wavefront.push(layer.grid[c.get_x() - 1][c.get_y()]);
				wavefront.push(layer.grid[c.get_x() + 1][c.get_y()]);
			
			
			//processing upper and lower vias
			

		}

	}
			


		
		
		
	/*while (pins_vector.size() > 0)
	{
		//source.push_back(pins_vector[0]);
	}*/
	// source = Cell[1][10][20]
	//	pins source = pins_vector[0], targ;


int main()
{
//	int no_layers;
//	cin >> no_layers;
	ifstream input_file;
	ofstream output_file;
	vector <pins> pins_vector;
	string input_line;
	vector <layer> layers(2);
	priority_queue<Cell, vector<Cell>, Comparator> pq;
	input_file.open("input_file.txt");
	output_file.open("output_file.txt", ios::out | ios::trunc);
	if (!input_file.is_open())
	{
		cout << "Unable to open input file\n";
		return 0;
	}
	while (!input_file.eof())
	{
		getline(input_file, input_line);
		// Reading input file
		read_line(input_line, pins_vector);
		// Sorting nearest to boundary for starting point selection technique
		sort(pins_vector.begin(), pins_vector.end(), [](const pins& lhs, const pins& rhs) {
			return lhs.distance_to_center > rhs.distance_to_center;
			});
		// CORE CODE HERE ..................... (for each line, u have some pins)
		bool routed;
		routed = route(pq, pins_vector);
		pins_vector.clear();
	}
	input_file.close();
	output_file.close();
	return 0;
}
// not needed - delete below

/*		for (int i = 0; i < pins_vector.size(); i++)
			cout << pins_vector[i].layer << " " << pins_vector[i].x_coordinate << " " << pins_vector[i].y_coordinate << " " << pins_vector[i].distance_to_center << "\t";
		cout << "\n";
*/

/*

	ifstream pins_infile;
	pins_infile.open("pins.txt");

	// CORE CODE HERE

	while (!pins_infile.eof())
	{

		getline(pins_infile, input_line);
		std::istringstream row(input_line);
		int number;
		while (row >> number)
			std::cout << number << '\t';
		std::cout << '\n';


		pins_infile >> pin.layer >> pin.x_coordinate >> pin.y_coordinate;
		pin.distance_to_center = calc_distance_to_center(pin);
		pins_vector.push_back(pin);
		// .... read other pins in the same line ...

	}


*/

/*

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define bend_cost 10
#define via_cost 20

struct pins {
	int layer, x_coordinate, y_coordinate;
};

struct grid_cell {
	;
};

int main()
{
	ifstream input_file;
	ofstream output_file;
	string input_line;
	int pin_substr, comma_substr, temp_pos;
	pins pin;
	vector <pins> pins;
	input_file.open("input_file.txt");
	output_file.open("pins.txt", ios::out | ios::trunc);
	if (!input_file.is_open())
	{
		cout << "Unable to open input file\n";
		return 0;
	}
	while (!input_file.eof())
	{
		getline(input_file, input_line);
		do {
			temp_pos = input_line.find("(") + 1;
			comma_substr = input_line.find(",");
			pin.layer = atoi(input_line.substr(temp_pos, comma_substr).c_str());
			input_line = input_line.substr(comma_substr + 2);
			comma_substr = input_line.find(",");
			pin.x_coordinate = atoi(input_line.substr(0, comma_substr).c_str());
			input_line = input_line.substr(comma_substr + 2);
			comma_substr = input_line.find(")");
			pin.y_coordinate = atoi(input_line.substr(0, comma_substr).c_str());
			pins.push_back(pin);
		} while (input_line.find("(") != -1);
		int no_pins = pins.size();
		for (int i = 0; i < no_pins; i++)
			output_file << pins[i].layer << " " << pins[i].x_coordinate << " " << pins[i].y_coordinate << " ";
		output_file << "\n";
		pins.clear();
	}
	input_file.close();
	output_file.close();
	ifstream input_file;
	input_file.open("pins.txt");

	// CORE CODE HERE

	while (!input_file.eof())
	{

	}
	return 0;
}

*/
