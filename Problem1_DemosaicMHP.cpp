/**Main.cpp

Input arguments program.cpp inputImage.raw outputImage.raw bytesperpixel width height
Author : Taruna Agrawal
Date: 20th september 2015
email:tagrawal@usc.edu
ID: 7650184685
*/

#include "image_proc.cpp"

//Global variables
Image inImage, outImage;


/*Function implements demosaicing using MHC algoritm*/
bool Image::demosaic_MHP()
{
   int newwidth = width+4;
   int newheight =height+4;
   unsigned char* temp;
   unsigned char* newdata; 
   temp = new unsigned char[width*height*3];  //to store final RGB image
   newdata = new unsigned char[(width+4)*(height+4)]; //to store expanded input image with first two rows/column reflected
   float* floatPixel; 
   floatPixel = new float[(width+4)*(height+4)]; //to store expanded input image with first two rows/column reflected
   
   /*copy input image to newdata - reflection of pixels -column wise*/
   for (int i=0; i < height; i++)
   {
      newdata[(i+2)*newwidth] = data[(i*width)]; //1st column reflection
      newdata[((i+2)*newwidth) +1] = data[(i*width)+1]; //2nd column reflection
      for(int j=0; j < width; j++)
      {
         newdata[((i+2)*newwidth) +(j+2)] = data[(i*width) + j]; //copy original pixel value
      }
      newdata[(i+3)*newwidth -1] = data[((i+1)*width)-1]; //2nd column reflection
      newdata[(i+3)*newwidth -2] = data[((i+1)*width)-2]; // last column reflection
   }
   /*copy input image to newdata - reflection of pixels -row wise*/
   for (int a=0; a < newwidth; a++)
   {
      newdata[a] = newdata[2*newwidth +a]; //1st row
      newdata[newwidth + a] = newdata[3*newwidth +a]; //2nd row
      newdata[newwidth*(newheight-2) + a] = newdata[(newheight-4)*newwidth +a]; //2nd last row
      newdata[newwidth*(newheight-1) + a] = newdata[(newheight-3)*newwidth +a]; //last row
   }
   // Demosaicing -MHC
  
   /*Normalize pixel values*/
   for (int y=0; y < newwidth*newheight; y++)
   {
         // int pix = (y*(newwidth*3)) + (x*3);
          floatPixel[y] =  (float)newdata[y]/(float)255; 
   }

   /*MHC based on 8 filters*/   
   for (int y=0; y < height; y++)
   {
      for (int x=0; x < width; x++)
      {
          int pixel = (y*(width*3)) + (x*3);
          float val[3];
         /*if even row the R G R G pattern*/
          if ((y % 2) == 0)
          { 
             /*even pixel is R*/
             if ((x % 2) == 0)
             {
                val[0] = (float)data[(y*width) + x]/(float)255; //Red
                /*G at R locations*/
                val[1] = (0.125)*((2*((floatPixel[((y+1)*newwidth) + (x+2)] + 
                               floatPixel[((y+3)*newwidth + (x+2))]) +
                                (floatPixel[((y+2)*newwidth) + (x+1)] + 
                                  floatPixel[((y+2)*(newwidth) + (x+3))]))) - 
                                  (1*(((floatPixel[(y*newwidth) + (x+2)] +
                                floatPixel[((y+4)*newwidth + (x+2))]) +
                                 (floatPixel[((y+2)*newwidth) + x] + 
                                  floatPixel[((y+2)*(newwidth) + (x+4))]))))+
                                  (4*floatPixel[((y+2)*(newwidth) + (x+2))]) ) ; //fill Green
                 /*B at red in R row, R column*/
                val[2] = (0.125)*((2*((floatPixel[((y+1)*newwidth) + (x+1)] + 
                                  floatPixel[((y+1)*(newwidth) + (x+3))]) +
                                   (floatPixel[((y+3)*newwidth) + (x+1)] + 
                                     floatPixel[((y+3)*(newwidth) + (x+3))]))) - 
                                  ((1.5)*(floatPixel[(y*newwidth) + (x+2)] +
                                floatPixel[((y+4)*newwidth + (x+2))] +
                                 floatPixel[((y+2)*newwidth) + x] + 
                                  floatPixel[((y+2)*(newwidth) + (x+4))])) +
                                  (6*floatPixel[((y+2)*(newwidth) + (x+2))])) ; //fill blue*/
                      


                }
             /*odd pixel is G*/
             else
             { 
                /*R at green in R row, B column*/
                val[0] = (0.125)* ((4*(floatPixel[((y+2)*newwidth) + (x+1)] + 
                                  floatPixel[((y+2)*(newwidth) + (x+3))])) -
                                  1*((floatPixel[(y+2)*(newwidth) + (x)])+ (floatPixel[(y+2)*(newwidth) +(x+4)]) +
                                   (floatPixel[(y+1)*newwidth + (x+1)]) + (floatPixel[(y+3)*newwidth + (x+1)]) +
                                   (floatPixel[(y+1)*newwidth + (x+3)]) + (floatPixel[(y+3)*newwidth + (x+3)])) +
                                  (0.5)*(floatPixel[(y*newwidth) + (x+2)] + floatPixel[((y+4)*newwidth) + (x+2)]) +
                                   5*floatPixel[((y+2)*newwidth) + (x+2)]); //fill red
                
                val[1] = (float)data[(y*width) + x]/(float)255; //Green
                
                /*Blue at green in R row, B column*/
                val[2] = (0.125)* ((4*(floatPixel[((y+1)*newwidth) + (x+2)] + 
                                  floatPixel[((y+3)*(newwidth) + (x+2))])) +
                                  ((0.5)*((floatPixel[(y+2)*(newwidth) + (x)])+ (floatPixel[(y+2)*(newwidth) +(x+4)]))) -
                                   1*((floatPixel[(y+1)*newwidth + (x+1)]) + (floatPixel[(y+3)*newwidth + (x+1)]) +
                                   (floatPixel[(y+1)*newwidth + (x+3)]) + (floatPixel[(y+3)*newwidth + (x+3)])) -
                                  ((1)*(floatPixel[(y*newwidth) + (x+2)] + floatPixel[((y+4)*newwidth) + (x+2)])) +
                                  (5*floatPixel[((y+2)*newwidth) + (x+2)])); //fill blue*/
             }
          }
          /*odd row B G B G B G pattern*/
          else
          {  /* Green pixel*/
             if ((x % 2) == 0)
             {
               /*R at green in B row, R column*/ 
                val[0] = (0.125)* ((4*(floatPixel[((y+1)*newwidth) + (x+2)] + 
                                  floatPixel[((y+3)*(newwidth) + (x+2))])) +
                                  (0.5)*((floatPixel[(y+2)*(newwidth) + x])+ (floatPixel[(y+2)*(newwidth) +(x+4)])) -
                                   1*((floatPixel[(y+1)*newwidth + (x+1)]) + (floatPixel[(y+3)*newwidth + (x+1)]) +
                                   (floatPixel[(y+1)*newwidth + (x+3)]) + (floatPixel[(y+3)*newwidth + (x+3)])) -
                                   1*(floatPixel[(y*newwidth) + (x+2)] + floatPixel[((y+4)*newwidth) + (x+2)]) +
                                  (5*floatPixel[((y+2)*newwidth) + (x+2)])); //fill red
                
                val[1] = (float)data[(y*width) + x]/(float)255; //Green
                /*B at green in B row, R column*/
                val[2] = (0.125)* ((4*(floatPixel[((y+2)*newwidth) + (x+1)] + 
                                  floatPixel[((y+2)*(newwidth) + (x+3))])) -
                                  1*((floatPixel[(y+2)*(newwidth) + (x)])+ (floatPixel[(y+2)*(newwidth) +(x+4)]) +
                                   (floatPixel[(y+1)*newwidth + (x+1)]) + (floatPixel[(y+3)*newwidth + (x+1)]) +
                                   (floatPixel[(y+1)*newwidth + (x+3)]) + (floatPixel[(y+3)*newwidth + (x+3)])) +
                                  ((0.5)*(floatPixel[(y*newwidth) + (x+2)] + floatPixel[((y+4)*newwidth) + (x+2)])) +
                                  (5*floatPixel[((y+2)*newwidth) + (x+2)])); //fill blue*/
             }
             /*Blue pixel*/
             else
             {
                 /*R at blue in B row, B column*/
                 val[0] = (0.125)*((2*((floatPixel[((y+1)*newwidth) + (x+1)] + 
                                  floatPixel[((y+1)*(newwidth) + (x+3))]) +
                                   (floatPixel[((y+3)*newwidth) + (x+1)] + 
                                     floatPixel[((y+3)*(newwidth) + (x+3))]))) - 
                                  ((1.5)*(((floatPixel[(y*newwidth) + (x+2)] +
                                floatPixel[((y+4)*newwidth + (x+2))]) +
                                 (floatPixel[((y+2)*newwidth) + x] + 
                                  floatPixel[((y+2)*(newwidth) + (x+4))])))) +
                                  (6*floatPixel[((y+2)*(newwidth) + (x+2))])) ; //fill Red
                 /*G at B locations*/
                 val[1] = (0.125)*((2*((floatPixel[((y+1)*newwidth) + (x+2)] + 
                               floatPixel[((y+3)*newwidth + (x+2))]) +
                                (floatPixel[((y+2)*newwidth) + (x+1)] + 
                                  floatPixel[((y+2)*(newwidth) + (x+3))]))) - 
                                  (1*(((floatPixel[(y*newwidth) + (x+2)] +
                                floatPixel[((y+4)*newwidth + (x+2))]) +
                                 (floatPixel[((y+2)*newwidth) + x] + 
                                  floatPixel[((y+2)*(newwidth) + (x+4))]))))+
                                  (4*floatPixel[((y+2)*(newwidth) + (x+2))]) ) ; //fill Green
                 /*Blue pixel*/
                 val[2] = (float)data[(y*width) + x]/(float)255; //Blue
             }             
          }
          for (int iVal= 0; iVal<3; iVal++)
          {
             if (val[iVal] < 0)
               val[iVal] = 0;
             else if(val[iVal] > 1)
               val[iVal] = 1;
          }             
          temp[pixel] = val[0]*255;
          temp[pixel+1] = val[1]*255;
          temp[pixel+2] = val[2]*255;
       } //end for loop width
   } // end for loop height
   
   data = new unsigned char[width*height*3];
   for (int i =0; i < height*width; i++)
   {
      data[3*i] =  temp[3*i];
      data[3*i+1] =  temp[3*i+1];
      data[3*i+2] =  temp[3*i+2];      
   }


   delete temp;
   delete newdata;
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
     cout << "MHP Demosaicing" <<endl;
     outImage = inImage;

     if (!outImage.demosaic_MHP())
     {
         cout << "Error:Could not demosaic Image"<<endl;
     }
   }
   outImage.setNumbytes(3); //RGB
   //write final image to file
   if (!outImage.writeImage(argv[2]))
   {
      cout <<"Could not Write Image"<< endl;
   }

  return 0;
}
