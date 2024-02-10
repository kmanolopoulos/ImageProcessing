#ifndef __COMPLEX_H__
#define __COMPLEX_H__

class Complex
{
protected:
	float real;
	float imaginary;
public:
	// Constructors
	Complex();
	Complex(float);
	Complex(float, float);
	// Getters/Setters
	float getReal();
	void setReal(float);
	float getImaginary();
	void setImaginary(float);
	Complex get();
	void set(float, float);
	// Various functions
	float magnitude();
	// Operator overloading
	Complex operator+(Complex);
	Complex operator-(Complex);
	Complex operator*(Complex);
	Complex operator/(Complex);
};


#endif	//__COMPLEX_H__
