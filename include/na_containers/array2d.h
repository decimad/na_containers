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

namespace detail {

	typedef std::size_t size_type;

	template< typename ValueType >
	class minor_element_iterator : public boost::iterator_facade< minor_element_iterator<ValueType>, ValueType, std::random_access_iterator_tag >  {
	public:
		minor_element_iterator()
			: ptr_(nullptr), stride_(1)
		{
		}

		minor_element_iterator( ValueType* ptr, difference_type stride )
			: ptr_( ptr ), stride_( stride )
		{
		}

		template< typename T >
		minor_element_iterator( minor_element_iterator<T>& other )
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

		bool equal( const minor_element_iterator& other ) const
		{
			return ptr_ == other.ptr_;
		}

		difference_type distance_to( const minor_element_iterator& other ) const
		{
			return (ptr_ - other.ptr_)/stride_;
		}

		void advance( difference_type diff )
		{
			ptr_ += diff * stride_;
		}

	private:
		friend class minor_element_iterator< typename strip_const< ValueType >::result >;
		friend class minor_element_iterator< const ValueType >;
		ValueType* ptr_;
		difference_type stride_;
	};

	template< typename ValueType >
	class major_element_iterator : public boost::iterator_facade< major_element_iterator<ValueType>, ValueType, std::random_access_iterator_tag >  {
	public:
		major_element_iterator()
			: ptr_(nullptr)
		{
		}

		major_element_iterator( ValueType* ptr )
			: ptr_( ptr )
		{
		}

		template< typename T >
		major_element_iterator( const major_element_iterator<T>& other )
			: ptr_( other.ptr_ )
		{
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

		bool equal( const major_element_iterator& other ) const
		{
			return ptr_ == other.ptr_;
		}

		difference_type distance_to( const major_element_iterator& other ) const
		{
			return ptr_ - other.ptr_;
		}

		void advance( difference_type diff )
		{
			ptr_ += diff;
		}

	private:
		friend class major_element_iterator< typename strip_const< ValueType >::result >;
		friend class major_element_iterator< const ValueType >;

		ValueType* ptr_;
	};

	template< typename ArrayType >
	class col_type {
	public:
		typedef typename ArrayType::col_element_iterator iterator;
		typedef typename ArrayType::const_col_element_iterator const_iterator;

		col_type( ArrayType* table, size_type col )
			: table_( table ), col_( col )
		{
		}

		template< typename T >
		col_type( const col_type<T>& other )
		  : table_(other.table_), col_(other.col_) {
		}

	private:
		friend class col_type<typename strip_const<ArrayType>::result>;
		friend class col_type<const ArrayType>;

		typename ArrayType::size_type col_;
		ArrayType* table_;

	public:
		typename ArrayType::col_element_iterator begin()
		{
			return table_->_get_col_begin( col_ );
		}

		typename ArrayType::const_col_element_iterator begin() const
		{
			return table_->_get_const_col_begin( col_ );
		}

		typename ArrayType::const_col_element_iterator cbegin() const
		{
			return table_->_get_const_col_begin( col_ );
		}

		typename ArrayType::col_element_iterator end()
		{
			return table_->_get_col_end( col_ );
		}

		typename ArrayType::const_col_element_iterator end() const
		{
			return table_->_get_const_col_end( col_ );
		}

		typename ArrayType::const_col_element_iterator cend() const
		{
			return table_->_get_const_col_end( col_ );
		}
	};

	template< typename ArrayType >
	class row_type {
	public:
		typedef typename ArrayType::row_element_iterator iterator;
		typedef typename ArrayType::const_row_element_iterator const_iterator;

		row_type( ArrayType* table, size_type row )
			: table_( table ), row_( row )
		{
		}

		template< typename T >
		row_type( const row_type<T>& other )
		  : table_(other.table_), row_(other.row_) {
		}

	private:
		friend class row_type<typename strip_const<ArrayType>::result>;
		friend class row_type<const ArrayType>;

		size_type row_;
		ArrayType* table_;

	public:
		typename ArrayType::row_element_iterator begin()
		{
			return table_->_get_row_begin( row_ );
		}

		typename ArrayType::const_row_element_iterator begin() const
		{
			return table_->_get_const_row_begin( row_ );
		}

		typename ArrayType::const_row_element_iterator cbegin() const
		{
			return table_->_get_const_row_begin( row_ );
		}

		typename ArrayType::row_element_iterator end()
		{
			return table_->_get_row_end( row_ );
		}

