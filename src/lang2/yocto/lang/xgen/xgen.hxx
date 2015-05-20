#ifndef YOCTO_LANG_XGEN_INCLUDED
#define YOCTO_LANG_XGEN_INCLUDED 1


#include "yocto/lang/parser.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ptr/alias.hpp"
#include "yocto/hashing/perfect.hpp"
#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    namespace lang
    {
        namespace syntax
        {
            class xgen
            {
            public:
                typedef alias_ptr<string,aggregate> agg_ptr;
                typedef set<string,agg_ptr>         agg_set;

                typedef alias_ptr<string,terminal>  term_ptr;
                typedef set<string,term_ptr>        term_set;

                typedef  rule *rule_p;

                const xnode      *root;
                auto_ptr<parser>  xprs;
                const char       *name;
                agg_set           agg;
                term_set          rxp;
                term_set          raw;

                hashing::perfect  cmph; //!< lxr codes: @drop, @endl, @comment
                hashing::perfect  mmph; //!< lxr meta: RXP|RAW
                hashing::perfect  kmph; //!< kind of item
                unsigned          indx; //!< for sub rules naming
                unsigned          jndx; //!< for lexical rules naming
                sorted_vector<rule_p> visited;


                xgen(const xnode *node);
                ~xgen() throw();

                template <typename DB>
                inline void check_connected( DB &db )
                {
                    for( typename DB::iterator i=db.begin(); i != db.end(); ++i )
                    {
                        rule &r = **i;
                        validate(&r);
                    }
                }

            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(xgen);

                void     process_rule_level1(const xnode *node);
                void     process_lxr__level1(const xnode *node);

                pattern *compile_lexical(const xnode *node,const size_t nsub);
                pattern *compile_lexical(const xnode *node);

                void     register_std(const string &label);
                void     register_rxp(const string &label, const string &expr); 
                void     register_raw(const string &label, const string &expr);

                size_t   count(const string &type, const string &expr) const;
                void     count_cb(size_t &num, const string &type, const string &expr, const xnode *node) const;

                void     process_rule_level2(const xnode *node);
                void     grow( logical *parent, const xnode *sub);
                void     grow_item( logical *parent, const xnode *sub);
                logical *new_sub();

                void     check_connectivity( rule *r);
                void     mark_visited( rule *r, const char *ctx);
                void     validate(rule *r);
            };

        }
    }
}

#endif

