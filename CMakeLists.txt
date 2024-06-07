include_directories(${PROJECT_SOURCE_DIR}/src)
# ============ Ddisasm Loader pass =================
set(DDISASM_LOADER_DATALOG_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/datalog/ddisasm_loader.dl)
set(DDISASM_LOADER_DATALOG_MAIN
        ${CMAKE_CURRENT_SOURCE_DIR}/datalog/ddisasm_loader.dl)

set(DDISASM_LOADER_CPP "${CMAKE_BINARY_DIR}/src/passes/souffle_ddisasm_loader.cpp")

add_custom_command(
        OUTPUT ${DDISASM_LOADER_CPP}
        # Souffle includes the path of the output file in the generated program
        # name. Change directory and use a relative path so the name does not
        # depend on build location.
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/passes"
        COMMAND
        ${SOUFFLE} ${DDISASM_LOADER_DATALOG_MAIN} -g souffle_ddisasm_loader.cpp -N
        souffle_ddisasm_loader -jauto ${SOUFFLE_PREPROCESSOR_ARG}
        DEPENDS ${DDISASM_LOADER_DATALOG_SOURCES})

add_library(ddisasm_loader_pass STATIC DdisasmLoaderPass.cpp ${DDISASM_LOADER_CPP})

if(SOUFFLE_INCLUDE_DIR)
    target_include_directories(ddisasm_loader_pass SYSTEM
            PRIVATE ${SOUFFLE_INCLUDE_DIR})
endif()

target_link_libraries(ddisasm_loader_pass gtirb gtirb_pprinter gtirb_decoder)

target_compile_definitions(ddisasm_loader_pass PRIVATE __EMBEDDED_SOUFFLE__)
target_compile_definitions(ddisasm_loader_pass PRIVATE RAM_DOMAIN_SIZE=64)
target_compile_options(ddisasm_loader_pass PRIVATE ${OPENMP_FLAGS})
if(SOUFFLE_INCLUDE_DIR)
    target_include_directories(ddisasm_loader_pass SYSTEM
            PRIVATE ${SOUFFLE_INCLUDE_DIR})
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set_common_msvc_options(ddisasm_loader_pass)

    set_souffle_msvc_options(ddisasm_loader_pass)
else()
    target_compile_options(ddisasm_loader_pass PRIVATE -O3)
endif()
# ============ CodeBlock Loader pass =================
add_library(code_block_loader_pass STATIC CodeBlockLoaderPass.cpp)

target_link_libraries(code_block_loader_pass gtirb gtirb_pprinter)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set_common_msvc_options(code_block_loader_pass)

    # The following warning is coming from the bgl, but only in release mode
    if(("${CMAKE_BUILD_TYPE}" STREQUAL "Release") OR ("${CMAKE_BUILD_TYPE}"
            STREQUAL "RelWithDebInfo"))
        target_compile_options(
                code_block_loader_pass PRIVATE -wd4172) # returning address of local variable or
        # temporary
    endif()
endif()
# ============ DataBlock Loader pass =================
add_library(data_block_loader_pass STATIC DataBlockLoaderPass.cpp)

target_link_libraries(data_block_loader_pass gtirb gtirb_pprinter)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set_common_msvc_options(data_block_loader_pass)

    # The following warning is coming from the bgl, but only in release mode
    if(("${CMAKE_BUILD_TYPE}" STREQUAL "Release") OR ("${CMAKE_BUILD_TYPE}"
            STREQUAL "RelWithDebInfo"))
        target_compile_options(
                data_block_loader_pass PRIVATE -wd4172) # returning address of local variable or
        # temporary
    endif()
endif()
# ============ ProxyBlock Loader pass =================
add_library(proxy_block_loader_pass STATIC ProxyBlockLoaderPass.cpp)

target_link_libraries(proxy_block_loader_pass gtirb gtirb_pprinter)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set_common_msvc_options(proxy_block_loader_pass)

    # The following warning is coming from the bgl, but only in release mode
    if(("${CMAKE_BUILD_TYPE}" STREQUAL "Release") OR ("${CMAKE_BUILD_TYPE}"
            STREQUAL "RelWithDebInfo"))
        target_compile_options(
            proxy_block_loader_pass PRIVATE -wd4172) # returning address of local variable or
        # temporary
    endif()
endif()
# ============ Symbol Loader pass =================
add_library(symbol_loader_pass STATIC SymbolLoaderPass.cpp)

