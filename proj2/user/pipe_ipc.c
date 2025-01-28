#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


typedef struct task_t {
    int x;
    int y;
    char op; 
    int result;
    int error;
} task_t;

int calc(int x, int y, char op, int *result) {
    switch (op) {
        case '+':
            *result = x + y;
            return 0; 
        case '-':
            *result = x - y;
            return 0; 
        case '*':
            *result = x * y;
            return 0; 
        case '/':
            if (y == 0) {
                return -1; 
            }
            *result = x / y;
            return 0; 
        default:
            return -1; 
    }
}

void server(int read_fd, int write_fd) {
    task_t task;
    while (read(read_fd, &task, sizeof(task_t)) > 0) {
        task.error = calc(task.x, task.y, task.op, &task.result);
        if (write(write_fd, &task, sizeof(task_t)) != sizeof(task_t)) {
            fprintf(1, "Server: Write error\n");
            exit(1);
        }
    }
    exit(0);
}

void client(int write_fd, int read_fd, task_t task) {
    if (write(write_fd, &task, sizeof(task_t)) != sizeof(task_t)) {
        fprintf(1, "Client: Write error\n");
        exit(1);
    }
    
    if (read(read_fd, &task, sizeof(task_t)) != sizeof(task_t)) {
        fprintf(1, "Client: Read error\n");
        exit(1);
    }
    
    if (task.error == 0) {
        fprintf(1, "Result: %d %c %d = %d\n", task.x, task.op, task.y, task.result);
    } else if (task.error == -1) {
        fprintf(1, "Error: Invalid operation or division by zero\n");
    }
    exit(0);
}

int pipe_ipc_main() {
    int p1[2], p2[2];
    
    if (pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(1, "Pipe creation failed\n");
        exit(1);
    }

    int num_clients = 3;
    for (int i = 0; i < num_clients; i++) {
        int pid = fork();
        if (pid < 0) {
            fprintf(1, "Fork failed\n");
            exit(1);
        }
        if (pid == 0) { // Child process
            close(p1[0]); // Close unused pipe ends
            close(p2[1]);
            
            task_t task;
            task.x = i + 1;
            task.y = i + 2;
            task.op = "+-*/"[i % 4]; // Use all operations
            
            client(p1[1], p2[0], task);
        }
    }

    // Parent process (server)
    close(p1[1]); 
    close(p2[0]); 
    
    server(p1[0], p2[1]);

    // Wait for all children
    for (int i = 0; i < num_clients; i++) {
        wait(0);
    }

    exit(0);
}