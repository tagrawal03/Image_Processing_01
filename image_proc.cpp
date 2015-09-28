/*
***image_proc.cpp: Reads, writes and modify the input image
***Author: Taruna Agrawal
***Date: 20th september 2015
   email:tagrawal@usc.edu
   ID: 7650184685
*/

#include <fstream>
#include <math.h>
#include "image_proc.h"

//Constructor
Image::Image()
{
   data = NULL;
   width = -1;
   height = -1;
   numbytes = -1;
}

//Destructor
Image::~Image()
{
   if (data)
      delete data;
}

Image::Image(Image *otherImage)
{
   height = otherImage->height;
   width = otherImage->width;
   numbytes = otherImage->numbytes;
   data = new unsigned char[width*height*numbytes];
 
   for (int i =0; i<(height*width*numbytes); i++)
   {
     data[i] = otherImage->data[i];
   }


}

//operator overload

Image& Image::operator= (const Image &otherImage)
{
   height = otherImage.height;
   width = otherImage.width;
   numbytes = otherImage.numbytes;
   data = new unsigned char[width*height*3];

   for (int i =0; i<(height*width*numbytes); i++)
   {
     data[i] = otherImage.data[i];
   }
   return *this; 
}

//Function to read Image
bool Image::readImage(char* filename)
{
   if (width < 0 || height < 0)
   {
      cout << "Invalid width and height"<<endl;
      return false;
   }
   FILE* file;
   data = new unsigned char[width*height*numbytes];
   if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	fread(data, sizeof(unsigned char), width*height*numbytes, file);
	fclose(file);

   return true;
}


bool Image::writeImage(char* filename)
{

   FILE* file;
   if (!(file=fopen(filename,"wb"))) {
	cout << "Cannot open file: " << filename << endl;
	exit(1);
   }
   fwrite(data, sizeof(unsigned char), width*height*numbytes, file);
   fclose(file);

   return true;
}


/*Function to extend the image along the width and height
  This function works for three channel image*/
bool Image::image_extension(int E)
{

   /*Extend image by one row/column pixel from all sides*/
   int extension = E; //number of rows/columns to extend
   int newwidth = width+(2*extension);
   int newheight =height+(2*extension);
   unsigned char* newdata; 
   newdata = new unsigned char[newwidth*newheight*3]; //to store expanded input image with first two rows/column reflected
  
   /*copy input image to newdata - reflection of pixels -column wise*/
   for (int i=0; i < height; i++)
   {
      for(int j=0; j < width; j++)
      {
         newdata[3*(((i+extension)*newwidth) +(j+extension))] = data[3*((i*width) + j)]; //copy original R pixel value
         newdata[3*(((i+extension)*newwidth) +(j+extension)) +1] = data[3*((i*width) + j) +1]; //copy original G pixel value
         newdata[3*(((i+extension)*newwidth) +(j+extension)) +2] = data[3*((i*width) + j) +2]; //copy original B pixel value
      }
      for (int c = 0; c <extension; c++)
      {   
         newdata[3*((i+extension)*newwidth + c)] = data[3*((i*width) + c)]; //cth column R reflection
         newdata[3*((i+extension)*newwidth + c)+1] = data[3*((i*width) + c)+1]; //cth column G reflection
         newdata[3*((i+extension)*newwidth + c)+2] = data[3*((i*width) + c)+2]; //cth column B reflection
         
         newdata[3*((i+(extension+1))*newwidth - (c+1))] = data[3*(((i+1)*width)-(c+1))]; //last cth R column reflection
         newdata[3*((i+(extension+1))*newwidth - (c+1)) +1] = data[3*(((i+1)*width)-(c+1))+1]; //last G cth column reflection
         newdata[3*((i+(extension+1))*newwidth - (c+1))+2] = data[3*(((i+1)*width)-(c+1))+2]; //last B cth column reflection
      }
   }
   /*copy input image to newdata - reflection of pixels -row wise*/
   for (int a=0; a < newwidth; a++)
   {
      for (int r = 0; r< extension; r++)
      {
         newdata[3*(r*newwidth + a)] = newdata[3*((extension+r)*newwidth +a)]; //copy rth row
         newdata[3*(r*newwidth + a)+1] = newdata[3*((extension+r)*newwidth +a)+1]; //copy rth row
         newdata[3*(r*newwidth + a)+2] = newdata[3*((extension+r)*newwidth +a)+2]; //copy rth row
         
         newdata[3*((newwidth*(newheight-(r+1))) + a)] = newdata[3*((newheight-(extension+r+1))*newwidth +a)]; //copy last rth row 
         newdata[3*((newwidth*(newheight-(r+1))) + a) +1] = newdata[3*((newheight-(extension+r+1))*newwidth +a)+1]; //copy last rth row 
         newdata[3*((newwidth*(newheight-(r+1))) + a)+2] = newdata[3*((newheight-(extension+r+1))*newwidth +a)+2]; //copy last rth row 
      }
   } 


   data = new unsigned char[newwidth*newheight*3];
   for (int i =0; i < newheight*newwidth; i++)
   {
      data[3*i] =  newdata[3*i];
      data[3*i+1] =  newdata[3*i+1];
      data[3*i+2] =  newdata[3*i+2];      
   }
   delete newdata;

  return true;
}

