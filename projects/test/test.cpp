#include <na_containers/na_vector.h>
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

	typedef na::na_vector2< float, na::policies::NaPolicyOptional<float> > na_vector2_optional;
	typedef na::na_vector2< float > na_vector2_special;
	typedef na::na_vector2< float, na::policies::NaPolicySV<float,nan_value> > na_vector2_special_nan;

	na_vector1_optional vec1;
	na_vector1_special  vec2;

	na_vector2_optional vec3;
	na_vector2_special  vec4;

	na_vector2_special_nan vec5;

	vec4.push_back( 14 );
	vec4.push_back( NA );
	vec4.push_back( 13 );

	vec4.insert( vec4.begin(), 10, NA );
	vec4.insert( vec4.begin(), 10, 4.5 );

	auto filtered = vec4.filtered();

	for( auto it = filtered.begin(); it != filtered.end(); ++it ) {
		wcout << *it << '\n';
	}

}