#include "stdio.h"
#include "locale.h"

int main(int argc, char* argv[]) {

    setlocale(LC_ALL, "Turkish");

    printf("şğı");

    return 0;
}
