# Chapter5 继承和派生

## 1.0 this指针
```C++
#include "pch.h"
#include <iostream>
using namespace std;

class Test
{
public:
	Test(int i)
	{
		this->mI = i;
	}
	int Change() const//在成员函数后面加const修饰的是隐藏的this指针，让this成为（const Test * const this）
	{
		//this++;this是常指针不能修改本身，(Test *const this)
		//this->mI = 0;

	}
private:
	int mI;
};
int main()
{
    
}

```

## 1.1 全局函数和成员函数
```C++
	//返回本身，实现连加,返回自己的别名
	Test   TestAdd04(Test &another)//如果返回的不是引用，那么在实现连加时，后续this都是一个临时变量，会被析构
	{
		this->a += another.a;
		this->b += another.b;

		//返回自己
		return *this;
	}//匿名的别名= *this ， 如果不是返回引用，那么在返回 *this会拷给给一个不同的地址，后续this不再代表 t1
```

## 1.2 数组类的封装
```C++
#include "pch.h"
#include <iostream>
using namespace std;


class Myarray
{
public:
	Myarray()
	{
		this->space = NULL;
		this->length = 0;
	}

	Myarray(int len)//开辟长度为 len的空间
	{
		if (len <= 0)
		{
			this->length = 0;
			return;
		}
		else
		{
			this->length = len;
			this->space = new int[this->length];//在堆上开辟了 长度为length的连续int 空间
			cout << "数组的构造函数被调用了" << endl;
		}
	}

	Myarray(const Myarray &another)
	{
		if (another.length >= 0)
		{
			this->length = another.length;
			this->space = new int[this->length];
			for (int i = 0; i < this->length; i++)
			{
				this->space[i] = another.space[i];
			}
		}
	}

	~Myarray()
	{
		cout << "执行析构函数" << endl;
		if (this->space)
		{
			delete[] this->space;
			this->length = 0;
			this->space = NULL;
		}

	}
	int Myarraygetlen()
	{
		return this->length;
	}

	void setData(int index, int value)
	{
		if (this->space)
		this->space[index] = value;
	}

	void printf()
	{
		for (int i = 0; i < this->length; i++)
			cout << this->space[i] << " ," << endl;
	}

private:
	int *space; //space在堆上开辟数组空间的首元素地址
	int length;

};

int main()
{
	Myarray array(10);//调用有参构造函数

	for (int i = 0; i < array.Myarraygetlen(); i++)
	{
		array.setData(i, i + 10);
	}

	array.printf(); 

	cout << "---一下是拷贝构造函数的测试----" << endl;
	Myarray array2 = array;
	array2.printf();
}
```

## 1.3 友元函数

>friend double PointDistance2(Point &p1, Point &p2);//友元函数，可以访问 私有变量
友元类：friend class B;

