#include <iostream>
#include "Serialization.hpp"


class Class
{
public:

	Class() = default;

	Class( int _a, float _b, double _c, bool _d )
		: a( _a )
		, b( _b )
		, c( _c )
		, d( _d )
	{ }

private:

	friend class Serialization;

	template < typename T >
	void Serialize( T& t ) const
	{
		t << a << b << c << d;
	}

	template < typename T >
	void SizeOf( T& a_Sizer ) const
	{
		a_Sizer & a & b & c & d & fl;
	}

	template < typename T >
	void Deserialize( T& t )
	{
		t >> a >> b >> c >> d;
	}

	int a;
	float b;
	double c;
	bool d;
	float fl[ 4 ];
};


class Class2
{
private:

	friend class Serialization;

	template < typename T >
	void Serialize( T& t ) const
	{
		t << a << b << c << d;
	}

	template < typename T >
	void Deserialize( T& t )
	{
		t >> a >> b >> c >> d;
	}

	template < typename T >
	void SizeOf( T& a_Sizer ) const
	{
		a_Sizer & a & b & c & d;
	}

public:

	Class a, b, c, d;
	std::string someS;
};

#include "STLSerialization.hpp"

int main()
{

}