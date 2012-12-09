#pragma once
#include <vector>
#include <boost/iterator/iterator_facade.hpp>

template< typename T >
struct strip_const {
	typedef T result;
};

template< typename T >
struct strip_const< const T > {
	typedef T result;
};

namespace order {
	struct row_major {};
	struct column_major {};
}

namespace tags {
	struct major_tag {};
	struct minor_tag {};
}

namespace detail {

	typedef std::size_t size_type;

	template< typename ValueType, typename Tag >
	class element_iterator;

	template< typename ValueType >
	class element_iterator< ValueType, tags::minor_tag > : public boost::iterator_facade< element_iterator<ValueType, tags::minor_tag>, ValueType, std::random_access_iterator_tag >  {
	public:
		element_iterator()
			: ptr_(nullptr), stride_(1)
		{
		}

		element_iterator( ValueType* ptr, difference_type stride )
			: ptr_( ptr ), stride_( stride )
		{
		}

		template< typename T >
		element_iterator( element_iterator<T,tags::minor_tag>& other )
			: ptr_( other.ptr_ ), stride_( other.stride_ )
		{
		}

		reference dereference()
		{
			return *ptr_;
		}

		reference dereference() const
		{
			return *ptr_;
		}

		void increment()
		{
			ptr_ += stride_;
		}

		void decrement()
		{
			ptr_ -= stride_;
		}

		bool equal( const element_iterator& other ) const
		{
			return ptr_ == other.ptr_;
		}

		difference_type distance_to( const element_iterator& other ) const
		{
			return (ptr_ - other.ptr_)/stride_;
		}

		void advance( difference_type diff )
		{
			ptr_ += diff * stride_;
		}

	private:
		friend class element_iterator< typename strip_const< ValueType >::result, tags::minor_tag >;
		friend class element_iterator< const ValueType, tags::minor_tag >;
		ValueType* ptr_;
		difference_type stride_;
	};

	template< typename ValueType >
	class element_iterator< ValueType, tags::major_tag > : public boost::iterator_facade< element_iterator<ValueType, tags::major_tag>, ValueType, std::random_access_iterator_tag >  {
	public:
		element_iterator()
			: ptr_(nullptr)
		{
		}

		element_iterator( ValueType* ptr )
			: ptr_( ptr )
		{
		}

		element_iterator( const element_iterator& other )
			: ptr_( other.ptr_ )
		{
		}

		operator element_iterator< const ValueType, tags::major_tag >() const
		{
			return element_iterator< const ValueType, tags::major_tag >( ptr_ );
		}

		reference dereference()
		{
			return *ptr_;
		}

		const reference dereference() const
		{
			return *ptr_;
		}

		void increment()
		{
			++ptr_;
		}

		void decrement()
		{
			--ptr_;
		}

		bool equal( const element_iterator& other ) const
		{
			return ptr_ == other.ptr_;
		}

		difference_type distance_to( const element_iterator& other ) const
		{
			return ptr_ - other.ptr_;
		}

		void advance( difference_type diff )
		{
			ptr_ += diff;
		}

	private:
		ValueType* ptr_;
	};

	template< bool Cond, typename S, typename T >
	struct static_if {
		typedef T type;
	};

	template< typename S, typename T >
	struct static_if<true,S,T>
	{
		typedef S type;
	};

	template< typename ArrayType, typename Tag >
	class slice_type {
		typedef typename element_iterator< typename ArrayType::value_type, Tag > _tmp_iterator;
	public:
		typedef typename element_iterator< const typename ArrayType::value_type, Tag > const_iterator;
		typedef typename static_if< std::is_const< ArrayType >::value, const_iterator, _tmp_iterator >::type iterator;
		typedef typename static_if< std::is_const< ArrayType >::value, const typename ArrayType::value_type, typename ArrayType::value_type >::type value_type;
		
		slice_type( ArrayType* table, size_type index )
			: table_( table ), index_( index )
		{
		}

