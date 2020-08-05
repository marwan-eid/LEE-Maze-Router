# Lee's Maze Algorithm Router
a maze router that depends on Lee's algorithm in solving the routing problem. It depends om djkstra's algorithm in finding the shortest path between two pins. It Uses bits of integer variables to encode required information in the two main data structures : 2D array two represent the grid and a priority queue to represent the wavefront [the active cells of the grid]. The code is developed in C++ . The def/lef parser the generates the input file is developed in python. the visualizer is developed in python.
All copyrights © goes to Mohammed Abuelwafa, Donia Ghazy and Marwan Eid. 
Acknowledging the American university in Cairo and digital Design 2 course.

# Assumptions
1) The input file follows this format: 
Net_name (pin_1_layer, pin_1_x, pin_1_y) (pin_2_layer, pin_2_x, pin_2_y) …

 For example
net1 (1, 10, 20) (2, 30, 50) (1, 5, 100)
net2 (2, 100, 200) (1, 300, 50)
net3 (1, 100, 50) (2, 300, 150) (2, 50, 50) (1, 2, 2)
.
.

2) The output file follows this format:
Net_name (cell_1_layer, cell_1_x, cell_1_y) (cell_2_layer, cell_2_x, cell_2_y) …
For example
net1 (1, 10, 20) (1, 11, 20) (1, 12, 20) …
net2 (2, 100, 200) (2, 100, 201) (2, 100, 202) …
.
.

3) The visualizer needs the input file to mark the main pins and the output file to know the wires and it prompts the user
to input the number of layers to display the layers accordingly.

--> The following heuristics are implemented to minimize the routing time:
1) Heuristic cost similar to A* algorithm in order to expand the closest cell to the target. the cost is calculated as 
abs (neighbour x - target x) + abs (neighbour y - target y).

2) Bend cost > via cost in order to enforce using vias more than bending in one layer.

# Main Complexities

accessing the routing grid O(n^2) --> used to store the main information of the predecessor cell 
accessing the wavefront O (log n) --> used for all the processing to ensure faster time complexity.

Space complexity --> our space complexity is minimal for the router since we use a long long variable of 64 bits to encode all the necessary information in it as the following:

gCell -> pred = 8 bits, obstacle = 1 bit, reached = 1 bit, layer = 4 bit, cost = 18 bits
wfCell -> x = 10 bits, y = 10 bits, layer = 4 bits, predecessor = 8 bits, pathcost = 32 bits

# Main Defines for bits locations

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

# Algorithm implementation main steps 

1) Expansion
2) BackTracking
3) Clean UP


# How to use

1) run the C++ script with the selected input_file [the input file can be generated from def/lef files using the provided parser]
2) an output file is generated
3) The visualizer takes the input_file , output_file and displays the routed paths. [the visualizer needs an installed matplotlib in python using pip command]


# limitations

1) Grid size is restricted to 1000 x 1000, the visualizer works fine with these dimenions or lower. Higher dimensions are not supported.
2) With Massive and huge number of nets, Our router may fail to route some pins like any router in the industry. however, Rip up and Route is not supported yet.


