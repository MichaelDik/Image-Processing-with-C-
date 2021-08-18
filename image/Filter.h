

#ifndef _FILTER
#define _FILTER

#include "Image.h"

namespace image {

	class Filter {

	protected:
		Filter() {}
		//Filter(const Filter& filter);
		virtual ~Filter() {}

	public:
		virtual Image operator <<(const Image& image) = 0;
	};

}

#endif
