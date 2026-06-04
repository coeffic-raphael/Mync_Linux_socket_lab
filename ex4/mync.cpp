#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int create_tcp_server(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket\n";
        return -1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error on binding\n";
        close(sockfd);
        return -1;
    }

    listen(sockfd, 5);
    return sockfd;
}

int create_tcp_client(const char* hostname, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket\n";
        return -1;
    }

    struct hostent* server = gethostbyname(hostname);
    if (server == NULL) {
        std::cerr << "Error, no such host\n";
        close(sockfd);
        return -1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error connecting\n";
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int create_udp_server(int port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket\n";
        return -1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error on binding\n";
        close(sockfd);
        return -1;
    }

    return sockfd;
}

int create_udp_client(const char* hostname, int port, struct sockaddr_in& serv_addr) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket\n";
        return -1;
    }

    struct hostent* server = gethostbyname(hostname);
    if (server == NULL) {
        std::cerr << "Error, no such host\n";
        close(sockfd);
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Error connecting\n";
        close(sockfd);
        return -1;
    }

    return sockfd;
}

void handle_alarm(int sig) {
    std::cout << "Timeout expired. Server is shutting down." << std::endl;
    exit(0);
}


int main(int argc, char *argv[]){
    char *command = NULL;
    int input_fd = STDIN_FILENO;
    int output_fd = STDOUT_FILENO;
    bool should_close = false;
    int timeout = 0;
    struct sockaddr_in serv_addr;
    int client_fd = -1, server_fd = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            command = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeout = atoi(argv[++i]);
            if (timeout > 0) {
                signal(SIGALRM, handle_alarm);
                alarm(timeout);
            }
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            i++;
            if (strncmp(argv[i], "TCPS", 4) == 0) {
                int port = atoi(argv[i] + 4);
                server_fd = create_tcp_server(port);
                if (server_fd < 0) return 1;
                client_fd = accept(server_fd, NULL, NULL);
                if (client_fd < 0) {
                    std::cerr << "Error accepting connection\n";
                    return 1;
                }
                input_fd = client_fd;
                should_close = true;
            } else if (strncmp(argv[i], "UDPS", 4) == 0) {
                int port = atoi(argv[i] + 4);
                server_fd = create_udp_server(port);
                if (server_fd < 0) return 1;
                input_fd = server_fd;
                should_close = true;
            }
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            i++;
            if (strncmp(argv[i], "TCPC", 4) == 0) {
                char *colon = strchr(argv[i] + 4, ',');
                if (colon) {
                    *colon = '\0';
                    int port = atoi(colon + 1);
                    client_fd = create_tcp_client(argv[i] + 4, port);
                    if (client_fd < 0) return 1;
                    output_fd = client_fd;
                    should_close = true;
                }
            } else if (strncmp(argv[i], "UDPC", 4) == 0) {
                char *colon = strchr(argv[i] + 4, ',');
                if (colon) {
                    *colon = '\0';
                    int port = atoi(colon + 1);
                    client_fd = create_udp_client(argv[i] + 4, port, serv_addr);
                    if (client_fd < 0) return 1;
                    output_fd = client_fd;
                    should_close = true;
                }
            }
        } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            i++;
            if (strncmp(argv[i], "TCPS", 4) == 0) {
                int port = atoi(argv[i] + 4);
                server_fd = create_tcp_server(port);
                if (server_fd < 0) return 1;
                client_fd = accept(server_fd, NULL, NULL);
                if (client_fd < 0) {
                    std::cerr << "Error accepting connection\n";
                    return 1;
                }
                input_fd = client_fd;
                should_close = true;
            }
         }
    }

    if (command) {
        pid_t pid = fork();
        if (pid == -1) {
            std::cerr << "Failed to fork\n";
            return 1;
        } else if (pid == 0) {
            // child
            if (input_fd != STDIN_FILENO) dup2(input_fd, STDIN_FILENO);
            if (output_fd != STDOUT_FILENO) dup2(output_fd, STDOUT_FILENO);
            execlp("/bin/sh", "sh", "-c", command, NULL);
            std::cerr << "Failed to execute command.\n";
            exit(1); //check that child finish if exelp failed
        } else {
            // parent
            int status;
            waitpid(pid, &status, 0); // wait for the child
            if (should_close) {
                if (input_fd != STDIN_FILENO) close(input_fd);
                if (output_fd != STDOUT_FILENO) close(output_fd);
            }
            if (WIFEXITED(status)) {
                std::cout << "Command exited with status: " << WEXITSTATUS(status) << std::endl;
            } else {
                std::cerr << "Command did not terminate normally.\n";
            }
        }
    } else {
        // we don't receive command
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(input_fd, buffer, BUFFER_SIZE)) > 0) {
            write(output_fd, buffer, bytes_read);
        }
        if (should_close) {
            if (input_fd != STDIN_FILENO) close(input_fd);
            if (output_fd != STDOUT_FILENO) close(output_fd);
        }
    }

    return 0;
}