		template< typename T >
		slice_type( const slice_type<T,Tag>& other )
			: table_(other.table_), index_(other.index_) {
		}

	private:
		friend class slice_type<typename strip_const<ArrayType>::result,Tag>;
		friend class slice_type<const ArrayType,Tag>;

		typename ArrayType::size_type index_;
		ArrayType* table_;

	public:
		iterator begin()
		{
			return table_->_get_element_begin( index_, Tag() );
		}

		const_iterator begin() const
		{
			return table_->_get_element_begin( index_, Tag() );
		}

		const_iterator cbegin() const
		{
			return table_->_get_element_begin( index_, Tag() );
		}

		iterator end()
		{
			return table_->_get_element_end( index_, Tag() );
		}

		const_iterator end() const
		{
			return table_->_get_element_end( index_, Tag() );
		}
		
		const_iterator cend() const
		{
			return table_->_get_element_end( index_, Tag() );
		}

		value_type& operator[]( size_type index )
		{
			return *(begin()+index);
		}

		const value_type& operator[]( size_type index ) const
		{
			return *(begin()+index);
		}
	};

	template< typename ArrayType, typename Tag >
	class slice_iterator : public boost::iterator_facade< slice_iterator<ArrayType,Tag>, slice_type< ArrayType, Tag >, std::random_access_iterator_tag, slice_type< ArrayType, Tag > >
	{
	public:
		slice_iterator()
			: table_( nullptr ), index_( 0 )
		{
		}

		slice_iterator( ArrayType* table, typename ArrayType::size_type index )
			: table_( table ), index_( index )
		{
		}

	
		slice_iterator( const slice_iterator& other )
			: table_( other.table_ ), index_( other.index_ )
		{
		}

		operator slice_iterator< const ArrayType, tags::major_tag >() const
		{
			return slice_iterator< const ArrayType, tags::major_tag >( table_, index_ );
		}

		reference dereference()
		{
			return value_type( table_, index_ );
		}

		const reference dereference() const
		{
			return value_type( table_, index_ );
		}

		void increment()
		{
			++index_;
		}

		void decrement()
		{
			--index_;
		}

		bool equal( const slice_iterator& other ) const
		{
			return index_ == other.index_;
		}

		difference_type distance_to( const slice_iterator& other ) const
		{
			return index_ - other.index_;
		}

		void advance( difference_type diff )
		{
			index_ += diff;
		}

	private:
		friend class slice_iterator< typename strip_const<ArrayType>::result, Tag >;
		friend class slice_iterator< const ArrayType, Tag >;

		ArrayType* table_;
		typename ArrayType::size_type index_;
	};

	template< typename ArrayType, typename Tag >
	class slice_sequence {
	private:
		typedef ArrayType array_type;
		typedef slice_iterator< ArrayType, Tag > iterator;
		typedef slice_iterator< const ArrayType, Tag > const_iterator;
	
	public:
		slice_sequence( array_type* arr )
			: array_( arr )
		{
		}

		operator slice_sequence< const ArrayType, Tag >() const
		{
			return slice_sequence< const ArrayType, Tag >( array_ );
		}

		iterator begin()
		{
			return array_->get_slice_begin( Tag() );
		}

		const_iterator begin() const
		{
			return array_->get_slice_begin( Tag() );
		}

		const_iterator cbegin() const
		{
			return array_->get_slice_begin( Tag() );
		}

		iterator end()
		{
			return array_->get_slice_end( Tag() );
		}

		const_iterator end() const
		{
			return array_->get_slice_end( Tag() );
		}

		const_iterator cend() const
		{
			return array_->get_slice_end( Tag() );
		}

	private:
		array_type* array_;
	};

	template< typename Order >
	class array2d_order;

	template<>
	class array2d_order< order::column_major >{
	public:
		typedef tags::major_tag column_tag;
		typedef tags::minor_tag row_tag;
	};

