#include <na_containers/na_vector.h>
#include <na_containers/array2d2.h>
#include <array>
#include <cmath>
#include <iostream>
using std::wcout;
using na::NA;

struct nan_value {
	static const float value;
};
const float nan_value::value = std::numeric_limits<float>::quiet_NaN();

int main()
{
	typedef na::na_vector< float, na::policies::NaPolicyOptional<float> > na_vector1_optional;
	typedef na::na_vector< float > na_vector1_special;
	typedef na::na_vector< float, na::policies::NaPolicySV<float,nan_value> > na_vector1_special_nan;

	typedef array2d< na_vector1_special, order::row_major > array_type1;
	typedef array2d< na_vector1_special, order::column_major > array_type2;

	array_type1 array1( 10, 10 );
	const array_type1 array2( 10, 10 );

	auto bla = array2.col_seq();
	for( auto col : bla ) {
		auto a = *col.begin();
		std::wcout << col[3] << '\n';
	}
	
	array1.resize(11,11);

	auto blubb = array1.col_seq();
	
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

	array1.dereference(1,2) = 12;

	for( auto& elem : col ) {
		elem += 4.0f;
	}

	for( auto row : array1.row_seq() ) {
		for( auto elem : row ) {
			std::wcout << elem << L", ";
		}
		std::wcout << L'\n';
	}

	array_type1::const_row_slice_iterator bka = array1.row_begin();

	wcout << sizeof( array_type1 ) << L'\n';
	wcout << sizeof( std::vector< double > ) + 2*sizeof(array_type1::size_type) << L'\n';

}