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
#define via_cost 20

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

// Pins data
struct pins {
	int layer, x_coordinate, y_coordinate;
	int distance_to_center;
};

int layer_1[1000][1000];
//int expanded_cells = 0;

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

int calc_distance_to_center(pins pin)
{
	return (int)sqrt(pow(abs(pin.x_coordinate - 500), 2) + pow(abs(pin.y_coordinate - 500), 2));
}

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

int heuristic_cost(const int& neighbor_x, const int& neighbor_y, const long long& target)
{
	int target_x = (target & wfCell_x_bits) >> 54;
	int target_y = (target & wfCell_y_bits) >> 44;
	//return (int)sqrt(pow(abs(neighbor_x - target_x), 2) + pow(abs(neighbor_y - target_y), 2));
	//return (int)sqrt((pow(abs(neighbor_x - target_x), 2)) + (pow(abs(neighbor_y - target_y), 2)), 2);
	//return (pow(abs(neighbor_x - target_x), 2) + pow(abs(neighbor_y - target_y), 2));
	return min(abs(neighbor_x - target_x), abs(neighbor_y - target_y));
}

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

long long convert_gridCell_to_wfCell(const int& gCell, const int& x, const int& y)
{
	/*
	pins vector --> x , y, layer, distance
	 wavefront --> x, y, layer, pathcost, predecessor
	 source --> indexed by x, y, layer (from pins_vector) (stores ..)
	 x = 10 bits , y = 10 bits ; layer = 4 bits , predecessor = 8 bits , pathcost = 19 bits
	#define gCell_pred_bits 0xFF000000
	#define gCell_obstacle_bit 0x00800000
	#define gCell_reached_bit 0x004000000
	#define gCell_layer_bits 0x003C0000
	#define gCell_cost_bits 0x0003FFFF
	#define wfCell_x_bits 0xFFC0000000000000
	#define wfCell_y_bits 0x003FF00000000000
	#define wfCell_layer_bits 0x00000F0000000000
	#define wfCell_pred_bits 0x000000FF00000000
	#define wfCell_cost_bits 0x00000000FFFFFFFF
	gCell -> pred = 8 bits, obstacle = 1 bit, reached = 1 bit, layer = 4 bit, cost = 18 bits
	wfCell -> x = 10 bits, y = 10 bits, layer = 4 bits, predecessor = 8 bits, pathcost = 32 bits

	gCell
	100-0101-0000-0100-0000-0000-0000-0010
	wfCell
	10-0100-0001-0100-0001-0100-0000-0000-0000-0000-0000-0000-0000-0000-0010

	*/
	//wfCell = (((long long(x) << 54) & wfCell_x_bits) | ((long long(y) << 44) & wfCell_y_bits) |
	//	(((long long(gCell) & gCell_layer_bits) << 22) & wfCell_layer_bits) |
	//	(((long long(gCell) & gCell_pred_bits) << 8) & wfCell_pred_bits) |
	//	((long long(gCell) & gCell_cost_bits) & wfCell_cost_bits));
//		((long long(gCell) & wfCell_layer_bits)));
		//((long long(gCell) & wfCell_pred_bits) << 8) |
		//((long long(gCell)) & wfCell_cost_bits));
	long long wfCell = 0;
	wfCell = (((long long(x) << 54)) | ((long long(y) << 44)) | ((long long(gCell) & gCell_layer_bits) << 22)
		| ((long long(gCell) & gCell_pred_bits) << 8) | (long long(gCell) & gCell_cost_bits));
	return wfCell;
}

void initialize_layer()
{
	//initializing cells
	int x_axis, y_axis;
	// cin >> x_axis >> y_axis; //////////////////////////////////////
	// 
	int layer = 1;
	for (int i = 0; i < 1000; i++)
		for (int j = 0; j < 1000; j++)
		{
			//layer_1[i][j] = (layer_1[i][j] & gCell_obstacle_bit);
			layer_1[i][j] &= gCell_obstacle_bit;
			layer_1[i][j] |= (layer << 18);
			layer_1[i][j] |= 1;
		}
}

void mark_pins_obstacles(const vector<pins>& pins_vector)
{
	int no_pins = pins_vector.size();
	for (int i = 0; i < no_pins; i++)
		layer_1[pins_vector[i].x_coordinate][pins_vector[i].y_coordinate] |= (gCell_obstacle_bit);
}

