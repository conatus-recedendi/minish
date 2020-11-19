# minish

The shell implementation project from System Programming in GIST

It remakes "zsh", but some(many) part are different because of the requirements from the given description.

! Because I rarely care about error handling(null from malloc, conflict with file system, permissions, processing, etc), this program may terminatted abnormally sometimes.

! This develope environment is not LINUX, CYGWIN and WSL. It developed in MacOS, So some result may be different I intended.

## Privacy

- Name	: 이경규
- ID	: 20195122
- Data	: 2020.11.19(Thu)

## How to Compile and How to execute?

### makefile

To compile "minish" program and command program, Enter the following command on your shell.
```
make
```

### How to Execute

To execute the "minish" program, Enter the following command on your shell.

```
./minish
```

### Develope Environement

```
% gcc --version
Configured with: --prefix=/Library/Developer/CommandLineTools/usr --with-gxx-include-dir=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/4.2.1
Apple clang version 11.0.3 (clang-1103.0.32.59)
Target: x86_64-apple-darwin19.6.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin
```

### Compile Flag
```
-Wall -Werror -Wextra
```

## What functionality was implemented?

### 1. Functionality to be implemented

#### 1-1. Basic Execution of commands

The following command are implemented as the independent files. They located in './srcs/bin/'

- cat
	* Stdin Mode (for 0 argument)
	* Deal with more than 2 arguments.

- wc
	* Stdin Mode (for 0 argument)
	* Deal with more than 2 arguments.

- ls
	* -l flag
	* Deal with more than 2 arguments.
	* Distinguish the regular file and the directory.
	* '.', '..' may be different from originals.

- sleep
	* Only for one argument.

#### 1-2. Signals

"minish" won't be killed with the signal 'SIGINT'. But every command line can be refreshed by it.

"quit" built-in command is implemented. It doesn't need any argument.

#### 1-3. Prompt

Every prompt line show the current directory path. It can be changed by "cd" command to be explained next.

Also other commands were affected by this path.

Command can be represented in the serveral lines like the original shell.

#### 1-4. Redirection

Some of redirection('>', '<', '>>') are implemented. 

But in this 'minish', Every command can have only one redirection. If you use the redirection in the same commands, then the first ones will be activated.

Also, I didn't implemented the speical redirections('#>&#').

#### 1-5. Pipe

Pipe are implemented. 

### 2. Advanced Functionality to be implemented

#### 2-1. Implementation of other built-in commands

- cd
	* change the current directory
	* It will affect the prompt, the other commands, but not parent process.

- path
	* path shows 'PATH' environment variable.
	* ```path <variables>``` can be replaced value of PATH to the given variables.

- status
	* status shows the result of prior command. If the first one ends successfully, then it shows 0, or not 1.
	* Therefore, It cannot show that what error occurs.

#### 2-2. Wildcard processing

- \*, ?
	* \* and ? wildcard are implemented.

## How was it implemented?

The main process of 'minish' is processing "prompt", "pipe" and "redirection".

To deal with them, str_split() in split.c products the linked-list data structure that has information of commands(pipe, redirection, command, arguments)

exec() function read the linked list data forwardly, fork the child process that check pipe and redirections and connects the file descriptions, and finally execute built-in or outter functions.

If the error occurs, without the parent process, all of child process exit immediately and parent process go upstream until it reached the main().
All of the error messages are dealing in STDERR_FILENO.

## Conclusion

그동안 자주 사용하였던 쉘을 구현하였다.
실제 쉘의 아주 일부분의 기능과 명령들을 구현하였다.
쉘의 기능 중에는 pipe, redirections 등을 구현하였다. 다만, 문제에서 주어진 background process나 &&, ||(파싱까지는 완료했다.)와 같은 추가적인 기능도 구현해보고 싶었으나 코드를 무작정 짜다보니 많이 어려움을 겪었다.

쉘의 기능을 구현하는데 시간을 많이 쓰느라 명령어들도 다양하고 깊게 구현하지 못했다. 문제의 에시로 주어진 cat, ls, sleep, wc 정도만 구현했고 이마저도 플래그나 기능을 완전히 구현하지는 못했다.

그래도 이번 프로젝트를 진행하면서 기존에 감으로만 잡고 있던 개념들을 구체적으로 잡을 수 있었고,
무엇보다도 내 손으로 구현해보았다는 것이 감명 깊었다.

나중에 코드 리펙토링이랑 기능 추가 같은 걸 해보아야겠다.

## References

### Code Reference
- https://stackoverflow.com/questions/20450556/linux-ls-l-implementation-using-file-descriptors-in-c-c, by Arunbalaji

### Informations
- https://www.man7.org/linux/man-pages/man2/



