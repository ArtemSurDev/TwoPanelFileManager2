cmake_minimum_required(VERSION 3.16)
project(file_manager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Test)

set(SOURCES
        src/main.cpp
        src/MainWindow.cpp
        src/FilePanel.cpp
        src/FileOperationsFacade.cpp
        src/FileCommand.cpp
        src/PanelMediator.cpp
        src/PathRestrictions.cpp
)

set(HEADERS
        include/MainWindow.h
        include/FilePanel.h
        include/FileOperationsFacade.h
        include/FileCommand.h
        include/PanelMediator.h
        include/PathRestrictions.h
)

add_executable(file_manager ${SOURCES} ${HEADERS})

target_include_directories(file_manager PRIVATE include)

target_link_libraries(file_manager Qt6::Core Qt6::Widgets Qt6::Gui)

find_package(Catch2 3 REQUIRED)

enable_testing()

set(TEST_SOURCES
        tests/TestMain.cpp
        tests/FileOperationsFacadeTest.cpp
        tests/FileCommandTest.cpp
        tests/FilePanelTest.cpp
        tests/PanelMediatorTest.cpp
        tests/MainWindowTest.cpp
)

add_executable(file_manager_tests
        ${TEST_SOURCES}
        src/FileOperationsFacade.cpp
        src/FileCommand.cpp
        src/FilePanel.cpp
        src/PanelMediator.cpp
        src/MainWindow.cpp
        src/PathRestrictions.cpp
        include/FilePanel.h
        include/MainWindow.h
        include/PathRestrictions.h
)

target_include_directories(file_manager_tests PRIVATE include tests)

target_link_libraries(file_manager_tests Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Test Catch2::Catch2)

set_target_properties(file_manager_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)

add_test(NAME file_manager_tests COMMAND file_manager_tests)

add_executable(scenario_copy_move tests/scenarios/ScenarioCopyMove.cpp src/FileOperationsFacade.cpp)
add_executable(scenario_create_rename tests/scenarios/ScenarioCreateRename.cpp src/FileOperationsFacade.cpp)
add_executable(scenario_directory_transfer tests/scenarios/ScenarioDirectoryTransfer.cpp src/FileOperationsFacade.cpp)
add_executable(scenario_error_handling tests/scenarios/ScenarioErrorHandling.cpp src/FileOperationsFacade.cpp)

target_include_directories(scenario_copy_move PRIVATE include tests)
target_include_directories(scenario_create_rename PRIVATE include tests)
target_include_directories(scenario_directory_transfer PRIVATE include tests)
target_include_directories(scenario_error_handling PRIVATE include tests)

target_link_libraries(scenario_copy_move Qt6::Core)
target_link_libraries(scenario_create_rename Qt6::Core)
target_link_libraries(scenario_directory_transfer Qt6::Core)
target_link_libraries(scenario_error_handling Qt6::Core)

set_target_properties(
        scenario_copy_move
        scenario_create_rename
        scenario_directory_transfer
        scenario_error_handling
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
)
