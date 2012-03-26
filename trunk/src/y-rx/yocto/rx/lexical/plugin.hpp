#ifndef YOCTO_RX_LEX_PLUGIN
#define YOCTO_RX_LEX_PLUGIN 1

#include "yocto/rx/lexical/sub-lexer.hpp"


namespace yocto 
{
    namespace regex
    {
        
        namespace lexical
        {
            
            class plugin : public regex::sublexer
            {
            public:
                virtual ~plugin() throw();
                
                virtual pattern * trigger() const = 0;
                
                const lexical::action &enter() const throw();
                const lexical::action &leave() const throw();
                
                
            protected:
                explicit plugin( const char *id );
                
               
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(plugin);
                void on_enter( const token & );
                void on_leave( const token & );
                const action __Enter;
                const action __Leave;
            };
            
            typedef intrusive_ptr<string,plugin> module;
        }
        
    }
    
}


#endif
