#ifndef YOCTO_CHEMICAL_GROUP_INCLUDED
#define YOCTO_CHEMICAL_GROUP_INCLUDED 1

#include "yocto/counted-object.hpp"
#include "yocto/string.hpp"
#include "yocto/ptr/intr.hpp"

namespace yocto
{
    namespace chemical
    {
        //! a group=(residue|label) with label/count
        class group : public counted_object
        {
        public:
            typedef intr_ptr<string,group> pointer;
            const string  label; //!< atom or residue
            int           count; //!< count
            const string &key() const throw();
            
            explicit group(const string &grpLabel,const int grpCount);
            virtual ~group() throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(group);
        };

    }

}

#include "yocto/associative/set.hpp"

namespace yocto
{

    namespace chemical
    {
        typedef set<string,group::pointer> _groups;

        //! managing groups
        class groups : public _groups
        {
        public:
            virtual ~groups() throw();
            explicit groups();

            void add( const string &grpLabel, const int grpCount);
            void add( const group &grp );
            void sub( const group &grp );

            void add( const _groups &other );
            void sub( const _groups &other );

            void sort() throw();

        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(groups);
            static  int compare_group(const group::pointer &lhs, const group::pointer &rhs) throw();

        };

    }
    
}


#endif
