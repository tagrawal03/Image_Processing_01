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


/*Code to implement oil painting with 64 colors per pixel*/
bool Image::oil_painting()
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
   int N; 
   cout << "Enter the neighbourhood value N"<<endl;
   cin >> N;

   if (N < 0 || N >width || N >height || (N%2 ==0))
   {
      cout << "Error: Enter correct value of N--odd, postive";
      return false;
   }

   /*Extend image by one row/column pixel from all sides*/
   int extension = floor(N/2); //number of rows/columns to extend
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


   /*Oil painting Effect*/
   unsigned char* modidata;   
   modidata = new unsigned char[newwidth*newheight*3]; //to store expanded input image with first two rows/column reflected
   /*Store the RGB value together in one pixel 8*3=24bit
    first 8 bits are R , second 8 bits G, last 8 bits as blue*/
   int* combImage; 
   combImage = new int[newwidth*newheight]; //to store RGB value combined into one pixel
   
   for (int i =0; i < newheight; i++)
   {
      for (int j =0; j < newwidth; j++)
      {
         int pixVal = (i*(newwidth*3)) + (j*3);
         combImage[(i*newwidth) +j] = (int)newdata[pixVal+2];
         combImage[(i*newwidth) +j] = combImage[(i*newwidth) +j] <<8;
         combImage[(i*newwidth) +j] += (int)newdata[pixVal+1];
         combImage[(i*newwidth) +j] = combImage[(i*newwidth) +j] <<8;
         combImage[(i*newwidth) +j] += (int)newdata[pixVal];
      }
   }

   for (int i = extension; i < height+extension; i++)
   {
      for (int j = extension; j < width+extension; j++)
      {
         int pixelcount[N][N];    
         int c=0, d=0, m=0, n=0;
         int blkrow, blkcol;
         for (int m = 0; m <N; m++)
         {
            for (int n =0; n < N; n++)
            {
                pixelcount[m][n] = 0;
            }
         }
         for (blkrow = i-extension; blkrow <= i+extension; blkrow++)
         {
             for (blkcol = j-extension; blkcol <= j+extension; blkcol++)
             {
                 int blkpixel = (blkrow*newwidth) + blkcol;
                 for (m = 0; m <N; m++)
                 {
                    for (n =0; n < N; n++)
                    {
                       int X = i-extension + m;
                       int Y = j-extension + n;
                       int combpix = X*newwidth +Y;
                       if (combImage[blkpixel] == combImage[combpix])
                          pixelcount[c][d]++;  //0-8
                    }
                 }
                 d++;
             } //end for loop blk width
             c++;
             d=0;
         }  //end for loop blk height
   
          /*Find bin which has maximum number of pixels*/
           int max = pixelcount[0][0];
           int indexX = 0;
           int indexY = 0;
           for (int iY=0; iY <N; iY++)
           {
              for (int iX=0; iX <N; iX++)
              {
                 if (pixelcount[iY][iX] > max)
                 {
                   max = pixelcount[iY][iX];
                   indexY = iY;      //stores value that occurs maximum in N*N neighbourhood 
                   indexX = iX;
                 } 
              }
           }

          int parseY = blkrow + indexY-N; 
          int parseX = blkcol + indexX-N;
          int parsePix = parseY*newwidth*3 + (3*parseX); 
          /*Fill the final color*/
           modidata[(3*i*newwidth) + 3*j] =  newdata[parsePix];
           modidata[(3*i*newwidth) +(3*j) +1] =  newdata[parsePix + 1];
           modidata[(3*i*newwidth)+ (3*j) +2] =  newdata[parsePix +2];      
   
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
   delete combImage;
   return true;
}



int main(int argc, char* argv[])
{

   int BytesPerPixel = 1;
   int Width = 424;
   int Height = 636;
   int outWidth, outHeight;
   int ExtensionVal;
 
   if (argc < 5)
   {
      cout << "Error - Incorrect Parameter Usage:" << endl;
      cout << "program_name input_image.raw output_image.raw BytesPerPixel width height" << endl;
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
     cout << "Oil painting- problem 2" <<endl;
     outImage = inImage;
#if 0
     /*Quantize the image with 64 colors*/
     if (!outImage.Quantization())
     {
         cout << "Error:Could not Quantize Image"<<endl;
     }

     /*Image extension*/
     cout << "Enter the image extension value" <<endl;
     cin >> ExtensionVal;
     if (!outImage.image_extension(ExtensionVal))
     {
         cout << "Error:Could not Extend Image"<<endl;
     }
#endif

     if (!outImage.oil_painting())
     {
         cout << "Error:Could not Quantize Image"<<endl;
     }

   }
  // outImage.setWidth(Width+4);
   //outImage.setHeight(Height+4);
   //outImage.setNumbytes(3); //RGB
   //write final image to file
   if (!outImage.writeImage(argv[2]))
   {
      cout <<"Could not Write Image"<< endl;
   }

  return 0;
}
