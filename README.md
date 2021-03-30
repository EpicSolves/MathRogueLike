# MathRogueLike

# Get SDL2 Development Library
https://www.libsdl.org/download-2.0.php (Under "Development Libraries", get SDL2-devel-2.0.14-VC.zip for Visual C++)

# Get SDL2 image library
https://www.libsdl.org/projects/SDL_image/ (Under "Development Libraries", get SDL2_image-devel-2.0.5-VC.zip for Visual C++)

# Get SDL2 mixer library
https://www.libsdl.org/projects/SDL_mixer/ (Under "Development Libraries", get SDL2_mixer-devel-2.0.4-VC.zip for Visual C++)

# Get True Text Font library
https://www.libsdl.org/projects/SDL_ttf/ (Under "Development Libraries", get SDL2_ttf-devel-2.0.15-VC.zip for Visual C++)

# Put them in the right spot
For each library, you will get a directory structure as follows:
- Library Name/  
 -- include/  
 --- a bunch of header files  
 --lib/  
 --x64/  
 ---one or more DLLs  
 --x86/  
 ---one or more DLLs  
      
 - Put the 4 library directories at the same level as your outermost git repo directory as follows:  
 -- git repo name/  
 -- SDL2/  
 -- SDL2_image-2.0.5/  
 -- SDL2_mixer-2.0.4/  
 -- SDL2_ttf-2.0.15/  
