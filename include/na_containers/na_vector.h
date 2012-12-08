#pragma once
#include <boost/integer_traits.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/optional.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <algorithm>
#include <vector>

#define noexcept

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

		typedef std::ptrdiff_t difference_type;

		typedef typename container_type::iterator       iterator;
		typedef typename container_type::const_iterator const_iterator;
				
		typedef typename container_type::reverse_iterator reverse_iterator;
		typedef typename container_type::const_reverse_iterator const_reverse_iterator;

	public:
		typedef typename container_type::iterator iterator;
		typedef typename container_type::const_iterator const_iterator;

		typedef detail::filtered_list< iterator, NaPolicy > filtered_list;

	public:
		static value_type get_na() {
			return NaPolicy::get_na();
		}

		bool is_na( const const_iterator& it ) {
			return NaPolicy::is_na( it );
		}

		filtered_list filtered()
		{
			return filtered_list( begin(), end() );
		}

		// vector interface
	public:
		explicit na_vector (const allocator_type& alloc = allocator_type())
			: data_( alloc )
		{

		}

		explicit na_vector (size_type n)
			: data_( n )
		{

		}
		
		na_vector (size_type n, const value_type& val,
			const allocator_type& alloc = allocator_type())
			: data_( n, val, alloc )
		{

		}

		template <class InputIterator>
		na_vector (InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type())
			: vector( first, last, alloc )
		{

		}
		
		na_vector (const na_vector& x)
			: data_( x )
		{

		}

		na_vector (const na_vector& x, const allocator_type& alloc)
			: data_( x, alloc )
		{

		}

		na_vector (na_vector&& x)
			: data_( std::move( x.data_ ) )
		{

		}

		na_vector (na_vector&& x, const allocator_type& alloc)
			: data_( std::move( x.data_ ), alloc )
		{
		}

		/*
		na_vector (initializer_list<value_type> il,
			const allocator_type& alloc = allocator_type());
		*/


		na_vector& operator= (const na_vector& x)
		{
			return vector::operator ==( x );
		}

		na_vector& operator= (na_vector&& x)
		{
			return vector::operator ==( std::move( x ) );
		}

	public:
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

		reverse_iterator rbegin() {
			return data_.rbegin();
		}

		reverse_iterator rend() {
			return data_.rend();
		}

		const_reverse_iterator rbegin() const {
			return data_.rbegin();
		}

		const_reverse_iterator rend() const {
			return data_.rend();
		}

		const_reverse_iterator crbegin() const {
			return rbegin();
		}

		const_reverse_iterator crend() const {
			return rend();
		}
				
		const_reference operator[]( size_type index ) const
		{
			return data_[index];
		}

		reference operator[]( size_type index )
		{
			return data_[index];
		}

		void push_back( const_reference val )
		{
			data_.push_back( val );
		}

		void push_back( detail::_na_type ) {
			data_.push_back( get_na() );
		}

		void pop_back()
		{
			data_.pop_back();
		}

		// erase
	public:
		iterator erase (iterator position)
		{
			return data_.erase( position );
		}

		iterator erase (iterator first, iterator last)
		{
			return data_.erase( first, last );
		}

		// insert
	public:
		iterator insert (iterator position, const value_type& x)
		{
			data_.insert( position, x );
		}

		void insert (iterator position, size_type n, const value_type& x) 
		{
			data_.insert( position, n, x );
		}

		iterator insert (iterator position, const detail::_na_type& )
		{
			data_.insert( position, get_na() );
		}

		void insert (iterator position, size_type n, const detail::_na_type& ) 
		{
			data_.insert( position, n, get_na() );
		}

		template <class InputIterator>
		void insert (iterator position, InputIterator first, InputIterator last)
		{
			data_.insert( position, first, last );
		}

		// assign
	public:
		template <class InputIterator>
		void assign (InputIterator first, InputIterator last)
		{
			data_.assign( first, last );
		}

		void assign (size_type n, const value_type& val)
		{
			data_.assign( n, val );
		}

		void assign (size_type n, const detail::_na_type& val)
		{
			data_.assign( n, get_na() );
		}

		// swap
	public:
		void swap ( container_type& x)
		{
			data_.swap( x );
		}

		size_type size() const
		{
			return data_.size();
		}

		size_type max_size() const noexcept
		{
			return data_.max_size();
		}
	
		// resize
	public:
		void resize (size_type n)
		{
			data_.resize( n, get_na() );
		}

		void resize (size_type n, const value_type& val)
		{
			data_.resize( n, val );
		}

		void resize (size_type n, const detail::_na_type& )
		{
			data_.resize( n, get_na() );
		}

		void reserve (size_type n)
		{
			data_.reserve( n );
		}

		void shrink_to_fit()
		{
			data_.shrink_to_fit();
		}

		value_type* data() noexcept
		{
			return data_.data();
		}

		const value_type* data() const noexcept
		{
			return data_.data();
		}

		bool empty() const
		{
			return data_.empty();
		}

		void clear() noexcept
		{
			data_.clear();
		}

		size_type capacity() const noexcept
		{
			return data_.capacity();
		}

	private:
		container_type data_;
	};

	// relational operators missing for now. never used those.

	/*

	template< typename ValueType, class NaPolicy = policies::NaPolicySV< ValueType >, typename Allocator = std::allocator<NaPolicy::value_type> >
	class na_vector2 : private NaPolicy, public std::vector< typename NaPolicy::value_type, Allocator > {
	public:
		typedef typename vector::value_type value_type;

	private:
		operator std::vector< typename NaPolicy::value_type, Allocator >();

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

	*/

}

