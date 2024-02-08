/*!*********************************************************************************************************************
\File			Matrix.cpp
\Title			Source file for the facilitation of matrices
\Author			PowerVR by Imagination, Developer Technology Team.
\Copyright		Copyright(c) Imagination Technologies Limited.
\brief			Adds the implementations for all the methods within Matrix.h
***********************************************************************************************************************/
#include "Matrix.h"
#include <iomanip>

Matrix::Matrix(uint32_t height, uint32_t width, float* m) : _height(height), _width(width) { _m = m; }

float Matrix::operator()(size_t row, size_t column)
{
	assert(row < _width && column < _height);
	return *(_m + (column * _width + row));
}

Matrix& Matrix::operator=(Matrix rhs)
{
	const_cast<uint32_t&>(_height) = rhs._height;
	const_cast<uint32_t&>(_width) = rhs._width;
	_m = rhs._m;
	rhs._m = nullptr;

	return *this;
}

float* Matrix::data() { return _m; }

const uint32_t Matrix::getWidth() { return _width; }

const uint32_t Matrix::getHeight() { return _height; }

Matrix Matrix::matMul(Matrix lhs, Matrix rhs)
{
	assert(lhs.getWidth() == rhs.getHeight());
	size_t newWidth = rhs.getWidth();
	size_t newHeight = lhs.getHeight();
	float* m;
	m = new float[newWidth * newHeight];
	// begin the multiplication
	for (size_t y = 0; y < newHeight; y++)
	{
		for (size_t x = 0; x < newWidth; x++)
		{
			// for each cell in the new array calculate the sum
			float sum = 0;
			for (size_t k = 0; k < lhs.getWidth(); k++) { sum += lhs(k, y) * rhs(x, k); }
			m[y * newWidth + x] = sum;
		}
	}
	return Matrix((uint32_t)newHeight, (uint32_t)newWidth, m);
}

Matrix Matrix::transpose(Matrix mat)
{
	float* m;
	size_t newWidth = mat.getHeight();
	size_t newHeight = mat.getWidth();
	m = new float[newWidth * newHeight];

	// The variables x and y will be local to the new matrix
	for (size_t y = 0; y < newHeight; y++)
	{
		for (size_t x = 0; x < newWidth; x++) { m[y * newWidth + x] = mat(y, x); }
	}
	return Matrix((uint32_t)newHeight, (uint32_t)newWidth, m);
}

Matrix Matrix::RandomMat(uint32_t height, uint32_t width)
{
	float* m = new float[(size_t)height * (size_t)width];
	for (size_t i = 0; i < (size_t)height * (size_t)width; i++) { m[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }
	return Matrix(height, width, m);
}

bool Matrix::validate(Matrix A, Matrix B, float epsilon)
{
	// if the matrices aren't the same size, they can't be the same
	if (A.getHeight() != B.getHeight() || A.getWidth() != B.getWidth()) { return false; }
	//Report the biggest difference found if it is outside of the epsilon
	float largestDelta = 0.0;
	bool diffFound = false;
	// check every element
	for (size_t i = 0; i < (size_t)A.getHeight() * (size_t)A.getWidth(); i++)
	{
		if (abs(A.data()[i] - B.data()[i]) > epsilon) 
		{
			largestDelta = abs(A.data()[i] - B.data()[i]);
			diffFound = true;
			break;
		}
	}
	if (diffFound) 
	{ 
		std::cout << "\tDifference is : " << largestDelta;
		return false;
	}
	return true;
}

std::string Matrix::stringRep()
{
	std::stringstream ss;
	for (size_t y = 0; y < _height; y++)
	{
		ss << "[\t ";
		for (size_t x = 0; x < _width; x++) { ss << std::left << std::setw(10) << this->operator()(x, y); }
		ss << " ]\n";
	}
	return ss.str();
}


Vector::Vector(uint32_t width, float* m) : _width(width) { _m = m; }

float Vector::operator()(size_t column)
{
	assert(column < _width);
	return *(_m + column);
}

Vector& Vector::operator=(Vector rhs)
{
	const_cast<uint32_t&>(_width) = rhs._width;
	_m = rhs._m;
	rhs._m = nullptr;

	return *this;
}

float* Vector::data() { return _m; }

const uint32_t Vector::getWidth() { return _width; }

Vector Vector::vecMul(Vector lhs, Vector mhs, Vector rhs, uint32_t loop_size)
{
	assert(lhs.getWidth() == rhs.getWidth());
	size_t newWidth = rhs.getWidth();
	float* m;
	m = new float[newWidth];
	// begin the multiplication
	const float const_array[10] = {2.5, 7.0, 1.5, 3.5, 7.5, 5.5, 8.5, 6.0, 0.5, 9.5};
	for (size_t x = 0; x < newWidth; x++)
	{
		float sum = 0;
		//{1, 1, 1} , {1, 1, 3}. {2, 2, 3}, {1, 2, 2}, {1, 3, 3}, {1, 2, 3}, {3, 3, 3}, {2, 3, 3}, {1, 1, 2}, {2, 2, 2}.
		for (size_t k = 0; k < loop_size; k++)
		{
			sum += (lhs(x) * lhs(x) * lhs(x) +
			        lhs(x) * lhs(x) * rhs(x) +
					mhs(x) * mhs(x) * rhs(x) +
					lhs(x) * mhs(x) * mhs(x) +
					lhs(x) * rhs(x) * rhs(x) +
					lhs(x) * mhs(x) * rhs(x) +
					rhs(x) * rhs(x) * rhs(x) +
					mhs(x) * rhs(x) * rhs(x) +
					lhs(x) * lhs(x) * mhs(x) +
					mhs(x) * mhs(x) * mhs(x) + const_array[k%10]);
		}
		m[x] = sum;
	}
	return Vector((uint32_t)newWidth, m);
}

Vector Vector::RandomVec(uint32_t width)
{
	float* m = new float[(size_t)width];
	for (size_t i = 0; i < (size_t)width; i++) { m[i] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX); }
	return Vector(width, m);
}

bool Vector::validate(Vector A, Vector B, float epsilon)
{
	// if the matrices aren't the same size, they can't be the same
	if (A.getWidth() != B.getWidth()) { return false; }
	//Report the biggest difference found if it is outside of the epsilon
	float largestDelta = 0.0;
	bool diffFound = false;
	// check every element
	for (size_t i = 0; i < (size_t)A.getWidth(); i++)
	{
		if (abs(A.data()[i] - B.data()[i]) > epsilon)
		{
			largestDelta = abs(A.data()[i] - B.data()[i]);
			diffFound = true;
			break;
		}
	}
	if (diffFound)
	{
		std::cout << "\tDifference is : " << largestDelta;
		return false;
	}
	return true;
}

std::string Vector::stringRep()
{
	std::stringstream ss;
	ss << "[\t ";
	for (size_t x = 0; x < _width; x++) { ss << std::left << std::setw(10) << this->operator()(x); }
	ss << " ]\n";
	return ss.str();
}
