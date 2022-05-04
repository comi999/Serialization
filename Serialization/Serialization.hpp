#pragma once
#include <stdio.h>
#include <type_traits>

template < typename >
class Serializer;

template < typename >
class Deserializer;

class Serialization
{
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
			Serializer< _Serializable >( a_Serializable ).Serialize( a_Serializer );
		}
	}

	template < typename _Deserializable, typename _Deserializer >
	inline static void Deserialize( _Deserializable& a_Deserializable, _Deserializer& a_Deserializer )
	{
		if constexpr ( HasDeserialization< _Deserializable > )
		{
			a_Deserializable.Deserialize( a_Deserializer );
		}
		else
		{
			Deserializer< _Deserializable >( a_Deserializable ).Deserialize( a_Deserializer );
		}
	}

private:

	Serialization( Serialization&& ) = delete;
};

template < typename _Stream >
class StreamSerializer
{
	typedef _Stream                     _Base;
	typedef StreamSerializer< _Stream > _This;

public:

	template < typename... Args >
	StreamSerializer( Args&&... a_Args )
		: m_Stream( std::forward< Args >( a_Args )... )
	{ }

	template < typename... Args >
	inline void Open( Args&&... a_Args )
	{
		m_Stream.Open( std::forward< Args >( a_Args )... );
	}

	inline void Close()
	{
		m_Stream.Close();
	}

	template < typename T >
	_This& operator << ( const T& a_Serializable )
	{
		Serialization::Serialize( a_Serializable, *this );
		return *this;
	}

private:

	template < typename > friend class Serializer;

	StreamSerializer( StreamSerializer&& ) = delete;

	_Stream m_Stream;
};

template < typename _Stream >
class StreamDeserializer
{
	typedef _Stream                       _Base;
	typedef StreamDeserializer< _Stream > _This;

public:

	template < typename... Args >
	StreamDeserializer( Args&&... a_Args )
		: m_Stream( std::forward< Args >( a_Args )... )
	{ }

	template < typename... Args >
	inline void Open( Args&&... a_Args )
	{
		m_Stream.Open( std::forward< Args >( a_Args )... );
	}

	inline void Close()
	{
		m_Stream.Close();
	}

	template < typename T >
	_This& operator >> ( T& a_Deserializable )
	{
		Serialization::Deserialize( a_Deserializable, *this );
		return *this;
	}

private:

	template < typename > friend class Deserializer;

	StreamDeserializer( StreamDeserializer&& ) = delete;

	_Stream m_Stream;
};

template < typename T >
class Serializer
{
public:

	Serializer( const T& a_Serializable )
		: m_Serializable( &a_Serializable )
	{ }

	Serializer( const T* a_Serializable )
		: m_Serializable( a_Serializable )
	{ }

private:

	friend class Serialization;

	template < typename _StreamSerializer >
	void Serialize( _StreamSerializer& a_Serializer ) const
	{
		a_Serializer.m_Stream.Write( m_Serializable, sizeof( T ) );
	}

	const T* m_Serializable;
};

template < typename T >
class Deserializer
{
public:

	Deserializer( T& a_Deserializable )
		: m_Deserializable( &a_Deserializable )
	{ }

	Deserializer( T* a_Deserializable )
		: m_Deserializable( a_Deserializable )
	{ }

private:

	friend class Serialization;

	template < typename _StreamDeserializer >
	void Deserialize( _StreamDeserializer& a_Deserializer ) const
	{
		a_Deserializer.m_Stream.Read( m_Deserializable, sizeof( T ) );
	}

	T* m_Deserializable;
};

// other overloads for common stl types.

class FileStream
{
public:

	FileStream()
		: m_File( nullptr )
	{ }

	FileStream( const char* a_Path )
	{
		Open( a_Path );
	}

	~FileStream()
	{
		Close();
	}

	inline void Open( const char* a_Path )
	{
		if ( m_File )
		{
			Close();
		}

		fopen_s( &m_File, a_Path, "wb+" );
		fseek( m_File, 0, SEEK_SET );
		_ASSERT_EXPR( m_File, "File does not exist." );
	}

	inline void Close()
	{
		fclose( m_File );
		m_File = nullptr;
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

	template < typename > friend class Serializer;
	template < typename > friend class Deserializer;

	FILE* m_File;
};

typedef StreamSerializer  < FileStream > FileSerializer;
typedef StreamDeserializer< FileStream > FileDeserializer;