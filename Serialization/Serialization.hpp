#pragma once
#include <stdio.h>
#include <type_traits>

template < typename >
class Serializer;

template < typename >
class Deserializer;

class Serialization
{
	#pragma region HasOnBeforeSerialize

	template < typename T >
	static constexpr auto _HasOnBeforeSerializeImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().OnBeforeSerialize() ), void >::type;

	template < typename >
	static constexpr std::false_type _HasOnBeforeSerializeImpl( ... );

	template < typename T >
	using _HasOnBeforeSerialize = decltype( _HasOnBeforeSerializeImpl< T >( 0 ) );

	#pragma endregion

	#pragma region HasSerialize

	template < typename T >
	static constexpr auto _HasSerializeImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().Serialize( std::declval< void*& >() ) ), void >::type;

	template < typename >
	static constexpr std::false_type _HasSerializeImpl( ... );

	template < typename T >
	using _HasSerialize = decltype( _HasSerializeImpl< T >( 0 ) );

	#pragma endregion

	#pragma region HasOnAfterSerialize

	template < typename T >
	static constexpr auto _HasOnAfterSerializeImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().OnAfterSerialize() ), void >::type;

	template < typename >
	static constexpr std::false_type _HasOnAfterSerializeImpl( ... );

	template < typename T >
	using _HasOnAfterSerialize = decltype( _HasOnAfterSerializeImpl< T >( 0 ) );

	#pragma endregion

	#pragma region HasOnBeforeDeserialize

	template < typename T >
	static constexpr auto _HasOnBeforeDeserializeImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().OnBeforeDeserialize() ), void >::type;

	template < typename >
	static constexpr std::false_type _HasOnBeforeDeserializeImpl( ... );

	template < typename T >
	using _HasOnBeforeDeserialize = decltype( _HasOnBeforeDeserializeImpl< T >( 0 ) );

	#pragma endregion

	#pragma region HasDeserialize

	template < typename T >
	static constexpr auto _HasDeserializeImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().Deserialize( std::declval< void*& >() ) ), void >::type;

	template < typename >
	static constexpr std::false_type _HasDeserializeImpl( ... );

	template < typename T >
	using _HasDeserialize = decltype( _HasDeserializeImpl< T >( 0 ) );

	#pragma endregion

	#pragma region HasOnAfterDeserialize

	template < typename T >
	static constexpr auto _HasOnAfterDeserializeImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().OnAfterDeserialize() ), void >::type;

	template < typename >
	static constexpr std::false_type _HasOnAfterDeserializeImpl( ... );

	template < typename T >
	using _HasOnAfterDeserialize = decltype( _HasOnAfterDeserializeImpl< T >( 0 ) );

	#pragma endregion

public:

	template < typename T >
	static constexpr bool HasOnBeforeSerialize = _HasOnBeforeSerialize< T >::value;

	template < typename T >
	static constexpr bool HasSerialize = _HasSerialize< T >::value;

	template < typename T >
	static constexpr bool HasOnAfterSerialize = _HasOnAfterSerialize< T >::value;

	template < typename T >
	static constexpr bool HasOnBeforeDeserialize = _HasOnBeforeDeserialize< T >::value;

	template < typename T >
	static constexpr bool HasDeserialize = _HasDeserialize< T >::value;

	template < typename T >
	static constexpr bool HasOnAfterDeserialize = _HasOnAfterDeserialize< T >::value;

	template < typename _Serializable, typename _Serializer >
	inline static void Serialize( const _Serializable& a_Serializable, _Serializer& a_Serializer )
	{
		if constexpr ( HasOnBeforeSerialize< _Serializable > )
		{
			const_cast< _Serializable& >( a_Serializable ).OnBeforeSerialize();
		}

		if constexpr ( HasSerialize< _Serializable > )
		{
			a_Serializable.Serialize( a_Serializer );
		}
		else
		{
			Serializer< _Serializable >( a_Serializable ).Serialize( a_Serializer );
		}

		if constexpr ( HasOnAfterSerialize< _Serializable > )
		{
			const_cast< _Serializable& >( a_Serializable ).OnAfterSerialize();
		}
	}

	template < typename _Deserializable, typename _Deserializer >
	inline static void Deserialize( _Deserializable& a_Deserializable, _Deserializer& a_Deserializer )
	{
		if constexpr ( HasOnBeforeDeserialize< _Deserializable > )
		{
			const_cast< _Deserializable& >( a_Deserializable ).OnBeforeDeserialize();
		}

		if constexpr ( HasDeserialize< _Deserializable > )
		{
			a_Deserializable.Deserialize( a_Deserializer );
		}
		else
		{
			Deserializer< _Deserializable >( a_Deserializable ).Deserialize( a_Deserializer );
		}

		if constexpr ( HasOnAfterDeserialize< _Deserializable > )
		{
			const_cast< _Deserializable& >( a_Deserializable ).OnAfterDeserialize();
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
	using Type = T;

public:

	Serializer( const Type& a_Serializable )
		: m_Serializable( &a_Serializable )
	{ }

	Serializer( const Type* a_Serializable )
		: m_Serializable( a_Serializable )
	{ }

private:

	friend class Serialization;

	template < typename _StreamSerializer >
	void Serialize( _StreamSerializer& a_Serializer ) const
	{
		a_Serializer.m_Stream.Write( m_Serializable, sizeof( Type ) );
	}

	const Type* m_Serializable;
};

template < typename T >
class Deserializer
{
	using Type = T;

public:

	Deserializer( Type& a_Deserializable )
		: m_Deserializable( &a_Deserializable )
	{ }

	Deserializer( Type* a_Deserializable )
		: m_Deserializable( a_Deserializable )
	{ }

private:

	friend class Serialization;

	template < typename _StreamDeserializer >
	void Deserialize( _StreamDeserializer& a_Deserializer ) const
	{
		a_Deserializer.m_Stream.Read( m_Deserializable, sizeof( Type ) );
	}

	Type* m_Deserializable;
};

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

		fopen_s( &m_File, a_Path, "rb+" );
		_ASSERT_EXPR( m_File, "File does not exist." );
	}

	inline void Close()
	{
		if ( !m_File )
		{
			return;
		}

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