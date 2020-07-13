#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

#include "G_Cell.h"
#include "W_Cell.h"
using namespace std;

#define bend_cost 10
#define via_cost 20

struct pins {
	int layer, x_coordinate, y_coordinate;
	float distance_to_center;
};

float calc_distance_to_center(pins pin)
{
	return sqrt(pow(abs(pin.x_coordinate - 500), 2) + pow(abs(pin.y_coordinate - 500), 2));
}

int main()
{
	ifstream input_file;
	ofstream output_file;
	string input_line;
	int comma_substr, temp_pos, temp;
	pins pin;
	vector <pins> pins_vector;

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
		// reading input file
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
		// sorting nearest to boundary for starting point selection technique
		sort(pins_vector.begin(), pins_vector.end(), [](const pins& lhs, const pins& rhs) {
			return lhs.distance_to_center > rhs.distance_to_center;
			});
		for (int i = 0; i < pins_vector.size(); i++)
			cout << pins_vector[i].layer << " " << pins_vector[i].x_coordinate << " " << pins_vector[i].y_coordinate << " " << pins_vector[i].distance_to_center << "\t";
		cout << "\n";
		pins_vector.clear();

		// CORE CODE HERE ..................... (for each line, u have some pins)

	}
	input_file.close();
	output_file.close();
	return 0;
}

// not needed - delete below
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
