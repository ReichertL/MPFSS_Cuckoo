#include <stdlib.h>     /* srand, rand */
#include <time.h>    
#include <stdio.h>
#include <omp.h>

int get_number(int i){
	i=i-1;
	printf(" %d\n", rand() );
	if(i<0){
		return i;
	}
	get_number(i);

}


int main(int argc, char const *argv[])
{
	int now=time(NULL)/21000;
 	srand (now);
 	//get_number(3);

    #pragma omp parallel for
 	for (int i = 0; i < 5; ++i)
 	{
      /*  int id = omp_get_thread_num();
        int total = omp_get_num_threads();
        printf("Greetings from process %d out of %d\n", id, total);
		*/
		printf("test\n");
 	}


	return 0;
}

