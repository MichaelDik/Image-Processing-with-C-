#include "Filter.h"

#ifndef _FILTERLINEAR
#define _FILTERLINEAR

typedef math::Vec3<float> Color;

namespace image {

	class FilterLinear : public Filter {
	private:
		Color a, c;
	public:
		FilterLinear() {}
		FilterLinear(Color x, Color y);
		FilterLinear(const FilterLinear& filter);
		Image operator <<(const Image& image);
		void setData(Color new_a, Color new_c);

	};

}

image::Image image::FilterLinear::operator <<(const image::Image& image) {

	image::Image filtered_image = image; //It was necessary to store the Image parameter in a non-const
										 //Image instance before its modification in order to be able to
										 //use non-const methods like the operator ()

	Color temp;
	for (unsigned int i = 0; i < image.getHeight(); i++) {
		for (unsigned int j = 0; j < image.getWidth(); j++) {
			temp = a * filtered_image(i, j) + c;
			filtered_image(i, j) = temp.clampToLowerBound(0.0f).clampToUpperBound(1.0f);
		}
	}
	return filtered_image;
}

image::FilterLinear::FilterLinear(Color x, Color y) : a(x), c(y) {}

image::FilterLinear::FilterLinear(const FilterLinear& filter) : a(filter.a), c(filter.c) {}

void image::FilterLinear::setData(Color new_a, Color new_c) {
	a = new_a;
	c = new_c;
}

#endif