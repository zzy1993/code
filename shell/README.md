
# Midterm Project of Operating System (EECE7352)

(c) Composed by Zhiyuan Zhao & Yichen Cheng, 4 Mar 2016

## Files provided:

	[1]	shell.c (c code implementation of shell)	
	[2]	test.c (c code printing environment variables)
	[3]	README.txt (instruction)
 
	# 4 optional functions included.

## Functions implemented:

### Minimum Required:

	* Print a prompt ($ ) when it is ready for the user to input a command.

	Done. 

	* When a command is launched in the foreground, the shell should wait until the last of its sub-commands finishes before it prints the prompt again.

	Done.

	* When a command is launched in the background using operator , the shell should print the pid of the process corresponding to the last sub-command. The pid should be printed using square bracket. It should then immediately display the prompt and accept new commands, even if any of the child processes are still running.

	Done.

	* When a sub-command is not found, a proper error message should be displayed, immediately followed by the next prompt.

	Done.

	* The input redirection operator should redirect the standard input of the first sub-command of a command. If the input file is not found, a proper message should be displayed.

	Done.

	* The output redirection operator should redirect the standard output of the last sub-command of a command. If the output file cannot be created, a proper message should be displayed.

	Done. 

	* Sub-commands separated by pipes should provide the desired redirections. Assuming N sub- commands, you can create N – 1 pipes in the parent process, and let them be inherited by the child processes every time fork() is executed.

	Done. The N-1 pipes connect N processes from previous output to next input.

### Optionals (4 in total, 2 listed, 2 extended):

	* Modify the command prompt so that it displays the current directory, and implement the functionality of built-in command 'cd' to change it.

	Done. Test case: 'cd ..', 'cd <sub-folder>'

	* Implement support for process-specific environment variables. Write a program that prints all environment variables available to the process, and launch it from your shell. Verify that the explicit environment variables are indeed being set in the child process.

	Done. Test case: 'VAR=Hello ./test'
	\* 'test.c' is required to be compiled as 'test'.

	* <My extension> Implement Append Standard Output '>>' in shell.

	Done. Test case: 'ls | wc >> num.txt'

	# <My extension> Implement User 

	Done. Prompt is displayed with user name: '<user>@Shell'
