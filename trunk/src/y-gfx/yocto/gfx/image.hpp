#ifndef YOCTO_GFX_IMAGE_INCLUDED
#define YOCTO_GFX_IMAGE_INCLUDED 1

#include "yocto/gfx/bitmap.hpp"
#include "yocto/gfx/rgb.hpp"
#include "yocto/threading/singleton.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace gfx
    {
        
        //! image formats handling
        class image : public singleton<image>
        {
        public:
            class format : public object, public counted
            {
            public:
                const string name;
                
                virtual ~format() throw();
                const string &key() const throw();
                
            protected:
                explicit format( const char *id );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(format);
            };
            
            
            //! register a new format
            /**
             \param format a raw pointer, taken care of in case of error
             */
            void operator()( format *fmt );
            
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(image);
           
            //__________________________________________________________________
            //
            // singleton API
            //__________________________________________________________________

            virtual ~image() throw();
            explicit image();
            friend class singleton<image>;
            static const char name[];
			static const threading::longevity life_time = 100; // TODO: check
            
            //__________________________________________________________________
            //
            // format mgmt
            //__________________________________________________________________
            
            typedef intr_ptr<string,format> fmt_ptr;
            typedef set<string,fmt_ptr>     fmt_db;
            fmt_db formats;
            
        public:
            typedef fmt_db::const_iterator iterator;
            
            size_t   size()  const throw();
            iterator begin() const throw();
            iterator end()   const throw();
        };
        
    }
}

#endif
