#EE569 Homework Assignment #1
#Date: September 20th
#Name: Taruna Agrawal
#Email: tagrawal@usc.edu

#Compiled on UBUNTU 14.04 with g++
#g++ version: g++ (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4


all : prob1a prob1b prob2a prob2b prob3a prob3b
prob1a:
	@echo " "
	@echo "Problem 1a of Resizing"
	@echo "Compiling Problem1_Resize.cpp"
	@echo " "
	g++ -o resize Problem1_Resize.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height"
	@echo " "
	./resize the_starry_night.raw starryNight_Resizeoutput.raw 3 512 512
	@echo "Output Image is starryNight_Resizeoutput.raw"

prob1b:
	@echo " "
	@echo "Problem 1b of Linear Demosaicing"
	@echo "Compiling Problem1_DemosaicLinear.cpp"
	@echo " "
	g++ -o linearDemosaic Problem1_DemosaicLinear.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height"
	@echo " "
	./linearDemosaic parrot_CFA.raw parrot_linDemoutput.raw 1 424 636
	@echo "Output Image is parrot_linDemoutput.raw"
	@echo " "
	@echo "Problem 1b of MHP Demosaicing"
	@echo "Compiling Problem1_DemosaicMHP.cpp"
	@echo " "
	g++ -o mhpDemosaic Problem1_DemosaicMHP.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height"
	@echo " "
	./mhpDemosaic parrot_CFA.raw parrot_mhpDemoutput.raw 1 424 636
	@echo "Output Image is parrot_mhpDemoutput.raw"

prob2a:
	@echo " "
	@echo "Problem 2a of histogram Equalization"
	@echo "Compiling Problem2_histEqTF.cpp"
	@echo " "
	g++ -o histEqtf Problem2_histEqTF.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height"
	@echo " "
	./histEqtf jet.raw jet_tfoutput.raw 3 512 512
	@echo "Output Image is jet_tfoutput.raw and histogram text files are generated"
	@echo " "
	@echo "Compiling Problem2_histEqCPH.cpp"
	@echo " "
	g++ -o histEqCPH Problem2_histEqCPH.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height"
	@echo " "
	./histEqCPH jet.raw jet_CPHoutput.raw 3 512 512
	@echo "Output Image is jet_CPHoutput.raw and histogram text files are generated"

prob2b:
	@echo " "
	@echo "Problem 2b of oil Painting"
	@echo "Compiling Problem2_oilpainting.cpp"
	@echo " "
	g++ -o oilpainting Problem2_oilpainting.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height"
	@echo " "
	./oilpainting barn.raw barn_oiloutput.raw 3 380 275
	./oilpainting coliseum.raw coliseum_oiloutput.raw 3 580 247
	@echo "Output Image is barn_oiloutput.raw and coliseum_oiloutput.raw"
	@echo "Compiling Problem2_oilpainting512color.cpp"
	@echo " "
	g++ -o oilpainting512 Problem2_oilpainting512color.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height"
	@echo " "
	./oilpainting512 barn.raw barn_oil512output.raw 3 380 275
	./oilpainting512 coliseum.raw coliseum_oil512output.raw 3 580 247
	@echo "Output Image is barn_oil512output.raw and coliseum_oil512output.raw with 512 colors"

prob3a:
	@echo " "
	@echo "Problem 3a of Mixed noise in color image"
	@echo "Compiling Problem3_MedianNoiseRem.cpp"
	@echo " "
	g++ -o medianfilter Problem3_MedianNoiseRem.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height original_image"
	@echo " "
	./medianfilter peppers_noisy.raw peppers_medianoutput.raw 3 512 512 peppers.raw
	@echo "Compiling Problem3_bilatFiltering.cpp"
	@echo " "
	g++ -o bilateralfilter Problem3_bilatFiltering.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height original_image"
	@echo " "
	./bilateralfilter peppers_medianoutput.raw peppers_bilateraloutput.raw 3 512 512 peppers.raw
	@echo "Median filtered Image is peppers_medianoutput.raw and final output image is peppers_bilateraloutput.raw"

prob3b:
	@echo " "
	@echo "Problem 3a of Mixed noise in color image"
	@echo "Compiling Problem3_MedianNoiseRem.cpp"
	@echo " "
	g++ -o medianfilter Problem3_MedianNoiseRem.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height original_image"
	@echo " "
	./medianfilter peppers_noisy.raw peppers_medianoutput.raw 3 512 512 peppers.raw
	@echo "Problem 3b of Guided image filtering in color image"
	@echo "Compiling Problem3_imageGuided.cpp"
	@echo " "
	g++ -o guidedfilter Problem3_imageGuided.cpp
	@echo "Usage: program_name input_image.raw output_image.raw BytesPerPixel width height original_image"
	@echo " "
	./guidedfilter peppers_medianoutput.raw peppers_guidedoutput.raw 3 512 512 peppers.raw
	@echo "Final output image is peppers_guidedoutput.raw"

clean :
	rm -rf resize starryNight_Resizeoutput.raw linearDemosaic parrot_linDemoutput.raw mhpDemosaic parrot_mhpDemoutput.raw histEqtf jet_tfoutput.raw histEqCPH jet_CPHoutput.raw oilpainting oilpainting512 barn_oiloutput.raw coliseum_oiloutput.raw barn_oil512output.raw coliseum_oil512output.raw medianfilter peppers_medianoutput.raw bilateralfilter peppers_bilateraloutput.raw guidedfilter peppers_guidedoutput.raw
