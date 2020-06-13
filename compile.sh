cd /home/eddy/Coding/C++/StudentSystem/bin
g++ -Wall -g ../sources/*.cpp -c 
cd ..
g++ bin/*.o -lmysqlcppconn -o System 
