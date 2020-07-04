#! /bin/bash

#get the number of command line arguments passed in
COUNT=$#

#get the name of the user so that you can find their home directory
user=$(whoami)

STUD_SOURCES="/home/$user/Coding/C++/StudSysCpp"

usage_str="update_student_system [-h*elp|-c*lean]"

if [ "$COUNT" -gt "1" ]; then
	echo $usage_str
	exit 1;
fi

if [ "$COUNT" -eq "1" ]; then
	arg="$1"
	if [ "$arg" == "-h" ]; then
		echo "Updates the executable for the student system. Pass in -c to clean any object files left over, or if build is failing because of undefined references. This often happens after a major change to the repository"
	elif [ "$arg" == "-c" ]; then
		echo "Changing to source file directory"
		cd $STUD_SOURCES
		
		echo "Cleaning object files"
		
		make -s clean
	fi
else
	echo "Changing to source file directory"
	cd $STUD_SOURCES	


	echo "Pulling latest version from GitHub repository prior to making files"
	git pull --quiet > /dev/null #suppress output by redirecting any output to the never ending bit bucket. 
	
	echo "Making the executable"
	make -s -f silent_make #-s suppresses output and -f allows you to choose a different file than the usual makefile. silent_make is used as it doesn't print anything

	exit_status=$?

	if [ "$exit_status" -eq "0" ]; then
		echo "Executable updated successfully"
	else
		echo "Executable not updated successfully"
	fi
fi
