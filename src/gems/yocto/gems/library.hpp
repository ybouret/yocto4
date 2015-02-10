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
                const atom_info::handle h(ptr);
                if(!atoms.insert(h))
                {
                    __throw_multiple_uuid(uuid);
                }
                return ptr;
            }

            template <typename T>
            inline atom<T> *create( const word_t uuid, const string &name )
            {
                return create<T>(uuid,(*this)[name]);
            }

            void display() const;


        private:
            properties::database       db;
            atom_info::table           atoms;
            void __throw_multiple_uuid(word_t uuid) const;
            YOCTO_DISABLE_COPY_AND_ASSIGN(library);
        };
        
        
    }
    
    
}

#endif