bool route_source_target_find_next_source(long long& source, long long& target, const long long& next_target, bool is_next_targ, string& path_str)
{
	priority_queue<long long, vector<long long>, Comparator> wavefront;
	stack<string> path;
	long long next_source_cell = 0;
	int min_distance_next_targ = INT_MAX, temp_distance;
	int next_target_x = (next_target & (wfCell_x_bits)) >> 54;
	int next_target_y = (next_target & (wfCell_y_bits)) >> 44;
	int source_x = (source & (wfCell_x_bits)) >> 54;
	int source_y = (source & (wfCell_y_bits)) >> 44;
	int target_x = (target & (wfCell_x_bits)) >> 54;
	int target_y = (target & (wfCell_y_bits)) >> 44;
	// clearing obstacle bit for source & target
	if (layer_1[source_x][source_y] & gCell_obstacle_bit)
		layer_1[source_x][source_y] &= ~(1UL << 23);
	if (layer_1[target_x][target_y] & gCell_obstacle_bit)
		layer_1[target_x][target_y] &= ~(1UL << 23);
	int x, y, l;
	char dir;
	bool s;
	wavefront.push(source);
	long long wf_top;
	while ((layer_1[(target & wfCell_x_bits) >> 54][(target & wfCell_y_bits) >> 44] & gCell_reached_bit) != 1)
	{
		if (wavefront.empty()) // path not found
			return false;
		else
		{
			//expanded_cells++;
			wf_top = wavefront.top();
			x = (wf_top & (wfCell_x_bits)) >> 54;
			y = (wf_top & (wfCell_y_bits)) >> 44;
			l = (wf_top & (wfCell_layer_bits)) >> 40;
			dir = (wf_top & wfCell_pred_bits) >> 32;
			if ((wf_top & (wfCell_x_bits | wfCell_y_bits | wfCell_layer_bits)) == (target & (wfCell_x_bits | wfCell_y_bits | wfCell_layer_bits)))
			{	// && (l != ((source & wfCell_layer_bits) >> 40)
				//while ((x != ((source & wfCell_x_bits) >> 54)) && (y != ((source & wfCell_y_bits) >> 44)))
				//while (x != source_x && y != source_y)
				while (source != layer_1[x][y])
				{
					// selecting best cell to start routing to next target from
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
					layer_1[x][y] |= (gCell_obstacle_bit);
					char dir_old = (layer_1[x][y] & gCell_pred_bits) >> 24;
					predec_location(l, x, y, dir_old, s);
					dir = (layer_1[x][y] & gCell_pred_bits) >> 24; // not needed
					if (s)
						break;
				}
				if (is_next_targ)
				{
					/*
					x = (wf_top & (wfCell_x_bits)) >> 54;
					y = (wf_top & (wfCell_y_bits)) >> 44;
					next_source_cell |= ((wfCell_x_bits) & (long long(x) << 54));
					next_source_cell |= ((wfCell_y_bits) & (long long(y) << 44));

					source |= ((wfCell_x_bits) & (long long(x) << 54));
					next_source_cell |= ((long long(y) << 44) & (wfCell_y_bits));
					*/
					next_source_cell |= ((wfCell_layer_bits) & (long long(l) << 40));
					next_source_cell |= 1;
					//source = next_source_cell;
					source = target;
					target = next_target;
					while (path.size() > 1)
					{
						path_str += path.top();
						path.pop();
					}
				}
				else
				{
					while (!path.empty())
					{
						path_str += path.top();
						path.pop();
					}
				}
				return true;
			}
			// pathcost(N) = pathcost(C) + cellcost(N) + heuristicCost(N + T)
			if (x > 0 && y > 0 && x < 1000 && y < 1000 && !(layer_1[x - 1][y] & gCell_reached_bit) && !(layer_1[x - 1][y] & gCell_obstacle_bit))
			{
				layer_1[x - 1][y] |= gCell_reached_bit;
				layer_1[x - 1][y] += (layer_1[x][y] & gCell_cost_bits) + heuristic_cost(x - 1, y, target) + bend_cost;
				layer_1[x - 1][y] |= (long long('E') << 24);
				wavefront.push(convert_gridCell_to_wfCell(layer_1[x - 1][y], x - 1, y));
			}
			if (x > 0 && y > 0 && x < 1000 && y < 1000 && !(layer_1[x][y - 1] & gCell_reached_bit) && !(layer_1[x][y - 1] & gCell_obstacle_bit))
			{
				layer_1[x][y - 1] |= gCell_reached_bit;
				// ADD BEND COST ONLY IF LAYER IS ODD ///////////
				layer_1[x][y - 1] += (layer_1[x][y] & gCell_cost_bits) + heuristic_cost(x, y - 1, target);
				layer_1[x][y - 1] |= (long long('S') << 24);
				wavefront.push(convert_gridCell_to_wfCell(layer_1[x][y - 1], x, y - 1));
			}
			if (x > 0 && y > 0 && x < 1000 && y < 1000 && !(layer_1[x + 1][y] & gCell_reached_bit) && !(layer_1[x + 1][y] & gCell_obstacle_bit))
			{
				layer_1[x + 1][y] = layer_1[x + 1][y] | gCell_reached_bit;
				// ADD BEND COST ONLY IF LAYER IS EVEN //////////
				layer_1[x + 1][y] += (layer_1[x][y] & gCell_cost_bits) + heuristic_cost(x + 1, y, target) + bend_cost;
				layer_1[x + 1][y] |= (long long('W') << 24);
				wavefront.push(convert_gridCell_to_wfCell(layer_1[x + 1][y], x + 1, y));
			}
			if (x > 0 && y > 0 && x < 1000 && y < 1000 && !(layer_1[x][y + 1] & gCell_reached_bit) && !(layer_1[x][y + 1] & gCell_obstacle_bit))
			{
				layer_1[x][y + 1] |= gCell_reached_bit;
				layer_1[x][y + 1] += (layer_1[x][y] & gCell_cost_bits) + heuristic_cost(x, y + 1, target);
				layer_1[x][y + 1] |= (long long('N') << 24);
				wavefront.push(convert_gridCell_to_wfCell(layer_1[x][y + 1], x, y + 1));
			}
			wavefront.pop();
		}
	}
}

