#ifndef YOCTO_GEMS_LIBRARY_INCLUDED
#define YOCTO_GEMS_LIBRARY_INCLUDED 1

#include "yocto/gems/atom.hpp"
#include "yocto/associative/set.hpp"

namespace yocto
{
    namespace gems
    {

        //! library of properties and atoms bookeeping
        /**
         all possible properties should be set before creating atoms
         */
        class library
        {
        public:
            explicit library();
            virtual ~library() throw();

            properties & insert( const string &name, const double mass);

            properties       & operator[](const string &name);
            const properties & operator[](const string &name) const;


            template <typename T>
            inline atom<T> *create( const word_t uuid, const properties &ppty )
            {
                atom<T> *ptr =  new atom<T>(uuid,ppty);
                const atom_info::tag tag(ptr);
                if(!atom_tags.insert(tag))
                {
                    __throw_multiple_uuid(uuid);
                }
                locked = true;
                return ptr;
            }

            template <typename T>
            inline atom<T> *create( const word_t uuid, const string &name )
            {
                return create<T>(uuid,(*this)[name]);
            }

            


        private:
            properties::database       db;
            set<word_t,atom_info::tag> atom_tags;
            bool                       locked;
            void __throw_multiple_uuid(word_t uuid) const;
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
        
        
    }
    
    
}

#endif