		typename ArrayType::const_row_element_iterator end() const
		{
			return table_->_get_const_row_end( row_ );
		}

		typename ArrayType::const_row_element_iterator cend() const
		{
			return table_->_get_const_row_end( row_ );
		}
	};

	template< typename ArrayType, typename ValueType >
	class base_iterator : public boost::iterator_facade< base_iterator<ArrayType,ValueType>, ValueType, std::random_access_iterator_tag, ValueType >
	{
	public:
		base_iterator()
			: table_( nullptr ), index_( 0 )
		{
		}

		base_iterator( ArrayType* table, typename ArrayType::size_type index )
			: table_( table ), index_( index )
		{
		}

		template< typename S, typename T >
		base_iterator( const base_iterator< S, T >& other )
			: table_( other.table_ ), index_( other.index_ )
		{
		}

		reference dereference()
		{
			return ValueType( table_, index_ );
		}

		const reference dereference() const
		{
			return ValueType( table_, index_ );
		}

		void increment()
		{
			++index_;
		}

		void decrement()
		{
			--index_;
		}

		bool equal( const base_iterator& other ) const
		{
			return index_ == other.index_;
		}

		difference_type distance_to( const base_iterator& other ) const
		{
			return index_ - other.index_;
		}

		void advance( difference_type diff )
		{
			index_ += diff;
		}

	private:
		friend class base_iterator< typename strip_const<ArrayType>::result, typename strip_const<ValueType>::result >;
		friend class base_iterator< const ArrayType, const ValueType >;

		ArrayType* table_;
		typename ArrayType::size_type index_;
	};

	template< typename ArrayType >
	class cols_sequence {
	private:
		typedef ArrayType array_type;
	
	public:
		cols_sequence( array_type* arr )
			: array_( arr )
		{
		}

		typename array_type::col_iterator begin()
		{
			return array_->col_begin();
		}

		typename array_type::const_col_iterator begin() const
		{
			return array_->col_begin();
		}

		typename array_type::const_col_iterator cbegin() const
		{
			return array_->col_cbegin();
		}

		typename array_type::col_iterator end()
		{
			return array_->col_end();
		}

		typename array_type::const_col_iterator end() const
		{
			return array_->col_end();
		}

		typename array_type::const_col_iterator cend() const
		{
			return array_->col_cend();
		}

	private:
		array_type* array_;
	};

	template< typename ArrayType >
	class rows_sequence {
	private:
		typedef ArrayType array_type;

	public:
		rows_sequence( array_type* arr )
			: array_( arr )
		{
		}

		typename array_type::row_iterator begin()
		{
			return array_->row_begin();
		}

		typename array_type::const_row_iterator begin() const
		{
			return array_->row_begin();
		}

		typename array_type::const_row_iterator cbegin() const
		{
			return array_->row_cbegin();
		}

		typename array_type::row_iterator end()
		{
			return array_->row_end();
		}

		typename array_type::const_row_iterator end() const
		{
			return array_->row_end();
		}

		typename array_type::const_row_iterator cend() const
		{
			return array_->row_cend();
		}

	private:
		array_type* array_;
	};






	template< template< typename ContainerType, typename OrderType > class ArrayType, typename ContainerType, typename OrderType >
	class array2d_types {
	public:
		typedef ArrayType< ContainerType, OrderType > array_type;
		typedef typename ContainerType::value_type value_type;
		typedef typename ContainerType::reference reference;
		typedef typename ContainerType::difference_type difference_type;
		typedef const reference const_reference;
		typedef std::size_t size_type;
		typedef ContainerType container_type;

		typedef detail::row_type< array_type > row_type;
		typedef const detail::row_type< const array_type > const_row_type;

		typedef detail::col_type< array_type > col_type;
		typedef const detail::col_type< const array_type > const_col_type;

		typedef base_iterator< array_type, col_type > col_iterator;
		typedef base_iterator< const array_type, const_col_type > const_col_iterator;

		typedef base_iterator< array_type, row_type > row_iterator;
		typedef base_iterator< const array_type, const_row_type > const_row_iterator;
	};

	// CRTP base class for array2d in order to provide column- and row-major ordering
	template< template<typename,typename> class ArrayType, typename ContainerType, typename OrderType >
	class array2d_base;
	