/*Finction to implement Quantization on RGB image 
reduced to 64 color set*/
bool Image::Quantization()
{

   double hist_R[256] = {0} ,hist_G[256] = {0},hist_B[256] = {0};
   double cd_histR[256] = {0} ,cd_histG[256] = {0},cd_histB[256] = {0};
   int r = 1;
   int g = 1;
   int b = 1;
   int binR[5] = {0};
   int binG[5] = {0};
   int binB[5] = {0};
   unsigned int cumR =0;
   unsigned int cumG =0;
   unsigned int cumB =0;

   /*Count number of pixels at each intensity location*/
   for (int i = 0; i <height*width; i++)
   {
      hist_R[data[3*i]]++;
      hist_G[data[3*i+1]]++;
      hist_B[data[3*i+2]]++;
   }
   for (int i = 0; i < 256; i++)
   {
      cumR = cumR + hist_R[i];
      cumG = cumG + hist_G[i];
      cumB = cumB + hist_B[i];

      cd_histR[i] = cumR;
      cd_histG[i] = cumG;
      cd_histB[i] = cumB;

      /*Calculate threshold values- Choose threshold(x axis) where 25%, 50%, 75% 100%
       pixels lie(y axis)*/
      //Red
      if (cd_histR[i] > (0.25*r*height*width))
      {
         //store y axis intensity value as threshold
         binR[r] = i-1;
         r = r+1;
      }
      else if (cd_histR[i] == (0.25*r*height*width))
      {
         //store y axis intensity value as threshold
         binR[r] = i;
         r = r+1;
      }
     //Green
      if (cd_histG[i] > (0.25*g*height*width))
      {
         //store y axis intensity value as threshold
         binG[g] = i-1;
         g = g+1;
      }
      else if (cd_histG[i] == (0.25*g*height*width))
      {
         //store y axis intensity value as threshold
         binG[g] = i;
         g = g+1;
      }
      //Blue
      if (cd_histB[i] > (0.25*b*height*width))
      {
         //store y axis intensity value as threshold
         binB[b] = i-1;
         b = b+1;
      }
      else if (cd_histB[i] == (0.25*b*height*width))
      {
         //store y axis intensity value as threshold
         binB[b] = i;
         b = b+1;
      }
      
   }
      /*Calculating mean of each bin mean= (freq*value)/freq*/
      int meanR[4]= {0}, meanG[4]={0}, meanB[4]={0};
      for(int j =1; j<5; j++)
      {
         int freqR=0, freqG=0, freqB=0;
         for (int k = binR[j-1]; k <= binR[j]; k++)
         {
            meanR[j-1] = meanR[j-1] + (hist_R[k+1]*(k+1));
            freqR= freqR + hist_R[k+1];
         }
         meanR[j-1] =  (float)meanR[j-1]/(float)freqR;
         for (int k = binG[j-1]; k <= binG[j]; k++)
         {
            meanG[j-1] = meanG[j-1] + (hist_G[k+1]*(k+1));
            freqG= freqG + hist_G[k+1];
         }
         meanG[j-1] =  (float)meanG[j-1]/(float)freqG;
         for (int k = binB[j-1]; k <= binB[j]; k++)
         {
             meanB[j-1] = meanB[j-1] + (hist_B[k+1]*(k+1));
             freqB= freqB + hist_B[k+1];
         }
         meanB[j-1] =  (float)meanB[j-1]/(float)freqB;
      }

   /*Check each pixel for the bin in which they fall
     replace pixel with bin value*/
   for (int i = 0; i < height*width; i++)
   {
      for (int l=0; l<4; l++)
      {
         if (data[3*i] >= binR[l] && data[3*i] <= binR[l+1])
         {  
              //data[3*i] = (float)(binR[l]+binR[l+1])/(float)2;
              data[3*i] = meanR[l];
              break;
         }
      }
      for (int l=0; l<4; l++)
      {
         if (data[3*i +1] >= binG[l] && data[3*i +1] <= binG[l+1])
         {  
              //data[3*i+1] = (float)(binG[l]+binG[l+1])/(float)2;
              data[3*i +1] = meanG[l];
              break;
         }
      }
      for (int l=0; l<4; l++)
      {
         if (data[3*i+2] >= binB[l] && data[3*i +2] <= binB[l+1])
         {  
              //data[3*i+1] = (float)(binB[l]+binB[l+1])/(float)2;
              data[3*i+2] = meanB[l];
              break;
         }
      }

   }

  return true;

}