```C++
#include "pch.h"
#include <iostream>
using namespace std;

class Point
{
public:
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	void printP()
	{
		cout << "x= " << x << " y= " << y << endl;
	}
	int getx()
	{
		return this->x;
	}
	int gety()
	{
		return this->y;
	}

	//1. 声明一个全局函数友元函数，可以访问 私有变量
	friend double PointDistance2(Point &p1, Point &p2);
	

private:
	//friend double PointDistance2(Point &p1, Point &p2);//友元函数，可以访问 私有变量写 public和 private都可以

	int x;
	int y;
};

//1.通过全局方式写一个 两点之间的距离
double PointDistance(Point &p1, Point &p2)
{
	int ddx = p1.getx() - p2.getx();
	int ddy = p1.gety() - p2.gety();
	return sqrt(ddx*ddx + ddy * ddy);
}

//2.通过友元来写，友元以后，可以访问私有变量
double PointDistance2(Point &p1, Point &p2)
{
	int ddx = p1.x- p2.x;
	int ddy = p1.y - p2.y;
	return sqrt(ddx*ddx + ddy * ddy);
}


int main()
{
	Point p1(10, 20);
	Point p2(20,10);
	cout << "Distance: " << PointDistance(p1, p2) << endl;
	cout << "Distance2: " << PointDistance2(p1, p2) << endl;



}
```
## 1.4 操作符重载
操着符重载支持两种，分别是 **成员函数重载** 和 **全局调用的方式**。
* 不允许用户自定义新的运算符，只允许重载已有的运算操作符
```c++
// 操作符重载.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

class Complex
{
public:
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

	Complex operator-(Complex &another)
	{
		Complex newC = Complex(this->a - another.a, this->b - another.b);
		return newC;
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
}
```
## 1.5 输入输出双目运算符 重载
```C++
//friend void operator<<(ostream &os, Complex &C);
	friend ostream & operator<<(ostream &os, Complex &C);
	friend istream & operator>>(istream & is, Complex &C);

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
	cout << "---------------" << endl;
	//operator<<(cout, c5);
	cout << c5;
	cout << c5 << c3 << "haha" << endl;

	Complex c6;
	//operator>>(cin, c6);
	cin >> c6;
	cout << c6;
```

## 1.6 双目运算符重载
* 在C++中可以使用 ++++a;不能使用 a++++;即 a++ 返回的是常量，不可再次修改
```C++
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


	c1 += c2;
	c1 += c2;
	c1.operator+=(c2);
	cout << c1;

	++c1;
	cout << c1;

	c1++;
	cout << c1;
```

## 1.7 等号操作符
```C++
// 等号操作符.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>

using namespace std;

class Student
{
public:
	Student(int id, const char *name)
	{
		this->id = id;
		this->name =  new char[strlen(name)+1];
		strcpy_s(this->name, strlen(name) + 1, name);
	}

	//提供一个深拷贝，防止浅拷贝
	Student(const Student & other)
	{
		this->id = other.id;
		this->name = new char[strlen(other.name) + 1];
		strcpy_s(this->name, strlen(other.name) + 1, other.name);
	}

	//重写等号 操作符，返回引用实现 连等
	Student & operator=(const Student & other)
	{
		if (this->name)
		{
			delete[] this->name;
			this->name = NULL;
			this->id = 0;
		}//首先删除原值
		this->id = other.id;
		this->name = new char[strlen(other.name) + 1];
		strcpy_s(this->name, strlen(other.name) + 1, other.name);
		return *this;
	}


	~Student()
	{
		if (this->name)
		{
			delete[] this->name;
			this->name = NULL;
			this->id = 0;
		}
	}

	void Prints()
	{
		cout << "id: " << id << "  name: " << this->name << endl;
	}
private:
	int id;
	char *name;
};

int main()
{
	Student s1(1, "zhangsan");
	Student s2(s1); //如果是浅拷贝会发生两遍对同一个内存地址 析构

	s1.Prints();

	Student s3(1, "lishi");
	s3 = s1;//这个是调用 等号 操作符,浅拷贝，因此要重载 等号操作符
	s3.Prints();
}
```

```C++
//使用string来实现，可以不用析构，等号操作符重载
class Student
{
private:
	int id;
	string name;
public:
	Student(int id, string name)
	{
		this->id = id;
		this->name = name;
	}
	void Prints()
	{
		cout << "id: " << this->id << "  name: " << this->name << endl;

	}
};





int main()
{
	Student s1(1, "zhangsan");
	Student s2(s1); //如果是浅拷贝会发生两遍对同一个内存地址 析构

	s1.Prints();

	Student s3(1, "lishi");
	s3 = s1;//这个是调用 等号 操作符,浅拷贝，因此要重载 等号操作符
	s3.Prints();
}
```

