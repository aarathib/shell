# **AOS Assignment2**
Implement a shell that supports POSIX shell commands.

## **Working**
1. Display
- Display like `_username_@_system_name_:_current_directory_>` is coming for the shell implemented asking for user input.

2. cd, echo, pwd are implemented.

3. ls with any of the specified flags is working.

4. System commands (background/fg), with and without arguments
- When command is typed with & operator, it creates a background process and print its PID. Eg: gedit & (with space between).
- When there is no & operator, it creates a foreground process for the given command.

5. pinfo
- Information about a process is obtained from the system files */proc/pid/stat* and */proc/pid/exe*.

6. search
- It searches for a particular file or directory in the current folder recursively and gives True/False as output based on file is present or not.

7. I/O redirection
- When there is an I/O redirection operator <, >, >>, respective redirections are done from stdin/stdout to files.
- To truncate output file, > is used and to append to file >> is used (take care of space between).

8. pipeline
- It redirects the output of the command on the left as input to the command on the right. 
- One or more commands can be piped with this implementation.

9. Redirection with pipeline
- The commands are split for pipe to work and inside that if redirections happen, they are made workiing with previous redirection implementation.

12. history

## **Files**
1. bgfg.cpp - For handling foreground/background processes
2. cd.cpp - Handles cd command
3. display.cpp - For displaying the username and hostname
4. echo.cpp - To handle echo command
5. history.cpp - To handle history commands
6. ls.cpp - To handle ls command
7. pcwd - To handle pwd command
8. pinfo.cpp - To handle pinfo command
9. pipe.cpp - To implement pipe
10. redirect.cpp - To implememt I/O redirections 
11. search.cpp - To handle search command
12. parseinput.cpp - To handle user input, tokenise
13. history.txt - Used to store history of commands across sessions
Corresponding header files are also created for each .cpp file