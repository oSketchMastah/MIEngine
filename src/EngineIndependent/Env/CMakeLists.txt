SET(MREL ${MREL}/Env)

add_subdirectory(Console)
list(APPEND SOURCE_EXE ${MREL}/MILogger.cpp)


SET(SOURCE_EXE ${SOURCE_EXE} PARENT_SCOPE)