	// column major
	template< template<typename,typename> class ArrayType, typename ContainerType >
	class array2d_base< ArrayType, ContainerType, order::column_major > 
		: public detail::array2d_types< ArrayType, ContainerType, order::column_major >
	{
	public:
		typedef array2d_base base;

		typedef order::column_major order_type;
		typedef ArrayType< ContainerType, order_type > array_type;
		typedef detail::array2d_types< ArrayType, ContainerType, order::column_major > types;

		typedef detail::major_element_iterator< value_type       > col_element_iterator;
		typedef detail::major_element_iterator< const value_type > const_col_element_iterator;

		typedef detail::minor_element_iterator< value_type       > row_element_iterator;
		typedef detail::minor_element_iterator< const value_type > const_row_element_iterator;

	protected:
		size_type _to_index( size_type row, size_type col ) {
			return col * to_array().rows() + row;
		}

		// I'd like to have the *_begins and *_ends to be in the interface of the concrete array2d class, that's why I upcast here.
		// These member functions help array2d to use the most efficient copy pattern.
		col_type _get_major_begin()             { return to_array().col_begin(); }
		const_col_type _get_major_begin() const { return to_arary().col_cbegin(); }
		
		row_type _get_minor_begin();
		const_row_type _get_minor_begin() const;

		col_element_iterator _get_col_begin( size_type col ) {
			return col_element_iterator( to_array().data_origin() + _to_index(0, col) );
		}

		const_col_element_iterator _get_const_col_begin( size_type col ) const {
			return const_col_element_iterator( to_array().data_origin() + _to_index(0, col) );
		}

		col_element_iterator _get_col_end( size_type col ) {
			return col_element_iterator( to_array().data_origin() + _to_index(to_array().rows(), col) );
		}

		const_col_element_iterator _get_const_col_end( size_type col ) const {
			return const_col_element_iterator( to_array().data_origin() +  _to_index(to_array().rows(), col) );
		}

		row_element_iterator _get_row_begin( size_type row ) {
			return row_element_iterator( to_array().data_origin() + _to_index( row, 0 ), static_cast<row_element_iterator::difference_type>(to_array().rows()) );
		}

		const_row_element_iterator _get_const_row_begin( size_type row ) const {
			return const_row_element_iterator( to_array().data_origin()  + _to_index( row, 0 ), static_cast<row_element_iterator::difference_type>(to_array().rows()) );
		}

		row_element_iterator _get_row_end( size_type row ) {
			return row_element_iterator( to_array().data_origin()  + _to_index( row, to_array().cols() ), static_cast<row_element_iterator::difference_type>(to_array().rows()) );
		}

		const_row_element_iterator _get_const_row_end( size_type row ) const {
			return const_row_element_iterator( to_array().data_origin() + _to_index( row, to_array().cols() ), static_cast<row_element_iterator::difference_type>(to_array().rows()) );
		}

	private:
		array_type& to_array() {
			return static_cast<array_type&>(*this);
		}
	};

	// row major
	template< template<typename,typename> class ArrayType, typename ContainerType >
	class array2d_base< ArrayType, ContainerType, order::row_major > 
		: public detail::array2d_types< ArrayType, ContainerType, order::row_major >
	{
	public:
		typedef array2d_base base;

		typedef order::row_major order_type;
		typedef ArrayType< ContainerType, order_type > array_type;
		typedef detail::array2d_types< ArrayType, ContainerType, order::row_major > types;

		typedef detail::minor_element_iterator< value_type       > col_element_iterator;
		typedef detail::minor_element_iterator< const value_type > const_col_element_iterator;

		typedef detail::major_element_iterator< value_type       > row_element_iterator;
		typedef detail::major_element_iterator< const value_type > const_row_element_iterator;

	protected:
		size_type _to_index( size_type row, size_type col ) {
			return row * to_array().cols() + col;
		}

		// I'd like to have the *_begins and *_ends to be in the interface of the concrete array2d class, that's why I upcast here.
		// These member functions help array2d to use the most efficient copy pattern.
		row_type _get_major_begin()             { return to_array().row_begin(); }
		const_row_type _get_major_begin() const { return to_arary().row_cbegin(); }

		col_type _get_minor_begin();
		const_col_type _get_minor_begin() const;


		col_element_iterator _get_col_begin( size_type col ) {
			return col_element_iterator( to_array().data_origin() + _to_index(0, col), to_array().cols() );
		}

		const_col_element_iterator _get_const_col_begin( size_type col ) const {
			return const_col_element_iterator( to_array().data_origin() + _to_index(0, col), to_array().cols() );
		}

		col_element_iterator _get_col_end( size_type col ) {
			return col_element_iterator( to_array().data_origin() + _to_index(to_array().rows(), col), to_array().cols() );
		}

		const_col_element_iterator _get_const_col_end( size_type col ) const {
			return const_col_element_iterator( to_array().data_origin() + _to_index(to_array().rows(), col), to_array().cols() );
		}

		row_element_iterator _get_row_begin( size_type row ) {
			return row_element_iterator( to_array().data_origin() + _to_index( row, 0 ) );
		}

		const_row_element_iterator _get_const_row_begin( size_type row ) const {
			return const_row_element_iterator( to_array().data_origin()  + _to_index( row, 0 ) );
		}

		row_element_iterator _get_row_end( size_type row ) {
			return row_element_iterator( to_array().data_origin()  + _to_index( row, to_array().cols() ) );
		}

		const_row_element_iterator _get_const_row_end( size_type row ) const {
			return const_row_element_iterator( to_array().data_origin() + _to_index( row, to_array().cols() ) );
		}

	private:
		array_type& to_array() {
			return static_cast<array_type&>(*this);
		}

		const array_type& to_array() const {
			return static_cast<const array_type&>(*this);
		}
	};



}

template< typename ContainerType, typename Ordering = order::column_major >
class array2d : public detail::array2d_base< ::array2d, ContainerType, Ordering >, virtual public detail::array2d_types< array2d, ContainerType, Ordering > {
	// array2d_types is empty anyways but this suppresses the muliple-base-class warning.
private:
	typedef detail::array2d_base< ::array2d, ContainerType, Ordering > base;

public:
	typedef typename base::col_element_iterator col_element_iterator;
	typedef typename base::const_col_element_iterator const_col_element_iterator;

