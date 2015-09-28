/**Main.cpp

Input arguments program.cpp inputImage.raw outputImage.raw bytesperpixel width height
Author : Taruna Agrawal
Date: 1st september
email:tagrawal@usc.edu
ID: 7650184685
*/

#include "image_proc.cpp"

//Global variables
Image inImage, outImage;


/* Function: Implements histogram Equalization using Cumulative probabilty technique*/
bool Image::hist_eqCPH()
{
   unsigned int pixel_R[height*width][2],pixel_G[height*width][2], pixel_B[height*width][2];
   double modihist_R[256] = {0} ,modihist_G[256] = {0},modihist_B[256] = {0};
   ofstream modi_histR,modi_histG,modi_histB;

   unsigned char* temp; 
   temp = new unsigned char[width*height*3];  //to store final equalized image

   /*Store in array all pixles having intensity values 0,1 ...255*/
   unsigned int countR = 0;
   unsigned int countG = 0;
   unsigned int countB = 0;
   for (int h = 0; h < 256; h++)
   {
      for (int i = 0; i <height; i++)
      {
         for (int j = 0; j <width; j++)
         {
            int pixel = (i*(width*3)) + (j*3);
            if(data[pixel] == h)
            {
               pixel_R[countR][0] = i;  //row index
               pixel_R[countR][1] = j;  //column index
               countR++;
            }
            if(data[pixel +1] == h)
            {
               pixel_G[countG][0] = i;  //row index
               pixel_G[countG][1] = j;  //column index
               countG++;
            }
            if(data[pixel + 2] == h)
            {
               pixel_B[countB][0] = i;  //row index
               pixel_B[countB][1] = j;  //column index
               countB++;
            }
         }
      }
   }
   /*Divide each array into equal size of 256 values*/
   unsigned int blockSize = round((height*width)/256);
   int intensityValue = 0;
   int blocksizeCount = 0;
   for (int i = 0; i < height*width; i++)
   {
       int pixelR = (pixel_R[i][0]*(width*3)) + (pixel_R[i][1]*3);
       int pixelG = (pixel_G[i][0]*(width*3)) + (pixel_G[i][1]*3);
       int pixelB = (pixel_B[i][0]*(width*3)) + (pixel_B[i][1]*3);
       
       data[pixelR] = intensityValue;
       data[pixelG+1] = intensityValue;
       data[pixelB+2] = intensityValue;
       blocksizeCount++;

       if (blocksizeCount > blockSize)
       {
          intensityValue++;
          blocksizeCount = 0;
       }
       if (intensityValue > 255)
          intensityValue = 255;
   }
   /*Calulates histogram values*/
   for (int i = 0; i <height*width; i++)
   {
      modihist_R[data[3*i]]++;
      modihist_G[data[3*i+1]]++;
      modihist_B[data[3*i+2]]++;
   }
   
   modi_histR.open("CPH_histR.txt");
   modi_histG.open("CPH_histG.txt");
   modi_histB.open("CPH_histB.txt");

    for (int i = 0; i <256; i++)
   {
      modi_histR << modihist_R[i] << '\n';
      modi_histG << modihist_G[i] << '\n';
      modi_histB << modihist_B[i] << '\n';
   }

   modi_histR.close();
   modi_histG.close();
   modi_histB.close();

   return true;
}



int main(int argc, char* argv[])
{

   int BytesPerPixel = 1;
   int Width = 424;
   int Height = 636;
   int outWidth, outHeight;

   if (argc < 5)
   {
      cout << "Error - Incorrect Parameter Usage:" << endl;
      cout << "program_name input_image.raw output_image.raw BytesPerPixel width height" << endl;
      return 0;
   }	
   BytesPerPixel = atoi(argv[3]);
   inImage.setNumbytes(BytesPerPixel);

      // Check if size is specified
   if (argc >= 5){
	 Width = atoi(argv[4]);
	 Height = atoi(argv[5]);
         inImage.setWidth(Width);
         inImage.setHeight(Height);
    }

   //Read Image
   if (!inImage.readImage(argv[1]))
   {
      cout <<"Could not Read Image"<< endl;
   }
   else
   {
     cout << "Cumulative probability-Histogram equalization" <<endl;
     outImage = inImage;
     //outImage.setNumbytes(3); //RGB

     if (!outImage.hist_eqCPH())
     {
         cout << "Failed to perform histogram equalization"<<endl;
     }
   }
   //write final image to file
   if (!outImage.writeImage(argv[2]))
   {
      cout <<"Could not Write Image"<< endl;
   }

  return 0;
}
