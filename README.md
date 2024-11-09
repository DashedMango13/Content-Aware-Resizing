# caResize
A Content Aware Resizing Algorithm By David Oluwagbemi, March 2023

Based On:
Seam Carving for Content-Aware Image Resizing
ACM Transactions on Graphics, Vol. 26, No. 3, Article 10, Publication date: July 2007.

External Libraries For Compiling:
Boost 1_86, 
OpenCV 4.10.0

# Command-Line Options

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

# Application Usage (Windows)
No dependencies are required to run the fully functional standalone executable
1. Download caResize-standalone.zip.
2. Extract caResize-standalone.exe.
3. Add the file path containing "caResize-standalone.exe" to the system environment PATH variables.
4. Navigate to the folder containing the images you would like to modify in File Explorer.
5. Type 'cmd' into to directory and press eneter to launch the command propmpt from that path.
6. .  Run "caResize-standalone" "-h" to see options.
   ```bash
    caResize-standalone -h
7. Run "caResize-standalone" and pass your image as an argument with "-v" to understand the images original properties. '-i' does not need to be written to input the image, it is default.
   ```bash
   caResize-standalone inputImage.png -v
8. Use '-x' and '-y' to achieve a target width and height for your image.
   ```bash
   caResize-standalone inputImage.png -x 750 -y 250
9. Use '-o' to save your output.
    ```bash
   caResize-standalone inputImage.png -x 750 -y 250 -o ultrawide.png
9. Use '-r' to remove a part of the image and maintain original size. Can use '-R' instead of '-r' to ignore original size.
   ```bash
   caResize-standalone inputImage.png -r
11. Use '-k' sparingly to outline an area of the image that should remain unchanged when necessary.
  ```bash
   caResize-standalone inputImage.png -x 750 -y 250 -o ultrawide.png
