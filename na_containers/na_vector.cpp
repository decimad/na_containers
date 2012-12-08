#include <ctype.h>
#include <na_containers/na_vector.h>

namespace na {

	detail::_na_type NA;
	
	namespace detail {
		const double special_value_default<double>::value = -DBL_MAX;
		const float  special_value_default<float>::value  = -FLT_MAX;
	}

}