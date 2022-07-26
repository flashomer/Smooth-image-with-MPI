#include <stdint.h>
#include <stdio.h>

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
    int width, height, bpp,smthNum=0;

	// Reading the image
    uint8_t* rgb_image = stbi_load(argv[1], &width, &height, &bpp, CHANNEL_NUM);

    printf("Width: %d  Height: %d \n",width,height);
    
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){

		   rgb_image[i*width + j]= smoothFilter(rgb_image,width,i,j);

		}
		
    }

    // Stoing the image 
    stbi_write_jpg(argv[2], width, height, CHANNEL_NUM, rgb_image, 100);
    stbi_image_free(rgb_image);

    return 0;
}

