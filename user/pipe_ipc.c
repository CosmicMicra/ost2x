typedef struct task_t {
 int x;
 int y;
 char op; // Supports "+", "-", "*", "/"
 int *result;
 int *error;
} task_t;

int calc(int x, int y, char* op, int *result) {
 calc task similar to the one in homework 1, but in the user space
}

void server(int read_fd, int write_fd) {
 while (read from read_fd) {
     execute calc();
     write the result to write_fd;
  }
  ...
  exit(0);
}

void client(int write_fd, int read_fd, task_t task) {
 write task to write_fd;
 read from read_fd and get result;
 ...
 exit(0);
}
int main() {
 create two pipes for input and output respectively;
 for (number of clients) {
       if (fork() == 0) { // child process to be client
           create a task;
           client(p1, p2, task);
           exit(0);
       }
    }

   // Server
   ...
   server(p1[0], p2[1]);
   wait(0);
   exit(0);
}
