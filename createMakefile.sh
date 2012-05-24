#!/bin/bash
# Copyright (c) 2009, Josef Nygrin, http://justcheckingonall.wordpress.com/
# All rights reserved.
#
# This is a script to generate Makefile based on source files (.c, .cpp, .c++
# and .cxx) present in current directory, using 'gcc -MM'.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY JOSEF NYGRIN ''AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL JOSEF NYGRIN BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Requirements
# - the script: gcc (and bash, ls, mkdir)
# - the 'make profile' target: gprof, gprof2dot.py, dot (graphwiz)
# - the 'make debug' target: gdb
# - the automatic build number increment: awk (and mv)
#
# Future extensions (todo list):
# - add the checks in -f, -l and -c options as indicated
# - add support for subdirectories
# - more checks done (valid compiler, executable name, ...)
# - support for other -MM compatible compilers
# - support for other command line syntaxes
# - allow different compilers (and settings) for .c, .cpp and .cxx individually

################################################################################
function print_usage_to_stderr
{
	echo "Usage:" > /dev/stderr;
	echo "    $0 [options] exec" > /dev/stderr;
	echo "Run '$0 -h' or '$0 --help' for more info." > /dev/stderr;
}

################################################################################
function print_help
{
	printf "Not so Simple Makefile generator, gen_make.sh, version 2.0.\n";
	printf "Copyright (c) 2009, Josef Nygrin, http://justcheckingonall.wordpress.com/\n";
	printf "\n";
	printf "Usage:\n";
	printf "    \033[1mgen_make.sh [options] exec\033[0m\n";
	printf "\n";
	printf "The resulting Makefile is printed to \033[1mstdout\033[0m.\n";
	printf "gen_make currently supports building of executables, static libraries, and\n";
	printf "shared libraries. This choice is done based on format of \033[1mexec\033[0m parameter.\n";
	printf "\n";
	printf "    \033[1mexec\033[0m\n";
	printf "        name of the output binary file to build\n";
	printf "\n";
	printf "Options:\n";
	printf "    \033[1m-h\033[0m\n";
	printf "    \033[1m--help\033[0m\n";
	printf "        Prints this help message.\n";
	printf "    \033[1m-s\033[0m\n";
	printf "        Silent mode. On default, gen_make script prints out some info about\n";
	printf "        selected configuration to \033[1mstderr\033[0m, this switch prevents this.\n";
	printf "    \033[1m-t\033[0m target\n";
	printf "        Build target. gen_make supports targets \033[1md, dp, r, rp\033[0m:\n";
	printf "        \033[1m'd'ebug\n";
	printf "        'd'ebug 'p'rofile\n";
	printf "        'r'elease\033[0m (default)\n";
	printf "        \033[1m'r'elease 'p'rofile\033[0m\n";
	printf "        For 'profile' targets, 'make profile' command is included, that creates\n";
	printf "        profile.txt using gprof(1), and profile.png using gprof2dot.py and dot.\n";
	printf "    \033[1m-f\033[0m 'flags'\n";
	printf "        Additional flags to pass to compiler.\n";
	printf "    \033[1m-l\033[0m 'libs'\n";
	printf "        Link libraries. Use \033[1m-l 'mylib urlib'\033[0m for '-lmylib -lurlib'.\n";
	printf "    \033[1m-c\033[0m compiler\n";
	printf "        Select compiler. Default is \033[1mg++\033[0m.\n";
	printf "    \033[1m-bn\033[0m\n";
	printf "        Enable build number. If selected, gen_make will create BuildNum.h header\n";
	printf "        file, if it does not exist yet, and will include script to automatically\n";
	printf "        increment the build number on each build.\n";
	printf "    \033[1m-od\033[0m dir_name\n";
	printf "        Output directory. gen_make chooses name of output directory based on\n";
	printf "        selected build target, and creates the directory. This switch allows\n";
	printf "        you to select different directory.\n";
	printf "    \033[1m-exc\033[0m 'files'\n";
	printf "        List of files excluded from build. E.g. \033[1m-exc 'file1.cpp file2.cpp'\033[0m.\n";
	printf "    \033[1m-par\033[0m 'parameters'\n";
	printf "        Run parameters for the executable. gen_make includes 'make run' and\n";
	printf "        'make debug' for executables, for executing and debugging using gdb(1),\n";
	printf "        respectively. Here you can provide any parameters to the executable.\n";
	printf "    \033[1m-post\033[0m 'command'\n";
	printf "        Provide any command that should be executed as a post-build step.\n";
	printf "        E.g. \033[1m-post 'echo We are done...'\033[0m (single quotes).\n";
	printf "        Command itself is not echo-ed by the make.\n";
}

