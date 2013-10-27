bash_shell
==========

multithreaded shell implementation in c

////-------------------> To run <-----------------------\\\\\
>> make
>> ./fsh

////-----------------> Functionality <-------------------\\\\\
Robust functionality for basic commands. More likely than not, the command you want works :D

>>exit [n] terminates the shell, either by calling the exit() standard 
library routine or causing a return from the shell's main(). 
If an argument (n) is given, the exit value of 
the shell's execution. Otherwise, the exit value is the value 
returned by the last executed command (or 0 if no commands were executed.)


>>cd [dir] uses the chdir() standard library routine to change the shell's 
working directory to the argument directory. If no argument is given, 
the value of the HOME environment variable is used.


>>. filename causes commands to be read from the file. When end-of-file is 
reached, the shell returns to reading commands from the keyboard.
If the command line doesn't invoke an internal command, the shell assumes 
it is of the form <executable name> <arg0> <arg1> .... <argN>
Uses the fork() standard library call, and execve(), 
to invoke the executable, passing it any command line arguments.
the "." works recursively for files. If there is a nested "." command in a file,
it will fork and execute those commands as well
