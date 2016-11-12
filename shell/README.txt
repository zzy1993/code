
Project 1 of Operating System (EECE7352)

(c) Composed by Zhiyuan Zhao & Yichen Cheng, 4 Mar 2016

1. Files provided:

	[1]	shell.c (c code implementation of shell)	
	[2]	test.c (c code printing environment variables)
	[3]	README.txt (instruction)
 
	# 4 optional functions included.

2. Functions implemented:

2-1. Minimum Required:

	(1) Print a prompt ($ ) when it is ready for the user to input a command.

	Done. 

	(2) When a command is launched in the foreground, the shell should wait until the last of its sub-commands finishes before it prints the prompt again.

	Done.

	(3) When a command is launched in the background using operator , the shell should print the pid of the process corresponding to the last sub-command. The pid should be printed using square bracket. It should then immediately display the prompt and accept new commands, even if any of the child processes are still running.

	Done.

	(4) When a sub-command is not found, a proper error message should be displayed, immediately followed by the next prompt.

	Done.

	(5) The input redirection operator should redirect the standard input of the first sub-command of a command. If the input file is not found, a proper message should be displayed.

	Done.

	(6) The output redirection operator should redirect the standard output of the last sub-command of a command. If the output file cannot be created, a proper message should be displayed.

	Done. 

	(7) Sub-commands separated by pipes should provide the desired redirections. Assuming N sub- commands, you can create N – 1 pipes in the parent process, and let them be inherited by the child processes every time fork() is executed.

	Done. The N-1 pipes connect N processes from previous output to next input.

2-2. Optionals (4 in total, 2 listed, 2 extended):

	(1) Modify the command prompt so that it displays the current directory, and implement the functionality of built-in command 'cd' to change it.

	Done. Test case: 'cd ..', 'cd <sub-folder>'

	(2) Implement support for process-specific environment variables. Write a program that prints all environment variables available to the process, and launch it from your shell. Verify that the explicit environment variables are indeed being set in the child process.

	Done. Test case: 'VAR=Hello ./test'
	\* 'test.c' is required to be compiled as 'test'.

	(3) <My extension> Implement Append Standard Output '>>' in shell.

	Done. Test case: 'ls | wc >> num.txt'

	(4) <My extension> Implement User 

	Done. Prompt is displayed with user name: '<user>@Shell'

- The end -