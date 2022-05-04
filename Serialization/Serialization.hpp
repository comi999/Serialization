#pragma once
#include <stdio.h>
#include <type_traits>

class Serialization
{
	template < typename > friend class Serializable;

	template < typename T >
	static constexpr auto _HasSerializationImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().Serialize( std::declval< void*& >() ) ), void >::type;

	template < typename >
	static constexpr std::false_type _HasSerializationImpl( ... );

	template < typename T >
	using _HasSerialization = decltype( _HasSerializationImpl< T >( 0 ) );

public:

	template < typename T >
	static constexpr bool HasSerialization = _HasSerialization< T >::value;

private:

	template < typename T >
	static constexpr auto _HasDeserializationImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().Deserialize( std::declval< void*& >() ) ), void >::type;

	template < typename >
	static constexpr std::false_type _HasDeserializationImpl( ... );

	template < typename T >
	using _HasDeserialization = decltype( _HasDeserializationImpl< T >( 0 ) );

public:

	template < typename T >
	static constexpr bool HasDeserialization = _HasDeserialization< T >::value;

	template < typename _Serializable, typename _Serializer >
	inline static void Serialize( const _Serializable& a_Serializable, _Serializer& a_Serializer )
	{
		if constexpr ( HasSerialization< _Serializable > )
		{
			a_Serializable.Serialize( a_Serializer );
		}
		else
		{
			Serializable< _Serializable >( a_Serializable ).Serialize( a_Serializer );
		}
	}

private:

	Serialization( Serialization&& ) = delete;
};

template < typename T >
class Serializable
{
public:

	Serializable( const T& a_Serializable )
		: m_Serializable( a_Serializable )
	{ }

private:

	friend class Serialization;

	template < typename _Serializer >
	void Serialize( _Serializer& a_Serializer ) const
	{

	}

	const T& m_Serializable;
};

// other overloads for common stl types.

template < typename _Stream >
class StreamSerializer
{
	typedef _Stream                     _Base;
	typedef StreamSerializer< _Stream > _This;

public:

	StreamSerializer( _Stream& a_Stream )
		: m_Stream( a_Stream )
	{ }

	template < typename T >
	_This& operator << ( const T& a_Serializable )
	{
		Serialization::Serialize( a_Serializable, *this );
		return *this;
	}

private:


	StreamSerializer( StreamSerializer&& ) = delete;

	_Stream& m_Stream;
};

class FileStream
{
public:

	FileStream()
		: m_File( 0 )
	{ }

	~FileStream()
	{
		Close();
	}

	inline void Create( const char* a_Path )
	{
		if ( m_File )
		{
			fclose( m_File );
		}

		fopen_s( &m_File, a_Path, "wb+" );
	}

	inline void Open( const char* a_Path )
	{
		if ( m_File )
		{
			fclose( m_File );
		}

		fopen_s( &m_File, a_Path, "rb+" );
		_ASSERT_EXPR( m_File, "File does not exist." );
	}

	inline void Close()
	{
		fclose( m_File );
	}

	inline void Write( const void* a_From, size_t a_Size )
	{
		fwrite( a_From, 1, a_Size, m_File );
	}

	inline void Read( void* a_To, size_t a_Size )
	{
		fread( a_To, 1, a_Size, m_File );
	}

	inline void Seek( size_t a_Position )
	{
		fseek( m_File, a_Position, SEEK_SET );
	}

	inline size_t Size() const
	{
		if ( !m_File )
		{
			return 0;
		}

		size_t Position = ftell( m_File );
		fseek( m_File, 0, SEEK_END );
		size_t Size = ftell( m_File );
		fseek( m_File, Position, SEEK_SET );
		return Size;
	}

private:

	template < typename > friend class SerializationStreamer;
	template < typename > friend class DeserializationStreamer;

	FILE* m_File;
};