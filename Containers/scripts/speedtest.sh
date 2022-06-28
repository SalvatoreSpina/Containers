#!/usr/bin/env bash

include_path="./"
srcs="./containers_test/srcs"
CC="clang++"
CFLAGS="-Wall -Wextra -Werror -std=c++98"

compile () {
	# 1=file 2=define used {ft/std} 3=output_file
	macro_name=$(echo "USING_${2}" | awk '{ print toupper($0) }')
	compile_cmd="$CC $CFLAGS -o ${3} -I./$include_path -D ${macro_name} ${1}"
	if [ -n "$4" ]; then
		compile_cmd+=" &>${4}"
	fi
	eval "${compile_cmd}"
	return $?
}

stl_time () {
	container=$(echo $1 | cut -d "/" -f 2)
	file=$(echo $1 | cut -d "/" -f 3)
	testname=$(echo $file | cut -d "." -f 1)
	std_bin="std.$container.out";
	compile "$1" "std" "$std_bin";
}

my_time() {
	container=$(echo $1 | cut -d "/" -f 2)
	file=$(echo $1 | cut -d "/" -f 3)
	testname=$(echo $file | cut -d "." -f 1)
	ft_bin="ft.$container.out";
	compile "$1" "ft"  "$ft_bin";
}

do_test () {
	# 1=container_name
	test_files=$(find "${srcs}/${1}" -type f -name '*.cpp' | sort)

    printf "Compiling $1...\n"
	start=$SECONDS
	for file in ${test_files[@]}; do
		my_time "${file}";
	done
	duration=$(( SECONDS - start ))
	printf "My Containers time to compile and execute: $duration seconds\n\n"

    printf "Compiling STL $1...\n"
	start=$SECONDS
	for file in ${test_files[@]}; do
		stl_time "${file}";
	done
	duration=$(( SECONDS - start ))
	printf "STL Containers time to compile and execute: $duration seconds\n\n"
}

function main () {
	containers=(vector map stack set)
	# containers=(vector map stack set)
	if [ $# -ne 0 ]; then
		containers=($@);
	fi

	for container in ${containers[@]}; do
		do_test $container 2>/dev/null
	done 
}
