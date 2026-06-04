#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main(int argc, char *argv[]) {
    // check argv
    if (argc != 3 || strcmp(argv[1], "-e") != 0) {
        std::cerr << "Usage: " << argv[0] << " -e \"command\"\n";
        return 1;
    }

    // create child prosses
    pid_t pid = fork();

    if (pid == -1) {
        // fork fail
        std::cerr << "Failed to fork.\n";
        return 1;
    } else if (pid == 0) {
        // we are in the child
        // execute the -e
        execlp("/bin/sh", "sh", "-c", argv[2], NULL);

        std::cerr << "Failed to execute command.\n";
        return 1;
    } else {
        // parent
        int status;
        waitpid(pid, &status, 0); // wait for the child

        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            std::cout << "Command exited with status: " << exit_status << std::endl;
        } else {
            std::cerr << "Command did not terminate normally.\n";
        }
    }

    return 0;
}