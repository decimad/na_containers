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

namespace detail {

	typedef std::size_t size_type;

	template< typename ValueType >
	class row_element_iterator : public boost::iterator_facade< row_element_iterator<ValueType>, ValueType, std::random_access_iterator_tag >  {
	public:
		row_element_iterator()
			: ptr_(nullptr), stride_(1)
		{
		}

		row_element_iterator( ValueType* ptr, difference_type stride )
			: ptr_( ptr ), stride_( stride )
		{
		}

		template< typename T >
		row_element_iterator( row_element_iterator<T>& other )
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

		bool equal( const row_element_iterator& other ) const
		{
			return ptr_ == other.ptr_;
		}

		difference_type distance_to( const row_element_iterator& other ) const
		{
			return (ptr_ - other.ptr_)/stride_;
		}

		void advance( difference_type diff )
		{
			ptr_ += diff;
		}

	private:
		friend class row_element_iterator< typename strip_const< ValueType >::result >;
		friend class row_element_iterator< const ValueType >;
		ValueType* ptr_;
		difference_type stride_;
	};

	template< typename ValueType >
	class col_element_iterator : public boost::iterator_facade< col_element_iterator<ValueType>, ValueType, std::random_access_iterator_tag >  {
	public:
		col_element_iterator()
			: ptr_(nullptr)
		{
		}

		col_element_iterator( ValueType* ptr )
			: ptr_( ptr )
		{
		}

		template< typename T >
		col_element_iterator( const col_element_iterator<T>& other )
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

		bool equal( const col_element_iterator& other ) const
		{
			return ptr_ == other.ptr_;
		}

		difference_type distance_to( const col_element_iterator& other ) const
		{
			return ptr_ - other.ptr_;
		}

		void advance( difference_type diff )
		{
			ptr_ += diff;
		}

	private:
		friend class col_element_iterator< typename strip_const< ValueType >::result >;
		friend class col_element_iterator< const ValueType >;

		ValueType* ptr_;
	};

	template< typename TableType >
	class col_type {
	public:
		typedef typename TableType::col_element_iterator iterator;
		typedef typename TableType::const_col_element_iterator const_iterator;

		col_type( TableType* table, size_type col )
			: table_( table ), col_( col )
		{
		}

		template< typename T >
		col_type( const col_type<T>& other )
		  : table_(other.table_), col_(other.col_) {
		}

	private:
		friend class col_type<typename strip_const<TableType>::result>;
		friend class col_type<const TableType>;

		typename TableType::size_type col_;
		TableType* table_;

	public:
		typename TableType::col_element_iterator begin()
		{
			return table_->_get_col_begin( col_ );
		}

		typename TableType::const_col_element_iterator begin() const
		{
			return table_->_get_const_col_begin( col_ );
		}

		typename TableType::const_col_element_iterator cbegin() const
		{
			return table_->_get_const_col_begin( col_ );
		}

		typename TableType::col_element_iterator end()
		{
			return table_->_get_col_end( col_ );
		}

		typename TableType::const_col_element_iterator end() const
		{
			return table_->_get_const_col_end( col_ );
		}

		typename TableType::const_col_element_iterator cend() const
		{
			return table_->_get_const_col_end( col_ );
		}
	};

	template< typename TableType >
	class row_type {
	public:
		typedef typename TableType::row_element_iterator iterator;
		typedef typename TableType::const_row_element_iterator const_iterator;

		row_type( TableType* table, size_type row )
			: table_( table ), row_( row )
		{
		}

		template< typename T >
		row_type( const row_type<T>& other )
		  : table_(other.table_), row_(other.row_) {
		}

	private:
		friend class row_type<typename strip_const<TableType>::result>;
		friend class row_type<const TableType>;

		size_type row_;
		TableType* table_;

	public:
		typename TableType::row_element_iterator begin()
		{
			return table_->_get_row_begin( row_ );
		}

		typename TableType::const_row_element_iterator begin() const
		{
			return table_->_get_const_row_begin( row_ );
		}

		typename TableType::const_row_element_iterator cbegin() const
		{
			return table_->_get_const_row_begin( row_ );
		}

		typename TableType::row_element_iterator end()
		{
			return table_->_get_row_end( row_ );
		}

