#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ $0 != "bin/package.sh" ]
then
	echo "bin/package.sh: must be executed from its parent directory"
	exit 1
fi
echo_exec() {
     echo $* 
     eval $*
}
# ----------------------------------------------------------------------------
#
# build the xml version of documentation for this distribution
# -----------------------------------------------------------------------------
# Get the version number from CMakeLists.txt
#
version=`grep '^SET *( *cppad_version ' CMakeLists.txt | \
	sed -e 's|^SET *( *cppad_version *"\([0-9]*\)" *)|\1|'`
if ! (echo $version | grep '[0-9]\{8\}$')
then
	echo 'package.sh: Cannot find verison number in CMakeLists.txt'
	exit 1
fi 
#
# Create the package directory
package_dir="cppad-$version"
if [ -e "$package_dir" ]
then
	echo_exec rm -r $package_dir
fi
echo_exec mkdir $package_dir
#
# Create list of files to distribute
if [ -e 'work' ]
then
	echo_exec rm -r work
fi
if [ -e 'doc' ]
then
	echo_exec rm -r doc
fi
file_list=`find . \
	\( -name '*.ac' \) -or \
	\( -name '*.am' \) -or \
	\( -name '*.c' \) -or \
	\( -name '*.cpp' \) -or \
	\( -name '*.h' \) -or \
	\( -name '*.hpp' \) -or \
	\( -name '*.in' \) -or \
	\( -name '*.omh' \) -or \
	\( -name '*.pc' \) -or \
	\( -name '*.sh' \) -or \
	\( -name '*.txt' \) | sed -e 's|^\./||'`
other_files="
	AUTHORS 
	ChangeLog 
	configure 
	COPYING 
	INSTALL 
	LICENSE 
	NEWS 
	README 
	uw_copy_040507.html
"
#
# Copy the files, creating sub-directories when necessary
for file in $file_list $other_files
do
	sub_dir=`echo $file | sed -e 's|\(.*\)/[^/]*$|\1|'`
	if [ "$sub_dir" != "$file" ]
	then
		if [ ! -e "$package_dir/$sub_dir" ]
		then
			echo_exec mkdir -p $package_dir/$sub_dir
		fi
	fi
	echo_exec cp $file $package_dir/$file
done
exit 0
# ----------------------------------------------------------------------------
# create an empty doc director and change into it.
echo_exec mkdir doc
echo_exec cd doc
#
cmd='omhelp ../doc.omh -noframe -debug -xml -l http://www.coin-or.org/CppAD/'
echo "$cmd > omhelp.xml.log"
if ! eval $cmd > ../omhelp.xml.log
then
	grep '^OMhelp Error:' ../omhelp.xml.log
	echo 'OMhelp could not build the CppAD xml documnentation.'
	echo 'See the complete error message in omhelp.xml.log'
	exit 1
fi
if grep '^OMhelp Warning:' ../omhelp.xml.log
then
	echo 'See the complete warning message in omhelp.xml.log'
	exit 1
fi
