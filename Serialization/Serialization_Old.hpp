#pragma once
#include <type_traits>
#include <stdio.h>

template < typename >
class SerializationStreamer;

template < typename >
class DeserializationStreamer;

class FileStream;

class Serialization
{
private:
public:

	template < typename > friend class SerializationStreamer;
	template < typename > friend class DeserializationStreamer;

	template < typename T >
	static constexpr auto _IsSerializableImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().Serialize( std::declval< void*& >() ) ), void >::type;

	template < typename >
	static constexpr std::false_type _IsSerializableImpl( ... );

	template < typename T >
	using _IsSerializable = decltype( _IsSerializableImpl< T >( 0 ) );

	template < typename T >
	static constexpr bool IsSerializable = _IsSerializable< T >::value;

	template < typename T >
	static constexpr auto _IsDeserializableImpl( T* ) ->
		typename std::is_same< decltype( std::declval< T& >().Deserialize( std::declval< void*& >() ) ), void >::type;

	template < typename >
	static constexpr std::false_type _IsDeserializableImpl( ... );

	template < typename T >
	using _IsDeserializable = decltype( _IsDeserializableImpl< T >( 0 ) );

	template < typename T >
	static constexpr bool IsDeserializable = _IsDeserializable< T >::value;

public:

	template < typename _Serializable, typename _Serializer >
	inline static void InvokeSerialization( const _Serializable& a_Serializable, _Serializer& a_Serializer )
	{
		a_Serializable.Serialize( a_Serializer );
	}

	template < typename _Deserializable, typename _Deserializer >
	inline static void InvokeDeserialization( _Deserializable& a_Deserializable, _Deserializer& a_Deserializer )
	{
		a_Deserializable.Deserialize( a_Deserializer );
	}
};



template < typename _Stream >
class SerializationStreamer
{
public:

	template < typename... Args >
	SerializationStreamer( Args&&... a_Args )
	{
		m_Stream.Open( std::forward< Args >( a_Args )... );
	}

	using ThisT = SerializationStreamer< _Stream >;

	template < typename T, bool _IsSerializable = Serialization::IsSerializable< T > >
	ThisT& operator<<( T&& a_Object )
	{
		if constexpr ( _IsSerializable )
		{
			Serialization::InvokeSerialization( a_Object, *this );
		}
		else
		{
			m_Stream.Write( &a_Object, sizeof( T ) );
		}

		return *this;
	}

	_Stream m_Stream;
};

template < typename _Stream >
class DeserializationStreamer
{
public:

	template < typename... Args >
	DeserializationStreamer( Args&&... a_Args )
	{
		m_Stream.Open( std::forward< Args >( a_Args )... );
	}

	using ThisT = DeserializationStreamer< _Stream >;

	template < typename T, bool _IsDeserializable = Serialization::IsDeserializable< T > >
	ThisT& operator >>( T& a_Object )
	{
		if constexpr ( _IsDeserializable )
		{
			Serialization::InvokeDeserialization( a_Object, *this );
		}
		else
		{
			m_Stream.Read( &a_Object, sizeof( T ) );
		}

		return *this;
	}

	_Stream m_Stream;
};

typedef SerializationStreamer  < FileStream > FileSerializer;
typedef DeserializationStreamer< FileStream > FileDeserializer;