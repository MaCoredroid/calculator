// calculator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
inline void error(const std::string& s)
{
	std::cout << "error" << '\n';
}

inline void error(const std::string& s, const std::string& s2)
{
	error(s + s2);
}



class Token
{
public:
	char kind;
	double value;
	Token(char ch) :kind(ch), value(0){}
	Token(char ch, double val):kind(ch),value(val){}

};
class Token_stream
{
public:
	Token_stream();
	Token get();
	void putback(Token t);
private:
	bool full;
	Token buffer;


};
Token_stream::Token_stream():full(false),buffer(0)
{

}
void Token_stream::putback(Token t)
{
	if (full)
	{
		error("putback into a full buffer");
	}
	buffer = t;
	full = true;
}
Token Token_stream::get()
{
	if (full)
	{
		full = false;
		return buffer;
	}
	char ch;
	std::cin >> ch;
	switch (ch) 
	{
	case';':
	case'q':
	case'(':case')':case'+':case'-':case'*':case'/':
	{
		return Token(ch); 
	}
	case'.':
	case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
	{
		std::cin.putback(ch);
		double val;
		std::cin >> val;
		return Token('8', val);

	}
	default:
		error("Bad Token");

	}


}
Token_stream ts;
double expression();

double primary()
{
	Token t = ts.get();
	switch (t.kind)
	{
	   case'(':
	   {
		   double d = expression();
		   t = ts.get();
		   if (t.kind != ')')
		   {
			   error("expected ')'");
		   }
		   return d;

	   }
	   case '8':
	   {
		   return t.value;
	   }
	   default:
	   {
		   error("primary expected");
	   }
	}

}
double term()
{
	double left = primary();
	Token t = ts.get();
	while (true)
	{
		switch (t.kind)
		{
		case'*':
		{
			left *= primary();
			t = ts.get();
			break;
		}
		case'/':
		{
			double d = primary();
			if (d == 0)
			{
				error("divided by zero");
			}
			else 
			{
				left /= d;
				t = ts.get();
				break;
			}
		}
		default:
			ts.putback(t);
			return left;
		}
	}

}
double expression()
{
	double left = term();
	Token t = ts.get();
	while (true) {
		switch (t.kind)
		{
		case'+':
		{
			left += term();
			t = ts.get();
			break;
		}

		case'-':
		{
			left -= term();
			t = ts.get();
			break;
		}
		default:
			ts.putback(t);
			return left;


		}

	}


}
int main()
{
	
		double val = 0;
		while (std::cin)
		{
			Token t = ts.get();
			if (t.kind == 'q')
			{
				break;
			}
			if (t.kind == ';')
			{
				std::cout << "=" << val<<'\n' ;
			}
			else
			{
				ts.putback(t);
			}
			val = expression();
		}
		
	
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