## 1.8 堆和栈的简介
一个由C/C++编译程序占用内存分为以下几个部分
1. **栈区**(stack) --由编译器自动分配释放，存放函数参数值，局部变量值等。其操作方式类似于数据结构中的栈。
2. **堆区**(heap)--一般由程序员分配释放，若程序员不释放，程序结束时可能由OS回收，注意它与数据结构中的堆是两回事，分配方式类似于链表。
3. **全局区**(静态区)(static)--	全局变量和静态变量存储是放在一块，初始化全局变量和静态变量在一块区域，未初始化全局变量和未初始化静态变量在相邻另一块区域。程序结束后由系统释放
4. **文字常量区** --常量字符串就是放在这里，程序结束后由系统释放
5. **程序代码区** --存放函数体二进制代码 code

```C++
#include <string>
int a=0;    //全局初始化区
char *p1;   //全局未初始化区
void main()
{
    int b;//栈
    char s[]="abc";   //栈
    char *p2;         //栈
    char *p3="123456";   //123456\0在常量区，p3在栈上。
    static int c=0;   //全局（静态）初始化区
    p1 = (char*)malloc(10);
    p2 = (char*)malloc(20);   //分配得来的10和20字节的区域就在堆区。
    strcpy(p1,"123456");   //123456\0放在常量区，编译器可能会将它与p3所向"123456\0"优化成一个地方。
}
```
>申请方式

stack:
由系统自动分配，例如，声明在函数中一个局部变量 int b;系统自动在栈中为b开辟空间。
heap:
需要程序员自己申请，并且指名大小，在C中：p1 = (char *)malloc(10),在C++中，p2 = new char[10]

>申请后系统响应

栈：只要栈剩余空间大于所申请空间，系统将为程序提供内存，否者将报异常提示栈溢出
堆：首先应该知道操作系统有一个记录空闲内存链表，当系统收到程序申请时，会遍历改链表，寻找第一个空间大于申请空间堆节点，然后将该节点从空闲节点链表中删除，并将该节点空间分配给程序，另外，对于大多数系统，会在这块内存空间中首地址处记录本次分配的大小，这样，代码中delete语句才能正确释放本内存空间。另外，由于找到的堆节点大小不一定正好等于申请大小，系统会自动将多余那部分重新放入空闲链表中。

>申请大小限制

栈：在windows中，栈是向低地址扩展数据结构，是一块连续内存区域。这句话意思是栈顶地址和栈最大容量是系统预先定义好的，在windows中，栈大小是2M,如果申请空间超过栈剩余空间时，将提示overflow。因此，能从栈获得空间较小。

堆：堆是向高地址扩展数据结构，是不连续内存区域，这是由于系统是用链表来存储空闲内存地址，自然是不连续，而链表遍历方向是由低地址向高地址，堆大小受限于计算机系统中有效虚拟内存，由此可见，堆获得空间比较灵活，也比较大。

## 1.9 自定义数组--重载等号操作符
=
```C++
	Myarray & operator=(const Myarray &other)
	{
		this->length = other.length;
		if (this->space)
		{
			delete[] this->space;
			this->space = NULL;
		}
		this->space = new int[this->length];
		for (int i = 0; i < this->length; i++)
			this->space[i] = other.space[i];
		return *this;
	}
	//实现等号操作符的重载

	cout << "---以下是等号操作符的重载的测试----" << endl;
	Myarray array4;
	array4 = array2;
	cout << array4;
```
==
```C++
//重载双等号操作符 ==
	bool operator==(const Myarray & other)
	{
		if (this->length == other.length) 
		{
			for (int i = 0; i < this->length; i++)
			{
				if (this->space[i] != other.space[i])
					return false;
			}
			return true;

		}

		return false;

	}
	cout << "---以下是双等号 == 重载的测试----" << endl;
	if (array4 == array2)
		cout << "array4 == array2" << endl;
```
## 2.0 智能指针操作符重载
智能指针式行为类似于 指针的类的对象，但这种对象还有其他功能。本节介绍三个课帮助管理动态内存分配的智能指针模板：当函数终止时，本地变量都将从栈内存中删除--因此指针占据的内存将被释放。但是指针指向的内存没有被释放。
* auto_ptr
* unique_ptr
* shared_ptr

