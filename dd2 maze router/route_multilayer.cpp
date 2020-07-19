#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
#include <sstream>
#include <stack>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

#define bend_cost 250
#define via_cost 25

#define gCell_pred_bits 0xFF000000
#define gCell_obstacle_bit 0x00800000
#define gCell_reached_bit 0x00400000
#define gCell_layer_bits 0x003C0000
#define gCell_cost_bits 0x0003FFFF
#define wfCell_x_bits 0xFFC0000000000000
#define wfCell_y_bits 0x003FF00000000000
#define wfCell_layer_bits 0x00000F0000000000
#define wfCell_pred_bits 0x000000FF00000000
#define wfCell_cost_bits 0x00000000FFFFFFFF

// Pins input data
struct pins {
	int layer, x_coordinate, y_coordinate;
	int distance_to_center;
};

// Layers structure -> a vector of 2d arrays (2d grids of size 1000x1000)
struct layer {
	int grid[1000][1000];
};
vector<layer> layers_vector(1);
int no_layers;

// A comparator operator (based on the cells' cost) used for the wavefront priority queue
class Comparator
{
public:
	long long operator() (long long& w1, long long& w2)
	{
		long long cost1 = w1 & (wfCell_cost_bits);
		long long cost2 = w2 & (wfCell_cost_bits);
		return cost1 > cost2;
	}
};

// A function used to find the nearest pin to center to choose the furthest from it first to route (first point selection)
int calc_distance_to_center(pins pin)
{
	return (int)sqrt(pow(abs(pin.x_coordinate - 500), 2) + pow(abs(pin.y_coordinate - 500), 2));
}

