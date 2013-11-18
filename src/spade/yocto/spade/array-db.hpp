//!
#ifndef YOCTO_SPADE_ARRAY_DB_INCLUDED
#define YOCTO_SPADE_ARRAY_DB_INCLUDED 1

#include "yocto/spade/varray.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/spade/linear-handles.hpp"

namespace yocto
{
    namespace spade
    {
   
        //! database or arrays
        /**
         used to store arrays and their memory.
         */
        class array_db
        {
        public:
            explicit array_db() throw();
            explicit array_db(size_t);
            virtual ~array_db() throw();
            
            void insert( const varray::ptr &arr );
            
            varray & operator[]( const string & );
            varray & operator[]( const char  *  );
            const varray & operator[]( const string & ) const;
            const varray & operator[]( const char  *  ) const;
            
            //! append handle of id to handles
            void query( linear_handles &handles, const string &id);
            
            //! append handle of id to handles
            void query( linear_handles &handles, const char   *id);
            
            //! C-style append handles of names to handles
            void query( linear_handles &handles, const char **names, const size_t count);
            
            //! C++ style append handles of names to handles
            void query( linear_handles &handles, const array<string> &names );
            
            //! load content of handles[source] into array
            template <typename T>
            inline void load( array<T> &arr, const linear_handles &handles, const size_t source ) const
            {
                assert( handles.size() <= arr.size() );
                for( size_t i=handles.size();i>0;--i)
                {
                    const linear_ptr p = handles[i]; assert(p!=NULL);
                    assert( p->item_size() <= sizeof(T));
                    memcpy( &arr[i], p->address_of(source), sizeof(T) );
                }
            }
            
            //! load content of handles[source] into array
            template <typename T>
            inline void save( linear_handles &handles, const array<T> &arr, const size_t target ) const
            {
                assert( handles.size() <= arr.size() );
                for( size_t i=handles.size();i>0;--i)
                {
                    linear_ptr p = handles[i]; assert(p!=NULL);
                    assert( p->item_size() <= sizeof(T));
                    memcpy( (void*)(p->address_of(target)), &arr[i], sizeof(T) );
                }
            }

            
        private:
            set<string,varray::ptr> arrays;
        };
        
    }
}


#endif
