#pragma once
#include <string>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <queue>

template < typename >
class Serializer;

template < typename >
class Deserializer;

template < typename... Args >
class Serializer< std::pair< Args... > >
{
	using Type = std::pair< Args... >;

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
		a_Serializer << m_Serializable->first << m_Serializable->second;
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::pair< Args... > >
{
	using Type = std::pair< Args... >;

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
		a_Deserializer >> m_Deserializable->first >> m_Deserializable->second;
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::tuple< Args... > >
{
	using Type = std::tuple< Args... >;

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
		if constexpr ( sizeof...( Args ) > 0 )
		{
			a_Serializer << m_Serializable->_Myfirst._Val;
		}

		if constexpr ( sizeof...( Args ) > 1 )
		{
			a_Serializer << m_Serializable->_Get_rest();
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::tuple< Args... > >
{
	using Type = std::tuple< Args... >;

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
		if constexpr ( sizeof...( Args ) > 0 )
		{
			a_Deserializer >> m_Deserializable->_Myfirst._Val;
		}

		if constexpr ( sizeof...( Args ) > 1 )
		{
			a_Deserializer >> m_Deserializable->_Get_rest();
		}
	}

	Type* m_Deserializable;
};

template < typename T, size_t _Size >
class Serializer< T( * )[ _Size ] >
{
	using Type = T( * )[ _Size ];

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
		a_Serializer << _Size;

		for ( size_t i = 0; i < _Size; ++i )
		{
			a_Serializer << ( *m_Serializable )[ i ];
		}
	}

	const Type* m_Serializable;
};

template < typename T, size_t _Size >
class Deserializer< T( * )[ _Size ] >
{
	using Type = T( * )[ _Size ];

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
		size_t Size;
		a_Deserializer >> Size;
		Size = Size > _Size ? _Size : Size;
		
		for ( size_t i = 0; i < Size; ++i )
		{
			a_Deserializer >> ( *m_Deserializable )[ i ];
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::basic_string< Args... > >
{
	using Type = std::basic_string< Args... >;

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
		a_Serializer << m_Serializable->size();
		a_Serializer.m_Stream.Write( m_Serializable->data(), sizeof( typename Type::value_type ) * m_Serializable->size() );
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::basic_string< Args... > >
{
	using Type = std::basic_string< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;
		m_Deserializable->resize( Size );
		a_Deserializer.m_Stream.Read( m_Deserializable->data(), sizeof( typename Type::value_type ) * Size );
	}

	Type* m_Deserializable;
};

template < typename T, size_t _Size >
class Serializer< std::array< T, _Size > >
{
	using Type = std::array< T, _Size >;

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
		a_Serializer << _Size;

		for ( auto& Element : *m_Serializable )
		{
			a_Serializer << Element;
		}
	}

	const Type* m_Serializable;
};

template < typename T, size_t _Size >
class Deserializer< std::array< T, _Size > >
{
	using Type = std::array< T, _Size >;

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
		size_t Size;
		a_Deserializer >> Size;
		Size = Size > _Size ? _Size : Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			a_Deserializer >> m_Deserializable->at( i );
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::vector< Args... > >
{
	using Type = std::vector< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Element : *m_Serializable )
		{
			a_Serializer << Element;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::vector< Args... > >
{
	using Type = std::vector< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;
		m_Deserializable->resize( Size );
		
		for ( auto& Element : *m_Deserializable )
		{
			a_Deserializer >> Element;
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::deque< Args... > >
{
	using Type = std::deque< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Element : *m_Serializable )
		{
			a_Serializer << Element;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::deque< Args... > >
{
	using Type = std::deque< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			a_Deserializer >> m_Deserializable->emplace_back();
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::forward_list< Args... > >
{
	using Type = std::forward_list< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Element : *m_Serializable )
		{
			a_Serializer << Element;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::forward_list< Args... > >
{
	using Type = std::forward_list< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			a_Deserializer >> m_Deserializable->emplace_back();
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::list< Args... > >
{
	using Type = std::list< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Element : *m_Serializable )
		{
			a_Serializer << Element;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::list< Args... > >
{
	using Type = std::list< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			a_Deserializer >> m_Deserializable->emplace_back();
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::map< Args... > >
{
	using Type = std::map< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Pair : *m_Serializable )
		{
			a_Serializer << Pair;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::map< Args... > >
{
	using Type = std::map< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			typename Type::key_type Key;
			a_Deserializer >> Key;
			a_Deserializer >> ( *m_Deserializable )[ Key ];
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::multimap< Args... > >
{
	using Type = std::multimap< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Pair : *m_Serializable )
		{
			a_Serializer << Pair;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::multimap< Args... > >
{
	using Type = std::multimap< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			typename Type::key_type Key;
			a_Deserializer >> Key;
			a_Deserializer >> ( *m_Deserializable )[ Key ];
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::unordered_map< Args... > >
{
	using Type = std::unordered_map< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Pair : *m_Serializable )
		{
			a_Serializer << Pair;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::unordered_map< Args... > >
{
	using Type = std::unordered_map< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			typename Type::key_type Key;
			a_Deserializer >> Key;
			a_Deserializer >> ( *m_Deserializable )[ Key ];
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::unordered_multimap< Args... > >
{
	using Type = std::unordered_multimap< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Pair : *m_Serializable )
		{
			a_Serializer << Pair;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::unordered_multimap< Args... > >
{
	using Type = std::unordered_multimap< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			typename Type::key_type Key;
			a_Deserializer >> Key;
			a_Deserializer >> ( *m_Deserializable )[ Key ];
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::set< Args... > >
{
	using Type = std::set< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Element : *m_Serializable )
		{
			a_Serializer << Element;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::set< Args... > >
{
	using Type = std::set< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			typename Type::value_type Value;
			a_Deserializer >> Value;
			m_Deserializable->emplace( Value );
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::multiset< Args... > >
{
	using Type = std::multiset< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Element : *m_Serializable )
		{
			a_Serializer << Element;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::multiset< Args... > >
{
	using Type = std::multiset< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			typename Type::value_type Value;
			a_Deserializer >> Value;
			m_Deserializable->emplace( Value );
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::unordered_set< Args... > >
{
	using Type = std::unordered_set< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Element : *m_Serializable )
		{
			a_Serializer << Element;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::unordered_set< Args... > >
{
	using Type = std::unordered_set< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			typename Type::value_type Value;
			a_Deserializer >> Value;
			m_Deserializable->emplace( Value );
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::unordered_multiset< Args... > >
{
	using Type = std::unordered_multiset< Args... >;

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
		a_Serializer << m_Serializable->size();

		for ( auto& Element : *m_Serializable )
		{
			a_Serializer << Element;
		}
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::unordered_multiset< Args... > >
{
	using Type = std::unordered_multiset< Args... >;

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
		size_t Size;
		a_Deserializer >> Size;

		for ( size_t i = 0; i < Size; ++i )
		{
			typename Type::value_type Value;
			a_Deserializer >> Value;
			m_Deserializable->emplace( Value );
		}
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::stack< Args... > >
{
	using Type = std::stack< Args... >;

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
		a_Serializer << m_Serializable->_Get_container();
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::stack< Args... > >
{
	using Type = std::stack< Args... >;

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
		a_Deserializer >> const_cast< typename Type::container_type& >( m_Deserializable->_Get_container() );
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::queue< Args... > >
{
	using Type = std::queue< Args... >;

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
		a_Serializer << m_Serializable->_Get_container();
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::queue< Args... > >
{
	using Type = std::queue< Args... >;

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
		a_Deserializer >> const_cast< typename Type::container_type& >( m_Deserializable->_Get_container() );
	}

	Type* m_Deserializable;
};

template < typename... Args >
class Serializer< std::priority_queue< Args... > >
{
	using Type = std::priority_queue< Args... >;

	struct Access : Type
	{
		auto& _Get_Container() const
		{
			return Type::c;
		}
	};

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
		a_Serializer << reinterpret_cast< const Access* >( m_Serializable )->_Get_Container();
	}

	const Type* m_Serializable;
};

template < typename... Args >
class Deserializer< std::priority_queue< Args... > >
{
	using Type = std::priority_queue< Args... >;

	struct Access : Type
	{
		auto& _Get_Container()
		{
			return Type::c;
		}
	};

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
		a_Deserializer >> reinterpret_cast< Access* >( m_Deserializable )->_Get_Container();
	}

	Type* m_Deserializable;
};