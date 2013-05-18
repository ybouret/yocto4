#ifndef YOCTO_LINGUA_SYNTAX_XNODE_INCLUDED
#define YOCTO_LINGUA_SYNTAX_XNODE_INCLUDED 1

#include "yocto/lingua/lexer.hpp"

namespace yocto
{
 
    namespace lingua
    {
        
        namespace syntax
        {
            enum node_property
            {
                is_regular
            };
            
            //! parse node
            class xnode : public object
            {
            public:
                
                typedef core::list_of<xnode> child_list;
                
                const string &label;  //!< referring to the creating syntax rule
                xnode        *prev;   //!< for tree/child_list
                xnode        *next;   //!< for tree/child_list
                xnode        *parent; //!< for tree
                
                 virtual ~xnode() throw();
                
                
                
                //! take care of lexeme in case of error
                static xnode * create( const string &label_ref, lexeme *lx, node_property p );
              
                //! GraphViz output
                void viz( ios::ostream &fp) const;
                void graphviz( const string &fn) const; //!< make a GraphViz file
                void graphviz( const char   *fn) const; //!< make a GraphViz file
                
                //! restore parsed node into lexer and delete it.
                static void restore( lexer &Lexer, xnode *node ) throw();
                
                //! unlink from parent
                xnode *unlink() throw();
                
            protected:
                explicit xnode( const string &label_ref, lexeme *lx, node_property p ) throw();
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xnode);
                uint64_t wksp[YOCTO_U64_FOR_ITEM(child_list)];
                
            public:
                lexeme    *&lex;
                child_list &children;
                
                const bool          terminal; //!< true if terminal => one lexeme
                const node_property property; //!< semantic property
            };
            
        }
        
    }
    
}

#endif
