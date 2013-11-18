/** \file 
 \brief offsetof macros
 */

#ifndef YCE_OFFSET_INCLUDED
#define YCE_OFFSET_INCLUDED 1

#define YOCTO_DUMMY_OBJECT_(CLASS)	     ( (CLASS *)( (void *) 0x100 ) )
#define YOCTO_DUMMY_MEMBER_(CLASS,MEMBER) (&(YOCTO_DUMMY_OBJECT_(CLASS)->MEMBER ))

#define YOCTO_OFFSET_OF(CLASS,MEMBER)                  \
(size_t)(                                              \
(uint8_t *)(void *)YOCTO_DUMMY_MEMBER_(CLASS,MEMBER)   \
- (uint8_t *)(void *)YOCTO_DUMMY_OBJECT_(CLASS)        \
)

#define YOCTO_OFF_(TYPE,addr,OP,offset) ( (TYPE *)( ((uint8_t *)(addr)) OP (offset) ) )
#define YOCTO_INC(addr,offset)       YOCTO_OFF_(void,addr,+,offset)
#define YOCTO_INC_CONST(addr,offset) YOCTO_OFF_(const void,addr,+,offset)

#define YOCTO_DEC(addr,offset)       YOCTO_OFF_(void,addr,-,offset)
#define YOCTO_DEC_CONST(addr,offset) YOCTO_OFF_(const void,addr,-,offset)

//! clean everything from member
#define YOCTO_HARD_RESET(CLASS,MEMBER,POINTER) memset( (void *)&(POINTER->MEMBER), 0, sizeof(CLASS) - YOCTO_OFFSET_OF(CLASS,MEMBER) )

#define YOCTO_SELF_RESET(CLASS,MEMBER) YOCTO_HARD_RESET(CLASS,MEMBER,this)
#endif
