#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <vector>

#include "Cell.h"
using namespace std;

#define bend_cost 10
#define via_cost 20

class Comparator
{
public:
	int operator() (Cell& w1, Cell& w2)
	{
		return w1.get_pathcost() > w2.get_pathcost();
	}
};

struct two_dim_grid{
	Cell layer_grid[1000][1000];
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

void route(priority_queue<Cell, vector<Cell>, Comparator>& wavefront, vector <pins>& pins_vector)
{
/*
	1 10 20		2 30 50		1 5 100 
	2 100 200	1 300 50
	1 100 50	2 300 150	2 50 50		1 2 2
*/
	Cell source, target;
	vector<Cell> sources;
	wavefront.push(source);

	while (wavefront.top() != target)
	{

	}


		
		
		
	while (pins_vector.size() > 0)
	{
		//source.push_back(pins_vector[0]);
	}
	// source = Cell[1][10][20]
	//	pins source = pins_vector[0], targ;
}

int main()
{
//	int no_layers;
//	cin >> no_layers;
	ifstream input_file;
	ofstream output_file;
	vector <pins> pins_vector;
	string input_line;
	vector <two_dim_grid> layers(2);
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
		route(pins_vector);
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
