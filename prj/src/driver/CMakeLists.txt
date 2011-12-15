PROJECT(driver)

ADD_EXECUTABLE(master master.cpp)
ADD_EXECUTABLE(slave1 slave1.cpp)
TARGET_LINK_YOCTO(master)
TARGET_LINK_YOCTO(slave1)

IF( WIN32 )

ADD_EXECUTABLE(win32-child win32-child.cpp)
ADD_EXECUTABLE(win32-create-child win32-create-child.cpp)

ENDIF()