	typedef typename base::row_element_iterator row_element_iterator;
	typedef typename base::const_row_element_iterator const_row_element_iterator;


	array2d( size_type rows, size_type cols )
		: data_( rows*cols ), cols_(cols), rows_(rows)
	{
	}

	array2d()
		: cols_(0), rows_(0)
	{
	}

public:
	reference dereference( size_type row, size_type col ) {
		return data_[_to_index( row, col )];
	}

	const_reference dereference( size_type row, size_type col ) const {
		return data_[_to_index( row, col )];
	}
		
	detail::cols_sequence<array2d> col_seq() {
		return detail::cols_sequence<array2d>(this);
	}

	detail::cols_sequence<const array2d> col_seq() const {
		return detail::cols_sequence<const array2d>(this);
	}

	detail::rows_sequence<array2d> row_seq() {
		return detail::rows_sequence<array2d>(this);
	}

	detail::rows_sequence<const array2d> row_seq() const {
		return detail::rows_sequence<const array2d>(this);
	}

	row_type GetRow( size_type row );
	const_row_type GetRow( size_type row ) const;

	col_type operator[]( size_type col )
	{
		return col_type( this, col );
	}

	const_col_type operator[]( size_type col ) const
	{
		return const_col_type( this, col );
	}

	size_type cols() const
	{
		return cols_;
	}

	size_type rows() const
	{
		return rows_;
	}

	row_iterator row_begin()
	{
		return row_iterator( this, 0 );
	}

	row_iterator row_end()
	{
		return row_iterator( this, rows_ );
	}

	const_row_iterator row_begin() const
	{
		return const_row_iterator( this, 0 );
	}

	const_row_iterator row_end() const
	{
		return const_row_iterator( this, rows_ );
	}

	const_row_iterator row_cbegin() const
	{
		return const_row_iterator( this, 0 );
	}

	const_row_iterator row_cend() const
	{
		return const_row_iterator( this, rows_ );
	}

	col_iterator col_begin()
	{
		return col_iterator( this, 0 );
	}

	col_iterator col_end()
	{
		return col_iterator( this, cols_ );
	}

	const_col_iterator col_begin() const
	{
		return const_col_iterator( this, 0 );
	}

	const_col_iterator col_end() const
	{
		return const_col_iterator( this, cols_ );
	}

	const_col_iterator col_cbegin() const
	{
		return const_col_iterator( this, 0 );
	}

	const_col_iterator col_cend() const
	{
		return const_col_iterator( this, cols_ );
	}

	value_type* data_origin()
	{
		return data_.data();
	}

	const value_type* data_origin() const
	{
		return data_.data();
	}

private:
	friend col_element_iterator;
	friend const_col_element_iterator;
	friend row_element_iterator;
	friend const_row_element_iterator;

	friend class col_type;
	friend class const_col_type;
	friend class row_type;
	friend class const_row_type;

	container_type data_;

	size_type cols_;
	size_type rows_;
};