target_link_libraries(symbol_loader_pass gtirb gtirb_pprinter)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set_common_msvc_options(symbol_loader_pass)

    # The following warning is coming from the bgl, but only in release mode
    if(("${CMAKE_BUILD_TYPE}" STREQUAL "Release") OR ("${CMAKE_BUILD_TYPE}"
            STREQUAL "RelWithDebInfo"))
        target_compile_options(
                symbol_loader_pass PRIVATE -wd4172) # returning address of local variable or
        # temporary
    endif()
endif()
# ============ SymbolicExpression Loader pass =================
add_library(symbolic_expression_loader_pass STATIC SymbolicExpressionLoaderPass.cpp)

target_link_libraries(symbolic_expression_loader_pass gtirb gtirb_pprinter)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set_common_msvc_options(symbolic_expression_loader_pass)

    # The following warning is coming from the bgl, but only in release mode
    if(("${CMAKE_BUILD_TYPE}" STREQUAL "Release") OR ("${CMAKE_BUILD_TYPE}"
            STREQUAL "RelWithDebInfo"))
        target_compile_options(
                symbolic_expression_loader_pass PRIVATE -wd4172) # returning address of local variable or
        # temporary
    endif()
endif()
# ============ Function Loader pass =================
add_library(function_loader_pass STATIC FunctionLoaderPass.cpp)

target_link_libraries(function_loader_pass gtirb gtirb_pprinter)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set_common_msvc_options(function_loader_pass)

    # The following warning is coming from the bgl, but only in release mode
    if(("${CMAKE_BUILD_TYPE}" STREQUAL "Release") OR ("${CMAKE_BUILD_TYPE}"
            STREQUAL "RelWithDebInfo"))
        target_compile_options(
                function_loader_pass PRIVATE -wd4172) # returning address of local variable or
        # temporary
    endif()
endif()
# ============ Datalog Superset pass =================
set(DATALOG_SUPERSET_DATALOG_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/datalog/datalog_superset.dl)
set(DATALOG_SUPERSET_DATALOG_MAIN
        ${CMAKE_CURRENT_SOURCE_DIR}/datalog/datalog_superset.dl)

set(DATALOG_SUPERSET_CPP "${CMAKE_BINARY_DIR}/src/passes/souffle_datalog_superset.cpp")

add_custom_command(
        OUTPUT ${DATALOG_SUPERSET_CPP}
        # Souffle includes the path of the output file in the generated program
        # name. Change directory and use a relative path so the name does not
        # depend on build location.
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/src/passes"
        COMMAND
        ${SOUFFLE} ${DATALOG_SUPERSET_DATALOG_MAIN} -g souffle_datalog_superset.cpp -N
        souffle_datalog_superset -jauto ${SOUFFLE_PREPROCESSOR_ARG}
        DEPENDS ${DATALOG_SUPERSET_DATALOG_SOURCES})

add_library(datalog_superset_pass STATIC DatalogSupersetPass.cpp ${DATALOG_SUPERSET_CPP})

if(SOUFFLE_INCLUDE_DIR)
    target_include_directories(datalog_superset_pass SYSTEM
            PRIVATE ${SOUFFLE_INCLUDE_DIR})
endif()

target_link_libraries(datalog_superset_pass gtirb gtirb_pprinter gtirb_decoder)

target_compile_definitions(datalog_superset_pass PRIVATE __EMBEDDED_SOUFFLE__)
target_compile_definitions(datalog_superset_pass PRIVATE RAM_DOMAIN_SIZE=64)
target_compile_options(datalog_superset_pass PRIVATE ${OPENMP_FLAGS})
if(SOUFFLE_INCLUDE_DIR)
    target_include_directories(datalog_superset_pass SYSTEM
            PRIVATE ${SOUFFLE_INCLUDE_DIR})
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    set_common_msvc_options(datalog_superset_pass)

    set_souffle_msvc_options(datalog_superset_pass)
else()
    target_compile_options(datalog_superset_pass PRIVATE -O3)
endif()
# ============ Gtirb Mixer =================
add_executable(gtirb-mixer Main.cpp ../../Functors.cpp)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    target_compile_options(gtirb-mixer PRIVATE -Wno-unused-parameter)
endif()

if(DDISASM_SOUFFLE_PROFILING)
    target_compile_definitions(gtirb-mixer PRIVATE DDISASM_SOUFFLE_PROFILING)
endif()

target_include_directories(
        gtirb-mixer PRIVATE $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>)
if(ehp_INCLUDE_DIR)
    target_include_directories(gtirb-mixer PRIVATE ${ehp_INCLUDE_DIR})
endif()
if(CAPSTONE_INCLUDE_DIR)
    target_include_directories(gtirb-mixer PRIVATE ${CAPSTONE_INCLUDE_DIR})
