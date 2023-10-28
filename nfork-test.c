#include "types.h"
#include "user.h"

int main(void) {
    nfork(5, "111;222;333;444;555;");
    exit();
}