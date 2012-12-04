#include <na_containers/na_vector.h>
#include <iostream>
using std::wcout;
using na::NA;

int main()
{
	typedef na::na_vector< float, na::detail::NaPolicyOptional<float> > na_vector1_optional;
	typedef na::na_vector< float > na_vector1_special;

	typedef na::na_vector2< float, na::detail::NaPolicyOptional<float> > na_vector2_optional;
	typedef na::na_vector2< float > na_vector2_special;

	na_vector1_optional vec1;
	na_vector1_special  vec2;

	na_vector2_optional vec3;
	na_vector2_special  vec4;

	vec4.push_back( 14 );
	vec4.push_back( NA );
	vec4.push_back( 13 );

	vec4.insert( vec4.begin(), 10, NA );

	auto filtered = vec4.filtered();

	for( auto it = filtered.begin(); it != filtered.end(); ++it ) {
		wcout << *it << '\n';
	}

}