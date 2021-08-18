#include "Filter.h"
#include "FilterBlur.h"

#ifndef _FILTERLAPLACE
#define _FILTERLAPLACE

typedef math::Vec3<float> Color;

namespace image {

	class FilterLaplace : public FilterBlur {
	private:
		Array2D<float> fl;
	public:
		FilterLaplace();
		Image operator <<(const Image& image);
	};
}

image::FilterLaplace::FilterLaplace() : FilterBlur(3), fl(3,3) {
	
	float temp[3][3] = { {0,1,0}, {1,-4,1}, {0,1,0} };
	fl.setData(temp[0]);	

}

image::Image image::FilterLaplace::operator <<(const image::Image& image) {

	image::Image filtered_image = image;
	image::Image temp = image;
	Color inside_sum, outside_sum;

	for (unsigned int i = 0; i < image.getHeight(); i++) {
		for (unsigned int j = 0; j < image.getWidth(); j++) {
			outside_sum = Color(0);
			for (int m = -1; m < 2; m++) {
				inside_sum = Color(0);
				if (i + m >= 0 && i + m < image.getHeight()) { //First check before the nested for loop in order not to get inside of the nested loop if index is invalid
					for (int n = -1; n < 2; n++) {
						if (j + n >= 0 && j + n < image.getWidth()) {
							inside_sum += temp(i + m , j + n ) * fl(m+1, n+1);
						}
					}
					outside_sum += inside_sum;
				}
			}
			outside_sum.clampToLowerBound(1);
			outside_sum.clampToUpperBound(0);
			filtered_image(i, j) = outside_sum;
		}
	}
	return filtered_image;




}



#endif