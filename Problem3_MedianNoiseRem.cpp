/**Main.cpp

Input arguments program.cpp inputImage.raw outputImage.raw bytesperpixel width height
Author : Taruna Agrawal
Date: 20th september
ID:7650184685
Email:tagrawal@usc.edu
*/

#include "image_proc.cpp"

//Global variables
Image inImage, outImage;


/*Problem 3a*/
unsigned char* Image::median_filtering()
{
  /*Plot histogram of images to check for noise type*/
   double hist_R[256] = {0} ,hist_G[256] = {0},hist_B[256] = {0};

   /*Count number of pixels at each intensity location*/
   for (int i = 0; i <height*width; i++)
   {
      hist_R[data[3*i]]++;
      hist_G[data[3*i+1]]++;
      hist_B[data[3*i+2]]++;
   }
   ofstream histR,histG,histB;

   histR.open("histProb3a_R.txt");
   histG.open("histProb3a_G.txt");
   histB.open("histProb3a_B.txt");
   for (int i = 0; i <256; i++)
   {
        histR << hist_R[i];
        histR <<endl;
        histG << hist_G[i];
        histG << endl;
        histB << hist_B[i];
        histB << endl;
   }
   histR.close();
   histG.close();
   histB.close();
   
   /*Impulse noise filtering- Median filtering*/
   int medianWin;
   cout << "Enter the window size for median filtering"<<endl;
   cin >> medianWin;

   
   if (medianWin < 0 || medianWin >width || medianWin >height || (medianWin % 2 ==0))
   {
      cout << "Error: Enter correct value of N--odd, postive";
      return NULL;
   }

   /*Extend image by extension row/column pixel from all sides*/
   int extension = floor(medianWin/2); //number of rows/columns to extend
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
   
   for (int i = extension; i < height+extension; i++)
   {
      for (int j = extension; j < width+extension; j++)
      {
         int pixelSortR[medianWin*medianWin];    
         int pixelSortG[medianWin*medianWin];    
         int pixelSortB[medianWin*medianWin];    
         int c=0,d=0, m=0, n=0;
         int blkrow, blkcol, temp=0;
         
         for (blkrow = i-extension; blkrow <= i+extension; blkrow++)
         {
             for (blkcol = j-extension; blkcol <= j+extension; blkcol++)
             {
                 int blkpixel = (blkrow*3*newwidth) + 3*blkcol;
                 pixelSortR[c] = newdata[blkpixel];
                 pixelSortG[c] = newdata[blkpixel +1];
                 pixelSortB[c] = newdata[blkpixel+2];
                 c++;
             } //end for loop blk width
         }  //end for loop blk height
         
         /*Block Inserton sort Red channel*/  
         for (int y = 0; y < (medianWin*medianWin); y++)
         {
             for (int z = 0; z< (medianWin*medianWin-y-1); z++)
             {
                 if (pixelSortR[z] > pixelSortR[z+1])
                 {
                    temp = pixelSortR[z]; 
                    pixelSortR[z] = pixelSortR[z+1];
                    pixelSortR[z+1] = temp;
                 }
             }
         }
         /*Block Sorting for Green channel*/  
         for (int y = 0; y < (medianWin*medianWin); y++)
         {
             for (int z = 0; z< (medianWin*medianWin-y-1); z++)
             {
                 if (pixelSortG[z] > pixelSortG[z+1])
                 {
                    temp = pixelSortG[z]; 
                    pixelSortG[z] = pixelSortG[z+1];
                    pixelSortG[z+1] = temp;
                 }
             }
         }


         /*Block Sorting for Blue channel*/  
         for (int y = 0; y < (medianWin*medianWin); y++)
         {
             for (int z = 0; z< (medianWin*medianWin-y-1); z++)
             {
                 if (pixelSortB[z] > pixelSortB[z+1])
                 {
                    temp = pixelSortB[z]; 
                    pixelSortB[z] = pixelSortB[z+1];
                    pixelSortB[z+1] = temp;
                 }
             }
         }

         int medianpos = round(medianWin*medianWin)/(float)2;

          /*Fill the final color*/
           modidata[(3*i*newwidth) + 3*j] =  pixelSortR[medianpos];
           modidata[(3*i*newwidth) +(3*j) +1] =  pixelSortG[medianpos];
           modidata[(3*i*newwidth)+ (3*j) +2] =  pixelSortB[medianpos];      
   
      }  //end for loop newwidth 
   
  } //end for loop new height

   
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
      cout << "Error - Incorrect Parameter Usage:" << endl;
      cout << "program_name input_image.raw output_image.raw BytesPerPixel width height original_image.raw" << endl;
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
     cout << "Median Noise removal" <<endl;
     outImage = inImage;
     //outImage.setNumbytes(3); //RGB
     outputImage = outImage.median_filtering();

     if (outputImage == NULL)
     {
         cout << "Error:Could not perform median filtering"<<endl;
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
