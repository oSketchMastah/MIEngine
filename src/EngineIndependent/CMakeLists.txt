SET( MREL ${MREL}/EngineIndependent )

add_subdirectory(BasicTypes)
add_subdirectory(Env)
add_subdirectory("Free Helpers")
add_subdirectory(Memory)
add_subdirectory(Parsers)

SET(SOURCE_EXE ${SOURCE_EXE} PARENT_SCOPE)
