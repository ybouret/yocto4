//! \file

#ifndef YOCTO_RX_PATTERN_INCLUDED
#define YOCTO_RX_PATTERN_INCLUDED 1


#include "yocto/rx/token.hpp"
#include "yocto/core/meta-list.hpp"
#include "yocto/rx/symbols.hpp"

namespace yocto
{
	
	namespace ios
	{
		class ostream;
		class istream;
	}
	
	namespace regex
	{
		
		class source;
		void  show_tag( ios::ostream &, const void *p );
		
        //! base class to accept a source of t_char
		class pattern : public token
		{
		public:
			const uint32_t type; //!< pattern class identifier
			void          *data; //!< pattern special data address
			pattern       *next; //!< for a pattern list
			pattern       *prev; //!< for a pattern list
			
			virtual ~pattern() throw();
			virtual pattern *clone() const = 0;        //!< virtual copy constructor
			virtual bool     accept( source &src) = 0; //!< matching ?
			virtual void     clear() throw();          //!< should recursively release t_char
			
			//! convert this pattern as a GraphViz graph
			void graphviz( ios::ostream &, const string &graph_name ) const;
			virtual void viz( ios::ostream & ) const = 0; //!< for each pattern
			void         tag( ios::ostream & ) const;     //!< uniq GraphViz node identifier
			static void  out( ios::ostream &, char c );   //!< GrapViz node readable label
			inline void  graphviz( ios::ostream &fp, const char *gn ) const { const string graph_name(gn); graphviz(fp,graph_name); }
			
			//! logical optimization, default is do nothing
			virtual void optimize();
			
			//! detect first acceptable symbols
			virtual void gather( first_chars &fch ) const = 0;
			
			//! binary output
			virtual void     brx( ios::ostream & ) const = 0;
			
			//! load
			static  pattern *load( ios::istream & );
			
            //! testing equality by brx, binary representation
            static bool is_same( const pattern &lhs, const pattern &rhs );
            
            //! equality wrapper
            inline friend bool operator==( const pattern &lhs, const pattern &rhs )
            {
                return pattern::is_same( lhs, rhs );
            }
            
            //! inequality wrapper
            inline friend bool operator!=( const pattern &lhs, const pattern &rhs )
            {
                return !pattern::is_same( lhs, rhs );
            }

            
            
		protected:
			explicit pattern(uint32_t t) throw();
		
						
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(pattern);
		};
		
		typedef core::meta_list<pattern> p_list;
		
	}
	
}


#endif
