#include <iostream>
#include "ppm/ppm.h"
#include "image/FilterLinear.h"
#include "image/FilterGamma.h"
#include "image/FilterBlur.h"
#include "image/FilterLaplace.h"
#include "image/Image.h"
#include "math/vec3.h"
#include "math/array2d.h"
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iterator>

#define FORMAT "ppm"

typedef math::Vec3<float> Color;

using namespace std;

int main() {
	
	enum Filter_Option { linear, gamma, blur, laplace }; // Possible filters
	Filter_Option f;

	//We use the unordered_map to find the pair of the string input filters to the enum Filter_Option set
	const unordered_map<string, Filter_Option> Filter_Option_toString({ 
		{"linear", linear},
		{"gamma", gamma},
		{"blur", blur},
		{"laplace", laplace}
	});
	
	cout << "Images accepted should be in the same directory with the .exe file" << endl;
	cout << "Please write the necessary filters : " << endl;
	string input; 
	getline(cin, input); //We extract all instructions together in order to get the filename in the
						 //last position
	if (input == string()) { cerr << "No filters were given!\nPress Enter to exit the program"; getchar(); exit(0); } //'Check if input is empty to avoid throwing an exception

	istringstream iss(input); // Iterator to split the input line to its members
	vector<string> buffer; //Container to store the separate words of the input string 
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(buffer)); //Split operation
	string filename = buffer[buffer.size() - 1];
	image::Image input_image(filename);
	unsigned int i = 1;
	int N; //for blur filter
	
	while (buffer[i] == "-f") {

		unordered_map<string, Filter_Option>::const_iterator iter = Filter_Option_toString.find(buffer[++i]);
		if (iter != Filter_Option_toString.end()) {
			f = iter->second;
		}
		else {
			cerr << "Filter given doesn't exist!" << endl;
			break;
		}

		image::FilterLinear filter_linear;
		image::FilterGamma filter_gamma;
		image::FilterBlur filter_blur;
		image::FilterLaplace filter_laplace;

		switch (f) {
		
		case linear: 
			//We turn input arguments to floats with stof 
			filter_linear.setData(Color(stof(buffer[i + 1]), stof(buffer[i + 2]), stof(buffer[i + 3])), Color(stof(buffer[i + 4]), stof(buffer[i + 5]), stof(buffer[i + 6])));
			input_image = filter_linear << input_image;
			i = i + 7;
			break;
		
		case gamma:
			filter_gamma.setData(stof(buffer[i + 1]));
			input_image = filter_gamma << input_image;
			i = i + 2;
			break;

		case blur: //In order to set this filter write "filter -f blur N", where N is the odd parameter
			N = stoi(buffer[i + 1]);
			filter_blur.setN(N);
			if (N % 2 == 1) { //Check if N is odd or not
				input_image = filter_blur << input_image;
			}
			else {
				cerr << "Parameter after filter blur must be an odd number. No filter was set.\nEnd of program!" << endl;
				exit(0);
			}
			i = i + 2;
			break;

		case laplace:
			input_image = filter_laplace << input_image;
			i = i + 1;
			break;
		}

	}
	input_image.save("filtered_" + filename, FORMAT);
	cout << "Filtered image was saved succesfully...";
	getchar();
	return 0;
}
