/**Main.cpp

Input arguments program.cpp inputImage.raw outputImage.raw bytesperpixel width height
Author : Taruna Agrawal
Date: 20th september
email:tagrawal@usc.edu
ID: 7650184685
*/

#include "image_proc.cpp"

//Global variables
Image inImage, outImage;

/*Demosaicing using bilinear interpolation*/
bool Image::demosaic_bilinear()
{
   int newwidth = width+4;
   int newheight =height+4;
   unsigned char* temp;
   unsigned char* newdata; 
   temp = new unsigned char[width*height*3];  //to store final RGB image
   newdata = new unsigned char[(width+4)*(height+4)]; //to store expanded input image with first two rows/column reflected
   
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

   //Color interpolation-Demosaicing 
   //All odd pixels are green in Bayer's pattern
   //All even pixels are red or blue in Bayer's pattern
   for (int y=0; y < height; y++)
   {
      for (int x=0; x < width; x++)
      {
          int pixel = (y*(width*3)) + (x*3);
         /*if even row the R G R G pattern*/
          if ((y % 2) == 0)
          { 
             /*even pixel is R*/
             if ((x % 2) == 0)
             {
                temp[pixel] = data[(y*width) + x]; //Red
                temp[pixel+1] = ((newdata[((y+1)*newwidth) + (x+2)] + 
                               newdata[((y+3)*newwidth + (x+2))]) +
                                (newdata[((y+2)*newwidth) + (x+1)] + 
                                  newdata[((y+2)*(newwidth) + (x+3))]))/4; //fill Green
                 temp[pixel+2] = ((newdata[((y+1)*newwidth) + (x+1)] + 
                                  newdata[((y+1)*(newwidth) + (x+3))]) +
                                   (newdata[((y+3)*newwidth) + (x+1)] + 
                                     newdata[((y+3)*(newwidth) + (x+3))]))/4; //fill Blue
             }
             /*odd pixel is G*/
             else
             {
                temp[pixel] = (newdata[((y+2)*newwidth) + (x+1)] + newdata[((y+2)*(newwidth) + (x+3))])/2; //fill Red
                temp[pixel+1] = data[(y*width) + x]; //Green
                temp[pixel+2] = (newdata[((y+1)*newwidth) + (x+2)] + newdata[((y+3)*newwidth + (x+2))])/2; //fill Red
             }
          }
          /*odd row B G B G B G pattern*/
          else
          {  /* Green pixel*/
             if ((x % 2) == 0)
             {
                temp[pixel] = (newdata[((y+1)*newwidth) + (x+2)] + newdata[((y+3)*newwidth + (x+2))])/2; //fill Red
                temp[pixel+1] = data[(y*width) + x]; //Green
                temp[pixel+2] = (newdata[((y+2)*newwidth) + (x+1)] + newdata[((y+2)*(newwidth) + (x+3))])/2; //fill Blue
             }
             /*Blue pixel*/
             else
             {
                 temp[pixel] = ((newdata[((y+1)*newwidth) + (x+1)] + 
                                  newdata[((y+1)*(newwidth) + (x+3))]) +
                                   (newdata[((y+3)*newwidth) + (x+1)] + 
                                     newdata[((y+3)*(newwidth) + (x+3))]))/4; //fill red
                temp[pixel+1] = ((newdata[((y+1)*newwidth) + (x+2)] + 
                               newdata[((y+3)*newwidth + (x+2))]) +
                                (newdata[((y+2)*newwidth) + (x+1)] + 
                                  newdata[((y+2)*(newwidth) + (x+3))]))/4; //fill Green
                temp[pixel+2] = data[(y*width) + x]; //Blue
             }             
          }
       }
   } 

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
      cout << "Error - Incorrect Parameter Usage: Please use as mentioned below" << endl;
      cout << "program_name input_image.raw output_image.raw BytesPerPixel width height" << endl;
      return 0;
   }	
   BytesPerPixel = atoi(argv[3]);
   inImage.setNumbytes(BytesPerPixel);

   // Check if size is specified
   if (argc >= 5)
   {
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
     cout << "Linear Demosaicing" <<endl;
     outImage = inImage;
     //outImage.setNumbytes(3); //RGB

     if (!outImage.demosaic_bilinear())
     {
         cout << "Error:Could not perform demosaicing "<<endl;
     }
   }
    //outImage.setWidth(Width+4);
    //outImage.setHeight(Height+4);
    outImage.setNumbytes(3); //RGB
   //write final image to file
   if (!outImage.writeImage(argv[2]))
   {
      cout <<"Could not Write Image"<< endl;
   }

  return 0;
}
