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

/*Function to perform the bilateral filetring of images
Input: Image with salt/pepper noise removed through median filtering
Output: Denoised Image*/

unsigned char* Image::bilateral_filtering(double sigmaS, double sigmaR)
{
   int winSize;
   cout << "Enter the window size for bilateral filtering"<<endl;
   cin >> winSize;

   
   if (winSize < 0 || winSize >width || winSize >height)
   {
      cout << "Error: Enter correct value of postive";
      return NULL;
   }

   /*Extend image by extension row/column pixel from all sides*/
   int extension = floor(winSize/2); //number of rows/columns to extend
   int newwidth = width+(2*extension);
   int newheight = height+(2*extension);
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
      }
   }

   unsigned char* modidata;   
   modidata = new unsigned char[newwidth*newheight*3]; //to store expanded input image with first two rows/column reflected
   /*store gaussain weights for spatial*/
   double **weightS = new double*[winSize];
   for (int iWin=0; iWin < winSize; iWin++)
       weightS[iWin] = new double[winSize]; 
   /*Stpre gaussian weights for range*/ 
   double **weightR = new double*[winSize];
   for (int iWin=0; iWin < winSize; iWin++)
       weightR[iWin] = new double[winSize];  
   double **weightG = new double*[winSize];
   for (int iWin=0; iWin < winSize; iWin++)
       weightG[iWin] = new double[winSize];  
   double **weightB = new double*[winSize];
   for (int iWin=0; iWin < winSize; iWin++)
       weightB[iWin] = new double[winSize];  

    /*Calculation of gaussian weights using sigmaS*/
    for (int m =0; m<winSize; m++)
    {
        for (int n =0; n< winSize; n++)
        {
            weightS[m][n] = exp((-1*(pow((double)(m-extension),(double)2) + 
                                               pow((double)(n-extension), (double)2.0)))/(double)(2.0*sigmaS*sigmaS));
            weightS[m][n] = weightS[m][n]/sqrt(2*3.14*sigmaS);  // check if this is needed 
        }
    }

   for (int i = extension; i < height+extension; i++)
   {
      for (int j = extension; j < width+extension; j++)
      {
          int indexX=0,indexY=0, X=0, Y=0;
          double sumBFR=0, sumBFG=0, sumBFB=0, WpR=0, WpG = 0, WpB =0;
         
          int imgpixel = (i*newwidth*3) +3*j;

           /*Calculation of gaussian weights using sigmaR*/
          for (int blkrow = i-extension; blkrow <= i+extension; blkrow++)
          {
             for (int blkcol = j-extension; blkcol <= j+extension; blkcol++)
             {
               
                 int blkpixel = (blkrow*3*newwidth) + 3*blkcol;
                 /*Red channel*/
                 weightR[indexX][indexY] = exp((-1*pow((double)(newdata[imgpixel] - 
                                               newdata[blkpixel]),(double)2))/(double)(2*sigmaR*sigmaR));
                 /*Green channel*/
                 weightG[indexX][indexY] = exp((-1*pow((double)(newdata[imgpixel+1] -
                                               newdata[blkpixel+1]),(double)2))/(double)(2*sigmaR*sigmaR));
                 /*Blue channel*/
                 weightB[indexX][indexY] = exp((-1*pow((double)(newdata[imgpixel+2] -
                                               newdata[blkpixel+2]),(double)2))/(double)(2*sigmaR*sigmaR));
                 indexY++;
             }
             indexX++;
             indexY = 0;
          }

         // sum(Ip * G(s) * G(r)) & sum of normalization factor
          X=0;Y=0;
          for (int blkrow = i-extension; blkrow <= i+extension; blkrow++)
          {
             for (int blkcol = j-extension; blkcol <= j+extension; blkcol++)
             {
                 int blkpixel = (blkrow*3*newwidth) + 3*blkcol;
                 sumBFR += (newdata[blkpixel]*weightR[X][Y]*weightS[X][Y]);
                 sumBFG += (newdata[blkpixel+1]*weightG[X][Y]*weightS[X][Y]);
                 sumBFB += (newdata[blkpixel+2]*weightB[X][Y]*weightS[X][Y]);

                 WpR += (weightR[X][Y]*weightS[X][Y]);
                 WpG += (weightG[X][Y]*weightS[X][Y]);
                 WpB += (weightB[X][Y]*weightS[X][Y]);
      
                 Y++;
             }
             X++;
             Y=0;
          }
       
        double bF[3];
        bF[0] = sumBFR/WpR;        
        bF[1] = sumBFG/WpG;        
        bF[2] = sumBFB/WpB;        

        for (int iVal=0; iVal<3; iVal++)
        {
            if (bF[iVal] > 255)
               bF[iVal] = 255;
            if (bF[iVal] < 0)
               bF[iVal] = 0;
        }

        modidata[imgpixel] = bF[0];
        modidata[imgpixel+1] = bF[1];
        modidata[imgpixel+2] = bF[2];

      } //end for lopp width
   } //end for loop height


   for (int i = extension; i < height+extension; i++)
   {
      for (int j = extension; j < width+extension; j++)
      {
         int pixel = 3*(i-extension)*width + 3*(j-extension);
         int pixelres = 3*i*newwidth +(3*j);
         data[pixel] =  modidata[pixelres];
         data[pixel+1] =  modidata[pixelres+1];
         data[pixel+2] =  modidata[pixelres+2];
      }   
   }

#if 0
   /*Clean up memory*/
   for (int iWin=0; iWin<winSize; iWin++)
   {
      delete [] weightS[iWin];
      delete [] weightR[iWin];
      delete [] weightG[iWin];
      delete [] weightG[iWin];
   }
   delete []weightS; 
   delete []weightR; 
   delete []weightG; 
   delete []weightB; 
#endif
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
   double sigmaS;
   double sigmaR;
   unsigned char* outputImage;

   if (argc < 6)
   {
      cout << "Error - Incorrect Parameter Usage:" << endl;
      cout << "program_name input_image.raw output_image.raw";
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
     cout << "Problem 3a-Noise removal bilateral filtering" <<endl;
     outImage = inImage;
     //outImage.setNumbytes(3); //RGB
     
     cout << "Input sigmaS" <<endl;
     cin >> sigmaS;
     cout << "Input sigmaR" <<endl;
     cin >> sigmaR;
     outputImage = outImage.bilateral_filtering(sigmaS,sigmaR);

     if (outputImage == NULL)
     //if (!outImage.bilateral_filtering(sigmaS,sigmaR))
     {
         cout << "Error:Could not perform bilateral filtering"<<endl;
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
