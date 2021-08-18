#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "ppm.h"

using namespace std;

float* image::ReadPPM(const char* filename, int* w, int* h) {

	string file_type;
	int max_possible_value;
	ifstream file(filename, ios::in | ios::binary);

	if (!file) {  //Check if file opened properly
		cerr << "Can't open file. Might not exist!" << endl;
		return nullptr;
	}

	file >> file_type >> (*w) >> (*h) >> max_possible_value; //Pass the first four info of ppm in variables
	if (file_type != "P6") { //Check if file type is P6
		cerr << "File type not accepted!" << endl;
		return nullptr;
	}

	if (max_possible_value != 255) { //Check if max value is valid
		cerr << "Invalid maximum accepted rgb size!" << endl;
		return nullptr;
	}

	const int image_bytes = 3 * (*w) * (*h); //Size of float array 
	float* data = new float[image_bytes]; //Creating dynamic array in order to make sure there is enough space for the data
	int extra_byte = file.get(); // Extra byte before the rgb data
	float byte;
	for (int i = 0; i < image_bytes; i++) {
		byte = file.get() / (float)max_possible_value; //Turn byte value into float in range[0,1]
		data[i] = byte; //Store float value in the array
	}

	file.close();

	return data; // Return array(address of its first element) if everything has gone well
}

bool image::WritePPM(const float* data, int w, int h, const char* filename) {

	if (data == nullptr) { //Check if there are data given
		cerr << "There are no data!";
		return false;
	}
	
	ofstream file(filename, ios::out | ios::binary); //Open file 
	
	if (!file) { //Check  if file opened properly 
		cerr << "Can't open file. Might not exist!" << endl;
		return false;
	}
	
	file << "P6\n" << w << "\n" << h << "\n" << 255 << endl; //Write the header data
	const int image_bytes = 3 * w * h;
	unsigned char* bytes = new unsigned char[image_bytes];

	for (int i = 0; i < image_bytes; i++) {
		bytes[i] = round(data[i] * 255); // We need to round it because we might get wrong bytes if we cast it immediately to int
	}
	file.write((char*)bytes, image_bytes); //Write all data to the file 
	delete[] bytes; 
	
	if (file.fail()) { //Check if data were written properly
		cerr << "Could not write data!" << endl;
		return false;
	}

	file.close();
	return true;

}
