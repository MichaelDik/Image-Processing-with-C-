#include "Filter.h"
#include <cmath>

#ifndef _FILTERGAMMA
#define _FILTERGAMMA

typedef math::Vec3<float> Color;

namespace image {
	
	class FilterGamma : public Filter {
	private:
		float g;
	public:
		FilterGamma() { g = 0.0f; }
		FilterGamma(float g);
		FilterGamma(const FilterGamma& filter);
		Image operator <<(const Image& image);
		void setData(float new_g);

	};

}

image::Image image::FilterGamma::operator <<(const Image& image) {

	image::Image filtered_image = image;
	Color temp;
	for (unsigned int i = 0; i < image.getHeight(); i++) {
		for (unsigned int j = 0; j < image.getWidth(); j++) {
			filtered_image(i, j) = Color(pow(filtered_image(i, j).r, g), pow(filtered_image(i, j).g, g), pow(filtered_image(i, j).b, g));
			//There is no need to check bounds, because the rgb values are surely in [0,1]
		}
	}
	return filtered_image;

}

image::FilterGamma::FilterGamma(float g) { this->g = g > 2.0f ? 2.0f : g < 0.5f ? 0.5f : g; }

image::FilterGamma::FilterGamma(const image::FilterGamma& filter) : g(filter.g) {}

void image::FilterGamma::setData(float new_g) {
	g = new_g;
}

#endif