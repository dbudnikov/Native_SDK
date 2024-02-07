/*!*********************************************************************************************************************
\File			Matrix.h
\Title			Header file for the facilitation of matrices
\Author			PowerVR by Imagination, Developer Technology Team.
\Copyright		Copyright(c) Imagination Technologies Limited.
\brief			Header file that primarily supplies the main project matrices, supplying a struct which allows
				a representation of matrices through buffers which are sent to the gpu.
***********************************************************************************************************************/
#pragma once
#include <PVRCore/PVRCore.h>
struct Matrix
{
private:
	const uint32_t _width;
	const uint32_t _height;

	float* _m;

public:
	Matrix(uint32_t width, uint32_t height, float* m);
	float operator()(size_t row, size_t column);
	Matrix& operator=(Matrix lhs);
	float* data();
	std::string stringRep();
	const uint32_t getWidth();
	const uint32_t getHeight();
	static Matrix matMul(Matrix lhs, Matrix rhs);
	static Matrix transpose(Matrix mat);
	static Matrix RandomMat(uint32_t height, uint32_t width);
	static bool validate(Matrix A, Matrix B, float epsilon);
};

struct Vector
{
private:
	const uint32_t _width;

	float* _m;

public:
	Vector(uint32_t width, float* m);
	float operator()(size_t column);
	Vector& operator=(Vector lhs);
	float* data();
	std::string stringRep();
	const uint32_t getWidth();
	static Vector vecMul(Vector lhs, Vector rhs, uint32_t loop_size);
	static Vector RandomVec(uint32_t width);
	static bool validate(Vector A, Vector B, float epsilon);
};