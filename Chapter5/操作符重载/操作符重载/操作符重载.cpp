﻿// 操作符重载.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

class Complex
{
public:
	Complex()
	{
		this->a = 0;
		this->b = 0;
	}
	Complex(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	void printf()
	{
		cout << "(" << a << "+" << b << "i" << ")" << endl;
	}
	friend Complex addComplex(Complex &c1, Complex &c2);
	friend Complex operator+(Complex &c1, Complex &c2);
	//friend void operator<<(ostream &os, Complex &C);
	friend ostream & operator<<(ostream &os, Complex &C);
	friend istream & operator>>(istream & is, Complex &C);
	//friend Complex operator+=(Complex &c1, Complex &c2);


	Complex operator-(Complex &another)
	{
		Complex newC = Complex(this->a - another.a, this->b - another.b);
		return newC;
	}

	Complex & operator +=(Complex &another)
	{
		this->a += another.a;
		this->b += another.b;
		return *this;
	}

	Complex & operator++()//成员函数传入参数为空,前加加重载
	{
		++this->a ;
		++this->b ;
		return *this;
	}

	const Complex operator++(int)//后加加，即放入一个 占位操作符，亚元,先返回后加加
	{
		Complex temp(this->a, this->b);
		a++;
		b++;
		return temp;
	}


private:
	int a;//实数部分
	int b;//虚数部分
};

Complex addComplex(Complex &c1, Complex &c2)
{
	Complex c3(c1.a + c2.a, c1.b + c2.b);
	return c3;
}

//操作符重载
Complex operator+(Complex &c1, Complex &c2)
{
	Complex c3(c1.a + c2.a, c1.b + c2.b);
	return c3;
}

//operator<<(cout,c)
//void operator<<(ostream &os, Complex &C)
//{
//	os<< "(" << C.a << "+" << C.b << "i" << ")" << endl;
//}

ostream & operator<<(ostream &os, Complex &C)//这个不能写在 成员函数里面
{
	os << "(" << C.a << "+" << C.b << "i" << ")" << endl;
	return os;
}

istream & operator>>(istream & is, Complex &C)
{
	cout << "a: ";
	is >> C.a;
	cout << "b: " ;
	is >> C.b;
	return is;
}

//Complex operator+=(Complex &c1, Complex &c2)
//{
//	Complex temp(c1.a + c2.b, c1.b + c2.b);
//	return temp;
//}

int main()
{
	Complex c1(10, 20);
	Complex c2(1, 2);

	c1.printf();
	c2.printf();

	Complex c3 = addComplex(c1, c2);
	c3.printf();

	//Complex c4 = operator+(c1, c2);
	Complex c4 = c1 + c2;//显示的调用了 “+” 号的 重载,支持全局调用的方式
	c4.printf();

	//Complex c5 = c1.operator-(c2);////显示的调用了 “-” 号的 重载 ,支持 成员函数的调用方式
	Complex c5 = c2 - c1;
	c5.printf();

	//cout << c5;//重载输出，双目运算符 //1. cout.operator<<(c5)
	//2. opeartor(cout,c5)
	cout << "---------------" << endl;
	//operator<<(cout, c5);
	cout << c5;
	cout << c5 << c3 << "haha" << endl;

	Complex c6;
	////operator>>(cin, c6);
	//cin >> c6;
	//cout << c6;

	c1 += c2;
	c1 += c2;
	c1.operator+=(c2);
	cout << c1;

	++c1;
	cout << c1;

	c1++;
	cout << c1;

}

