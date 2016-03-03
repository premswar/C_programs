
1. Goto part3. Give the make command. It will generate the libsig_lib.a and application file.
2. run the application in background using '&' and send the SIGIO signal to the process using kill -SIGIO pid.
3. We will observe the signal handler has been invoked from all the 3 threads. Program will terminate after it received 3 SIGIO from user.
4.The max threads for library has be defined as macro MAX_THREADS. change it and make again if more threads are needed.