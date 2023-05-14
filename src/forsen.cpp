#include <iostream>

int main()
{
    printf("\n!===== forsenLevel !\n");
    
    clock_t start, end;
    start = clock();

    //TODO: Everything

    end = clock();

    double time_taken = double(end-start)/double(CLOCKS_PER_SEC);
    printf("Time taken by program is : %.2f sec\n", time_taken);

    return 0;
}