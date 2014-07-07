#ifndef YOCTO_TOKENIZER_INCLUDED
#define YOCTO_TOKENIZER_INCLUDED

#include "yocto/string/basic.hpp"
#include "yocto/chars.hpp"
#include "yocto/container/sequence.hpp"

namespace yocto {
	
	namespace core {
		
		template <typename T>
		class tokenizer : public object
			{
			public:
				
				virtual ~tokenizer() throw();
				explicit tokenizer( const basic::string<T> &input ) throw();
				
				
				bool get_next( bool (*is_separator)( T ) throw() ) throw();
				
				inline const T *token() const throw() { return token_; }
				inline size_t   units() const throw() { return units_; }
				inline size_t   count() const throw() { return count_; }
                
				inline basic::string<T> to_string(void) const
                { return basic::string<T>( token_, units_); }
                
				static void split( sequence< basic::string<T> > &seq, const basic::string<T> &input, bool (*is_separator)( T ) throw() );
				
                
                
			private:
				YOCTO_DISABLE_COPY_AND_ASSIGN(tokenizer);
				const T *curr_;
				const T *last_;
				const T *token_; //!< token position
				size_t   units_; //!< token length, may be 0
				size_t   count_; //!< token count
			};
		
		
		
	}
	
	typedef core::tokenizer<char>    tokenizer;
	typedef core::tokenizer<wchar_t> wtokenizer;
	
}

#endif
