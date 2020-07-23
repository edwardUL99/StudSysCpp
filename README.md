# StudSysCpp Version 1.0
A personal project implementing a Command Line User Interface
Student Administration Program. 

It is programmed with C++ entirely and can be run on Linux with g++

## It allows the following:
   A student to login and browse each module they are registered for
       Each module site has learning resources and also exams that can be taken
              As of version 1.0, the only exam format allowed is a multiple choice exam with each question having equal weights. Also only supports single answers
              his may be updated with further versions
   A lecturer to login and browse the module(s) they are registered as course leader for
       With this, they can create messages, resources and exams
              They can also edit exams that are already created
       Viewing results for each student who took the exam
       An administrator to login and do the following:
       Create module/course/lecturer/student
       Delete module/course/lecturer/student
       Version 1.0 does not currently allow editing these, they have to be removed and then created with updated info
       The System has methods to update each entity however so it would be very easy to have it in Version 2.0

## How to login:
   1. On the WelcomeScreen follow the instructions to login as Lecturer or Student
   2. Press E and then enter your e-mail address and press Enter
   3. Press P and then enter your password. Note that you will not be able to see the password that you type for security. Press Enter to insert password into the system
   4. Press S to submit the details and you should be logged in

## Problems in Version 1.0 that I plan to work on:
   1. When you're in the middle of input for example entering a name, there is no way of cancelling.
      I plan to implement a way so that you can type C or Cancel instead of a name. To make this work I may have to read in everything as a string and not int or float as any string entered into cin returns 0. 
      I can't check if it's 0 a user wants to cancel because they may genuinely want to type a 0
   2. This list isn't exhaustive with more testing I may add more

## Information Storage/Database:
   To store artifacts related to the system such as students, lecturers, modules, exams etc. a MySQL server is used to store the info


## Dependencies:
   This program uses Boost's libraries so you will need to have boost libraries installed
   As MySQL server is used, this program requires the MySQL C++ connector version 8.0, it can be downloaded with this link:
        https://dev.mysql.com/downloads/connector/cpp/
        
        1. In the operating system drop-down pick Linux - Generic
        2. Choose in OS Version drop-down choose glib 2.12 64 bit
           My OS is 64 bit which I developed it in, but you could try it with 32 bit but it's not guaranteed to work
	3. Ensure you install the correct MySQL C++ connector. Can't install above on Raspberry Pi for example but there are plenty of installation guides on google

   A MySQL server, configured as of below in Database Setup
   Also works with mariadb-server-10.0 (this is what you'll need on Raspberry Pi)

## How to compile:
   There are 3 options to compile the program:
        1. Use compile.sh in the directory, but replace any paths with your path to where it's installed
        2. Use make with the Makefile provided:
               do make clean to clean up
        3. Use g++ compiler:
               a. in the sources directory type:
                       g++ *.cpp -c 
                  to generate the object files. if you want to compile these to a specific folder you could create a folder called bin for example and compile as following while inside bin:
                       g++ ../sources/*.cpp -c
               b. Change to the root of the directory and type:
                       g++ sources/*.o -l mysqlcppconn -o <program-name>
                  or if you have object files in a separate folder, type:
                       g++ <object_folder>/*.o -l mysqlcppconn -o <program-name>
               c. I recommend using studsys as program name

## Database Setup:
   You need a MySQL server either MySQL or MariaDB
   Steps:
       1. Login to MySQL using mysql -u [user-name] -p (this user must be able to create databases and users)
       2. Type CREATE DATABASE <database-name>;
       3. Exit the MySQL shell
       4. Now type mysql -u [user-name] --password=[user-password] [database-name] < [file-name.sql]. You will need to do this with schema.sql, triggers.sql and procedures.sql
       5. Log back into the mysql shell and create a user for the system to use and password by typing CREATE USER '<user-name>'@'localhost' IDENTIFIED BY 'user-password';
       6. Then type GRANT ALL PRIVILEGES ON [database-name].* TO '[user-name]'@'%';
       7. Then type FLUSH PRIVILEGES; and then exit
       8. Ensure bind-address in mysql config is set to 0.0.0.0 on the database server machine
   The MySQL server is now set-up and ready to use with the system

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

