#include <stdint.h>
#include <stdio.h>
#include <time.h>		/* For every time random number	*/
#include <mpi.h>		/* For MPI functions	*/

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#define CHANNEL_NUM 1


int smoothFilter(uint8_t* imgArray, int width, int x, int y){
   
   int result=0;


   for(int i=0; i<3; i++){
	
	for(int j=0; j<3; j++){
	
	   result += imgArray[(i+x)*width + (j+y)];
	   
	}

   }
	
   result = result/9;

   return result;

}


int main(int argc,char* argv[]) {

    double time1, time2,duration,global;

    int width, height, bpp,smthNum=0;
    int	comm_sz; 						/* Number of processes */
    int	my_rank; 						/* My process rank	*/  
    
    int ProcessNum;  

    // Reading the image
    uint8_t* rgb_image = stbi_load(argv[1], &width, &height, &bpp, CHANNEL_NUM);

    
  	//Tells MPI to do all the necessary setup.
	MPI_Init(NULL, NULL);
	
	//number  of processes in the communicator
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
	
	//the process making this call
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	time1 = MPI_Wtime();
		 

	ProcessNum = height/comm_sz;	 
	  
	for(int i=(ProcessNum*(my_rank+2));i<(ProcessNum*(my_rank+1));i++){
		for(int j=0;j<width;j++){

		   rgb_image[i*width + j]= smoothFilter(rgb_image,width,i,j);

		}
		
    	}

    
	MPI_Barrier(MPI_COMM_WORLD);
      
	time2 = MPI_Wtime();
	duration = time2 - time1;
   
	MPI_Reduce(&duration,&global,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);  
    
   
	printf("Pixel: %d x %d - Rank: %d - Time: %f \n",(ProcessNum*(my_rank)),(ProcessNum*(my_rank+1)),my_rank,time2);
   
 
	if(my_rank == 0) {


           // Stoing the image 
           stbi_write_jpg(argv[2], width, height, CHANNEL_NUM, rgb_image, 100);
           stbi_image_free(rgb_image);
       
           printf("Total Time: %f\n",global);
	}  
   
   
	MPI_Finalize();
   
   
    return 0;
}

