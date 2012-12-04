#include "na_vector.h"
#include <boost/iterator/filter_iterator.hpp>
#include <boost/optional.hpp>
#include <iostream>
using std::wcout;
using na::NA;

int main()
{
	typedef na::na_vector< float, na::detail::NaPolicyOptional<float> > vector_type;

	vector_type vec;

//	boost::optional<int> myInt;
	

	vec.push_back( 14 );
	vec.push_back( NA );
	vec.push_back( 13 );
//	vec.push_back( vector_type::NA );

	auto filtered = vec.filtered();

	for( auto it = filtered.begin(); it != filtered.end(); ++it ) {
		wcout << it->get() << '\n';
	}


}