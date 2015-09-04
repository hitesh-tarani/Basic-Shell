# BASIC-SHELL
Simple Shell implementation

##Introduction
The Shell or Command Line Interpreter is a fundamental User Interface to an Operating
System.

####Features

1. cd <directory> 
Changes the current default directory to <directory> . If the <directory> argument is not present, the current directory is reported. If the directory
doesn’t exist an appropriate error message is reported. This command also changes the PWD environment variable.

2. clr 
Clears the screen. 

3. dir <directory> 
Lists the contents of directory <directory>. 

4. environ 
Lists all the environment strings. 

5. echo <comment> 
Display <comment> on the display followed by a new line. ( Multiple spaces/tabs are reduced to a single space).

6. pause
Pause Operation of shell until ‘Enter ’ is pressed.

7. help
Displays User Manual. 

8. quit 
Quits the shell. 

9. The shell environment contains shell=<pathname>/myshell where
<pathname>/myshell is full path for the shell executable (not a hardwired path back to your
directory, but the one from which it was executed). 

10. The shell is be able to take its command line input from a file. That is, if the command line is
invoked with a command line argument:
myshell batchfile
then batchfile is assumed to contain a set of command lines for the shell to process. When the endoffile is reached, the shell exits.

####How To Use

1. Just compile the main.c with gcc.

2. Run the program and the shell is ready to be used.