int main()
{
	//cout << "83 shifted " << (long long('S') << 24) << endl << "87 shifted " << (long long('W') << 24) << endl;
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
		initialize_layer();
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
		initialize_layer();
		getline(input_file, input_line);
		// Reading input file
		read_line(input_line, pins_vector);
		// Sorting nearest to boundary for starting point selection /////////////// NEED TO EDIT THIS /////////////////////
		//sort(pins_vector.begin(), pins_vector.end(), [](const pins& lhs, const pins& rhs) {
			//return lhs.distance_to_center > rhs.distance_to_center;
			//});
		//for (int i = 0; i < pins_vector.size(); i++)
			//cout << pins_vector[i].x_coordinate << " " << pins_vector[i].y_coordinate << " " << pins_vector[i].layer << " " << pins_vector[i].distance_to_center << "\n";
		//cout << "\n";
		// CORE CODE HERE ..................... (for each line, u have some pins)
		string path = "";
		output_file << "net" << net_no++;
		if (pins_index == 1)
		{
			pins_index++;
			int pin1_x = pins_vector[0].x_coordinate;
			int pin1_y = pins_vector[0].y_coordinate;
			int pin2_x = pins_vector[1].x_coordinate;
			int pin2_y = pins_vector[1].y_coordinate;
			source = ((wfCell_x_bits) & (long long(pin1_x) << 54));
			source |= ((long long(pin1_y) << 44) & (wfCell_y_bits));
			source |= ((long long(pins_vector[0].layer) << 40) & (wfCell_layer_bits));
			source |= 1; //cost is to the right most
			layer_1[pin1_x][pin1_y] = layer_1[pin1_x][pin1_y] | (gCell_reached_bit);
			target = ((wfCell_x_bits) & (long long(pin2_x) << 54));
			target |= ((long long(pin2_y) << 44) & (wfCell_y_bits));
			target |= ((long long(pins_vector[1].layer) << 40) & (wfCell_layer_bits));
			target |= 1; //cost is to the rightmost
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
					cout << "Couldn't route net " << net_no - 1 << "\n";
				pins_vector.erase(pins_vector.begin());
				initialize_layer();
			}
			path = "";
			if (route_source_target_find_next_source(source, target, 0, 0, path))
				output_file << path;
			else
				cout << "Couldn't route net " << net_no - 1 << "\n";
		}
		pins_vector.clear();
		output_file << "\n";
	}
	input_file.close();
	output_file.close();
	return 0;
}

// manual testcases
/*
net1 (1, 42, 67) (1, 60, 14)
net2 (1, 10, 20) (1, 30, 50)
net3 (1, 104, 201) (1, 303, 502)

net1 (1, 10, 10) (1, 15, 15)
net2 (1, 14, 15) (1, 20, 20)
net3 (1, 12, 14) (1, 17, 18)



net1 (1, 10, 10) (1, 15, 15) (1, 26, 24)
net2 (1, 14, 15) (1, 20, 20) (1, 46, 52) (1, 63, 75)
net3 (1, 12, 14) (1, 17, 18) (1, 72, 57)
net4 (1, 54, 64) (1, 43, 76) (1, 105, 53)



net1 (1, 10, 10) (1, 14, 14)
net2 (1, 12, 12) (1, 16, 17)
// net1 (1, 14, 15) (1, 20, 20) (1, 46, 52)
// path of 1st two ->  (1, 14, 15)  (1, 15, 15)  (1, 16, 15)  (1, 17, 15) ...
// (1, 18, 15)  (1, 19, 15)  (1, 20, 15)  (1, 20, 16)  (1, 20, 17)  (1, 20, 18)  (1, 20, 19)  (1, 20, 20)


net1 (1, 340, 485) (1, 739, 188) (1, 988, 387) (1, 173, 710) (1, 970, 843) (1, 953, 997) (1, 423, 441) (1, 153, 905)
net2 (1, 694, 671) (1, 871, 57) (1, 556, 750) (1, 38, 328) (1, 288, 843) (1, 950, 49)
*/
