# StudSysCppCLI 
A personal project implementing a Command Line User Interface
Student Administration Program. 

This was all the same repository with the Library before I diverged them so a lot of issues in this repository relate to the library

It is programmed with C++ entirely and can be compiled on Linux with g++

## It allows the following:
   A student to login and browse each module they are registered for
       Each module site has learning resources and also exams that can be taken, grades info and student/lecturer details
              As of version 1.0, the only exam format allowed is a multiple choice exam with each question having equal weights. Also only supports single answers
              his may be updated with further versions
   A lecturer to login and browse the module(s) they are registered as course leader for
       With this, they can create messages, resources and exams
              They can also edit exams that are already created
       Viewing results for each student who took the exam
       An administrator to login and do the following:
       Create module/course/lecturer/student
       Delete module/course/lecturer/student
       Version 1.0 does not currently allow editing these (or only in a very limited manner), they have to be removed and then created with updated info
       The System has methods to update each entity however so it would be very easy to have it in later versions

## How to login:
   1. On the WelcomeScreen follow the instructions to login as Lecturer or Student
   2. Press E and then enter your e-mail address and press Enter
   3. Press P and then enter your password. Note that you will not be able to see the password that you type for security. Press Enter to insert password into the system
   4. Press S to submit the details and you should be logged in

## Problems in Version 1.0 that I plan to work on:
   1. When you're in the middle of input for example entering a name, there is no way of cancelling.
      I plan to implement a way so that you can type C or Cancel instead of a name. To make this work I may have to read in everything as a string and not int or float as any string entered into cin returns 0. 
      I can't check if it's 0 a user wants to cancel because they may genuinely want to type a 0
   2. There are no features currently implemented for Tasks. This can be circumvented by using Announcements
   3. After saving an exam that was created successfully, the user is left on the prompt they already used to save the exam in the first place, which could mean they could go editing/saving an exam that was already saved. This is a known bug and is going to be fixed ASAP. Also, after cancelling this prompt, to update the list of exams you need to go back to the Module Homepage and go back into exams. This bug-fix should be in the next release
   4. You cannot delete an exam that was created. This will be fixed in future versions
   5. This list isn't exhaustive with more testing I may add more

## Information Storage/Database:
   To store artifacts related to the system such as students, lecturers, modules, exams etc. a MySQL server is used to store the info


## Dependencies:
   This program uses Boost's libraries so you will need to have boost libraries installed
   The libstudsys library that can be obtained from https://github.com/edwardUL99/StudSysLibrary
   As MySQL server is used, this program requires the MySQL C++ connector version 8.0, it can be downloaded with this link:
        https://dev.mysql.com/downloads/connector/cpp/
        
        1. In the operating system drop-down pick Linux - Generic
        2. Choose in OS Version drop-down choose glib 2.12 64 bit
           My OS is 64 bit which I developed it in, but you could try it with 32 bit but it's not guaranteed to work
	3. Ensure you install the correct MySQL C++ connector. Can't install above on Raspberry Pi for example but there are plenty of installation guides on google

   A MySQL server, configured as of Database Setup in the readme of StudSysCppLibrary
   Also works with mariadb-server-10.0 (this is what you'll need on Raspberry Pi)

## How to compile (linux):
   Obtain the latest studsys library release from github.com/edwardUL99/StudSysCppLibrary or build the library using instructions from its README
   Copy the library to a folder in the root of the CLI directory (recommended a folder called lib)
   Obtain the headers from the studsys library and put them in headers/studsys
   Compile with the make file provided by typing make in the root directory
   Run the command: export LD_LIBRARY_PATH=<path_to_root_of_lib>:$LD_LIBRARY_PATH
   
If you don't have access to the database to create databases or users, or don't know the host, contact the database administrator to create a database if not already exists.
The admin will then be able to give you the IP address to the database server to login to

## How to run:
   From root of the download directory type ./<program-name> -d <database_name> -u <user_name> -p <pass_word> -h <host>
      or
   ./<program-name> login_file

   Example login_file is:
   	user = [user-name]
	pass = [pass]
	host = [hostname/ip]
	database = [database]

## studsysdbadmin:
   Very limited program to administrate the database, compile using:
          g++ studsysdbadmin.cpp -l mysqlcppconn -o studsysdbadmin 
   Only command line argument at the moment is -c which clears the entire database

## Administration:
   Use hidden command A from the WelcomeScreen
   Username is adminITT20
   Password is ittAdminPass20
   
## Logging:
Errors can be logged if the STUD_LOGS environment variable is set to the directory of where to store the logs. If this is not set, no logging of errors will take place
On Linux the variable must be exported using export STUD_LOGS=<log-path>