################################################################################
# if no parameter passed, print out usage and quit
if [ $# -le '0' ]
then
print_usage_to_stderr;
exit;
fi

# name of executable
exec="autoPilot1.0";
# target - 'd'ebug, 'r'elease, 'd'ebug 'p'rofile, 'r'elease 'p'rofile
# for profile targets, "make profile" will be also created, using the gprof2dot.py
target="";
# generate 'make profile' target?
profile_target="no";
# common flags to use based on target
cflags="";
# additional flags provided by user
addflags="";
# list of additional link libraries, mylib for -lmylib
link_libs="pthread";
# compiler to use
compiler="g++";
# generate BuildNum.h and include script to increase build num?
build_num="no";
# output directory
out_dir="release";
# list of files to exclude from builds
exclude_files="";
# parameters to pass to executable on "make run" and "make debug"
run_params="";
# be silent? i.e. do not print conf info to stderr?
silent="no";
# post-build step command(s)
postbuild_step="";

################################################################################
# parse input params
until [ -z "$1" ]
do
	case "$1" in
	############################################################################
	"-h")		# print help
		print_help;
		exit;
		;;
	"--help")	# print help
		print_help;
		exit;
		;;
	############################################################################
	"-s")		# silent?
		silent="yes";
		;;
	############################################################################
	"-t")		# target
		shift;	# now we need the param after '-t'
		target="$1";
		case $target in
		"d")
			cflags="-g -Wall -Werror -DDEBUG -D_DEBUG";
			;;
		"dp")
			cflags="-p -g -Wall -Werror -DDEBUG -D_DEBUG";
			profile_target="yes";
			;;
		"r")
			cflags="-O3";
			;;
		"rp")
			cflags="-p -O3";
			profile_target="yes";
			;;
		*)
			echo "Unsupported build target provided with -t, '$target'!" > /dev/stderr;
			echo "Supported targets: d, r, dp, rp." > /dev/stderr;
			exit;
		esac
		;;
	############################################################################
	"-f")		# additional flags
		shift;
		# TODO: how can we check if flags are correct ones?
		addflags="$1";
		;;
	############################################################################
	"-l")		# link libraries
		shift;
		# save the list; TODO: how can we check correctness?
		link_libs="$1";
		;;
	############################################################################
	"-c")		# compiler
		shift;
		# TODO: how can we check this really is a compiler? run it?
		compiler="$1";
		;;
	############################################################################
	"-bn")		# build number enabled
		build_num="yes";
		;;
	############################################################################
	"-od")		# output directory
		shift;
		out_dir="$1";
		;;
	############################################################################
	"-exc")		# list of exclude files
		shift;
		exclude_files="$1";
		;;
	############################################################################
	"-par")		# run parameters to the executable
		shift;
		run_params="$1";
		;;
	############################################################################
	"-post")	# post-build step command(s)
		shift;
		postbuild_step="$1";
		;;
	############################################################################
	*)		# mistake, or executable name
		exec="$1";
		
		# if first letter is '-', this is most probably invalid option
		if [ ${exec:0:1} = "-" ]
		then
			echo "Unknown option '$exec' passed!" > /dev/stderr;
			print_usage_to_stderr;
			exit;
		fi
		
		# this should be last param to script (i.e. '-z $2')
		if [ ! -z $2 ]
		then
			echo "The provided executable name '$exec' should be last parameter!" > /dev/stderr;
			print_usage_to_stderr;
			exit;
		fi
		;;
esac
shift;
done

################################################################################
# the only required information is name of executable
if [ -z $exec ]
then
	echo "No name of executable provided!" > /dev/stderr;
	print_usage_to_stderr;
	exit;
fi

################################################################################
# determine executable's type
exec_type="exe";
if [[ "$exec" == lib*.a || "$exec" == *.lib ]]
then exec_type="lib";
elif [[ "$exec" == lib*.so || "$exec" == *.dll ]]
then exec_type="dll";
fi

################################################################################
# if target is not set, presume 'r'elease
if [ -z $target ]
then
	target="r";
	cflags="-O3";
fi