>auto_ptr 存在问题：
1. auto_ptr<string> ps (new string("I want to eat an apple"));
auto_ptr<string> vocation;
vocations = ps;
这个会让两个指针指向同一个string对像，这是不能接受的，因为程序将试图删除同一个对象两次，要避免这个问题的方法如下：
* 定义赋值运算符，使之执行 **深复制**，这样两个指针将指向不同的对象，其中一个对象是另外一个对象的副本。
* 建立 **所有权**（ownership）概念，对于特定的对象，只能有一个智能指针可以拥有它，这样只有拥有对象的智能指针的构造函数会删除该对象，然后，让赋值操作转让所有权。这就是用于 auto_ptr 和 unique_ptr的策略，但 unique_ptr的策略更严格。
* 创建智能更高的指针，跟踪引用特定对象的智能指针数，这称为引用计数（reference counting）.例如，赋值时，计数将加1，而指针过期时，计数将减1，仅当最后一个指针过期时，才调用delete.这个是  **shared_ptr** 采用的策略。

## 2.1 类和类之间的关系
* has-A 包含关系，用以描述一个类由多个“部件类”构成。实现has-A关系用类成员表示，即一个类中的数据成员是另一种已经定义的类。
* uses-A 一个类部分地使用另一个类。通过类之间成员函数的相互联系，定义友元或对象参数传递实现。
* is-A 机制称为 **继承**。关系具有传递性不具有对称性。

```C++
#include "pch.h"
#include <iostream>
using namespace std;

class A
{
public:
	void funcA()
	{

	}
};

//B has A
class B
{
public:
	void funcB()
	{
		a.funcA();
	}
	A a;
};

//C中的某一个成员函数， 将A类当一个形参传递竟来
//C use A
class C
{
public:
	void funcc(A  *a)
	{
		a->funcA();
	}
};

//D继承A D is A
class D:public A
{
public:
	void funcD()
	{
		A::funcA();//直接使用A类的方法
		funcA();
	}
};

int main()
{
    std::cout << "Hello World!\n"; 
}
```
一个B类继承A类，或者从类A派生类B,类A称为基类(父类)，类B称为派生类(子类)
1. 派生类全盘接受，除了 **构造器** 和 **析构器**。基类有可能会造成 派生类的成员冗余，所以说基类是需要设计的。
2. 派生类有了自己的个性，使派生类有了意义

## 2.2 继承的方式--单继承
一个派生类可以同时有多个基类，这种情况称为多重继承，派生类只有一个基类，称为单继承，下面从单继承讲起。
派生类成员的标识和访问
|  |public|protected|private
|:-----:|:----:|:---:|:---:|
|公有继承(public)|public|protected|不可见
|保护继承(protected)|protected|protected|不可见
|私有继承(private)|private|private|不可见

```C++
// 单继承.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

#if 0



class Base
{
public:
	int pub;
private:
	int pri;
protected:
	int pro;
};

class PUB :public Base
{
public:
	void func()
	{
		Base::pro = 10;
		Base::pub = 100;
		//pri=1000;不能继承私有属性
	}
};


class PRO :protected Base
{
public:
	void func1()
	{
		Base::pro = 10;
		Base::pub = 100;

	}
};
#endif // 0

class A
{
private:
	int a;
protected:
	int b;

public:
	int c;
	A()
	{
		a = 0;
		b = 0;
		c = 0;
	}
	void set(int a, int b, int c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
	}

};

class B :public A
{
public:
	void print()
	{
		//cout << "a = " << a;//这个不能访问
		cout << "b=" << b << endl;//这个在外部不能直接访问 b
		cout << "c=" << c << endl;
	}
};

class C :protected A
{
public:
	void print()
	{
		//cout << "a = " << a;//这个不能访问
		cout << "b=" << b << endl;//这个在外部不能直接访问 b
		cout << "c=" << c << endl;
	}
};

class D :private A
{
public:
	void print()
	{
		//cout << "a = " << a;//这个不能访问
		cout << "b=" << b << endl;//这个在外部不能直接访问 b
		cout << "c=" << c << endl;
	}
};

int main()
{

	B	bb;
	bb.print();
	bb.c;

	C  cc;
	//cc.c; c不可访问
	cc.print();

	D dd;
	//dd.c;c不可访问
	dd.print();
#if 0
	Base b;
	b.pub = 10;
	//b.pro =100;
	//b.pri =1000;

	PUB p;
	p.pub = 0;
    std::cout << "Hello World!\n"; 

	PRO pro;
#endif // 0
}
```

