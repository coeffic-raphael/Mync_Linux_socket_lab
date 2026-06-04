# Linux Mini Netcat Lab

A Linux C++ systems-programming lab that builds a small `netcat`-style utility step by step, using process control, descriptor redirection, TCP/UDP sockets, Unix Domain Sockets, timeouts, and coverage instrumentation.

## Overview

The main tool in this repository is `mync`, a small command-line program inspired by `nc`. It can execute a shell command, redirect the command's input/output through sockets, and relay data between standard streams and network endpoints.

The project is organized as a progression of exercises:

- `ex1`: a small command-line tic-tac-toe program used as a test command.
- `ex2`: command execution using `fork` and `exec`.
- `ex3`: TCP client/server input and output redirection.
- `ex3.5`: bidirectional TCP mode.
- `ex4`: TCP and UDP modes with timeout support.
- `ex6`: extended `mync` with TCP, UDP, Unix Domain Socket stream/datagram modes, command execution, descriptor redirection, timeout support, and coverage build flags.

## Features

- Execute shell commands through `/bin/sh -c`.
- Use `fork`, `exec`, `waitpid`, and `dup2`.
- Redirect command input and output to file descriptors.
- TCP server and TCP client modes.
- UDP server and UDP client modes.
- Unix Domain Socket stream and datagram modes.
- Bidirectional mode for TCP server connections.
- Timeout handling with `alarm`.
- Linux coverage instrumentation with `gcov`.

## Repository Structure

```text
.
├── Makefile
├── ex1/
│   ├── Makefile
│   └── ttt.cpp
├── ex2/
│   ├── Makefile
│   └── mync.cpp
├── ex3/
│   ├── Makefile
│   └── mync.cpp
├── ex3.5/
│   ├── Makefile
│   └── mync.cpp
├── ex4/
│   ├── Makefile
│   └── mync.cpp
└── ex6/
    ├── Makefile
    └── mync.cpp
```

## Requirements

- Linux
- `g++`
- `make`
- POSIX sockets
- Unix Domain Sockets
- `gcov` for the coverage target in `ex6`

This repository targets Linux. Some socket and process-control code is POSIX-style, but the supported environment is Linux because the final exercise uses Linux/GNU coverage tooling.

## Build

Build all exercises:

```bash
make
```

Build a specific exercise:

```bash
make ex4
make ex6
```

Clean generated files:

```bash
make clean
```

## Example Usage

Run a command locally:

```bash
./ex2/mync -e "ls -la"
```

Send command output to a TCP listener:

```bash
# Terminal 1
nc -l 9000

# Terminal 2
./ex4/mync -e "date" -o TCPC127.0.0.1,9000
```

Listen for TCP input and pass it to a command:

```bash
# Terminal 1
./ex4/mync -e "cat" -i TCPS9000

# Terminal 2
printf "hello\n" | nc 127.0.0.1 9000
```

Use the final Unix Domain Socket stream mode:

```bash
./ex6/mync -e "cat" -i UDSSS/tmp/mync.sock
./ex6/mync -e "printf hello" -o UDSCS/tmp/mync.sock
```

## Address Modes

The final `mync` implementation recognizes compact endpoint specifiers:

- `TCPS<port>`: TCP server
- `TCPC<host,port>`: TCP client
- `UDPS<port>`: UDP server
- `UDPC<host,port>`: UDP client
- `UDSSD<path>`: Unix Domain Socket datagram server
- `UDSCD<path>`: Unix Domain Socket datagram client
- `UDSSS<path>`: Unix Domain Socket stream server
- `UDSCS<path>`: Unix Domain Socket stream client

Flags:

- `-e <command>`: execute a shell command
- `-i <endpoint>`: use endpoint as input
- `-o <endpoint>`: use endpoint as output
- `-b <endpoint>`: bidirectional mode
- `-t <seconds>`: stop after a timeout

## Coverage

The final exercise includes a coverage target:

```bash
cd ex6
make coverage
```

This generates `gcov` output for `mync.cpp`.

## Implementation Notes

This is an educational systems-programming project. It is useful for demonstrating practical familiarity with Linux process control, descriptor manipulation, socket APIs, local IPC, and coverage tooling. It is not intended to be a production replacement for `netcat`.

## License

This project is released under the MIT License. See [LICENSE](LICENSE).
