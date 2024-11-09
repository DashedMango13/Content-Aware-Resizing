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
1. Download "caResize-standalone.zip".
2. Extract "caResize.exe".
3. Add the file path containing "caResize.exe" to the system environment PATH variables.
4. Navigate to the folder containing the images you would like to modify in File Explorer.
5. Type 'cmd' into to directory and press eneter to launch the command propmpt from that path.
6. .  Run "caResize.exe" with "-h" to see the options. You can copy-paste the line below into command prompt.
   ```bash
    caResize -h
7. Run "caResize.exe" and pass your image as an argument with verbose,"-v", to understand the images original properties. '-i' does not need to be written to input the image, it is default.
   ```bash
   caResize inputImage.png -v

<p align="center">
  <table>
    <tr>
      <th>Input Image</th>
      <th>Verbose Info</th>
    </tr>
    <tr>
      <td><img src="https://github.com/user-attachments/assets/d9b45ae2-6989-4997-ac5b-f59bfcd10727" height="250"></td>
      <td><img src="https://github.com/user-attachments/assets/b0428a56-b10a-45e1-b079-705f4fed88e8" height="250"></td>
    </tr>
  </table>
</p>

8. Use '-x' and '-y' to achieve a target width and height for your image.
   ```bash
   caResize inputImage.png -x 750 -y 250
9. Use '-o' to save your output.
    ```bash
   caResize inputImage.png -x 750 -y 250 -o ultrawide.png
<p align="center">
  <table>
    <tr>
      <th>Input Image (Ultra-Wide)</th>
    </tr>
    <tr>
      <td><img src="https://github.com/user-attachments/assets/b7722594-ce54-4032-abac-2e852d5524d0" height="250"></td>
    </tr>
  </table>
</p>
![erased](https://github.com/user-attachments/assets/c1b6aa7c-ab57-4f83-bd87-9a2db24fd037)



9. Use '-r' to remove a part of the image and maintain original size. Can use '-R' instead of '-r' to ignore original size.
   ```bash
   caResize inputImage.png -r

<p align="center">
  <table>
    <tr>
      <th>Input Image </th>
      <th>Removal Selection</th>
      <th>Output Image</th>
    </tr>
    <tr>
      <td><img src="https://github.com/user-attachments/assets/29cacbc9-751e-4631-baa3-9cd86bb2cf9d" height="200"></td>
      <td><img src="https://github.com/user-attachments/assets/c85e69ef-2436-414e-8d5b-75bccf593827" height="200"></td>
      <td><img src="https://github.com/user-attachments/assets/c1b6aa7c-ab57-4f83-bd87-9a2db24fd037" height="200"></td>
    </tr>
  </table>
</p>

11. Use '-k' sparingly to outline an area of the image that should remain unchanged when necessary.
    ```bash
    caResize inputImage.png -x 400 -k -o output.jpg

<p align="center">
  <table>
    <tr>
      <th>Input Image </th>
      <th>Shrink (Without Keep)</th>
    </tr>
    <tr>
      <td><img src="https://github.com/user-attachments/assets/e7dd8bf7-0794-4568-924e-42ea9e965b87" height="370"></td>
      <td><img src="https://github.com/user-attachments/assets/bc3c4d26-d96e-4715-8428-e2dbae7d697c" height="370"></td>
    </tr>
  </table>
</p>

<p align="center">
  <table>
    <tr>
      <th>Keep </th>
      <th>Shrink (With Keep Selection)</th>
    </tr>
    <tr>
      <td><img src="https://github.com/user-attachments/assets/4da30257-c9d2-4d74-b7a7-0a2044319961" height="350"></td>
      <td><img src="https://github.com/user-attachments/assets/0cb136cf-dc7d-4c04-8636-b4d7e91ef92c" height="350"></td>
    </tr>
  </table>
</p>\
