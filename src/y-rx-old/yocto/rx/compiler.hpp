//! \file

#ifndef YOCTO_RX_COMPILER_INCLUDED
#define YOCTO_RX_COMPILER_INCLUDED 1

#include "yocto/rx/pattern/db.hpp"

namespace yocto
{
	namespace regex
	{
		pattern *compile( const string &expr, const pattern_db *db );
		pattern *compile( const char   *expr, const pattern_db *db );
	}
}

#endif
