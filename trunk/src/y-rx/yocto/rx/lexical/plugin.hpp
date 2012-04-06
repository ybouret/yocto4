#ifndef YOCTO_RX_LEX_PLUGIN
#define YOCTO_RX_LEX_PLUGIN 1

#include "yocto/rx/lexical/sub-lexer.hpp"


namespace yocto 
{
    namespace regex
    {
        
        namespace lexical
        {
            
            typedef functor<void,TL1(void*)> callback;
            
            class plugin : public regex::sublexer
            {
            public:
                virtual ~plugin() throw();
                
                pattern *trigger() const;
                
                
                //! will call this->enter() to initialize the plugin
                const action &on_call() const throw();
                void          do_nothing(void) throw();
                
            protected:
                //! constructor
                /**
                 \param id name for  sublexer
                 \param enter_expr   regular expression that triggers the plugin
                 \param leave_expr   regular expression that finishes the plugin
                 \param cb           a post-processing callback on this->data().
                 
                 the constructor register a back(leave_expr,on_leave) instruction,
                 and the on_leave method will call the virtual leave() method,
                 then the user defined callback.
                 */
                explicit plugin(const char     *id,
                                const char     *enter_expr,
                                const char     *leave_expr,
                                const callback &cb,
                                const string   *will_jump);
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(plugin);
                auto_ptr<pattern> trigger_;
                const action      enter_;
                callback          finish_;
                void on_enter( const token & );
                void on_leave( const token & );
                
                virtual void  enter() = 0;
                virtual void  leave() = 0;
                virtual void  *data() throw() = 0;
            };
            
            typedef intrusive_ptr<string,plugin> module;
        }
        
    }
    
}


#endif
