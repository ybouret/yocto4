#ifndef YOCTO_STRING_MATCH_INCLUDED
#define YOCTO_STRING_MATCH_INCLUDED 1

#include "yocto/string/basic.hpp"
namespace yocto {
	
	namespace core {
		
		template <typename T>
		class string_match : public  object {
		public:
			explicit string_match( const T x[], const size_t m );
			virtual ~string_match() throw();
			
			
			void find( bool (*on_find)( size_t, void*) , void *args, const T y[], const size_t n, const size_t nmax=0);
			
		private:
			const ptrdiff_t m_; //!< string size
			T              *x_; //!< the string
			ptrdiff_t      *k_; //!< KMP next index
			size_t          l_; //!< memory
			YOCTO_DISABLE_COPY_AND_ASSIGN(string_match);
		};
		
	}
	
	typedef core::string_match<char>    string_match;
	typedef core::string_match<wchar_t> wstring_match;
	
}

#endif
