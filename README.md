# caResize
A Content Aware Resizing Algorithm By David Oluwagbemi, March 2023

Based On:
Seam Carving for Content-Aware Image Resizing
ACM Transactions on Graphics, Vol. 26, No. 3, Article 10, Publication date: July 2007.

External Libraries For Compiling:
Boost 1_86, 
OpenCV 4.10.0

# Run caResize.exe in Command Prompt "-h" to see options:

  -h [ --help ]                     display this message
  
  -v [ --verbose ]                  verbose [default is silent. can be used
                                    alone or with other options for picture
                                    info]
                                    
  -k [ --keep ]                     keep area unchanged when resizing [use with
                                    other options to influence outputs.
                                    Ex."caResize input.jpg -r -k"]
                                    
  -r [ --remove and resize ]        removes selection then returns image to
                                    original size [Use -R instead for no
                                    resizing after removal]
                                    
  -R [ --remove ]                   remove selection without maintaining image
                                    size
                                    
  -i [ --source-filename ] arg      source filename
  
  -o [ --destination-filename ] arg destination filename
  
  -x [ --tgt-width ] arg            target width, columns [can be used with k
                                    to change results]
                                    
  -y [ --tgt-height ] arg           target height [can be used with k to change
                                    results]

#Application Usage (Windows)
No dependencies are required to run the fully functional standalone executable
1. Download caResize-standalone.zip
2. Extract caResize-standalone.exe
3. Add the file path containing "caResize-standalone.exe" to the system environment PATH variables.
4. Navigate to the folder containing the images you would like to modify in File Explorer
5. Type 'cmd' into to directory and press eneter to launch the command propmpt from that path
7.  Run "caResize-standalone" "-h" to see options
   ```bash
    caResize-standalone -h.
7. Run "caResize-standalone" and pass your image as an argument with "-v" to understand the images original properties
   ```bash
   caResize-standalone inputImage.jpg -v
8. Use '-x' and '-y' to achieve a target size
