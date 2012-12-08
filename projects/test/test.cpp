#include <na_containers/na_vector.h>
#include <na_containers/array2d.h>
#include <cmath>
#include <iostream>
using std::wcout;
using na::NA;

struct nan_value {
	static const float value;
};
const float nan_value::value = std::numeric_limits<float>::quiet_NaN();


template< typename A, typename B >
void func( const std::vector< A, B >& vec )
{

}

int main()
{
	typedef na::na_vector< float, na::policies::NaPolicyOptional<float> > na_vector1_optional;
	typedef na::na_vector< float > na_vector1_special;
	typedef na::na_vector< float, na::policies::NaPolicySV<float,nan_value> > na_vector1_special_nan;

	typedef array2d< na_vector1_special, order::row_major > array_type1;
	typedef array2d< na_vector1_special, order::column_major > array_type2;
	array_type1 array1( 10, 10 );
	array_type2 array2( 10, 10 );

	for( auto col : array1.col_seq() ) {
		for( auto& elem : col ) {
			elem = 1.0f;
		}
	}

	for( auto row : array1.row_seq() ) {
		for( auto& elem : row ) {
			elem += 1.0f;
		}
	}

	auto col = *(array1.col_begin() + 3);
	for( auto& elem : col ) {
		elem += 4.0f;
	}

	for( auto row : array1.row_seq() ) {
		for( auto elem : row ) {
			std::wcout << elem << L", ";
		}
		std::wcout << L'\n';
	}

	wcout << sizeof( array_type1 ) << L'\n';
	wcout << sizeof( std::vector< double > ) + 2*sizeof(array_type1::size_type) << L'\n';

}