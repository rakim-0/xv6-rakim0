#include "types.h"
#include "user.h"

int main(void) {
    int pid = fork();
    if (pid == 0) {
        // printf(1, "Hello from child!\n");
        sleep(1);
        exit();
    }
    pid = fork();
    if (pid == 0) {
        // printf(1, "Hello from child!\n");
        sleep(1);
        exit();
    }
    
    int children = childcount();
    printf(1, "Hello from parent!\n");
    printf(1, "Number of Children Processes: %d\n", children);
    wait();
    wait();
    exit();
}