	template<>
	class array2d_order< order::row_major >{
	public:
		typedef tags::major_tag row_tag;
		typedef tags::minor_tag column_tag;
	};
	
	template< template< typename, typename > class ArrayType, typename ContainerType, typename OrderType >
	class array2d_types {
	public:
		typedef ArrayType< ContainerType, OrderType > array_type;
		typedef OrderType order_type;
		typedef typename ContainerType::value_type value_type;
		typedef typename ContainerType::reference reference;
		typedef typename ContainerType::difference_type difference_type;
		typedef const reference const_reference;
		typedef std::size_t size_type;
		typedef ContainerType container_type;

		typedef typename array2d_order< OrderType >::row_tag row_tag;
		typedef typename array2d_order< OrderType >::column_tag column_tag;
		
		typedef slice_type< array_type, row_tag > row_slice;
		typedef const slice_type< const array_type, row_tag > const_row_slice;

		typedef slice_type< array_type, column_tag > column_slice;
		typedef const slice_type< const array_type, column_tag > const_column_slice;

		typedef slice_iterator< array_type, column_tag > col_slice_iterator;
		typedef slice_iterator< const array_type, column_tag > const_col_slice_iterator;

		typedef slice_iterator< array_type, row_tag > row_slice_iterator;
		typedef slice_iterator< const array_type, row_tag > const_row_slice_iterator;

		typedef slice_iterator< array_type, tags::major_tag > major_slice_iterator;
		typedef slice_iterator< const array_type, tags::major_tag > const_major_slice_iterator;
		typedef slice_iterator< array_type, tags::minor_tag > minor_slice_iterator;
		typedef slice_iterator< const array_type, tags::minor_tag > const_minor_slice_iterator;

		typedef element_iterator< value_type, tags::major_tag > major_element_iterator;
		typedef element_iterator< const value_type, tags::major_tag > const_major_element_iterator;
		typedef element_iterator< value_type, tags::minor_tag > minor_element_iterator;
		typedef element_iterator< const value_type, tags::minor_tag > const_minor_element_iterator;

		typedef element_iterator< value_type, column_tag > col_element_iterator;
		typedef element_iterator< value_type, row_tag    > row_element_iterator;

		typedef element_iterator< const value_type, column_tag > const_col_element_iterator;
		typedef element_iterator< const value_type, row_tag > const_row_element_iterator;

		typedef slice_sequence< array_type, tags::major_tag > major_slice_sequence;
		typedef slice_sequence< const array_type, tags::major_tag > const_major_slice_sequence;

		typedef slice_sequence< array_type, tags::minor_tag > minor_slice_sequence;
		typedef slice_sequence< const array_type, tags::minor_tag > const_minor_slice_sequence;

		typedef slice_sequence< array_type, column_tag > column_slice_sequence;
		typedef slice_sequence< const array_type, column_tag > const_column_slice_sequence;

		typedef slice_sequence< array_type, row_tag > row_slice_sequence;
		typedef slice_sequence< const array_type, row_tag > const_row_slice_sequence;
	};

}

template< typename ContainerType, typename Ordering = order::column_major >
class array2d : public detail::array2d_types< ::array2d, ContainerType, Ordering > {
public:
	array2d( size_type rows, size_type cols )
	{
		_size( row_tag() )    = rows;
		_size( column_tag() ) = cols;

		data_.resize( minor_size_ * major_size_ );
	}

	array2d()
	{
		_size( row_tag() )    = 0;
		_size( column_tag() ) = 0;
	}

	array2d( const array2d& other )
		: data_( other.data_ ), major_size_( other.major_size_ ), minor_size_( other.minor_size_ )
	{
	}
	
	array2d( array2d&& other )
		: data_( std::move(other.data_) ), major_size_( other.major_size_ ), minor_size_( other.minor_size_ )
	{
	}
	
public:
	void swap( array2d& other ) 
	{
		data_.swap( other.data_ );
		std::swap( minor_size_, other.minor_size_ );
		std::swap( major_size_, other.major_size_ );
	}

