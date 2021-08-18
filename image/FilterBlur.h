#include "Filter.h"
#include <cmath>

#ifndef _FILTERBLUR
#define _FILTERBLUR

typedef math::Vec3<float> Color;

namespace image {

	class FilterBlur : public Filter, public math::Array2D<float> {
	private:
		int N;
		Array2D<float> f;
	public:
		FilterBlur() : N(), f() {}
		FilterBlur(int N);
		Image operator <<(const Image& image);
		void setN(int new_N);
	};



}

image::Image image::FilterBlur::operator <<(const image::Image& image) {

	image::Image filtered_image = image;
	image::Image temp = image; //We need that second image copy instance in order to have the initial form of the image object for the operations
	Color inside_sum , outside_sum;
	

	for (unsigned int i = 0; i < image.getHeight(); i++) {
		for (unsigned int j = 0; j < image.getWidth(); j++) {
			outside_sum = Color(0);
			for (float m = -N / 2.0f; m < N / 2.0f; m++) {
				inside_sum = Color(0);
				if (i + m >= -0.5f && i + m < image.getHeight() - 0.5f) { //First check before the nested for loop in order not to get inside of the nested loop if index is invalid
					for (float n = -N / 2.0f; n < N / 2.0f; n++) {
						
						if (j + n >= -0.5f && j + n < image.getWidth() - 0.5f) {
							inside_sum += temp(i + m + 0.5f, j + n + 0.5f) * f(m + N / 2, n + N / 2);
							
						}
					}
					outside_sum += inside_sum;
				}
			}
			
			filtered_image(i, j) = outside_sum;
		}
	}
	return filtered_image;

}

image::FilterBlur::FilterBlur(int N) : N(N), f(N, N) {
	
	const int size = N * N;
	vector<float> weight(size, 1.0 / size);
	f.setData(weight.data());

}

void image::FilterBlur::setN(int new_N) {

	N = new_N;
	f = Array2D<float>(N, N);
	const int size = N * N;
	vector<float> weight(size, 1.0 / size);
	f.setData(weight.data());

}

#endif

