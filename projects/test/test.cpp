#include <na_containers/na_vector.h>
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
	
	na_vector1_optional vec1;
	na_vector1_special  vec2;
	na_vector1_special_nan vec3;

	vec2.push_back( 14 );
	vec2.push_back( NA );
	vec2.push_back( 13 );

	vec2.insert( vec2.begin(), 10, NA );
	vec2.insert( vec2.begin(), 10, 4.5 );

	auto filtered = vec2.filtered();

	for( auto it = filtered.begin(); it != filtered.end(); ++it ) {
		wcout << *it << '\n';
	}

	// func( vec2 ); // must not compile!
}