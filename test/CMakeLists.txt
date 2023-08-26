add_executable(stack01 stack01.cpp)
target_link_options(stack01 PRIVATE -fsanitize=address)
target_compile_options(stack01 PRIVATE -Wall -g -fPIC -pie -fsanitize=address)
add_test(
	NAME stack01
	COMMAND stack01
)

add_executable(stack02 stack02.cpp)
target_link_options(stack02 PRIVATE -fsanitize=thread)
target_compile_options(stack02 PRIVATE -Wall -g -fPIC -pie -fsanitize=thread)
add_test(
	NAME stack02
	COMMAND stack02
)
