#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// parameters needed to geenrate testcases
#define no_nets 15 // number of nets
#define max_no_layers 10 // max number of layers
#define max_no_connected_pens 9 // max number of connected pens
#define filename "Test Cases File 7.txt" // output file name

int main()
{
	int no_pens;
	ofstream output_file;
	output_file.open(filename, ios::out | ios::trunc);
	for (int i = 0; i < no_nets; i++)
	{
		output_file << "net" << i + 1;
		no_pens = rand()% max_no_connected_pens + 2;
		for (int j = 0; j < no_pens; j++)
			output_file << " (" << rand() % max_no_layers << ", " << rand() % 999 << ", " << rand() % 999 << ")";
		output_file << "\n";
	}
	output_file.close();
	return 0;
}