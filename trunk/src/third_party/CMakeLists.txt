#Directory for third party libraries
MACRO(YOCTO_THIRD_PARTY dir)
	MESSAGE( STATUS "===>>> third_party/${dir}" )
	ADD_SUBDIRECTORY(${dir})
ENDMACRO(YOCTO_THIRD_PARTY)

YOCTO_THIRD_PARTY(lzo)
YOCTO_THIRD_PARTY(z)


