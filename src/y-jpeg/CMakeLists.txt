CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(jpeg)

SET(JCONFIG ${CMAKE_CURRENT_SOURCE_DIR}/jconfig.h)
ADD_CUSTOM_COMMAND(
   OUTPUT   ${JCONFIG}
   COMMAND  sh ${CMAKE_CURRENT_SOURCE_DIR}/configure --disable-shared --without-x --quiet
   DEPENDS   
   COMMENT "Generating Platform Adapated <jconfig.h>"
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
   )
   
SET(LIBSOURCES 
	jcapimin.c jcapistd.c jctrans.c jcparam.c jdatadst.c jcinit.c jcmaster.c jcmarker.c jcmainct.c jcprepct.c jccoefct.c jccolor.c jcsample.c jchuff.c jcphuff.c jcdctmgr.c jfdctfst.c jfdctflt.c jfdctint.c jdapimin.c jdapistd.c jdtrans.c jdatasrc.c jdmaster.c jdinput.c jdmarker.c jdhuff.c jdphuff.c jdmainct.c jdcoefct.c jdpostct.c jddctmgr.c jidctfst.c jidctflt.c jidctint.c jidctred.c jdsample.c jdcolor.c jquant1.c jquant2.c jdmerge.c jcomapi.c jutils.c jerror.c jmemmgr.c jmemnobs.c)
        
ADD_LIBRARY(jpeg STATIC ${JCONFIG} ${LIBSOURCES})

ADD_EXECUTABLE(cjpeg cjpeg.c rdppm.c rdgif.c rdtarga.c rdrle.c rdbmp.c rdswitch.c cdjpeg.c)
TARGET_LINK_LIBRARIES(cjpeg jpeg)

ADD_EXECUTABLE(djpeg djpeg.c wrppm.c wrgif.c wrtarga.c wrrle.c wrbmp.c rdcolmap.c cdjpeg.c)
TARGET_LINK_LIBRARIES(djpeg jpeg)

ADD_EXECUTABLE(jpegtran jpegtran.c rdswitch.c cdjpeg.c transupp.c)
TARGET_LINK_LIBRARIES(jpegtran jpeg)


ADD_CUSTOM_COMMAND(
	TARGET djpeg
	POST_BUILD
	COMMAND djpeg -dct int -ppm -outfile      ${CMAKE_CURRENT_BINARY_DIR}/testout.ppm ${CMAKE_CURRENT_SOURCE_DIR}/testorig.jpg
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testout.ppm ${CMAKE_CURRENT_SOURCE_DIR}/testimg.ppm
	COMMAND djpeg -dct int -bmp -colors 256 -outfile ${CMAKE_CURRENT_BINARY_DIR}/testout.bmp  ${CMAKE_CURRENT_SOURCE_DIR}/testorig.jpg
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testout.bmp ${CMAKE_CURRENT_SOURCE_DIR}/testimg.bmp
	COMMAND djpeg -dct int -ppm -outfile ${CMAKE_CURRENT_BINARY_DIR}/testoutp.ppm ${CMAKE_CURRENT_SOURCE_DIR}/testprog.jpg
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testoutp.ppm ${CMAKE_CURRENT_SOURCE_DIR}/testimg.ppm
	COMMENT "Testing djpeg"
	)

ADD_CUSTOM_COMMAND(
	TARGET cjpeg
	POST_BUILD
	COMMAND cjpeg -dct int -outfile ${CMAKE_CURRENT_BINARY_DIR}/testout.jpg  ${CMAKE_CURRENT_SOURCE_DIR}/testimg.ppm
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testout.jpg ${CMAKE_CURRENT_SOURCE_DIR}/testimg.jpg
	COMMAND ./cjpeg -dct int -progressive -opt -outfile ${CMAKE_CURRENT_BINARY_DIR}/testoutp.jpg ${CMAKE_CURRENT_SOURCE_DIR}/testimg.ppm
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testoutp.jpg ${CMAKE_CURRENT_SOURCE_DIR}/testimgp.jpg
	COMMENT "Testing cjpeg"
	)
	
ADD_CUSTOM_COMMAND(
	TARGET jpegtran
	POST_BUILD
	COMMAND jpegtran -outfile ${CMAKE_CURRENT_BINARY_DIR}/testoutt.jpg ${CMAKE_CURRENT_SOURCE_DIR}/testprog.jpg
	COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/testoutt.jpg ${CMAKE_CURRENT_SOURCE_DIR}/testorig.jpg
	COMMENT "Testing jpegtran" )
