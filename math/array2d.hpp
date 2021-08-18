#include <iostream> 
#include <vector>
#include "array2d.h"

using namespace std;

template <typename T>
const unsigned int math::Array2D<T>::getWidth() const {
	return width;
}

template <typename T>
const unsigned int math::Array2D<T>::getHeight() const {
	return height;
}

template <typename T>
T* math::Array2D<T>::getRawDataPtr() {
	return buffer.data();
}

template <typename T>
void math::Array2D<T>::setData(const T* const& data_ptr) {
	if (width != 0 && height != 0) {
		unsigned int capacity = width * height;
		if (capacity < buffer.max_size()) {
			buffer = vector<T>(data_ptr, data_ptr + capacity);
		}
	}
}

template <typename T>
T& math::Array2D<T>::operator () (unsigned int x, unsigned int y) {
	if (x < 0 || y < 0 || x > height || y > width) {
		cerr << "Index out of bounds";
		exit(0);
	}
	return buffer[x * width + y];
}

template <typename T>
math::Array2D<T>::Array2D(unsigned int width, unsigned int height, const T* data_ptr) : width(width), height(height) {
	if (width != 0 && height != 0 && data_ptr == 0) {
		buffer = vector<T>(width * height, T(0));
	}
	else {
		setData(data_ptr);
	}
}

template <typename T>
math::Array2D<T>::Array2D(const Array2D& src) { //We will make a deep copy of the elements in src.buffer in our buffer
	width = src.getWidth();
	height = src.getHeight();
	setData(src.buffer.data()); //We would be able to use src.getRawDataPtr() if getRawDataPtr() was const, since the object reference src is const.
}

template <typename T>
math::Array2D<T>::~Array2D() {

}

template <typename T>
math::Array2D<T>& math::Array2D<T>::operator = (const Array2D& right) {
	width = right.getWidth();
	height = right.getHeight();
	setData(right.buffer.data());
	return *this;
}