// A function to parse the input line and store the pins data in a vcetor of pins
void read_line(string input_line, vector<pins>& pins_vector)
{
	pins pin;
	size_t comma_substr, temp_pos;
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

/*
A heuristic function to calculate an estimated cost between the expanded cell's neighbor and the target cell.
An odd neighbor layer (M1, M3) lessens the cost of horizontal movement and increases the cost of vertical movement,
and vice versa for even neighbor layers (M2, M4).
This prioritizes the usage of via (for correct positional movement) over routing vertically on odd layers or horizontally on even layers.
*/
int heuristic_cost(const int& neighbor_x, const int& neighbor_y, const int& neighbor_layer, const long long& target)
{
	int delta_x = abs(int((neighbor_x)-((target & wfCell_x_bits) >> 54)));
	int delta_y = abs(int((neighbor_y)-((target & wfCell_y_bits) >> 44)));
	int delta_layer = abs(int((neighbor_layer)-((target & wfCell_layer_bits) >> 40)));
	if (neighbor_layer % 2)
	{
		delta_x /= 2;
		delta_y *= 2;
	}
	if (!(neighbor_layer % 2))
	{
		delta_x *= 2;
		delta_y /= 2;
	}
	return max(delta_layer, max(delta_x, delta_y));
}

// A function to change the coordinates of a cell based on the direction of its precedent cell
void predec_location(int& l, int& x, int& y, const char& direction, bool& source)
{
	source = false;
	switch (direction)
	{
	case 'N':
		y--;
		break;
	case 'S':
		y++;
		break;
	case 'W':
		x--;
		break;
	case 'E':
		x++;
		break;
	case 'U':
		l--;
		break;
	case 'D':
		l++;
		break;
	default:
		source = true;
		break;
	}
}

/*
A function to convert a grid cell (32 bits) to a wavefront cell (64 bits).
The function sets the x, y, and layer values based on the coordinates of the input cell, and sets the precedent bits and cost bits.
*/
long long convert_gridCell_to_wfCell(const int& gCell, const int& x, const int& y, const int& layer)
{
	long long wfCell = 0;
	wfCell = (((long long(x) << 54)) | ((long long(y) << 44)) | ((long long(layer)) << 40)
		| ((long long(gCell) & gCell_pred_bits) << 8) | (long long(gCell) & gCell_cost_bits));
	return wfCell;
}

// A function that initializes each layer (obstacle, layer, and cost bits) before each routing call
void initialize_layer(int layer_index)
{
	for (int i = 0; i < 1000; i++)
		for (int j = 0; j < 1000; j++)
		{
			(layers_vector[layer_index].grid)[i][j] &= gCell_obstacle_bit;
			(layers_vector[layer_index].grid)[i][j] |= (layer_index << 18);
			(layers_vector[layer_index].grid)[i][j] |= 1;
			//(layers_vector[layer_index].grid)[i][j] &= ~(gCell_reached_bit);
			//if ((layers_vector[layer_index].grid)[i][j] & gCell_reached_bit)
			//if ((layers_vector[layer_index].grid)[i][j] & gCell_reached_bit)
			//	(layers_vector[layer_index].grid)[i][j] &= ~(1UL << 22);
		}
}

// A function to mark all the input pins as obstacles such that no pin is used to route a former net which is not connected to it
void mark_pins_obstacles(const vector<pins>& pins_vector)
{
	size_t no_pins = pins_vector.size();
	for (int i = 0; i < no_pins; i++)
		(layers_vector[pins_vector[i].layer].grid)[pins_vector[i].x_coordinate][pins_vector[i].y_coordinate] |= gCell_obstacle_bit;
}

/*
The main function used to route between two pins and find theoptimal cell to be chosen as
the source pin for the next target pin (if any). The function returns the path between the two pins if possible
and sets the used cells in the path to be obstacles for the future routes.
The usage of high bend cost and slightly high via cost priotirizes the correct positional movement on layers,
followed by the usage of vias to ensure correct positional movoment on layers, and lastly by incorrect positional
movement on layers either if that is the only way or if the cost for doing so based on the chosen parameters is less than that of other options.
*/
bool route_source_target_find_next_source(long long& source, long long& target, const long long& next_target, bool is_next_targ, string& path_str)
{
	priority_queue<long long, vector<long long>, Comparator> wavefront;
	stack<string> path;
	int expanded_cells = 0;
	long long next_source_cell = 0;
	int min_distance_next_targ = INT_MAX, temp_distance;
	int source_x = (source & (wfCell_x_bits)) >> 54;
	int source_y = (source & (wfCell_y_bits)) >> 44;
	int source_layer = (source & (wfCell_layer_bits)) >> 40;
	int target_x = (target & (wfCell_x_bits)) >> 54;
	int target_y = (target & (wfCell_y_bits)) >> 44;
	int target_layer = (target & (wfCell_layer_bits)) >> 40;
	int next_target_x = (next_target & (wfCell_x_bits)) >> 54;
	int next_target_y = (next_target & (wfCell_y_bits)) >> 44;
	int next_target_layer = (next_target & (wfCell_layer_bits)) >> 40;
	if ((layers_vector[source_layer].grid)[source_x][source_y] & gCell_obstacle_bit)
		(layers_vector[source_layer].grid)[source_x][source_y] &= ~(1UL << 23);
	if ((layers_vector[target_layer].grid)[target_x][target_y] & gCell_obstacle_bit)
		(layers_vector[target_layer].grid)[target_x][target_y] &= ~(1UL << 23);
	int x, y, l;
	char dir;
	bool s;
	wavefront.push(source);
	long long wf_top;
	while (((layers_vector[target_layer].grid)[target_x][target_y] & gCell_reached_bit) != 1)
	{
		if (wavefront.empty())
			return false;
		else
		{
			wf_top = wavefront.top();
			x = (wf_top & (wfCell_x_bits)) >> 54;
			y = (wf_top & (wfCell_y_bits)) >> 44;
			l = (wf_top & (wfCell_layer_bits)) >> 40;
			dir = (wf_top & wfCell_pred_bits) >> 32;
			if ((wf_top & (wfCell_x_bits | wfCell_y_bits | wfCell_layer_bits)) == (target & (wfCell_x_bits | wfCell_y_bits | wfCell_layer_bits)))
			{
				while ((source & (wfCell_x_bits | wfCell_y_bits | wfCell_layer_bits))
					!= (convert_gridCell_to_wfCell((layers_vector[l].grid)[x][y], x, y, l) & (wfCell_x_bits | wfCell_y_bits | wfCell_layer_bits)))
				{
					if (is_next_targ)
					{
						temp_distance = max(abs(x - next_target_x), abs(y - next_target_y));
						if (temp_distance < min_distance_next_targ)
						{
							min_distance_next_targ = temp_distance;
							next_source_cell = 0;
							next_source_cell |= ((wfCell_x_bits) & (long long(x) << 54));
							next_source_cell |= ((wfCell_y_bits) & (long long(y) << 44));
						}
					}
					string substr = "(" + to_string(l) + ", " + to_string(x) + ", " + to_string(y) + ") ";
					path.push(substr);
					(layers_vector[l].grid)[x][y] |= (gCell_obstacle_bit);
					char dir_old = ((layers_vector[l].grid)[x][y] & gCell_pred_bits) >> 24;
					predec_location(l, x, y, dir_old, s);
					dir = ((layers_vector[l].grid)[x][y] & gCell_pred_bits) >> 24;
					if (s)
						break;
				}
				if (is_next_targ)
				{
					string substr = "(" + to_string(source_layer) + ", " + to_string(source_x) + ", " + to_string(source_y) + ") ";
					path.push(substr);
					next_source_cell |= ((wfCell_layer_bits) & (long long(l) << 40));
					next_source_cell |= 1;
					source = next_source_cell;
					//source = target;
					target = next_target;
					while (path.size() > 0)
					{
						path_str += path.top();
						path.pop();
					}
				}
				else
				{
					string substr = "(" + to_string(source_layer) + ", " + to_string(source_x) + ", " + to_string(source_y) + ") ";
					path.push(substr);
					while (path.size() > 0)
					{
						path_str += path.top();
						path.pop();
					}
				}
				return true;
			}
			if (x > 0 && !((layers_vector[l].grid)[x - 1][y] & gCell_reached_bit) && !((layers_vector[l].grid)[x - 1][y] & gCell_obstacle_bit))
			{
				(layers_vector[l].grid)[x - 1][y] |= gCell_reached_bit;
				(layers_vector[l].grid)[x - 1][y] += ((layers_vector[l].grid)[x][y] & gCell_cost_bits) + heuristic_cost(x - 1, y, l, target) + bend_cost;
				if (!(l % 2))
					(layers_vector[l].grid)[x - 1][y] += bend_cost;
				(layers_vector[l].grid)[x - 1][y] |= (long long('E') << 24);
				wavefront.push(convert_gridCell_to_wfCell((layers_vector[l].grid)[x - 1][y], x - 1, y, l));
			}
			if (y > 0 && !((layers_vector[l].grid)[x][y - 1] & gCell_reached_bit) && !((layers_vector[l].grid)[x][y - 1] & gCell_obstacle_bit))
			{
				(layers_vector[l].grid)[x][y - 1] |= gCell_reached_bit;
				(layers_vector[l].grid)[x][y - 1] += ((layers_vector[l].grid)[x][y] & gCell_cost_bits) + heuristic_cost(x, y - 1, l, target);
				if (l % 2)
					(layers_vector[l].grid)[x][y - 1] += bend_cost;
				(layers_vector[l].grid)[x][y - 1] |= (long long('S') << 24);
				wavefront.push(convert_gridCell_to_wfCell((layers_vector[l].grid)[x][y - 1], x, y - 1, l));
			}
			if (x < 999 && !((layers_vector[l].grid)[x + 1][y] & gCell_reached_bit) && !((layers_vector[l].grid)[x + 1][y] & gCell_obstacle_bit))
			{
				(layers_vector[l].grid)[x + 1][y] = (layers_vector[l].grid)[x + 1][y] | gCell_reached_bit;
				(layers_vector[l].grid)[x + 1][y] += ((layers_vector[l].grid)[x][y] & gCell_cost_bits) + heuristic_cost(x + 1, y, l, target) + bend_cost;
				if (!(l % 2))
					(layers_vector[l].grid)[x + 1][y] += bend_cost;
				(layers_vector[l].grid)[x + 1][y] |= (long long('W') << 24);
				wavefront.push(convert_gridCell_to_wfCell((layers_vector[l].grid)[x + 1][y], x + 1, y, l));
			}
			if (y < 999 && !((layers_vector[l].grid)[x][y + 1] & gCell_reached_bit) && !((layers_vector[l].grid)[x][y + 1] & gCell_obstacle_bit))
			{
				(layers_vector[l].grid)[x][y + 1] |= gCell_reached_bit;
				(layers_vector[l].grid)[x][y + 1] += ((layers_vector[l].grid)[x][y] & gCell_cost_bits) + heuristic_cost(x, y + 1, l, target);
				if (l % 2)
					(layers_vector[l].grid)[x][y + 1] += bend_cost;
				(layers_vector[l].grid)[x][y + 1] |= (long long('N') << 24);
				wavefront.push(convert_gridCell_to_wfCell((layers_vector[l].grid)[x][y + 1], x, y + 1, l));
			}
			if (l > 1 && !((layers_vector[long long(l) - 1].grid)[x][y] & gCell_reached_bit) && !((layers_vector[long long(l) - 1].grid)[x][y] & gCell_obstacle_bit))
			{
				(layers_vector[long long(l) - 1].grid)[x][y] |= gCell_reached_bit;
				(layers_vector[long long(l) - 1].grid)[x][y] += ((layers_vector[l].grid)[x][y] & gCell_cost_bits) + heuristic_cost(x, y, l - 1, target) + via_cost + abs(l - 1 - target_layer);
				(layers_vector[long long(l) - 1].grid)[x][y] |= (long long('D') << 24);
				wavefront.push(convert_gridCell_to_wfCell((layers_vector[long long(l) - 1].grid)[x][y], x, y, l - 1));
			}
			if (l < no_layers && !((layers_vector[long long(l) + 1].grid)[x][y] & gCell_reached_bit) && !((layers_vector[long long(l) + 1].grid)[x][y] & gCell_obstacle_bit))
			{
				(layers_vector[long long(l) + 1].grid)[x][y] |= gCell_reached_bit;
				(layers_vector[long long(l) + 1].grid)[x][y] += ((layers_vector[l].grid)[x][y] & gCell_cost_bits) + heuristic_cost(x, y, l + 1, target) + via_cost + abs(l + 1 - target_layer);
				(layers_vector[long long(l) + 1].grid)[x][y] |= (long long('U') << 24);
				wavefront.push(convert_gridCell_to_wfCell((layers_vector[long long(l) + 1].grid)[x][y], x, y, l + 1));
			}
			wavefront.pop();
		}
	}
}

/*
The main function used to drive the program. The user is prompted to enter the number of
layers (maximum number of a layer used in the input file), reads the input file, and routes each net.
*/
int main()
{
	cout << "Please enter the number of layers:\n";
	cin >> no_layers;
	layers_vector.resize(long long(no_layers) + 1);
	int net_no = 1;
	long long source, target, next_target;
	ifstream input_file;
	ofstream output_file;
	vector <pins> pins_vector, sources;
	string input_line;
	input_file.open("input_file.txt");
	output_file.open("output_file.txt", ios::out | ios::trunc);
	if (!input_file.is_open())
	{
		cout << "Unable to open input file\n";
		return 0;
	}
	// mark pins' cells as obstacles
	while (!input_file.eof())
	{
		for (int i = 1; i <= no_layers; i++)
			initialize_layer(i);
		getline(input_file, input_line);
		// Reading input file
		read_line(input_line, pins_vector);
	}
	mark_pins_obstacles(pins_vector);
	pins_vector.clear();
	input_file.close();
	input_file.open("input_file.txt");
	while (!input_file.eof())
	{
		int pins_index = 1;
		for (int i = 1; i <= no_layers; i++)
			initialize_layer(i);
		getline(input_file, input_line);
		// Reading input file
		read_line(input_line, pins_vector);
		// Sorting nearest to boundary for starting point selection /////////////// NEED TO EDIT THIS /////////////////////
		//sort(pins_vector.begin(), pins_vector.end(), [](const pins& lhs, const pins& rhs) {
			//return lhs.distance_to_center > rhs.distance_to_center;
			//});
		string path = "";
		output_file << "net " << net_no++ << " ";
		if (pins_index == 1)
		{
			pins_index++;
			int pin1_x = pins_vector[0].x_coordinate;
			int pin1_y = pins_vector[0].y_coordinate;
			int pin1_layer = pins_vector[0].layer;
			int pin2_x = pins_vector[1].x_coordinate;
			int pin2_y = pins_vector[1].y_coordinate;
			int pin2_layer = pins_vector[1].layer;
			source = ((wfCell_x_bits) & (long long(pin1_x) << 54));
			source |= ((long long(pin1_y) << 44) & (wfCell_y_bits));
			source |= ((long long(pin1_layer) << 40) & (wfCell_layer_bits));
			source |= 1;
			(layers_vector[pin1_layer].grid)[pin1_x][pin1_y] |= gCell_reached_bit;
			target = ((wfCell_x_bits) & (long long(pin2_x) << 54));
			target |= ((long long(pin2_y) << 44) & (wfCell_y_bits));
			target |= ((long long(pin2_layer) << 40) & (wfCell_layer_bits));
			target |= 1;
			pins_vector.erase(pins_vector.begin());
			pins_vector.erase(pins_vector.begin());
		}
		if (pins_vector.size() == 0)
		{
			path = "";
			if (route_source_target_find_next_source(source, target, 0, 0, path))
				output_file << path;
			else
				cout << "Couldn't route net " << net_no - 1 << "\n";
		}
		else
		{
			while (pins_vector.size() > 0)
			{
				path = "";
				next_target = ((wfCell_x_bits) & (long long(pins_vector[0].x_coordinate) << 54));
				next_target |= ((long long(pins_vector[0].y_coordinate) << 44) & (wfCell_y_bits));
				next_target |= ((long long(pins_vector[0].layer) << 40) & (wfCell_layer_bits));
				next_target |= 1;
				if (route_source_target_find_next_source(source, target, next_target, 1, path))
					output_file << path;
				else
					cout << "Couldn't route pin " << pins_index++ << " in net " << net_no - 1 << " to the net.\n";
				pins_vector.erase(pins_vector.begin());
				for (int i = 0; i < no_layers; i++)
					initialize_layer(i);
			}
			path = "";
			if (route_source_target_find_next_source(source, target, 0, 0, path))
				output_file << path;
			else
			{
				for (int i = 1; i <= no_layers; i++)
					initialize_layer(i);
				if (route_source_target_find_next_source(source, target, 0, 0, path))
					output_file << path;
				else
					cout << "Couldn't route last pin in net " << net_no - 1 << ".\n";
			}
		}
		pins_vector.clear();
		output_file << "\n";
	}
	input_file.close();
	output_file.close();
	return 0;
}
