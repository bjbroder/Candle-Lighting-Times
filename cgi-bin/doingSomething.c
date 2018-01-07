/*
 * doingSomething.c - a minimal CGI program that does something; specifically, it takes a lat, long, and the date and returns the appropriate candle lighting time
 */

#include "csapp.h"

int main(void) {
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE], arg3[MAXLINE], content[MAXLINE];

    /* Extract the three arguments */
    if ((buf = getenv("QUERY_STRING")) != NULL) {
	p = strchr(buf, '&');
	*p = '\0';
	strcpy(arg1, buf);
	char *b = strchr(p+1, '&');
	*b = '\0';
	strcpy(arg2, p+1);
	strcpy(arg3, b+1);
    }
    int clientfd;
    char *host, *port;
    rio_t rio;
    char *command;
    char buff[MAXLINE];
    char buffer[MAXLINE];
    
    host = "api.sunrise-sunset.org";
    port = "80";
    sprintf(buffer, "GET https://api.sunrise-sunset.org/json?%s&%s&%s\n", arg1, arg2, arg3);
    
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    Rio_writen(clientfd, buffer, MAXLINE);
    Rio_readlineb(&rio, buff, MAXLINE);

    Close(clientfd);

    
    sprintf(content,"%s%s\n",content,buff);
    
    char sunset[MAXLINE];
    char *r = strchr(buff, ',');
    *r = '\0';
    strcpy(sunset, r+1);

    int countColons = 0;
    int i = 0;
    int h = 0;
    int m = 0;
    char gmtHour[3];
    char gmtMinute[3];

    while(countColons < 2){
      if (sunset[i+10] == ':')
	{
	  countColons++;
	  if (countColons == 2)
	    {
	      break;
	    }
	}
      else
	{
	  if (countColons == 0){
	    gmtHour[h] = sunset[10+i];
	    h++;
	  }
	  else{
	    gmtMinute[m] = sunset[10+i];
	    m++;
	  }
	}
      i++;
    }

    gmtHour[2] = '\0';
    gmtMinute[2] = '\0';

    sprintf(content, "%s%s\n", content,gmtHour);
    
    //reach out to api and get time back
    host = "api.timezonedb.com";
    port = "80";
    
    sprintf(buffer, "GET http://api.timezonedb.com/v2/get-time-zone?key=842Z7KTEY3QI&format=json&by=position&%s&%s\n",arg1,arg2);
     
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    Rio_writen(clientfd, buffer, MAXLINE);
    Rio_readlineb(&rio, buff, MAXLINE);

    Close(clientfd);

    
    int j = 95;
    int foundTime = 0;
    char zoneDiff[MAXLINE]; 
	
    if (buff[11] != 'O')
      {
       sprintf(content,"%sFailed to get candlighting time\n",content);
       //add in other print statements here and exit
      }
    else
      {
	while (foundTime == 0)
	  {
	    if (buff[j] == 'g' && buff[j+1] == 'm' && buff[j+2] == 't')
	      {
		int k = j+11;
		int l = 0;
		while (buff[k] != ',')
		  {
		    zoneDiff[l] = buff[k];
		    k++;
		    l++;
		  }
		foundTime = 1;
		l++;
		zoneDiff[l] = '\0';
	      }
	    else
	      j++;
	  }
      }

    int zone;
    sscanf(zoneDiff, "%d", &zone);
    zone = zone / 60 / 60;
    
    int ghour;
    sscanf(gmtHour, "%d", &ghour);

    int gminute;
    sscanf(gmtMinute, "%d", &gminute);

    if (ghour == 1) ghour = 13;
    int hour = ghour + zone;
    if (hour > 12) hour = 12 - hour;
    
    int min = 0;

    if(gminute >= 18) min = gminute - 18;
    else
      {
	int remainder = 18 - gminute;
	hour--;
	min = 60 - remainder;
      }


    /* Response body */
    /*            sprintf(content,"%s%d\n", content,hour);
    sprintf(content, "%szone = %d\r\n",content, zone);
    sprintf(content, "%szoneDiff = %s\r\n",content, zoneDiff);
    */ 
    
    sprintf(content, "<h1>Welcome to Brielle & Paulette's Webpage!<h1>");
    sprintf(content, "%sThe candlelighting time when %s\r\n",content,arg3);
    sprintf(content, "%sand location is %s, %s is\r\n<p>",content,arg1,arg2);
    sprintf(content, "%s%d:%02d\r\n<p>",content, hour, min);
    sprintf(content, "%sThanks for visiting!\r\n", content);
    
    /* Generate the HTTP response */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);

    exit(0);
}
/*end of doingSomething.c*/