	reference dereference( size_type row, size_type col ) {
		return data_[to_index( row, col )];
	}

	const_reference dereference( size_type row, size_type col ) const {
		return data_[to_index( row, col )];
	}
		
	column_slice_sequence col_seq() {
		return column_slice_sequence(this);
	}

	const_column_slice_sequence col_seq() const {
		return const_column_slice_sequence(this);
	}

	row_slice_sequence row_seq() {
		return row_slice_sequence(this);
	}

	const_row_slice_sequence row_seq() const {
		return const_row_slice_sequence(this);
	}

	column_slice operator[]( size_type col )
	{
		return column_slice( this, col );
	}

	size_type cols() const
	{
		return _size( column_tag() );
	}

	size_type rows() const
	{
		return _size( row_tag() );
	}

	row_slice_iterator row_begin()
	{
		return row_slice_iterator( this, 0 );
	}

	row_slice_iterator row_end()
	{
		return row_slice_iterator( this, rows_ );
	}

	const_row_slice_iterator row_begin() const
	{
		return const_row_slice_iterator( this, 0 );
	}

	const_row_slice_iterator row_end() const
	{
		return const_row_slice_iterator( this, rows_ );
	}

	const_row_slice_iterator row_cbegin() const
	{
		return const_row_slice_iterator( this, 0 );
	}

	const_row_slice_iterator row_cend() const
	{
		return const_row_slice_iterator( this, rows_ );
	}

	col_slice_iterator col_begin()
	{
		return col_slice_iterator( this, 0 );
	}

	col_slice_iterator col_end()
	{
		return col_slice_iterator( this, cols_ );
	}

	const_col_slice_iterator col_begin() const
	{
		return const_col_slice_iterator( this, 0 );
	}

	const_col_slice_iterator col_end() const
	{
		return const_col_slice_iterator( this, cols_ );
	}

	const_col_slice_iterator col_cbegin() const
	{
		return const_col_slice_iterator( this, 0 );
	}

	const_col_slice_iterator col_cend() const
	{
		return const_col_slice_iterator( this, cols_ );
	}

	void resize( size_type rows, size_type cols )
	{
		array2d other( rows, cols );

		auto major1_end = major_slice_end();
		auto major2_end = other.major_slice_end();

		for( auto major1 = major_slice_begin(), major2 = other.major_slice_begin();
			 major1 != major1_end && major2 != major2_end;
			 ++major1,++major2 )
		{
			for( auto minor1 = major1->begin(), minor2 = major2->begin();
				 minor1 != major1->end() && minor2 != major2->end();
				 ++minor1,++minor2 ) {
				*minor2 = *minor1;
			}
		}

		swap( other );
	}
	
	major_slice_iterator major_slice_begin()
	{
		return major_slice_iterator( this, 0 );
	}

	const_major_slice_iterator major_slice_begin() const
	{
		return const_major_slice_iterator( this, 0 );
	}

	const_major_slice_iterator major_slice_cbegin() const
	{
		return const_major_slice_iterator( this, 0 );
	}

	minor_slice_iterator minor_slice_begin()
	{
		return minor_slice_iterator( this, 0 );
	}
	
	const_minor_slice_iterator minor_slice_begin() const
	{
		return const_minor_slice_iterator( this, 0 );
	}

	const_minor_slice_iterator minor_slice_cbegin() const
	{
		return const_minor_slice_iterator( this, 0 );
	}
	
	major_slice_iterator major_slice_end()
	{
		return major_slice_iterator( this, major_size_ );
	}

	const_major_slice_iterator major_slice_end() const
	{
		return const_major_slice_iterator( this, major_size_ );
	}

	const_major_slice_iterator major_slice_cend() const
	{
		return const_major_slice_iterator( this, major_size_ );
	}

