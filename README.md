# Pacman Animation with OpenGL C++  (2013)

My final project for CSC431 Computer Graphics.

![Demo Snapshot](https://thetruecaptian.github.io/thumbnails/pacman-anim3.png "Demo Snapshot")

## Setup for Developing in OpenGL and Running Code on Windows

Taken from Chansophea Chuon. Edited by Hanafi to match development setup (Windows 10).

### 1. Install GLUT

1. Download and unzip glut-3.7.6-bin.zip from <http://www.xmission.com/~nate/glut.html>.

2. Copy `glut32.dll` to `C:\Windows\System32`.

3. Copy `glut32.lib` to `C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib`

4. Copy `glut.h` to `C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL`. Note that you may have to create the GL directory.

### 2. Install GLEW

1. Download `glext.h` from <http://www.opengl.org/registry/api/glext.h> to `C:\Program Files (x86)\Microsoft Visual Studio
10.0\VC\include\GL`.

2. Download and unzip the file `glew-1.5.4-win32.zip` from <http://glew.sourceforge.net/>.

3. Copy `glew32.dll` from the bin directory to `C:\Windows\System32`.

4. Copy `glew32.lib` and `glew32s.lib` from the lib directory to `C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib`

5. Copy `glew.h`, `glxew.h` and `wglew.h` from the `include\GL directory` to `C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\GL`.

### 3. Running OpenGL code
Run by loading project into Visual C++ 2010. Execute code by builing the solution and then debugging it. 



