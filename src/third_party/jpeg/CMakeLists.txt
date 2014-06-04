CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(jpeg C)
MESSAGE( STATUS "" )
MESSAGE( STATUS "---------------- <jpeg> ----------------" )

SET(JROOT  ${CMAKE_CURRENT_SOURCE_DIR}/yocto/jpeg)
MESSAGE( STATUS "@${JROOT}" )
SET(JCONFIG ${JROOT}/jconfig.h)

ADD_DEFINITIONS( -D_CRT_SECURE_NO_WARNINGS )

SET(LIBSOURCES 
	jcapimin.c jcapistd.c jctrans.c jcparam.c jdatadst.c jcinit.c jcmaster.c jcmarker.c jcmainct.c jcprepct.c jccoefct.c jccolor.c jcsample.c jchuff.c jcphuff.c jcdctmgr.c jfdctfst.c jfdctflt.c jfdctint.c jdapimin.c jdapistd.c jdtrans.c jdatasrc.c jdmaster.c jdinput.c jdmarker.c jdhuff.c jdphuff.c jdmainct.c jdcoefct.c jdpostct.c jddctmgr.c jidctfst.c jidctflt.c jidctint.c jidctred.c jdsample.c jdcolor.c jquant1.c jquant2.c jdmerge.c jcomapi.c jutils.c jerror.c jmemmgr.c jmemnobs.c)
MESSAGE( STATUS "Generating libjpeg sources...")
SET(SOURCES)
FOREACH(f ${LIBSOURCES})
	LIST( APPEND SOURCES "${JROOT}/${f}" )
ENDFOREACH()

ADD_LIBRARY(y-jpeg STATIC ${JCONFIG} ${SOURCES})

IF(YOCTO_AUTO_TEST)

IF(NOT MSVC)

ADD_EXECUTABLE(cjpeg 
	${JROOT}/cjpeg.c 
	${JROOT}/rdppm.c
	${JROOT}/rdgif.c
	${JROOT}/rdtarga.c 
	${JROOT}/rdrle.c
	${JROOT}/rdbmp.c 
	${JROOT}/rdswitch.c 
	${JROOT}/cdjpeg.c)

TARGET_LINK_LIBRARIES(cjpeg y-jpeg)

ADD_EXECUTABLE(djpeg 
	${JROOT}/djpeg.c 
	${JROOT}/wrppm.c 
	${JROOT}/wrgif.c 
	${JROOT}/wrtarga.c 
	${JROOT}/wrrle.c 
	${JROOT}/wrbmp.c 
	${JROOT}/rdcolmap.c 
	${JROOT}/cdjpeg.c)
TARGET_LINK_LIBRARIES(djpeg y-jpeg)

ADD_EXECUTABLE(jpegtran 
	${JROOT}/jpegtran.c 
	${JROOT}/rdswitch.c 
	${JROOT}/cdjpeg.c 
	${JROOT}/transupp.c)
TARGET_LINK_LIBRARIES(jpegtran y-jpeg)

ADD_CUSTOM_COMMAND(
	TARGET djpeg
	POST_BUILD
	COMMAND djpeg -dct int -ppm -outfile      ${CMAKE_CURRENT_BINARY_DIR}/testout.ppm ${JROOT}/testorig.jpg
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testout.ppm ${JROOT}/testimg.ppm
	COMMAND djpeg -dct int -bmp -colors 256 -outfile ${CMAKE_CURRENT_BINARY_DIR}/testout.bmp  ${JROOT}/testorig.jpg
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testout.bmp ${JROOT}/testimg.bmp
	COMMAND djpeg -dct int -ppm -outfile ${CMAKE_CURRENT_BINARY_DIR}/testoutp.ppm ${JROOT}/testprog.jpg
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testoutp.ppm ${JROOT}/testimg.ppm
	COMMENT "Auto Testing djpeg"
	)


ADD_CUSTOM_COMMAND(
	TARGET cjpeg
	POST_BUILD
	COMMAND cjpeg -dct int -outfile ${CMAKE_CURRENT_BINARY_DIR}/testout.jpg  ${JROOT}/testimg.ppm
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testout.jpg ${JROOT}/testimg.jpg
	COMMAND cjpeg -dct int -progressive -opt -outfile ${CMAKE_CURRENT_BINARY_DIR}/testoutp.jpg ${JROOT}/testimg.ppm
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testoutp.jpg ${JROOT}/testimgp.jpg
	COMMENT "Auto Testing cjpeg"
	)
	
ADD_CUSTOM_COMMAND(
	TARGET jpegtran
	POST_BUILD
	COMMAND jpegtran -outfile ${CMAKE_CURRENT_BINARY_DIR}/testoutt.jpg ${JROOT}/testprog.jpg
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testoutt.jpg ${JROOT}/testorig.jpg
	COMMENT "Auto Testing jpegtran" )
ENDIF(NOT MSVC)

ENDIF(YOCTO_AUTO_TEST)

INSTALL( FILES 
	${JROOT}/jconfig.h
	${JROOT}/jpeglib.h
	${JROOT}/jerror.h
	${JROOT}/jmorecfg.h
 DESTINATION include/yocto/jpeg )
INSTALL( TARGETS y-jpeg ARCHIVE DESTINATION lib )

ADD_EXECUTABLE(jpeg-io ${JROOT}/jpeg-io.c ${JROOT}/example.c)
TARGET_LINK_LIBRARIES(jpeg-io y-jpeg)


MESSAGE( STATUS "---------------- </jpeg> ----------------" )
MESSAGE( STATUS "" )

