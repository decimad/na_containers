#pragma once
#include <boost/integer_traits.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/optional.hpp>
#include <algorithm>
#include <vector>

namespace na {
	
	enum _na_enum {
		NA
	};

	namespace detail {

		template< typename T > struct special_value_default {
			static const T value = boost::integral_traits<T>::const_min;
		};
		template<> struct special_value_default< double > {
			static const double value;
		};
		template<> struct special_value_default< float > {
			static const float value;
		};

		template< typename ValueType, typename ValueStruct = special_value_default<ValueType> >
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

	template< typename ValueType, class NaPolicy = detail::NaPolicySV< ValueType >, typename Allocator = std::allocator<ValueType> >
	class na_vector : public NaPolicy {
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

		class filtered_list {
		public:
			filtered_list( iterator& _begin, iterator& _end )
				: begin_(_begin), end_(_end)
			{
			}

			struct is_na_predicate {
				bool operator()( const_reference val ) {
					return val != NaPolicy::get_na();
				}
			};

			typedef boost::filter_iterator<is_na_predicate, typename na_vector::iterator> iterator;
			typedef boost::filter_iterator<is_na_predicate, typename na_vector::const_iterator>  const_iterator;

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
			typename na_vector::iterator begin_, end_;
		};

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

		void push_back( na_type )
		{
			data_.push_back( NaPolicy::get_na( data_.size() ) );
		}

		void push_back( _na_enum ) {
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




}