endif()
if(SOUFFLE_INCLUDE_DIR)
    target_include_directories(gtirb-mixer SYSTEM PRIVATE ${SOUFFLE_INCLUDE_DIR})
endif()

target_compile_definitions(gtirb-mixer PRIVATE __EMBEDDED_SOUFFLE__)
target_compile_definitions(gtirb-mixer PRIVATE RAM_DOMAIN_SIZE=64)
target_compile_options(gtirb-mixer PRIVATE ${OPENMP_FLAGS})

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    target_compile_definitions(gtirb-mixer PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(gtirb-mixer PRIVATE _CRT_NONSTDC_NO_WARNINGS)

    set_msvc_lief_options(gtirb-mixer)
    set_common_msvc_options(gtirb-mixer)
else()
    target_compile_options(gtirb-mixer PRIVATE -O3)
    target_compile_options(gtirb-mixer PRIVATE -Wall)
    target_compile_options(gtirb-mixer PRIVATE -Wextra -Wpointer-arith)
    target_compile_options(gtirb-mixer PRIVATE -Werror)
endif()

if(${GTIRB_USE_SYSTEM_BOOST} MATCHES "OFF")
    add_dependencies(gtirb-mixer Boost)
endif()

if(DDISASM_STATIC_DRIVERS)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
        # We do not want to statically link in the STL here, as MSVC is a bit
        # prickly about STL ABIs.
    else()
        target_link_libraries(gtirb-mixer PRIVATE -static-libstdc++ -static-libgcc)
    endif()
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    target_link_libraries(gtirb-mixer PRIVATE 
            scc_pass 
            code_block_loader_pass 
            data_block_loader_pass 
            proxy_block_loader_pass
            symbol_loader_pass 
            symbolic_expression_loader_pass 
            function_loader_pass 
            ddisasm_loader_pass
            datalog_superset_pass
            no_return_pass function_inference_pass)

    target_link_options(
            gtirb-mixer PRIVATE /WHOLEARCHIVE:ddisasm_loader_pass$<$<CONFIG:Debug>:d>
            /WHOLEARCHIVE:datalog_superset_pass$<$<CONFIG:Debug>:d>
            /WHOLEARCHIVE:code_block_loader_pass<$<CONFIG:Debug>:d>
            /WHOLEARCHIVE:data_block_loader_pass<$<CONFIG:Debug>:d>
            /WHOLEARCHIVE:proxy_block_loader_pass<$<CONFIG:Debug>:d>
            /WHOLEARCHIVE:symbol_loader_pass<$<CONFIG:Debug>:d>
            /WHOLEARCHIVE:symbolic_expression_loader_pass<$<CONFIG:Debug>:d>
            /WHOLEARCHIVE:function_loader_pass<$<CONFIG:Debug>:d>
            /WHOLEARCHIVE:no_return_pass$<$<CONFIG:Debug>:d>
            /WHOLEARCHIVE:function_inference_pass$<$<CONFIG:Debug>:d>)
else()
    if(APPLE)
        target_link_libraries(
                gtirb-mixer PRIVATE 
                scc_pass 
                code_block_loader_pass 
                data_block_loader_pass
                proxy_block_loader_pass
                symbol_loader_pass 
                symbolic_expression_loader_pass 
                function_loader_pass                  
                -Wl,-all_load ddisasm_loader_pass
                datalog_superset_pass
                no_return_pass function_inference_pass -Wl,-noall_load)
    else()
        target_link_libraries(
                gtirb-mixer
                PRIVATE 
                scc_pass 
                code_block_loader_pass 
                data_block_loader_pass
                proxy_block_loader_pass
                symbol_loader_pass 
                symbolic_expression_loader_pass 
                function_loader_pass
                -Wl,--whole-archive ddisasm_loader_pass
                datalog_superset_pass
                no_return_pass function_inference_pass -Wl,--no-whole-archive)
    endif()
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    if(DDISASM_STATIC_DRIVERS)
        target_link_libraries(gtirb-mixer PRIVATE -l:libgomp.a)
    else()
        target_link_libraries(gtirb-mixer PRIVATE gomp)
    endif()
endif()

target_link_libraries(
        gtirb-mixer
        PRIVATE
        ddisasm_pipeline
        gtirb
        gtirb_pprinter
        gtirb_builder
        gtirb_decoder
        generic_pass
        ${Boost_LIBRARIES}
        ${EXPERIMENTAL_LIB}
        ${LIBCPP_ABI}
        ${DDISASM_EXTRA_LIBS}
        ${LIBSTDCXX_FS})

install(TARGETS gtirb-mixer
        COMPONENT gtirb-mixer
        DESTINATION bin)
