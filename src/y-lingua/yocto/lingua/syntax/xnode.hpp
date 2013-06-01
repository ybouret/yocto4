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
                is_regular     = 0,
                is_discardable = 1, //!< no semantic meaning
                is_specialized = 2, //!< univocal meaning, erase content
                is_merging_all = 4, //!< merge with parent's children
                is_merging_one = 8  //!< merge with parent's children if only one child
            };
            
            //! parse node
            class xnode : public object
            {
            public:
                
                class child_list : public core::list_of<xnode>
                {
                public:
                    explicit child_list() throw();
                    virtual ~child_list() throw();
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(child_list);
                };
                
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
                
                child_list       & children() throw();       //!< if !terminal
                lexeme          *& lex() throw();            //!< if terminal
                const child_list & children() const throw(); //!< if !terminal
                lexeme * const   & lex() const throw();      //!< if terminal
                
                //! convert to an abstract node
                static xnode *AST(xnode *node,int &depth) throw();
                
            protected:
                explicit xnode( const string &label_ref, lexeme *lx, node_property p ) throw();
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xnode);
                uint64_t wksp[YOCTO_U64_FOR_ITEM(child_list)];
                
            public:
                
                const bool          terminal; //!< true if terminal => one lexeme
                const node_property property; //!< semantic property
            };
            
        }
        
    }
    
}

#endif