	minor_slice_iterator minor_slice_end()
	{
		return minor_slice_iterator( this, minor_size_ );
	}

	const_minor_slice_iterator minor_slice_end() const
	{
		return const_minor_slice_iterator( this, minor_size_ );
	}

	const_minor_slice_iterator minor_slice_cend() const
	{
		return const_minor_slice_iterator( this, minor_size_ );
	}

	value_type* data() {
		return data_.data();
	}

	const value_type* data() const {
		return data_.data();
	}

	size_type to_index( size_type row, size_type col )
	{
		return _to_index( row, col, order_type() );
	}

private:
	friend col_element_iterator;
	friend const_col_element_iterator;
	friend row_element_iterator;
	friend const_row_element_iterator;
	friend column_slice_sequence;
	friend row_slice_sequence;
	friend major_slice_iterator;
	friend const_major_slice_iterator;
	friend minor_slice_iterator;
	friend const_minor_slice_iterator;
	friend major_slice_sequence;
	friend const_major_slice_sequence;
	friend minor_slice_sequence;
	friend const_minor_slice_sequence;

	friend column_slice;
	friend const_column_slice;
	friend row_slice;
	friend const_row_slice;

	major_slice_iterator get_slice_begin( tags::major_tag )
	{
		return major_slice_begin();
	}

	const_major_slice_iterator get_slice_begin( tags::major_tag ) const
	{
		return const_major_slice_begin();
	}

	minor_slice_iterator get_slice_begin( tags::minor_tag )
	{
		return minor_slice_begin();
	}

	const_minor_slice_iterator get_slice_begin( tags::minor_tag ) const
	{
		return minor_slice_begin();
	}

	major_slice_iterator get_slice_end( tags::major_tag )
	{
		return major_slice_end();
	}

	const_major_slice_iterator get_slice_end( tags::major_tag ) const
	{
		return major_slice_end();
	}

	minor_slice_iterator  get_slice_end( tags::minor_tag )
	{
		return minor_slice_end();
	}

	const_minor_slice_iterator get_slice_end( tags::minor_tag ) const
	{
		return minor_slice_cend();
	}
	
	size_type& _size( tags::major_tag )
	{
		return major_size_;
	}

	size_type& _size( tags::minor_tag )
	{
		return minor_size_;
	}
	
	size_type _to_index( size_type row, size_type col, order::column_major )
	{
		return col * major_size_ + row;
	}

	size_type _to_index( size_type row, size_type col, order::row_major )
	{
		return col + row * major_size_;
	}

	size_type _stride() const
	{
		return minor_size_;
	}

	major_element_iterator _get_element_begin( size_type index, tags::major_tag )
	{
		return major_element_iterator( data() + index * major_size_ );
	}

	const_major_element_iterator _get_element_begin( size_type index, tags::major_tag ) const
	{
		return major_element_iterator( data() + index * major_size_ );
	}

	minor_element_iterator _get_element_begin( size_type index, tags::minor_tag )
	{
		return minor_element_iterator( data() + index, _stride() );
	}

	const_minor_element_iterator _get_element_begin( size_type index, tags::minor_tag ) const
	{
		return const_minor_element_iterator( data() + index, _stride() );
	}

	major_element_iterator _get_element_end( size_type index, tags::major_tag )
	{
		return major_element_iterator( data() + index * major_size_ + major_size_ );
	}

	const_major_element_iterator _get_element_end( size_type index, tags::major_tag ) const
	{
		return major_element_iterator( data() + index * major_size_ + major_size_ );
	}

	minor_element_iterator _get_element_end( size_type index, tags::minor_tag )
	{
		return minor_element_iterator( data() + index + minor_size_ * major_size_, _stride() );
	}

	const_minor_element_iterator _get_element_end( size_type index, tags::minor_tag ) const
	{
		return const_minor_element_iterator( data() + index + minor_size_ * major_size_, _stride() );
	}


	container_type data_;

	size_type minor_size_;
	size_type major_size_;
};