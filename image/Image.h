
#include "../math/vec3.h"
#include "../math/array2d.h"
#include "imageio.h"
#include "../ppm/ppm.h"
#include <algorithm>

#ifndef _IMAGE
#define _IMAGE

#define FORMAT "ppm"

typedef math::Vec3<float> Color;

namespace image {

	class Image : public math::Array2D<Color>, public ImageIO {
	private:
		bool extension_matches_format(const string& filename, const string& format) const;
	public:
		Image(string filename);
		Image() {}
		bool load(const std::string& filename, const std::string& format);
		bool save(const std::string& filename, const std::string& format);
	};
}

image::Image::Image(string filename) {

	bool constructed = load(filename, FORMAT);
	if (!constructed) {
		cerr << "Image is not loaded properly. End of program...";
		exit(0);
	}

}

bool image::Image::extension_matches_format(const string& filename, const string& format) const {
	size_t pos = filename.rfind('.'); //We find the last dot in the filename
	if (pos == string::npos) { //Check if there was any dot
		return false;
	}
	string extension = filename.substr(++pos);
	for_each(extension.begin(), extension.end(), [](char& c) { //Turn extension to lowercase
		c = tolower(c);
		});
	return extension == format;
}

bool image::Image::load(const std::string& filename, const std::string& format) {

	if (!extension_matches_format(filename, format)) {
		cerr << "Extension not found or not valid. .ppm is the only valid extension" << endl;
		return false;
	}

	//We cast w and h pointers from unsigned int to int because ReadPPM accepts int* arguments
	//and we know that width and height are going to be positive int

	float* data = image::ReadPPM(filename.c_str(), reinterpret_cast<int*>(&width), reinterpret_cast<int*>(&height));
	if (data == nullptr) {
		return false;
	}
	const int capacity = 3 * width * height;

	Color* colors = new Color[width * height]; Color* temp = colors;
	for (int i = 0; i < capacity; i = i + 3) { //Move data from float array to color array
		*temp = Color(data[i], data[i + 1], data[i + 2]);
		temp++;
	}
	this->setData(colors);
	delete[] data, colors;
	return true;
}

bool image::Image::save(const std::string& filename, const std::string& format) {

	if (!extension_matches_format(filename, format)) {
		cerr << "Extension not found or not valid. .ppm is the only valid extension" << endl;
		return false;
	}

	if (buffer.empty()) { //Checking if there are data to save
		cerr << "There is no image loaded in this object. No data to be saved. End of program..." << endl;
		return false;
	}

	float* data = new float[3 * width * height]; float* temp = data;

	for (Color c : buffer) {
		float rgb[3] = { c.r, c.g, c.b };
		copy(rgb, rgb + 3, temp);
		temp = temp + 3;
	}
	bool written = image::WritePPM(data, width, height, filename.c_str());
	delete[] data;
	return written;

}

#endif