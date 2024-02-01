# CodeShell
Basic shell script in C Language  
In this project, you'll build a simple Unix shell. The shell is the heart of the command-line interface, and thus is central to the Unix/C programming environment. Mastering use of the shell is necessary to become proficient in this world; knowing how the shell itself is built is the focus of this project. There are three specific objectives to this assignment:  To further familiarize yourself with the Unix/Linux programming environment.  To learn how processes are created, destroyed, and managed.  To gain exposure to the necessary functionality in shells.

# Program Specifications
Basic Shell: CodeShell Your basic shell, is basically an interactive loop: it repeatedly prints a prompt dash> (note the space after the greater-than sign), parses the input, executes the command specified on that line of input, and waits for the command to finish. This is repeated until the user types exit. The name of your final executable should be dash. The shell can be invoked with either no arguments or a single argument; anything else is an error. Here is the no-argument way: At this point, dash is running, and ready to accept commands. Type away! The mode above is called interactive mode, and allows the user to type commands directly. The shell also supports a batch mode, which instead reads input from a batch file and executes commands from therein. Here is how you run the shell with a batch file named batch.txt: prompt> ./dash batch.txt One difference between batch and interactive modes: in interactive mode, a prompt is printed (dash>). In batch mode, no prompt should be printed. You should structure your shell such that it creates a process for each new command (the exception are built-in commands, discussed below). Your basic shell should be able to parse a command and run the program corresponding to the command. For example, if the user types ls -la /tmp, your shell should run the program /bin/ls with the given arguments -la and /tmp (how does the shell know to run /bin/ls? It's something called the shell path).

# Testing
Run in Linux Environment(WSL).  
Make sure you compile your program as follows: gcc Project1.c -o Project1 -Wall -Werror -O  
Run the shell using: ./Project1  

# Functionalities Implemented

• Implemented commands such as cd, exit, and path for navigation and environment configuration.  
• Integrated a path command to manage the shell's executable paths dynamically.  
• Utilized fork and execv for command execution and handled errors appropriately.  
• Incorporated support for batch input, allowing users to provide commands from a file.  
• Implemented redirection functionality for output with the > symbol.  
• Used fork and multi-processing concepts to execute commands concurrently when & is encountered.  
• Ensured proper error handling and messages for user guidance.    
• Enhanced user experience by allowing interactive command input and batch processing.  

PS: Try out different stuff with the shell functionalities :)
