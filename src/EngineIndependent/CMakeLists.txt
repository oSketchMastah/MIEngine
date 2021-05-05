SET(MREL ${MREL}/EngineIndependent)

add_subdirectory("Free Helpers")
list(APPEND SOURCE_EXE ${MREL}/MIBuffer.cpp ${MREL}/MIFile.cpp ${MREL}/MIFileMode.cpp ${MREL}/MILogger.cpp ${MREL}/MIString.cpp)


SET(SOURCE_EXE ${SOURCE_EXE} PARENT_SCOPE)

