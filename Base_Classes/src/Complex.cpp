#include <math.h>
#include "Complex.h"

Complex::Complex()
{
	real = 0.0;
	imaginary = 0.0;
}

Complex::Complex(float r)
{
	real = r;
	imaginary = 0.0;
}

Complex::Complex(float r, float i)
{
	real = r;
	imaginary = i;
}

float Complex::getReal()
{
	return real;
}

void Complex::setReal(float r)
{
	real = r;
}

float Complex::getImaginary()
{
	return imaginary;
}

void Complex::setImaginary(float i)
{
	imaginary = i;
}

Complex Complex::get()
{
	Complex result(real, imaginary);
	return result;
}

void Complex::set(float r, float i)
{
	real = r;
	imaginary = i;
}

float Complex::magnitude()
{
	return sqrt(real * real + imaginary * imaginary);
}

Complex Complex::operator+(Complex c)
{
	Complex result;
	
	result.real = real + c.real;
	result.imaginary = imaginary + c.imaginary;
	
	return result;
}

Complex Complex::operator-(Complex c)
{
	Complex result;
	
	result.real = real - c.real;
	result.imaginary = imaginary - c.imaginary;
	
	return result;
}

Complex Complex::operator*(Complex c)
{
	Complex result;
	
	result.real = real * c.real - imaginary * c.imaginary;
	result.imaginary = imaginary * c.real + real * c.imaginary;
	
	return result;
}

Complex Complex::operator/(Complex c)
{
	Complex result;
	
	result.real = (real * c.real + imaginary * c.imaginary) / (c.real * c.real + c.imaginary * c.imaginary);
	result.imaginary = (imaginary * c.real - real * c.imaginary) / (c.real * c.real + c.imaginary * c.imaginary);
	
	return result;
}
