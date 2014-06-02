#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/surface.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/associative/lexicon.hpp"

namespace yocto
{
    
    namespace gfx
    {
        
        class image : public singleton<image>
        {
        public:
            
            class format : public counted_object
            {
            public:
                virtual ~format() throw();
                
                const string name;
                
                const string &key() const throw();
                
                typedef intr_ptr<string,format>         pointer;
                typedef lexicon<string,format::pointer> database;
            
                virtual bool lossless() const throw() = 0;
                
            protected:
                explicit format(const char *id);
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(format);
            };
            
            void declare( format *fmt );
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(image);
            friend class singleton<image>;
            virtual ~image() throw();
            explicit image();
            static const threading::longevity life_time = 100;
            static const char                 name[];
            format::database db;
        };
    }
    
}

#endif
