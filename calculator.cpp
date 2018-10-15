

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>

const char number = '8';
const char quit = 'q';
const char print = ';';
std::ifstream in("input.txt");
std::ofstream out("output.txt");
double ANS = 0;
void clean_up_mess();
inline void error(const std::string& s)
{
	if (!in.eof())
	{
		out << "error" << '\n';
	}
	clean_up_mess();
	throw std::runtime_error(s);
}



class Token
{
public:
	char kind;
	double value;
	Token(char ch) :kind(ch), value(0) {}
	Token(char ch, double val) :kind(ch), value(val) {}

};
class Token_stream
{
public:
	Token_stream();
	Token get();
	void putback(Token t);
	void ignore(char c);
private:
	bool full;
	Token buffer;


};

void Token_stream::ignore(char c)
{
	if (full&&c == buffer.kind)
	{
		full = false;
		return;
	}
	full = false;
	char ch = 0;
	while (in >> ch)
	{
		if (ch == c)
		{
			return;
		}
	}
}
Token_stream::Token_stream() :full(false), buffer(0)
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

	in >> ch;
	switch (ch)
	{
	case'A': {
		char temp1;
		char temp2;
		in >> temp1;
		in >> temp2;
		if (temp1 == 'N'&&temp2 == 'S')
		{
			return Token(number, ANS);
		}
		else
		{
			error("Bad Token");
		}
		break;
	}
	case print:
	case quit:
	case'(':
	case')':
	case'+':
	case'-':
	case'*':
	case'!':
	case'/':
	{
		return Token(ch);
	}
	case'.':
	case'0':case'1':case'2':case'3':case'4':case'5':case'6':case'7':case'8':case'9':
	{
		in.putback(ch);
		double val;
		in >> val;
		return Token(number, val);

	}
	default:
		error("Bad Token");

	}




}
Token_stream ts;
double expression();

void clean_up_mess()
{
	ts.ignore(print);
}


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
	case number:
	{
		return t.value;
	}
	case'+':
	{
		return primary();
	}
	case'-':
	{
		return -primary();
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
		case'%':
		{
			double d = term();
			int i1 = int(left);
			if (i1 != left)
			{
				error("left-hand of oprand not int");
			}
			int i2 = int(d);
			if (i2 != d)
			{
				error("right-hand of oprand not int");
			}
			if (i2 == 0)
			{
				error("divided by zero");
			}
			left = i1 % i2;
			t = ts.get();
			break;
		}
		case'!':
		{
			int temp = 1;
			if (left != int(left))
			{
				error("only int has factorial");
			}
			if (left < 0)
			{
				error("only positive int has factorial");
			}
			else
			{
				for (int i = 1; i <= left; i++)
				{
					temp *= i;
				}
				left = temp;
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
void calculate()
{
	while (!in.eof())
	{
		double ans = 0;
		try
		{
		Token t = ts.get();
		while (t.kind == print)
		{
			t = ts.get();
		}
		if (t.kind == quit)
		{
			return;
		}
		ts.putback(t);
		ans = expression();
		ANS = ans;
		out << ans << '\n';
		}
		catch (...)
		{
			continue;
		}
	}
}

int main()
{

	calculate();
	return 0;


}