################################################################################
# if no output directory set, choose based on target
if [ -z $out_dir ]
then
	case $target in
	"d")
		out_dir="debug_build";
		;;
	"dp")
		out_dir="debug_profile";
		;;
	"r")
		out_dir="release_build";
		;;
	"rp")
		out_dir="release_profile";
		;;
	esac
fi

# create the binary output directory, if it doesn't exist yet
mkdir $out_dir &> /dev/null;

################################################################################
# print info to stderr for user to review, if not silent
if [ $silent = "no" ]
then
	# print the collected information to stderr
	echo "Name of executable: $exec." > /dev/stderr;
	echo "Type of executable: $exec_type." > /dev/stderr;
	echo "Selected target: $target." > /dev/stderr;
	echo "Compiler flags: $cflags." > /dev/stderr;
	if [ ! -z "$addflags" ]; then echo "Additional flags: $addflags." > /dev/stderr;
	else echo "Additional flags: (none)." > /dev/stderr; fi
	if [ ! -z "$link_libs" ]; then echo "Link libraries: $link_libs." > /dev/stderr;
	else echo "Link libraries: (none)." > /dev/stderr; fi
	echo "Selected compiler: $compiler." > /dev/stderr;
	echo "Build number header and auto-increment: $build_num." > /dev/stderr;
	echo "Output directory: $out_dir." > /dev/stderr;
	if [ ! -z "$exclude_files" ]; then echo "Files excluded from compilation: $exclude_files." > /dev/stderr;
	else echo "Files excluded from compilation: (none)." > /dev/stderr; fi
	if [ ! -z "$run_params" ]; then echo "Run parameters: $run_params." > /dev/stderr;
	else echo "Run parameters: (none)." > /dev/stderr; fi
	echo "Include 'make profile': $profile_target." > /dev/stderr;
	if [ ! -z "$postbuild_step" ]; then echo "Post-build step: $postbuild_step." > /dev/stderr;
	else echo "Post-build step: (none)." > /dev/stderr; fi
fi

################################################################################
# print out the common header
printf "#\n";
printf "# Makefile for '$exec'.\n";
printf "#\n";
printf "# Type 'make' or 'make $exec' to create the binary.\n";
printf "# Type 'make clean' or 'make clear' to delete all temporaries.\n";
if [ $exec_type = "exe" ]
then
	printf "# Type 'make run' to execute the binary.\n";
	printf "# Type 'make debug' to debug the binary using gdb(1).\n";
fi
if [[ $exec_type = "exe" && $profile_target = "yes" ]]
then
	printf "# Type 'make profile' to create profile image using gprof(1) and gprof2dot.py\n";
	printf "#    after a successful run of the executable; will create profile.txt and .png.\n";
fi
printf "#\n";
printf "\n";
printf "# build target specs\n";
printf "CC = $compiler\n";
printf "CFLAGS = $cflags $addflags\n";
printf "OUT_DIR = $out_dir\n";
printf "LIBS =";
for lib in $link_libs
do
	printf " -l$lib";
done
printf "\n\n";
printf "# first target entry is the target invoked when typing 'make'\n";
printf "default: $exec\n";
printf "\n";

################################################################################
# print for each file with provided extension ($1 - first
# parameter) string " $(OUT_DIR)/filename.o"
function bin_deps_for_ext
{
	ls *.$1 &> /dev/null;
	# did the 'ls' exit with 0? (all ok)
	if [ $? -eq '0' ]
	then
		for i in `ls *.$1`
		do
			# is this an excluded file? if so, do not list it
			excl=0;
			for file in $exclude_files; do
				if [ $file = $i ]; then excl=1; break; fi
			done
			if [ $excl -eq 0 ]; then printf " \$(OUT_DIR)/$i.o"; fi
		done
	fi
}

# print the binary's deps (depends on all object files,
# one for each source .c, .cpp and .cxx file)
# if the exec's name ends with '.a' or '.lib', treat it as static library
printf "$exec:";

# output binary depends on all created object files
bin_deps_for_ext c;
bin_deps_for_ext cpp;
bin_deps_for_ext c++;
bin_deps_for_ext cxx;
printf "\n";

