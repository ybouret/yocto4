#ifndef YOCTO_STRING_BASIC_INCLUDED
#define YOCTO_STRING_BASIC_INCLUDED

#include "yocto/memory/buffer.hpp"
#include "yocto/chars.hpp"
#include <iosfwd>

namespace yocto
{
	
	namespace basic
	{
		
		template <typename T>
		class string : public memory::rw_buffer
		{
		public:
			//------------------------------------------------------------------
			//
			// canonical form
			//
			//------------------------------------------------------------------
			virtual ~string() throw();
			
			string();
			string( const string & );
			inline string &operator=( const string &s ) { assign(s); return *this; }
			
			//------------------------------------------------------------------
			//
			// information
			//
			//------------------------------------------------------------------
			size_t         size() const throw();
			size_t         capacity() const throw();
			virtual size_t length() const throw();
			
			void swap_with( string &other ) throw();
			
			//------------------------------------------------------------------
			//
			// constructors
			//
			//------------------------------------------------------------------
			string( size_t n, const as_capacity_t & );
			string( size_t n, const T C );
			string( const T C );
			string( const T *s );
			string( const T *s, size_t n );
			
			//------------------------------------------------------------------
			//
			// assign
			//
			//------------------------------------------------------------------
			void clear() throw();
			void        assign( T C ) throw();
			void        assign( const T      *s, size_t n );
			inline void assign( const T      *s ) { assign(s,length_of(s)); }
			inline void assign( const string &s ) { assign(s.char_,s.size_);      }
			inline string & operator=( const T  C ) throw() { assign(C); return *this; }
			inline string & operator=( const T *s ) throw() { assign(s); return *this; }
			
			//------------------------------------------------------------------
			//
			// append
			//
			//------------------------------------------------------------------
			void        append( const T *s, size_t n );
			inline void append( const T      *s ) { append(s,length_of(s));  }
			inline void append( const string &s ) { append(s.char_,s.size_); }
			inline void append( const T       C ) { append( &C, 1 );         }
			
			inline string & operator+=( const string &s ) { append(s); return *this; }
			inline string & operator+=( const T      *s ) { append(s); return *this; }
			inline string & operator+=( const T       C ) { append(C); return *this; }
			
			friend inline
			string operator+( const string &lhs, const string &rhs ) { 
				return string( lhs.size_ + rhs.size_, lhs.char_, lhs.size_, rhs.char_, rhs.size_);
			}
			
			friend inline
			string operator+( const string &lhs, const T *rhs ) { 
				const size_t rhs_size = length_of(rhs);
				return string( lhs.size_ + rhs_size, lhs.char_, lhs.size_, rhs, rhs_size);
			}
			
			friend inline
			string operator+( const T *lhs, const string &rhs ) { 
				const size_t lhs_size = length_of(lhs);
				return string( lhs_size + rhs.size_, lhs, lhs_size, rhs.char_, rhs.size_);
			}
			
			friend inline
			string operator+( const string &lhs, const T C )
			{
				return string( lhs.size_ + 1, lhs.char_, lhs.size_, &C, 1 );
			}
			
			friend inline
			string operator+( const T C, const string &rhs )
			{
				return string( 1 + rhs.size_, &C, 1, rhs.char_, rhs.size_);
			}
			
			//------------------------------------------------------------------
			//
			// I/O
			//
			//------------------------------------------------------------------
			std::ostream & output( std::ostream &) const;
			std::ostream & output_visible( std::ostream &os ) const;
			
			friend inline std::ostream & operator<<( std::ostream &str_os, const string &str_arg)
			{
				return str_arg.output( str_os );
			}
			
			
			
			inline T       & operator[](size_t index) throw()       { assert(index<=size_); return char_[index]; }
			inline const T & operator[](size_t index) const throw() { assert(index<=size_); return char_[index]; }
			inline const T *c_str() const throw() { return char_; }
			
			//------------------------------------------------------------------
			//
			// comparison
			//
			//------------------------------------------------------------------
			static inline int compare( const string &lhs, const string &rhs ) throw() { return compare_blocks( lhs.char_, lhs.size_, rhs.char_, rhs.size_); }
			static inline int compare( const string &lhs, const T      *rhs ) throw() { return compare_blocks( lhs.char_, lhs.size_, rhs, length_of(rhs) ); }
			static inline int compare( const T      *lhs, const string &rhs ) throw() {	return compare_blocks( lhs, length_of(lhs),  rhs.char_, rhs.size_); }																			
			static inline int compare( const string &lhs, const T       rhs ) throw() { return compare_blocks( lhs.char_, lhs.size_, &rhs, 1);              }
			static inline int compare( const T       lhs, const string &rhs ) throw() {	return compare_blocks( &lhs, 1,  rhs.char_, rhs.size_); }
			
#define YOCTO_STRING_OP(OP) \
friend inline bool operator OP ( const string &_lhs, const string &_rhs ) throw() { return compare(_lhs,_rhs) OP 0; }\
friend inline bool operator OP ( const string &_lhs, const T      *_rhs ) throw() { return compare(_lhs,_rhs) OP 0; }\
friend inline bool operator OP ( const T      *_lhs, const string &_rhs ) throw() { return compare(_lhs,_rhs) OP 0; }\
friend inline bool operator OP ( const string &_lhs, const T       _rhs ) throw() { return compare(_lhs,_rhs) OP 0; }\
friend inline bool operator OP ( const T       _lhs, const string &_rhs ) throw() { return compare(_lhs,_rhs) OP 0; }
			
			YOCTO_STRING_OP(==)
			YOCTO_STRING_OP(!=)
			YOCTO_STRING_OP(<)
			YOCTO_STRING_OP(>)
			YOCTO_STRING_OP(<=)
			YOCTO_STRING_OP(>=)
			
			//------------------------------------------------------------------
			//
			// Transform
			//
			//------------------------------------------------------------------
			void trim(size_t n) throw();
			void skip(size_t n) throw();
            void trim( bool (*bad)(T) ) throw();
            void skip( bool (*bad)(T) ) throw();
            void clean( bool (*bad)(T) ) throw();
			void to_upper() throw();
            void to_lower() throw();
            
		private:
			size_t size_; //!< #chars
			size_t full_; //!< memory units
			T     *char_; //!< memory location
			size_t maxi_; //!< in units: full-1
			
			string( size_t sum_len, const T *a, size_t a_len, const T *b, size_t b_len );
			static int compare_blocks( const T *a, size_t p, const T *b, size_t q ) throw();
			
			virtual const void *get_address() const throw();
		};
		
	}
	
}

#endif
