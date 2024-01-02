SET(MATADOR_DB_LIBRARIES)

IF(ODBC_FOUND)
  MESSAGE(STATUS "Appending ODBC libs: ${ODBC_LIBRARIES}")
  LIST(APPEND MATADOR_DB_LIBRARIES ${ODBC_LIBRARIES})
  MESSAGE(STATUS "Adding ODBC include directory: ${ODBC_INCLUDE_DIRS}")
  INCLUDE_DIRECTORIES(${ODBC_INCLUDE_DIRS})
ENDIF()

IF(MYSQL_FOUND)
  MESSAGE(STATUS "Appending MySQL libs: ${MYSQL_LIBRARY}")
  LIST(APPEND MATADOR_DB_LIBRARIES ${MYSQL_LIBRARY})
  MESSAGE(STATUS "Adding MySQL include directory: ${MYSQL_INCLUDE_DIR}")
  INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
ENDIF()

IF(SQLite3_FOUND)
  MESSAGE(STATUS "Found SQLite3  version ${SQLite3_VERSION}")
  MESSAGE(STATUS "Appending SQLite3 libs: ${SQLite3_LIBRARIES}")
  LIST(APPEND MATADOR_DB_LIBRARIES ${SQLite3_LIBRARIES})
  MESSAGE(STATUS "Adding SQLite3 include directory: ${SQLite3_INCLUDE_DIRS}")
  INCLUDE_DIRECTORIES(${SQLite3_INCLUDE_DIRS})
ENDIF()

IF(PostgreSQL_FOUND)
  MESSAGE(STATUS "Found PostgreSQL version ${PostgreSQL_VERSION_STRING}")
  MESSAGE(STATUS "Appending PostgreSQL libs: ${PostgreSQL_LIBRARIES}")
  LIST(APPEND MATADOR_DB_LIBRARIES ${PostgreSQL_LIBRARIES})
  MESSAGE(STATUS "Adding PostgreSQL include directory: ${PostgreSQL_INCLUDE_DIRS}")
  INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS})
ENDIF()