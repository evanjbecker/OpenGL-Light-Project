# OpenGL 3D Lighting Project
This lighting project involves point lights in a 3D OpenGL environment written in C++ (using Visual Studio 2010). It involves loading a .off file polygon and coloring it, transformations on said polygon (rotation, scale, translation), translation and animation of 2 light sources, and different material properties/light diffuse colors inside the created shader. 
This was my final project for CS-459 at UWM. 

## Getting Started

There are 3 ways to view the project. The quickest way to view the project would be to pull the project and run the executable application file within /Release. You must ensure that sample_polygon.off is in the folder. The quickest way to work with the source code would be to to pull the entire repository and open the project in Microsoft Studio 2010 or greater and installing the OpenGL libraries given in the folder structure. Additionally, you could simply edit the .cpp file located at FinalProject/EvanBecker_Final.cpp.

### Prerequisites

You will need OpenGL downloaded on your system along with its associated dll's attached in the project. Downloading the project file in a whole should include it. When running the executable, this is not necessary.

## Deployment

Use the Release/FinalProject.exe for the most up to date version. Additionally, you could run the Debug version given as well.

## Built With

* [OpenGL](https://www.opengl.org/) - C++ Graphics Engine

## Author

* **Evan Becker** - [Website](http://www.evanbecker.com/)

## Screenshots

Screenshot 1 - This GIF is an example of Light 1 rotating about the object (Using a sine and cosine function based on the current step/frame).
![Light Animation 1](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/Light1Anim.gif?raw=true)

Screenshot 2 - This GIF is an example of Light 2 being animated up and down (Using a sine and cosine function based on the current step/frame).
![Light Animation 2](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/Light2Anim.gif?raw=true)

Screenshot 3 - This is an example of the console while loading the .off file.
![Console](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/console.png?raw=true)

Screenshot 4 - This is an image of the shape drawing only its edge lines (as opposed to filled faces).
![Line](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/line.png?raw=true)

Screenshot 5 - This is an image of the shape drawing only its point vertices (as opposed to filled faces).
![Point](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/point.png?raw=true)

Screenshot 6 - An example of Rotation.
![Rotation](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/rotation.png?raw=true)

Screenshot 7 - An example of Translation.
![Rotation](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/translation.png?raw=true)

Screenshot 8 - An example of Scaling.
![Rotation](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/scalein.png?raw=true)

Screenshot 9 - An example of using both red lights and blue lights. The right is using the blue light, which when diffused on a pure red surface provides no actual light, whereas the left light does. This is a good base to compare with screenshots 10-12.
![Blue And Red Light](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/blueredlight-redpoly.png?raw=true)

Screenshot 10 - An example of using both red lights and blue lights. The left is using the red light, which when diffused on a cyan surface (Blue and Green) surface provides no actual light, whereas the right light does.
![Blue And Red Light 2](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/blue_n_red_light-poly_red-ambdif_cyan.png?raw=true)

Screenshot 11 - An example of using both red lights and blue lights. Since this is on a Purple (Blue and Red) surface, it diffuses both colored lights properly.
![Blue And Red Light 3](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/blue_n_red_light-poly_red-ambdif_purple.png)

Screenshot 12 - An example of using both red lights and blue lights. Since this is on a Green emission surface, it diffuses both lights as mixtures of the colors, which is why you see both Yellow and Cyan.
![Blue And Red Light 4](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/blue_n_red_light-poly_red-ambdif_purple-emiss_green.png)

Screenshot 13 - An example of using just white lights on randomly colored surface polygons. This is a good base to compare with screenshots 14-17.
![White and Red Light](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/white_light-poly_random.png?raw=true)

Screenshot 14 - An example of using a red light and a white light on randomly colored surface polygons. This makes it much easier to see the red point light on the object.
![White and Red Light](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/white_n_red_light-poly_random.png?raw=true)

Screenshot 15 - An example of using a blue light and a white light on randomly colored surface polygons.
![White and Blue Light](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/white_n_blue_light-poly_random.png?raw=true)

Screenshot 16 - An example of using both a red light and a blue light on randomly colored surface polygons.
![Red and Blue Light](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/red_n_blue_light-poly_random.png?raw=true)

Screenshot 17 - An example of using both a red light and a blue light on randomly colored surface polygons with green emission. Note the difference between screenshot 17 and 12.
![Red and Blue Light](https://github.com/beckerej/OpenGL-Light-Project/blob/master/Screenshots/red_n_blue_light-poly_random-emiss_green.png?raw=true)




