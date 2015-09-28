/**Main.cpp

Input arguments program.cpp inputImage.raw outputImage.raw bytesperpixel width height
Author : Taruna Agrawal
Date: 20th september
ID:7650148685
Email:tagrawal@usc.edu
*/

#include "image_proc.cpp"

//Global variables
Image inImage, outImage;


/*Guided Image filtering
newdata stores extended image and is used as guided image*/

unsigned char* Image::guided_imageFiltering()
{
  
   int winSize;
   float epsilon;
   cout << "Enter the window size for Guided Image filtering"<<endl;
   cin >>winSize;
   cout << "Enter the regularization parameter"<<endl;
   cin >> epsilon;
   
   if (winSize < 0 || winSize >width || winSize >height || epsilon < 0)
   {
      cout << "Error: Enter correct value"<<endl;
      return NULL;
   }
   /*Extend image by extension row/column pixel from all sides*/
   int extension = floor(winSize/2); //number of rows/columns to extend
   int newwidth = width+(2*extension);
   int newheight = height+(2*extension);
   double* newdata; 
   newdata = new double[newwidth*newheight*3]; //to store expanded input image with first two rows/column reflected
  
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
      }
   }

   /*Guided Image filter implmentation*/
   int wI = winSize*winSize;  //num of pixels in window w(k) of I
   double meanRI=0,meanGI=0,meanBI=0,totalR=0,totalG=0,totalB=0;
   double meanRICor=0,meanGICor=0,meanBICor=0,totalRCor=0,totalGCor=0,totalBCor=0;
   double varR=0, varG=0,varB=0;
   double* modidata;   
   modidata = new double[newwidth*newheight*3]; 
   double* modidataA;   
   modidataA = new double[newwidth*newheight*3]; 
   double* modidataB;   
   modidataB = new double[newwidth*newheight*3]; 
   double* modidata1; 
   modidata1 = new double[newwidth*newheight*3]; 
   double* meanMat; 
   meanMat = new double[width*height]; 
   double* corrdata;   
   corrdata = new double[newwidth*newheight*3];
   double* varMat;   
   varMat = new double[width*height*3];
   double* corrMat;   
   corrMat = new double[width*height*3];
   double* epsilonMat;   
   epsilonMat = new double[width*height*3];
   double* aMat;   
   aMat = new double[width*height*3];
   double* bMat;   
   bMat = new double[width*height*3];
   double* coeffA;   
   coeffA = new double[width*height*3];
   double* coeffB;   
   coeffB = new double[width*height*3];
   /*Normalize pixel values*/
   for (int y=0; y < newwidth*newheight*3; y++)
   {
         // int pix = (y*(newwidth*3)) + (x*3);
          newdata[y] =  (float)newdata[y]/(float)255; 
   }
   /*Step 1*/
   /*I.*I*/
   for (int index=0; index < newheight*newwidth; index++)
   {
       corrdata[3*index] = newdata[3*index]*newdata[3*index];
       corrdata[3*index + 1] = newdata[3*index + 1]*newdata[3*index +1];
       corrdata[3*index + 2] = newdata[3*index +2]*newdata[3*index + 2];
   }
 
   for (int i = extension; i < height+extension; i++)
   {
      for (int j = extension; j < width+extension; j++)
      {
         
          meanRI=0;meanGI=0;meanBI=0;totalR=0;totalG=0;totalB=0;
          meanRICor=0;meanGICor=0;meanBICor=0;totalRCor=0;totalGCor=0;totalBCor=0;
          int imgpixel = (i*newwidth*3) +3*j;

           /*Calculation of mean of window*/
          for (int blkrow = i-extension; blkrow <= i+extension; blkrow++)
          {
             for (int blkcol = j-extension; blkcol <= j+extension; blkcol++)
             {
               
                 int blkpixel = (blkrow*3*newwidth) + 3*blkcol;
                 /*channels*/
                 totalR = totalR + newdata[blkpixel];
                 totalG = totalG + newdata[blkpixel+1];
                 totalB = totalB + newdata[blkpixel+2];
                 totalRCor = totalRCor + corrdata[blkpixel];
                 totalGCor = totalGCor + corrdata[blkpixel+1];
                 totalBCor = totalBCor + corrdata[blkpixel+2];
             }
          }
          meanRI = totalR/(winSize*winSize);
          meanGI = totalG/(winSize*winSize);
          meanBI = totalB/(winSize*winSize);
          meanRICor = totalRCor/(winSize*winSize);
          meanGICor = totalGCor/(winSize*winSize);
          meanBICor = totalBCor/(winSize*winSize);

          modidata[imgpixel] = meanRI;
          modidata[imgpixel+1] = meanGI;
          modidata[imgpixel+2] = meanBI;
                    
          modidata1[imgpixel] = meanRICor;
          modidata1[imgpixel+1] = meanGICor;
          modidata1[imgpixel+2] = meanBICor;
       }
   }

   for (int i = extension; i < height+extension; i++)
   {
      for (int j = extension; j < width+extension; j++)
      {
         int pixel = 3*(i-extension)*width + 3*(j-extension);
         int pixelres = 3*i*newwidth +(3*j);
         meanMat[pixel] =  modidata[pixelres];
         meanMat[pixel+1] =  modidata[pixelres+1];
         meanMat[pixel+2] =  modidata[pixelres+2];
         corrMat[pixel] =  modidata1[pixelres];
         corrMat[pixel+1] =  modidata1[pixelres+1];
         corrMat[pixel+2] =  modidata1[pixelres+2];
      }   
   }
   /*Step 2*/
   for (int iPix=0; iPix <height*width; iPix++)
   {
       varMat[3*iPix] = corrMat[3*iPix] - (meanMat[3*iPix] * meanMat[3*iPix]);
       varMat[3*iPix +1] = corrMat[3*iPix +1] - (meanMat[3*iPix +1] * meanMat[3*iPix +1]);
       varMat[3*iPix+2] = corrMat[3*iPix+2] - (meanMat[3*iPix+2] * meanMat[3*iPix+2]);
   }

   /*Step 3*/
   /*create epsilon matrix*/
   for (int iPix=0; iPix <height*width; iPix++)
   {
       epsilonMat[3*iPix] = epsilon;
       epsilonMat[3*iPix +1] = epsilon;
       epsilonMat[3*iPix+2] = epsilon;
   }
   
   for (int iPix=0; iPix <height*width; iPix++)
   {
      aMat[3*iPix] = varMat[3*iPix]/(varMat[3*iPix] + epsilonMat[3*iPix]); 
      aMat[3*iPix +1] = varMat[3*iPix +1]/(varMat[3*iPix+1] + epsilonMat[3*iPix+1]); 
      aMat[3*iPix+2] = varMat[3*iPix+2]/(varMat[3*iPix+2] + epsilonMat[3*iPix+2]); 
   
      bMat[3*iPix] = meanMat[3*iPix] - (aMat[3*iPix]*meanMat[3*iPix]);
      bMat[3*iPix +1] = meanMat[3*iPix +1] - (aMat[3*iPix +1]*meanMat[3*iPix +1]);
      bMat[3*iPix +2] = meanMat[3*iPix +2] - (aMat[3*iPix+2]*meanMat[3*iPix+2]);
   }
  /*extend aMat and bMat*/
   double* anewdata; 
   anewdata = new double[newwidth*newheight*3]; //to store expanded input image with first two rows/column reflected
   double* bnewdata; 
   bnewdata = new double[newwidth*newheight*3]; //to store expanded input image with first two rows/column reflected
  
   /*copy input image to newdata - reflection of pixels -column wise*/
   for (int i=0; i < height; i++)
   {
      for(int j=0; j < width; j++)
      {
         anewdata[3*(((i+extension)*newwidth) +(j+extension))] = aMat[3*((i*width) + j)]; //copy original R pixel value
         anewdata[3*(((i+extension)*newwidth) +(j+extension)) +1] = aMat[3*((i*width) + j) +1]; //copy original G pixel value
         anewdata[3*(((i+extension)*newwidth) +(j+extension)) +2] = aMat[3*((i*width) + j) +2]; //copy original B pixel value
 
         
         bnewdata[3*(((i+extension)*newwidth) +(j+extension))] = bMat[3*((i*width) + j)]; //copy original R pixel value
         bnewdata[3*(((i+extension)*newwidth) +(j+extension)) +1] = bMat[3*((i*width) + j) +1]; //copy original G pixel value
         bnewdata[3*(((i+extension)*newwidth) +(j+extension)) +2] = bMat[3*((i*width) + j) +2]; //copy original B pixel value
     }
      for (int c = 0; c <extension; c++)
      {   
         anewdata[3*((i+extension)*newwidth + c)] = aMat[3*((i*width) + c)]; //cth column R reflection
         anewdata[3*((i+extension)*newwidth + c)+1] = aMat[3*((i*width) + c)+1]; //cth column G reflection
         anewdata[3*((i+extension)*newwidth + c)+2] = aMat[3*((i*width) + c)+2]; //cth column B reflection
         
         anewdata[3*((i+(extension+1))*newwidth - (c+1))] = aMat[3*(((i+1)*width)-(c+1))]; //last cth R column reflection
         anewdata[3*((i+(extension+1))*newwidth - (c+1)) +1] = aMat[3*(((i+1)*width)-(c+1))+1]; //last G cth column reflection
         anewdata[3*((i+(extension+1))*newwidth - (c+1))+2] = aMat[3*(((i+1)*width)-(c+1))+2]; //last B cth column reflection
      

         bnewdata[3*((i+extension)*newwidth + c)] = bMat[3*((i*width) + c)]; //cth column R reflection
         bnewdata[3*((i+extension)*newwidth + c)+1] = bMat[3*((i*width) + c)+1]; //cth column G reflection
         bnewdata[3*((i+extension)*newwidth + c)+2] = bMat[3*((i*width) + c)+2]; //cth column B reflection
         
         bnewdata[3*((i+(extension+1))*newwidth - (c+1))] = bMat[3*(((i+1)*width)-(c+1))]; //last cth R column reflection
         bnewdata[3*((i+(extension+1))*newwidth - (c+1)) +1] = bMat[3*(((i+1)*width)-(c+1))+1]; //last G cth column reflection
         bnewdata[3*((i+(extension+1))*newwidth - (c+1))+2] = bMat[3*(((i+1)*width)-(c+1))+2]; //last B cth column reflection

       }
   }
   /*copy input image to newdata - reflection of pixels -row wise*/
   for (int a=0; a < newwidth; a++)
   {
      for (int r = 0; r< extension; r++)
      {
         anewdata[3*(r*newwidth + a)] = anewdata[3*((extension+r)*newwidth +a)]; //copy rth row
         anewdata[3*(r*newwidth + a)+1] =anewdata[3*((extension+r)*newwidth +a)+1]; //copy rth row
         anewdata[3*(r*newwidth + a)+2] = anewdata[3*((extension+r)*newwidth +a)+2]; //copy rth row
         
         anewdata[3*((newwidth*(newheight-(r+1))) + a)] = anewdata[3*((newheight-(extension+r+1))*newwidth +a)]; //copy last rth row 
      

         bnewdata[3*(r*newwidth + a)] = bnewdata[3*((extension+r)*newwidth +a)]; //copy rth row
         bnewdata[3*(r*newwidth + a)+1] =bnewdata[3*((extension+r)*newwidth +a)+1]; //copy rth row
         bnewdata[3*(r*newwidth + a)+2] = bnewdata[3*((extension+r)*newwidth +a)+2]; //copy rth row
         
         bnewdata[3*((newwidth*(newheight-(r+1))) + a)] = bnewdata[3*((newheight-(extension+r+1))*newwidth +a)]; //copy last rth row 

      }
   }


   for (int i = extension; i < height+extension; i++)
   {
      for (int j = extension; j < width+extension; j++)
      {
         
          double meanRa=0,meanGa=0,meanBa=0,totalRa=0,totalGa=0,totalBa=0;
          double meanRb=0,meanGb=0,meanBb=0,totalRb=0,totalGb=0,totalBb=0;
          int imgpixel = (i*newwidth*3) +3*j;

           /*Calculation of mean of window*/
          for (int blkrow = i-extension; blkrow <= i+extension; blkrow++)
          {
             for (int blkcol = j-extension; blkcol <= j+extension; blkcol++)
             {
               
                 int blkpixel = (blkrow*3*newwidth) + 3*blkcol;
                 /*channels*/
                 totalRa = totalRa + anewdata[blkpixel];
                 totalGa = totalGa + anewdata[blkpixel+1];
                 totalBa = totalBa + anewdata[blkpixel+2];
                 totalRb = totalRb + bnewdata[blkpixel];
                 totalGb = totalGb + bnewdata[blkpixel+1];
                 totalBb = totalBb + bnewdata[blkpixel+2];
             }
          }
          meanRa = totalRa/(winSize*winSize);
          meanGa = totalGa/(winSize*winSize);
          meanBa = totalBa/(winSize*winSize);
          meanRb = totalRb/(winSize*winSize);
          meanGb = totalGb/(winSize*winSize);
          meanBb = totalBb/(winSize*winSize);

          modidataA[imgpixel] = meanRa;
          modidataA[imgpixel+1] = meanGa;
          modidataA[imgpixel+2] = meanBa;
                    
          modidataB[imgpixel] = meanRb;
          modidataB[imgpixel+1] = meanGb;
          modidataB[imgpixel+2] = meanBb;
       }
   }

   for (int i = extension; i < height+extension; i++)
   {
      for (int j = extension; j < width+extension; j++)
      {
         int pixel = 3*(i-extension)*width + 3*(j-extension);
         int pixelres = 3*i*newwidth +(3*j);
         coeffA[pixel] =  modidataA[pixelres];
         coeffA[pixel+1] =  modidataA[pixelres+1];
         coeffA[pixel+2] =  modidataA[pixelres+2];
         coeffB[pixel] =  modidataB[pixelres];
         coeffB[pixel+1] =  modidataB[pixelres+1];
         coeffB[pixel+2] =  modidataB[pixelres+2];
   //      cout << coeffA[pixel]<<endl;
     //   cout << coeffB[pixel]<<endl;
      }   
   }
   
   for (int iPix=0; iPix < width*height; iPix++)
   {
       data[3*iPix] =  ((coeffA[3*iPix]*((double)data[3*iPix]/(double)255)) + coeffB[iPix*3])*255;
       data[3*iPix+1] =  ((coeffA[3*iPix+1]*((double)data[3*iPix+1]/(double)255)) + coeffB[iPix*3+1])*255;
       data[3*iPix+2] =  ((coeffA[3*iPix+2]*((double)data[3*iPix+2]/(double)255)) + coeffB[iPix*3+2])*255;
       
 //      data[3*iPix] =  ((coeffA[3*iPix]*data[3*iPix]) + coeffB[iPix*3]);
   //    data[3*iPix+1] =  ((coeffA[(3*iPix) + 1]*data[(3*iPix)+1]) + coeffB[(iPix*3)+1]);
     //  data[3*iPix+2] =  ((coeffA[(3*iPix)+2]*data[(3*iPix)+2]) + coeffB[(iPix*3)+2]);
   }

 delete newdata;
 delete modidata;

 return data;
}