## 2.3 继承中的构造和析构
1. 子类对象可以当作父类对象使用
2. 子类对象可以直接赋值给父类对象
3. 子类对象可以直接初始化父类对象
4. 父类指针可以直接指向子类对象
5. 父类引用可以直接引用子类对象	

```C++
#include "pch.h"
#include <iostream>
using namespace std;
/*
子类对象可以当作父类对象使用
子类对象可以直接赋值给父类对象
子类对象可以直接初始化父类对象
父类指针可以直接指向子类对象
父类引用可以直接引用子类对象
*/

class Parent
{
public:
	void printP()
	{
		cout << "Parent::printP" << endl;
	}
};

class Child :public Parent
{
public:
	void printC()
	{
		cout << "Child::printc()" << endl;
	}
};
int main()
{
	Child c;
	c.printP();//子类对象可以当作父类对象使用

	Parent p = c;//由于子类拥有全部父类的内存空间，子类能够保证父类初始化完整
	//Child c2 = p;//父类不能完整的初始化 子类，子类的内存空间包含了 父类

	//父类指针可以直接指向子类对象。
	Parent *pp = &c;
	pp->printP();
	//Child *cc = &p;子类对象不能直接指向父类

    std::cout << "Hello World!\n"; 
}
```
* 子类是调用父类的 构造函数 和 析构函数来完成 构造和析构。
```C++
class Parent
{
public:
	void printP()
	{
		cout << "Parent::printP" << endl;
	}
	Parent(int a)
	{
		this->a = a;
		cout << "Parent::Parent(int a)" << endl;

	}
	~Parent()
	{
		cout << "~Parent()" << endl;
	}

private:
	int a;
};

class Child :public Parent
{
public:
	//子类继承于父类，父类中的成员变量应该用 父类的构造函数来初始化，使用初始化构造列表
	Child(int a,int b):Parent(a)
	{
		this->b = b;
		cout << "Child(int a,int b):Parent(a)" << endl;

	}
	~Child()
	{
		cout << "~Child()" << endl;
	}

	void printC()
	{
		Parent::printP();
		cout << "Child::printc()" << endl;
	}
private:
	int b;
};

int main()
{
	std::cout << "Hello World!\n"; 
	test1();
}

Hello World!
Parent::Parent(int a)
Child(int a,int b):Parent(a)
Parent::printP
Child::printc()
~Child()
~Parent()
```
1. 子类对象在创建时会首先调用父类的构造函数
2. 父类构造函数执行结束后，执行子类的构造函数
3. 当父类的构造函数有参数时，需要在子类的初始化列表中显示调用
4. 析构函数调用的先后顺序和 构造函数相反

## 2.4 构造和组合并存，构造和析构原则
* 先构造父类、再构造成员变量、最后构造自己
* 先析构自己、再析构成员变量、最后析构父类

