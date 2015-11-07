#ifndef YOCTO_CHEMICAL_GROUP_INCLUDED
#define YOCTO_CHEMICAL_GROUP_INCLUDED 1

#include "yocto/counted-object.hpp"
#include "yocto/string.hpp"


namespace yocto
{
    namespace chemical
    {
        //! a group=(residue|label) with label/count
        class group : public counted_object
        {
        public:            
            const string  label; //!< atom or residue
            int           count; //!< count
            const string &key() const throw();
            
            explicit group(const string &grpLabel,const int grpCount);
            virtual ~group() throw();
            
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(group);
        };
        
#if 0
        //! managing groups algebra
        class groups : public group::database
        {
        public:
            virtual ~groups() throw() {}
            groups() : group::database()
            {
            }
            
            void add( const string &grpLabel, const int grpCount)
            {
                if(grpCount!=0)
                {
                    group::pointer *ppG = search(grpLabel);
                    if(ppG)
                    {
                        int &count = (**ppG).count;
                        count += grpCount;
                    }
                    else
                    {
                        const group::pointer grp( new group(grpLabel,grpCount) );
                        if(!insert(grp))
                        {
                            throw exception("add new group '%s' unexpected failure!",grpLabel.c_str());
                        }
                    }
                }
            }
            
        private:
            YOCTO_DISABLE_COPY_AND_ASSIGN(groups);
        };
#endif
        
    }
    
}


#endif