int main(int argc, char* argv[])
{

   int BytesPerPixel = 1;
   int Width = 424;
   int Height = 636;
   int outWidth, outHeight;
   unsigned char* outputImage;

   if (argc < 6)
   {
      cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
      cout << "program_name input_image.raw output_image.raw" <<endl;
      cout << " BytesPerPixel width height original_image.raw" << endl;
      return 0;
   }	
   else 
   {
      BytesPerPixel = atoi(argv[3]);
      inImage.setNumbytes(BytesPerPixel);

      // Check if size is specified
      if (argc >= 5){
	 Width = atoi(argv[4]);
	 Height = atoi(argv[5]);
         inImage.setWidth(Width);
         inImage.setHeight(Height);
       }
   }

   //Read Image
   if (!inImage.readImage(argv[1]))
   {
      cout <<"Could not Read Image"<< endl;
   }
   else
   {
     cout << "Problem 3b-Guided image filtering" <<endl;
     outImage = inImage;
     
     outputImage = outImage.guided_imageFiltering();    
     if (outputImage == NULL)
     {
         cout << "Error:Could not perform guided image filtering ."<<endl;
     }
   }
   //write final image to file
   if (!outImage.writeImage(argv[2]))
   {
      cout <<"Could not Write Image"<< endl;
   }

   /*Read original image*/

   FILE* fileId;

   unsigned char* origdata;
   origdata = new unsigned char[Width*Height*BytesPerPixel];
   if (!(fileId=fopen(argv[6],"rb"))) {
		cout << "Cannot open file: " <<endl;
		exit(1);
	}
	fread(origdata, sizeof(unsigned char), Width*Height*BytesPerPixel, fileId);
	fclose(fileId);

   /*Calculate PSNR*/
   double errorRedMSE =0;
   double errorBlueMSE =0;
   double errorGreenMSE =0;
   double rPSNR =0;
   double gPSNR =0;
   double bPSNR =0;

   for (int iPix =0; iPix< Height*Width; iPix++)
   {
      errorRedMSE += (outputImage[3*iPix] - 
                        origdata[3*iPix])*(outputImage[3*iPix] - origdata[3*iPix]);
      errorGreenMSE+= (outputImage[3*iPix +1] - 
                       origdata[3*iPix +1])*(outputImage[3*iPix +1] - origdata[3*iPix +1]);
      errorBlueMSE+= (outputImage[3*iPix +2] - 
                         origdata[3*iPix +2])*(outputImage[3*iPix+2] - origdata[3*iPix+2]);
   }
   errorRedMSE = errorRedMSE/(double)(Width*Height);
   errorGreenMSE = errorGreenMSE/(double)(Width*Height);
   errorBlueMSE = errorBlueMSE/(double)(Width*Height);

   rPSNR = 10*log10((255.0*255.0)/errorRedMSE);
   gPSNR = 10*log10((255.0*255.0)/errorGreenMSE);
   bPSNR = 10*log10((255.0*255.0)/errorBlueMSE);

   cout << "PSNR of Red Channel is "<<rPSNR<<endl;
   cout << "PSNR of Green Channel is "<<gPSNR<<endl;
   cout << "PSNR of Blue Channel is "<<bPSNR<<endl;
  
   return 0;
}
