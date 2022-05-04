#pragma once
#include <string>

template < typename >
class Serializer;

template < typename >
class Deserializer;

template <>
class Serializer< std::string >
{
public:

	Serializer( const std::string& a_Serializable )
		: m_Serializable( &a_Serializable )
	{ }

	Serializer( const std::string* a_Serializable )
		: m_Serializable( a_Serializable )
	{ }

private:

	friend class Serialization;

	template < typename _StreamSerializer >
	void Serialize( _StreamSerializer& a_Serializer ) const
	{
		size_t Size = m_Serializable->size();
		a_Serializer.m_Stream.Write( &Size, sizeof( size_t ) );
		a_Serializer.m_Stream.Write( m_Serializable->c_str(), sizeof( char ) * m_Serializable->size() );
	}

	const std::string* m_Serializable;
};

template <>
class Deserializer< std::string >
{
public:

	Deserializer( std::string& a_Deserializable )
		: m_Deserializable( &a_Deserializable )
	{ }

	Deserializer( std::string* a_Deserializable )
		: m_Deserializable( a_Deserializable )
	{ }

private:

	friend class Serialization;

	template < typename _StreamDeserializer >
	void Deserialize( _StreamDeserializer& a_Deserializer ) const
	{
		//a_Deserializer.m_Stream.Read( m_Deserializable, sizeof( T ) );
		size_t Size;
		a_Deserializer.m_Stream.Read( &Size, sizeof( size_t ) );
		m_Deserializable->resize( Size );
		a_Deserializer.m_Stream.Read( m_Deserializable->data(), sizeof( char ) * Size );
	}

	std::string* m_Deserializable;
};