#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include <sfeMovie/Movie.hpp>
#include <iostream>
#include <algorithm>
#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */

#include <termios.h> /* POSIX terminal control definitions */


//https://github.com/wjwwood/serial
//(1080/2)-(805/2))
//(1920/2)-(805/2)
int tvx=557; //557,5
int tvy=137; //137.5


int vidw=540;
int vidh=vidw/(1920/1080)+10;

int vidx=(1920/2)-(vidw/2);
int vidy=((1080/2)-(vidh/2))+30;

//serial vars
int fd=-1;
char buff[1024]={0};
int buffFull=0;


bool displayTv=false;

sf::Font font;
sf::Text text;

//background
sf::Texture bgT;
sf::Sprite bgS;

//tv
sf::Texture tvT;
sf::Sprite tvS;

//video
std::string mediaFile="movies/1choc.mp4";
sfe::Movie movie;

void error_display()
{
}


void readData()
{
   char buf[1]={0};
		
   ssize_t res=read(fd, buf, 1);

   if(res!=0) {
   	strncat(buff,buf,1);      	
   }  

   if (buf[0] == '\n')
   {
   	buffFull=1;
   }
}

void openPort()
{
	struct termios options;

    fd = open("/dev/ttyACM0", O_RDWR | O_NDELAY | O_NOCTTY);
    if(fd<0)
    {
       printf("port not open");
       //return -1;
    }
    printf("aa");
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
    //Set the new options for the port...
	tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
	
}


int main()
{
	
	//init serial
		openPort();
	std::cout << "Port ok opened" << std::endl;
	
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "The Simpsons !", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
	
	const char *separators=":";



	//load font
	if (!font.loadFromFile("homer.ttf"))
	{
		// error...
	}

	// select the font
	text.setFont(font); // font is a sf::Font

	// set the string to display
	text.setString("Hello world");

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);





  if (!movie.openFromFile(mediaFile))
    {
        //error
		std::cout << "Error opening file " << mediaFile << std::endl;
        return 1;
    }

	std::cout << "file :" << mediaFile << std::endl;
	
    movie.pause();



	//bg
	if (!bgT.loadFromFile("pics/fond.png")) {
		perror("Couldn't load texture \"myTexture\".");
		//error
	}  
	bgS.setTexture(bgT);

	//tv
	if (!tvT.loadFromFile("pics/tv.png")) {
		perror("Couldn't load texture \"myTexture\".");
		//error
	}  
	tvS.setTexture(tvT);

    while (window.isOpen())
    {
        sf::Event event;
		
		window.clear();
		
		bgS.setPosition(0,0);
		window.draw(bgS);
		

		readData();
		//printf("%s",buff);

		if (buffFull==1)
		{
			char *ptr;  
    		ptr = strtok(buff, ":");  
    		// use while loop to check ptr is not null  
    		while (ptr != NULL)  
    		{  
        		if (strcmp(ptr,"video")!=0 && strcmp(ptr,"\n")!=0)
				{
					printf("play video number %s yy\n",ptr);
					if (movie.getStatus() == sfe::Playing)
				{
					movie.pause();
				}
                else
                {
					displayTv=true;
					movie.fit(vidx, vidy, vidw, vidh);
					movie.play();
				}
				}  
        		ptr = strtok (NULL, ":");  
    		}  
			memcpy(buff, "", sizeof(buff[0])*1024);
			buffFull=0;
		}
		/*
		
		char * strToken = strtok ( buff, separators );
		while ( strToken != NULL ) {
			//printf ( "token  %s\n", strToken );
			if (strcmp(strToken,"video")!=0)
			{
				printf("play video number %s\n",strtok(strToken, "\n"));
				if (movie.getStatus() == sfe::Playing)
				{
					movie.pause();
				}
                else
                {
					displayTv=true;
					movie.fit(vidx, vidy, vidw, vidh);
					movie.play();
				}
			}
			// On demande le token suivant.
			strToken = strtok ( NULL, separators );
			
		}
		memcpy(buff, "", sizeof(buff[0])*1024);

		*/
		
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					std::cout << "the escape key was pressed" << std::endl;
					std::cout << "control:" << event.key.control << std::endl;
					std::cout << "alt:" << event.key.alt << std::endl;
					std::cout << "shift:" << event.key.shift << std::endl;
					std::cout << "system:" << event.key.system << std::endl;
					exit(0);
					break;
				}
				
				if (event.key.code == sf::Keyboard::Space)
				{
					if (movie.getStatus() == sfe::Playing)
						movie.pause();
                    else
						displayTv=true;
					
					        //vid.draw(scrn, ((HALF_WIDTH-(VIDW/2), (HALF_HEIGHT-(VIDH/2))+36)), force_draw=False) 
							//scrn.blit(im_tv, (HALF_WIDTH-(tv_width/2), HALF_HEIGHT-(tv_height/2))) 
							//960/720
						movie.fit(vidx, vidy, vidw, vidh);
                        movie.play();
                    break;
					
				}
			}
		}


		movie.update();

		window.draw(movie);
		
		if (displayTv==true)
		{
			tvS.setPosition(tvx,tvy);
			window.draw(tvS);			
		}
		
		window.draw(text);

		window.display();
    }

    return 0;
}
