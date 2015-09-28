/*Problem1_Resize.cpp

Input arguments program.cpp inputImage.raw outputImage.raw bytesperpixel width height
Author : Taruna Agrawal
ID: 7650184685
Email: tagrawal@usc.edu
Date: 20th september

*/

#include "image_proc.cpp"

//Global variables
Image inImage, outImage;

/*Function to resize the image using bilinear interpolation*/
bool Image::resizing_biInter(int newwidth, int newheight)
{
   double ratio_col = (double)width/(double)newwidth;
   double ratio_row = (double)height/(double)newheight;
   unsigned char* temp;
   temp = new unsigned char[newwidth*newheight*numbytes];
  
   for (int y=0; y < newheight; y++)
   {
      for (int x=0; x < newwidth; x++)
      {
          double ynew = y*ratio_row;
          double xnew = x*ratio_col;
          int yold = floor(ynew);
          int xold = floor(xnew);
          if (yold < 0)
             yold = 0;
          else if (yold > height-1)
             yold = height - 1;
          if (xold < 0)
             xold = 0;
          else if (xold > height-1)
             xold = height - 1;
          int a = ynew - yold;
          int b = xnew - xold;
          int pixel = (y*(newwidth*3)) + (x*3);
          int data_co1 = yold*(width*3) + xold*3;
          int data_co2 = (yold+1)*(width*3) + xold*3;
          int data_co3 = (yold*(width*3)) + (xold+1)*3;
          int data_co4 = (yold+1)*(width*3) + (xold+1)*3;
          //Interpolation
          
          temp[pixel] = (1-a)*(1-b)*data[data_co1] + a*(1-b)*data[data_co2] + \
                                  (1-a)*b*data[data_co3] + a*b*data[data_co4];
          temp[pixel+1] = (1-a)*(1-b)*data[data_co1 + 1] + a*(1-b)*data[data_co2 + 1] + \
                                  (1-a)*b*data[data_co3 + 1] + a*b*data[data_co4 +1];
          temp[pixel+2] = (1-a)*(1-b)*data[data_co1 + 2] + a*(1-b)*data[data_co2 +2] + \
                                (1-a)*b*data[data_co3 + 2] + a*b*data[data_co4 + 2];
      }
   }

   data = new unsigned char[newwidth*newheight*3];
   for (int i =0; i < newheight*newwidth; i++)
   {
      data[3*i] =  temp[3*i];
      data[3*i+1] =  temp[3*i+1];
      data[3*i+2] =  temp[3*i+2];      
   }
   delete temp;
   return true;
} 


int main(int argc, char* argv[])
{

   int BytesPerPixel = 1;
   int Width = 512;
   int Height = 512;
   int outWidth, outHeight;

   if (argc < 5)
   {
      cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
      cout << "program_name input_image.raw output_image.raw BytesPerPixel width height" << endl;
      return 0;
   }	
   // Check if image is grayscale or color
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
     cout << "Image resizing" <<endl;
     cout << "Please enter new size of Image width height" <<endl;
     cin >> outWidth >> outHeight;
     outImage = inImage;
     if (!outImage.resizing_biInter(outWidth, outHeight))
     {
         cout << "Error:Could not resize Image"<<endl;
     }
   }
    outImage.setWidth(outWidth);
    outImage.setHeight(outHeight);
   //write final image to file
   if (!outImage.writeImage(argv[2]))
   {
      cout <<"Could not Write Image"<< endl;
   }

  return 0;
}
