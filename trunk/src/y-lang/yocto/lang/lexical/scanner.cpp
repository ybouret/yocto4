#include "yocto/lang/lexical/scanner.hpp"
#include "yocto/exception.hpp"
#include "yocto/auto-ptr.hpp"
#include "yocto/rx/compiler.hpp"

namespace yocto 
{
    namespace lang
    {
        
        namespace lexical
        {
            
            scanner:: ~scanner() throw() { no_dict(); }
            
#define Y_LANG_LEX_SCANNER_CTOR() \
name(id),\
line(line_ref),\
rules_(),\
forward( this, &scanner::__forward ),\
discard( this, &scanner::__discard ),\
newline( this, &scanner::__newline ),\
parent_(0),\
dict_(0)
            
            scanner:: scanner( const string &id, size_t &line_ref ) :
            Y_LANG_LEX_SCANNER_CTOR()
            {
            }
            
            scanner:: scanner( const char *id, size_t &line_ref ) :
            Y_LANG_LEX_SCANNER_CTOR()
            {
            }
            
            
            const string & scanner:: key() const throw()
            {
                return name;
            }
            
            bool scanner:: __forward( const regex::token &) throw() { return true;  }
            bool scanner:: __discard( const regex::token &) throw() { return false; }
            bool scanner:: __newline( const regex::token &) throw() { ++line; return true; }
            
            void scanner:: operator()( const string &label, regex::pattern *motif, const action *proc  )
            {
                assert(motif);
                //--------------------------------------------------------------
                // no multiple rule
                //--------------------------------------------------------------
                for( rule *r = rules_.head; r; r=r->next)
                {
                    if( r->label == label )
                    {
                        delete motif;
                        throw exception("%s(multiple rule '%s')", name.c_str(), label.c_str() );
                    }
                }
                
                //--------------------------------------------------------------
                //  append the rule
                //--------------------------------------------------------------
                rules_.push_back( rule::create(label,motif,proc? *proc: forward) );
                
            }
            
            regex::pattern_dict & scanner:: dict() 
            {
                if(!dict_) dict_ = new regex::pattern_dict();
                return *dict_;
            }
            
            void scanner:: no_dict() throw()
            {
                if( dict_ ) { delete dict_; dict_ = 0; }
            }
            
            
            
            void scanner:: operator()( const string &label, const string &expr, const action *proc )
            {
                (*this)(label, regex::compile(expr,dict_), proc);
            }
            
            void scanner:: operator()( const char *label, const char *expr, const action *proc )
            {
                const string L(label);
                (*this)(label, regex::compile(expr,dict_), proc);
            }
            
            
            void scanner:: reset() throw()
            {
                for( rule *r = rules_.head; r; r=r->next)
                {
                    r->motif->clear();
                }
                line = 1;
            }
            
            void scanner:: link_to( lexer &parent ) throw()
            {
                parent_ = &parent;
            }
            
        }
        
    }
    
}

