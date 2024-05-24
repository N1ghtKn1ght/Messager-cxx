# CMakeList.txt: проект CMake для Backend; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


set(PROJECT_NAME "Backend")
project (${PROJECT_NAME})

include(${CMAKE_TOOLCHAIN_FILE})

find_package(bsoncxx CONFIG REQUIRED)
find_package(mongocxx CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)

add_executable (${PROJECT_NAME} "main.cpp"  
								"Service/Session.cpp"
								"Service/Session.hpp" 
								"Service/Server.cpp" 
								"Service/Server.hpp" 
								"Service/Database/Models/User.cpp"
								"Service/Database/Models/User.hpp"
								"Service/Database/Session.cpp" 
								"Service/Database/Session.hpp" 
								"Service/Database/Models/Message.hpp"
								"Service/Database/Models/Message.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:mongo::bsoncxx_static>,mongo::bsoncxx_static,mongo::bsoncxx_shared>)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:mongo::mongocxx_static>,mongo::mongocxx_static,mongo::mongocxx_shared>)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()



