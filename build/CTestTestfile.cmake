# CMake generated Testfile for 
# Source directory: /home/armando/Desktop/project/kchat-client
# Build directory: /home/armando/Desktop/project/kchat-client/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test "/home/armando/Desktop/project/kchat-client/bin/chess")
set_tests_properties(test PROPERTIES  WORKING_DIRECTORY "/home/armando/Desktop/project/kchat-client/bin")
subdirs(src/AI)
