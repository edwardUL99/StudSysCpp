#!/bin/bash

#Script to run StudentSystem

#Get the count of command line arguments
COUNT=$#

database=""
user=""
pass=""
host=""
file=""
separator=""
adminUsername=""
adminPass=""
configFile="false"

usage_str="student_system [-d] [database_name] (if not specified, default is student_sys) -u <user_name> -p <password> [-h] [host] (if host not specified, default is localhost) -au <admin_username> -ap <admin_password>"

usage_str_file="student_system -f <file_name>"

if [ "$COUNT" -eq "0" ]; then
	echo "Do you want to enter a path to a configuration file: (y/n)"
	read choice

	while [ $choice != "y" ] && [ $choice != "n" ];
	do
		echo "Enter y or n: "
		read choice
	done

	if [ $choice == "y" ]; then
		configFile="true"
		echo "Enter the file name: "
		read file
	else 
		configFile="false"
	
    	echo "Enter the database name: "
    	read database

   	 	echo "Enter the username: "
    	read user

    	echo "Enter the password: "
    	read -s pass

    	echo "Enter the host: "
    	read host

		echo "Enter the admin username: "
		read adminUsername

		echo "Enter the admin password: "
		read -s adminPass
	fi
fi

if [ "$COUNT" -eq "2" ]; then
    if [ "$1" == "-f" ]; then
		file="$2"
    else
		echo $usage_str_file
    fi
fi

if [ -z $file ] && [ $configFile != "true" ]; then #if file is empty, it means there were the actual parameters passed in on command line, not a commandline
   while [ "$COUNT" -gt "0" ];
   	do
    		case "$1" in
			-d ) database="$2"
	     		 shift 2
	             COUNT=$(expr $COUNT - 2)
	             ;;
			-u ) user="$2"
	     		 shift 2
	     		 COUNT=$(expr $COUNT - 2)
	     		;;
        	-p ) pass="$2"
	     		 shift 2
	     		 COUNT=$(expr $COUNT - 2)
	     		;;
			-h ) host="$2"
	     		shift 2
	     		COUNT=$(expr $COUNT - 2)
	     		;;
		 	-au ) adminUsername="$2"
			 	  shift 2
				  COUNT=$(expr $COUNT - 2)
				  ;;
			-ap ) adminPass="$2"
				  shift 2
				  COUNT=$(expr $COUNT - 2)
				  ;;
			* ) echo $usage_str
				exit 1
	    	esac
	done
fi
	
#the location of where "installed" (your own programs) are found
PROGRAM_LOCATION="/progs/"
PROGRAM="studsys"

if [ "$COUNT" -eq 0 ] && [ -z "$file" ]; then
	#you have went through case statement so more than 1 argument was given
	if [ -z "$database" ]; then
   		database="student_sys"
	fi
	
	if [ -z "$user" ]; then
   		echo $usage_str
   		exit 1
	fi	
	
	if [ -z "$pass" ]; then
   		echo -n "Enter password: "
   		read -s pass	
		echo 
	fi
	
	if [ -z "$host" ]; then
   		host="localhost"
   	fi

	if [ -z "$adminUsername" ]; then
		echo $usage_str
		exit 1
	fi	

	if [ -z "$adminPass" ]; then
		echo $usage_str
		exit 1
	fi
elif [ "$COUNT" -eq "2" ]; then
	if [ -z $file ]; then
		echo $usage_str
		exit 1
	fi
fi

#call the program

if [ -z "$file" ]; then
	#If the file is empty, you need to pass in arguments
	$PROGRAM_LOCATION$PROGRAM -d $database -u $user -p $pass -h $host -au $adminUsername -ap $adminPass
else
	$PROGRAM_LOCATION$PROGRAM -f $file
fi
