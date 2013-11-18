//! \file

#ifndef YOCTO_RX_COMPILER_INCLUDED
#define YOCTO_RX_COMPILER_INCLUDED 1

#include "yocto/rx/pattern/dict.hpp"

namespace yocto
{
	namespace regex
	{
        //! compile an expression into an accepting pattern
		pattern *compile( const string &expr, const pattern_dict *dict );
		pattern *compile( const char   *expr, const pattern_dict *dict );
	}
}

#endif
