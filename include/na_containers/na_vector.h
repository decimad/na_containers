#pragma once
#include <boost/integer_traits.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/optional.hpp>
#include <algorithm>
#include <vector>

namespace na {

	namespace detail {
		
		struct _na_type {
		};

		template< typename T > struct special_value_default {
			static const T value = boost::integral_traits<T>::const_min;
		};
		template<> struct special_value_default< double > {
			static const double value;
		};
		template<> struct special_value_default< float > {
			static const float value;
		};

		template< typename Iterator, typename Filter >
		class filtered_list {
		public:
			filtered_list( Iterator& _begin, Iterator& _end )
				: begin_(_begin), end_(_end)
			{
			}

			struct is_na_predicate {
				bool operator()( const typename Iterator::reference val ) {
					return !Filter::is_na( val );
				}
			};

			typedef boost::filter_iterator<is_na_predicate, Iterator> iterator;
			typedef boost::filter_iterator<is_na_predicate, Iterator>  const_iterator;

			iterator begin()
			{
				return iterator( begin_, end_ );
			}

			const_iterator cbegin() const
			{
				return const_iterator( begin_, end_ );
			}

			const_iterator begin() const
			{
				return const_iterator( end_, end_ );
			}

			iterator end()
			{
				return iterator( end_, end_ );
			}

			const_iterator cend() const
			{
				return const_iterator( end_, end_ );
			}

			const_iterator end() const
			{
				return const_iterator( end_, end_ );
			}

		private:
			typename Iterator begin_, end_;
		};

	}

	namespace policies {
		
		template< typename ValueType, typename ValueStruct = detail::special_value_default<ValueType> >
		class NaPolicySV {
		public:
			typedef ValueType value_type;

		public:
			static bool is_na( const value_type& val )
			{
				return val == get_na();
			}

			static const value_type get_na()
			{
				return ValueStruct::value;
			}
		};

		template< typename ValueType >
		class NaPolicyOptional {
		public:
			typedef boost::optional< ValueType > value_type;

		public:
			static bool is_na( const value_type& val )
			{
				return val.is_initialized();
			}

			static const value_type get_na()
			{
				return value_type();
			}
		};

	}

	extern detail::_na_type NA;

	template< typename ValueType, class NaPolicy = policies::NaPolicySV< ValueType >, typename Allocator = std::allocator<ValueType> >
	class na_vector : private NaPolicy {
	public:
		typedef typename NaPolicy::value_type value_type;
		typedef std::vector< value_type, Allocator > container_type;

		typedef value_type& reference;
		typedef const value_type& const_reference;

		typedef Allocator allocator_type;
		typedef typename container_type::size_type size_type;

		typedef typename container_type::iterator       iterator;
		typedef typename container_type::const_iterator const_iterator;
				
		struct na_type_t { };
		typedef na_type_t na_type;

	public:
		typedef typename container_type::iterator iterator;
		typedef typename container_type::const_iterator const_iterator;

		typedef detail::filtered_list< iterator, NaPolicy > filtered_list;

		iterator begin() {
			return data_.begin();
		}

		iterator end() {
			return data_.end();
		}


		const_iterator begin() const {
			return data_.begin();
		}

		const_iterator end() const {
			return data_.end();
		}

		const_iterator cbegin() const {
			return begin();
		}

		const_iterator cend() const {
			return end();
		}

		bool is_na( const const_iterator& it ) {
			return NaPolicy::is_na( it );
		}
		
		const_reference operator[]( size_type index ) const
		{
			return data_[index];
		}

		void push_back( const_reference val )
		{
			data_.push_back( val );
		}

		void push_back( detail::_na_type ) {
			data_.push_back( NaPolicy::get_na() );
		}

		void pop_back()
		{
			data_.pop_back();
		}

		filtered_list filtered()
		{
			return filtered_list( begin(), end() );
		}
		
	private:
		container_type data_;
	};


	template< typename ValueType, class NaPolicy = policies::NaPolicySV< ValueType >, typename Allocator = std::allocator<NaPolicy::value_type> >
	class na_vector2 : private NaPolicy, public std::vector< typename NaPolicy::value_type, Allocator > {
	public:
		typedef typename vector::value_type value_type;

		// na interface
	public:
		// using vector::vector;	// *sniff*

		typedef detail::filtered_list< iterator, NaPolicy > filtered_list;

		static value_type get_na() {
			return NaPolicy::get_na();
		}

		filtered_list filtered()
		{
			return filtered_list( begin(), end() );
		}

		// vector interface
	public:

		using vector::push_back;

		void push_back( detail::_na_type ) {
			vector::push_back( get_na() );
		}

		using vector::insert;

		iterator insert( const_iterator _Where, size_type _Count, const detail::_na_type& ) {
			return vector::insert( _Where, _Count, get_na() );
		}

		iterator insert( const_iterator _Where, const detail::_na_type&& ) {
			return vector::insert( _Where, get_na() );
		}

		iterator insert( const_iterator _Where, const detail::_na_type& ) {
			return vector::insert( _Where, get_na() );
		}

		using vector::resize;

		void resize(size_type _Newsize, const detail::_na_type& )
		{
			vector::resize( _Newsize, get_na() );
		}
		
		using vector::assign;

		void assign( size_type _Count, const detail::_na_type& _Val )
		{
			vector::assign( _Count, get_na() );
		}

	};

}

