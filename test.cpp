#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>
#include <sfeMovie/Movie.hpp>
#include <iostream>
#include <algorithm>
#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <vector>
#include <termios.h> /* POSIX terminal control definitions */
#include "AnimatedGIF.h"
using namespace std;

typedef struct Data
{
	string player;
	string score;
	string video;
	string animation;
	//text ?
} Data;


Data serialData;

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

//user vars
int score=0;

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

const vector<string> explode(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;
	
	for(auto n:s)
	{
		if(n != c) buff+=n; else
		if(n == c && buff != "") { v.push_back(buff); buff = ""; }
	}
	if(buff != "") v.push_back(buff);
	
	return v;
}


void parseSerialData(char *buffData)
{
	vector<string> v{explode(buffData, ':')};
	
	for (int i=0; i<v.size(); i++)
	{
		
		if (strcmp(v[i].c_str(),"player")==0)
		{
			//player
			i++;
	
			serialData.player=v[i];
			//cout << "Player " << serialData.player << endl;		
		}

		if (strcmp(v[i].c_str(),"score")==0)
		{
			//score
			i++;
	
			serialData.score=v[i];
			//cout << "Score " << serialData.score << endl;		
		}

		if (strcmp(v[i].c_str(),"video")==0)
		{
			//video
			i++;
	
			serialData.video=v[i];
			//cout << "Video " << serialData.video << endl;		
		}

		if (strcmp(v[i].c_str(),"animation")==0)
		{
			//animation
			i++;
	
			serialData.animation=v[i];
			//cout << "Animation " << serialData.animation << endl;		
		}
	}
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

void updateScore(string val)
{
	text.setString(val);
	//std::cout << "space" << std::endl;
}



void initFont()
{

	//load font
	if (!font.loadFromFile("homer.ttf"))
	{
		// error...
	}

	// select the font
	text.setFont(font); // font is a sf::Font

	// set the string to display
	

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::White);

	// set the text style
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);

}

void playAnim(string num)
{

	//play animation num 01 bumpers

}

int main()
{
	
	//init serial
	openPort();
	
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "The Simpsons !", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
	
	
    //open GIT
    AnimatedGIF gif("pics/pp.gif");
    sf::Vector2i size = gif.getSize();
    sf::Sprite spriteGIF;

	initFont();	


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

			parseSerialData(buff);
			memcpy(buff, "", sizeof(buff[0])*1024);
			buffFull=0;
		}

		//on traite les info
		//update player
		//updatePlayer(serialData.player);

		//update ball
		//updateBall(serial.Databall);

		//update score
		updateScore(serialData.score);

		//anim ?
		if (strcmp(serialData.animation.c_str(),"00")!=0)
		{
			//on joue l'annimation
		}

		//video ?
		if (strcmp(serialData.video.c_str(),"00")!=0)
		{
			//on joue la video
		}

		//ajout texte ? minuterie ? lock ? video mode ? autre info ?
		
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					exit(0);
					break;
				}
				
				if (event.key.code == sf::Keyboard::Space)
				{
					std::cout << "space" << std::endl;
					if (movie.getStatus() == sfe::Playing)
					{
						movie.pause();
					}
                    else
                    {
						displayTv=true;
							//960/720
						movie.fit(vidx, vidy, vidw, vidh);
                        movie.play();
                    }
                    break;
					
				}
				if (event.key.code == sf::Keyboard::S)
				{
					std::cout << "SSS" << std::endl;
					updateScore("1000");
					break;
				}
				if (event.key.code == sf::Keyboard::B)
				{
					//play anim=true
					playAnim("01");
					
					break;
				}
			}
		}


		movie.update();
		
		gif.update(spriteGIF);
		window.draw(spriteGIF);
		
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
