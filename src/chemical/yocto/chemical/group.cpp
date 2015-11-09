#include "yocto/chemical/group.hpp"


namespace yocto
{

    namespace chemical
    {
        const string & group::key() const throw() { return label; };



        group::group(const string &grpLabel,
                     const int     grpCount) :
        label(grpLabel),
        count(grpCount)
        {
        }

        group:: ~group() throw()
        {
        }



    }

}

#include "yocto/exception.hpp"

namespace yocto
{

    namespace chemical
    {
        int groups:: compare_group(const group::pointer &lhs, const group::pointer &rhs) throw()
        {
            return string::compare(lhs->key(), rhs->key());
        }


        void groups::sort() throw()
        { sort_by(compare_group); }

        groups:: groups() : _groups(8,as_capacity)
        {
        }

        groups:: ~groups() throw() {}

        void groups:: add( const string &grpLabel, const int grpCount)
        {
            if(grpCount!=0)
            {
                group::pointer *ppG = search(grpLabel);
                if(ppG)
                {
                    int &count = (**ppG).count;
                    count += grpCount;
                    if(count==0)
                    {
                        if(!remove(grpLabel))
                        {
                            throw exception("group::remove('%s') unexpected failure!",grpLabel.c_str());
                        }
                    }
                }
                else
                {
                    const group::pointer grp( new group(grpLabel,grpCount) );
                    if(!insert(grp))
                    {
                        throw exception("group::append( '%d %s' ) unexpected failure!",grpCount,grpLabel.c_str());
                    }
                }
            }
        }

        void groups:: add( const group &grp )
        {
            add( grp.label, grp.count );
        }

        void groups:: sub( const group &grp )
        {
            add( grp.label, -grp.count );
        }

        void groups:: add( const _groups &other )
        {
            for( _groups::const_iterator i=other.begin();i!=other.end();++i)
            {
                add(**i);
            }
        }

        void groups:: sub( const _groups &other )
        {
            const _groups &self = *this;
            if( &self == &other )
            {
                free();
            }
            else
            {
                for( _groups::const_iterator i=other.begin();i!=other.end();++i)
                {
                    sub(**i);
                }
            }

        }



    }

}