```C++
// 继承和组合并存，构造和析构原则.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

class Object
{
public:
	Object(const char*s)
	{
		cout << "Object(const char*s)" << " " << s << endl;
	}
	~Object()
	{
		cout << "~Object()" << endl;
	}
	int as;
};

class Parent :public Object
{
public:
	Parent(const char *s):Object(s)
	{
		cout << "Parent(const char *s)" << endl;
	}
	~Parent()
	{
		cout << "~Parent()" << endl;
	}
};

class Child :public Parent
{
public:
	Child() :o2("o2"), o1("o1"), Parent("Parameter from Child")
	{
		cout << "Child()" << endl;
	}
	~Child()
	{
		cout << "~Child()" << endl;

	}
private:
	Object o1;//这里是组合
	Object o2;
};

void run()
{
	Child child;
}

int main()
{
	run();
    std::cout << "Hello World!\n"; 
}

Object(const char*s) Parameter from Child
Parent(const char *s)
Object(const char*s) o1
Object(const char*s) o2
Child()
~Child()
~Object()
~Object()
~Parent()
~Object()
Hello World!
```

## 2.5 继承中同名变量处理方法
1. 当子类成员变量与父类成员变量 **同名**时
2. 子类依然从父类继承同名成员
3. 当子类中通过作用域分辨符::进行同名成员区分(在派生类中使用基类的同名成员，显示地使用类名限定符)
4. 同名成员存储在内存中的不同位置
5. 基类成员的作用域延伸到所有派生类
6. 派生类的同名成员屏蔽基类的同名成员
![](picture/2019-05-04-21-21-08.png)
```C++
#include "pch.h"
#include <iostream>
class base
{
public:
	int a, b;
};
class derived :public base
{
public:
	int b, c;

};
void f()
{
	derived d;
	d.a = 1;
	d.base::b = 2;//同名成员变量和成员函数通过作用域分辨符区分
	d.b = 3;
	d.c = 4;
}
int main()
{
	std::cout << "Hello World!\n";
	f();
}
```

## 2.6 派生类中的static关键字
* 基类定义的静态成员，将被所有派生类 **共享**
* 根据静态成员自身的访问特性和派生类的继承方式，在类层次体系中具有不同的访问性质(遵循派生类的访问控制)
* 派生类中访问静态成员，用一下形式显示说明：
	类名::成员
或通过对象访问
	对象名.成员
```C++
// 派生类中的static关键字.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
using namespace std;

class A
{
public:
	static int s;
private:
};

int A::s = 0;//静态成员变量要在类的外部初始化

class B :public A
{
public:
private:

};

class C :protected B
{
public:

};

int main()
{
	B b;
	cout << b.s << endl;//可以在B中访问s
	B::s = 100;
	cout << b.s << endl;
	cout << A::s << endl;
	//C::s = 300;不可访问

    std::cout << "Hello World!\n"; 
}
```

## 2.7 多继承
* 语法：
派生类名::派生类名(参数总表):基类名 1 (参数表 1 )，基类名 2 (参数名 2 ).。。。。 基类名 n (参数名 n ),内嵌子对象 1 (参数表 1 )， 内嵌子对象 2 (参数表 2 ).。。。。。内嵌子对象 n (参数表 n )
{
	派生类新增成员初始化语句；
}

* 一个类有多个直接基类的继承关系称为多继承
![](picture/2019-05-05-09-16-49.png)

## 2.8 虚继承
如果一个派生类从多个基类派生，而这些基类又有一个共同的基类，则在对该基类中声明的名字进行访问时，可能产生二义性。
![](picture/2019-05-05-09-52-27.png)
![](picture/2019-05-05-09-52-50.png)
* 如果一个派生类从多个基类派生，而这些基类又有一个共同的基类，则在对改基类中声明的名字进行访问时，可能产生二义性。
* 如果在多条继承路径上有一个公共基类，那么在继承路径的某处汇合点，这个公共基类就会在派生类的对象中产生多个基类子对象。
* 要使这个公共基类在派生类中只产生一个子对象，必须对这个基类声明为 虚继承，使这个基类成为 虚基类。
* 虚继承声明使用关键字 
![](picture/2019-05-05-10-00-42.png)
![](picture/2019-05-05-10-01-18.png)



