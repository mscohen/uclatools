INCLUDE_DIRECTORIES(${UCLATOOLS_SOURCE_DIR}/ImgLib)

ADD_EXECUTABLE(ShowAnaHdr ShowAnaHdr.c)

TARGET_LINK_LIBRARIES(ShowAnaHdr ImgLib)

INSTALL(TARGETS ShowAnaHdr RUNTIME DESTINATION bin)
