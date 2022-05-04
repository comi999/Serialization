#include <iostream>
#include "Serialization.hpp"


class Class
{
public:

	Class() = default;

	Class( int _a, float _b, double _c, bool _d )
		: a(_a)
		, b(_b)
		, c(_c)
		, d(_d)
	{}

private:

	friend class Serialization;

	template < typename T >
	void Serialize( T& a_Serializer ) const
	{
		a_Serializer << a << b << c << d;
	}

	template < typename T >
	void Deserialize( T& a_Deserializer )
	{
		a_Deserializer >> a >> b >> c >> d;
	}

	int a;
	float b;
	double c;
	bool d;
};


class Class2
{
public:

private:

	friend class Serialization;

	template < typename T >
	void Serialize( T& a_Serializer ) const
	{
		a_Serializer << a << b << c << d;
	}

	template < typename T >
	void Deserialize( T& a_Deserializer )
	{
		a_Deserializer >> a >> b >> c >> d;
	}

public:

	Class a, b, c, d;
};

#include "STLSerialization.hpp"

int main()
{
	/*Class2 c = {
		{ 1, 1.1, 1.11, false },
		{ 2, 2.2, 2.22, true },
		{ 3, 3.3, 3.33, false },
		{ 4, 4.4, 4.44, true }
	};*/

	/*FileSerializer f( "./testfile.file" );
	
	std::string str = "SomeString";
	f << str;

	f.Close();

	std::string out;
	FileDeserializer fd( "./testfile.file" );
	fd >> out;
	fd.Close();*/

	FILE* File;
	fopen_s( &File, "./test.file", "wb+" );
	fwrite( "Test", sizeof( char ), 4, File );
	fclose( File );

	char text[ 4 ];
	fopen_s( &File, "./test.file", "wb+" );
	fread( &text, sizeof( char ), 4, File );
	fclose( File );
}