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

int main()
{
	
	//init serial
	//ajout texte ? minuterie ? lock ? video mode ? autre info ?
	char buff[1024]="player:1:score:2000:video:04:animation:00\n";


	parseSerialData(buff);

	cout << "test" << serialData.video << endl;

	  
	memcpy(buff, "", sizeof(buff[0])*1024);
	





    return 0;
}