# print the build number increment script, and create BuildNum.h, if necessary
if [ $build_num = "yes" ]
then
	# if the BuildNum.h does not exist yet, create it
	ls BuildNum.h &> /dev/null;
	if [ $? -ne '0' ]
	then
		printf "// Version and build number header for $exec.\n" > BuildNum.h;
		printf "// Generated by gen_make.sh, build number is auto-incremented on build.\n" >> BuildNum.h;
		printf "//\n\n" >> BuildNum.h;
		printf "#ifndef __BuildNum_h__\n" >> BuildNum.h;
		printf "#define __BuildNum_h__\n\n" >> BuildNum.h;
		printf "#define Version1\t1\n" >> BuildNum.h;
		printf "#define Version2\t0\n" >> BuildNum.h;
		printf "#define Version3\t0\n" >> BuildNum.h;
		printf "#define BuildNum\t1\n\n" >> BuildNum.h;
		printf "#endif//__BuildNum_h__\n\n" >> BuildNum.h;
	fi
	# add the script to increase build number on each build
	printf "\t@echo -n 'Increasing build number in BuildNum.h... '\n";
	printf "\t@awk '!/define BuildNum/ {print} /define BuildNum/ {print \"#define BuildNum\\\\t\"\$\$3+1}' BuildNum.h > BuildNum.h~\n";
	printf "\t@mv BuildNum.h~ BuildNum.h\n";
	printf "\t@echo Done.\n";
fi

# print command used to put the output binary together, depending on type
printf "\t@echo -n 'Linking $exec... '\n";
if [ "$exec_type" == "lib" ]; then printf "\t@ar rsc $exec";
elif [ "$exec_type" == "dll" ]; then printf "\t@\$(CC) -shared -Wl,-soname,$exec -o $exec";
else printf "\t@\$(CC) \$(CFLAGS) -o $exec"; fi

# and print its input - all created object files
bin_deps_for_ext c;
bin_deps_for_ext cpp;
bin_deps_for_ext c++;
bin_deps_for_ext cxx;
printf " \$(LIBS)\n";
printf "\t@echo Done.\n";

if [ ! -z "$postbuild_step" ]; then printf "\t@echo Executing post-build step...\n"; printf "\t@$postbuild_step\n"; fi
printf "\n";

################################################################################
# print out dependencies and build rules for all files
# with extension passed as parameter ($1)
function print_deps_for_ext
{
	ls *.$1 &> /dev/null;
	# did the 'ls' exit with 0? (all ok)
	if [ $? -eq '0' ]
	then
		for i in `ls *.$1`
		do
			# is this an excluded file? if so, do not list it
			excl=0;
			for file in $exclude_files; do
				if [ $file = $i ]; then excl=1; break; fi
			done
			if [ $excl -eq 0 ];
			then
				gcc -MM -MG -MT \$\(OUT_DIR\)/$i.o $i;
				printf "\t@echo -n 'Compiling $i... '\n";
				printf "\t@\$(CC) \$(CFLAGS) -o \$(OUT_DIR)/$i.o -c $i\n";
				printf "\t@echo Done.\n\n";
			fi
		done
	fi
}

# print out individual deps for object files
print_deps_for_ext c;
print_deps_for_ext cpp;
print_deps_for_ext c++;
print_deps_for_ext cxx;

################################################################################
# add the 'run' and 'debug' target
if [ $exec_type = "exe" ]
then
	printf "run:\n";
	printf "\t./$exec $run_params\n\n";
	printf "debug:\n";
	if [ -z "$run_params" ]; then
		printf "\tgdb ./$exec\n\n";
	else
		printf "\tgdb --args ./$exec $run_params\n\n";
	fi
fi

################################################################################
# add the 'create profile' target
#
# this target generates profile.txt using gprof(1),
# and profile.png using gprof2dot.py script and dot tool
if [[ $exec_type = "exe" && $profile_target = "yes" ]]
then
	printf "profile:\n"
	printf "\t@echo -n 'Creating profile.txt and profile.png... '\n";
	printf "\t@gprof $exec > profile.txt && ./gprof2dot.py profile.txt | dot -Tpng -o profile.png\n";
	printf "\t@echo Done.\n\n";
fi

################################################################################
# add the 'clean up' target - 'make clean', 'make clear'
printf "clean:\n"
printf "\t@echo -n 'Removing all temporary binaries... '\n";
printf "\t@rm -f $exec \$(OUT_DIR)/*.o\n";
printf "\t@echo Done.\n\n";
printf "clear:\n"
printf "\t@echo -n 'Removing all temporary binaries... '\n";
printf "\t@rm -f $exec \$(OUT_DIR)/*.o\n";
printf "\t@echo Done.\n\n";


