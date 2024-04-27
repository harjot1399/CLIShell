# CLIShell

## Description
This project is a custom shell implemented in C++ designed to handle basic process management tasks. It allows users to run commands directly related to process creation, manipulation, and monitoring. The shell is capable of handling multiple processes, checking their statuses, and managing their execution in the foreground or background.

## Features
- **Process Creation**: Launch new processes either in the foreground or background.
- **Process Monitoring**: List active processes with details about their status and execution time.
- **Process Manipulation**: Includes functionality to kill, suspend, resume, and wait for processes.
- **Output Redirection**: Supports redirection of a command's output to a file.
- **Input Redirection**: Allows specifying input for commands via files.
- **Signal Handling**: Manages background processes and cleans up zombie processes using signal handling.

## Acknowledgments:

1. https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/#:~:text=We%20can%20use%20a%20combination,functions%20to%20trim%20the%20string (string trimtrail function)
2. https://stackoverflow.com/questions/60686865/how-to-get-a-word-vector-from-a-string (store function)
3. https://stackoverflow.com/questions/42493101/how-to-convert-a-vectorstring-to-a-vectorchar (to_char_vector function)]
4. https://thispointer.com/c-how-to-read-a-file-line-by-line-into-a-vector/ (get_input_from_file function)