		typename TableType::const_row_element_iterator end() const
		{
			return table_->_get_const_row_end( row_ );
		}

		typename TableType::const_row_element_iterator cend() const
		{
			return table_->_get_const_row_end( row_ );
		}
	};

	template< typename TableType, typename ValueType >
	class base_iterator : public boost::iterator_facade< base_iterator<TableType,ValueType>, ValueType, std::random_access_iterator_tag, ValueType >
	{
	public:
		base_iterator()
			: table_( nullptr ), index_( 0 )
		{
		}

		base_iterator( TableType* table, typename TableType::size_type index )
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
		friend class base_iterator< typename strip_const<TableType>::result, typename strip_const<ValueType>::result >;
		friend class base_iterator< const TableType, const ValueType >;

		TableType* table_;
		typename TableType::size_type index_;
	};

	template< typename TableType, typename ContainerType >
	class TableTypes {
	public:
		typedef typename ContainerType::value_type value_type;
		typedef typename ContainerType::reference reference;
		typedef typename ContainerType::difference_type difference_type;
		typedef const reference const_reference;
		typedef std::size_t size_type;
		typedef ContainerType container_type;

		typedef detail::col_element_iterator< value_type       > col_element_iterator;
		typedef detail::col_element_iterator< const value_type > const_col_element_iterator;

		typedef detail::row_element_iterator< value_type       > row_element_iterator;
		typedef detail::row_element_iterator< const value_type > const_row_element_iterator;

		typedef detail::row_type< TableType > row_type;
		typedef const detail::row_type< const TableType > const_row_type;

		typedef detail::col_type< TableType > col_type;
		typedef const detail::col_type< const TableType > const_col_type;

		typedef base_iterator< TableType, col_type > col_iterator;
		typedef base_iterator< const TableType, const_col_type > const_col_iterator;

		typedef base_iterator< TableType, row_type > row_iterator;
		typedef base_iterator< const TableType, const_row_type > const_row_iterator;
	};

}

template< typename ContainerType >
class Table : public detail::TableTypes< Table<ContainerType>, ContainerType > {
public:
	Table( size_type rows, size_type cols )
		: data_( rows*cols ), cols_(cols), rows_(rows)
	{
	}

	Table()
		: cols_(0), rows_(0)
	{
	}

public:
	typedef detail::TableTypes< Table, ContainerType > types;

	reference dereference( size_type row, size_type col ) {
		return data_[row*width_+col_];
	}

	const_reference dereference( size_type row, size_type col ) const {
		return data_[row*width_+col_];
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

private:
	friend col_element_iterator;
	friend const_col_element_iterator;
	friend row_element_iterator;
	friend const_row_element_iterator;

	friend col_type;
	friend const_col_type;
	friend row_type;
	friend const_row_type;

	col_element_iterator _get_col_begin( size_type col ) {
		return col_element_iterator( data_.data() + col * rows_ );
	}

	const_col_element_iterator _get_const_col_begin( size_type col ) const {
		return const_col_element_iterator( data_.data() + col * rows_ );
	}

	col_element_iterator _get_col_end( size_type col ) {
		return col_element_iterator( data_.data() + col * rows_ + rows_ );
	}

	const_col_element_iterator _get_const_col_end( size_type col ) const {
		return const_col_element_iterator( data_.data() + col * rows_ + rows_ );
	}

	row_element_iterator _get_row_begin( size_type row ) {
		return row_element_iterator( data_.data() + row, static_cast<row_element_iterator::difference_type>(rows_) );
	}

	const_row_element_iterator _get_const_row_begin( size_type row ) const {
		return const_row_element_iterator( data_.data() + row, static_cast<row_element_iterator::difference_type>(rows_) );
	}

	row_element_iterator _get_row_end( size_type row ) {
		return row_element_iterator( data_.data() + row + cols_*rows_, static_cast<row_element_iterator::difference_type>(rows_) );
	}

	const_row_element_iterator _get_const_row_end( size_type row ) const {
		return const_row_element_iterator( data_.data() + row + cols_*rows_, static_cast<row_element_iterator::difference_type>(rows_) );
	}

	container_type data_;

	size_type cols_;
	size_type rows_;
};