/**Main.cpp

Input arguments program.cpp inputImage.raw outputImage.raw bytesperpixel width height
Author : Taruna Agrawal
Date: 20th september
email:tagrawal@usc.edu
ID: 7650184685
*/

#include "image_proc.cpp"


/* Function: Implements histogram Equalization using transfer function technique*/
bool Image::hist_eqTF()
{
   double hist_R[256] = {0} ,hist_G[256] = {0},hist_B[256] = {0};
   double modihist_R[256] = {0} ,modihist_G[256] = {0},modihist_B[256] = {0};
   double cumR = 0, cumG = 0, cumB = 0;
   double cd_histR[256] = {0},cd_histG[256] = {0},cd_histB[256] = {0};
   
   ofstream histR,histG,histB;
   ofstream tf_histR, tf_histG, tf_histB;
   ofstream modi_histR,modi_histG,modi_histB;

   histR.open("hist_R.txt");
   histG.open("hist_G.txt");
   histB.open("hist_B.txt");
   tf_histR.open("tf_histR.txt");
   tf_histG.open("tf_histG.txt");
   tf_histB.open("tf_histB.txt");
   modi_histR.open("modi_histR.txt");
   modi_histG.open("modi_histG.txt");
   modi_histB.open("modi_histB.txt");

   unsigned char* temp; 
   temp = new unsigned char[width*height*3];  //to store final equalized image

   /*Count number of pixels at each intensity location*/
   for (int i = 0; i <height*width; i++)
   {
      hist_R[data[3*i]]++;
      hist_G[data[3*i+1]]++;
      hist_B[data[3*i+2]]++;
   }
   /*Calculate Cumulative distribution function -Transfer function*/
   for (int i = 0; i < 256; i++)
   {
      cumR = cumR + hist_R[i];
      cumG = cumG + hist_G[i];
      cumB = cumB + hist_B[i];

      cd_histR[i] = cumR/(width*height);
      cd_histG[i] = cumG/(width*height);
      cd_histB[i] = cumB/(width*height);
   }

   /*g = 255 =  ((Gmax-Gmin)cd_histX[]) + Gmin*/
   for (int i = 0; i < width*height; i++)
   {
       temp[3*i] = cd_histR[data[3*i]]*255;
       temp[3*i +1] = cd_histG[data[3*i+1]]*255;
       temp[3*i +2] = cd_histB[data[3*i+2]]*255;

   }
   /*Write the histogram and transfer function to file*/
   for (int i = 0; i <256; i++)
   {
        histR << hist_R[i];
        histR <<endl;
        histG << hist_G[i];
        histG << endl;
        histB << hist_B[i];
        histB << endl;
        tf_histR << cd_histR[i];
        tf_histR << endl;
        tf_histG << cd_histG[i];
        tf_histG << endl;
        tf_histB << cd_histB[i];
        tf_histB <<endl;
   }

   data = new unsigned char[width*height*3];
   for (int i =0; i < height*width; i++)
   {
      data[3*i] =  temp[3*i];
      data[3*i+1] =  temp[3*i+1];
      data[3*i+2] =  temp[3*i+2];      
   }

   for (int i = 0; i <height*width; i++)
   {
      modihist_R[data[3*i]]++;
      modihist_G[data[3*i+1]]++;
      modihist_B[data[3*i+2]]++;
   }
   
    for (int i = 0; i <256; i++)
   {
      modi_histR << modihist_R[i] << '\n';
      modi_histG << modihist_G[i] << '\n';
      modi_histB << modihist_B[i] << '\n';
   }

   histR.close();
   histG.close();
   histB.close();
   tf_histR.close();
   tf_histG.close();
   tf_histB.close();
   modi_histR.close();
   modi_histG.close();
   modi_histB.close();

   return true;
}



//Global variables
Image inImage, outImage;

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
     cout << "Transfer function-Histogram equalization" <<endl;
     outImage = inImage;
     //outImage.setNumbytes(3); //RGB

     if (!outImage.hist_eqTF())
     {
         cout << "Error:Failed to do histogram equalization"<<endl;
     }
   }
   //write final image to file
   if (!outImage.writeImage(argv[2]))
   {
      cout <<"Could not Write Image"<< endl;
   }

  return 0;
}
