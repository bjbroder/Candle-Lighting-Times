/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int main(void) {
    char *buf, *p;
    char arg1[MAXLINE], arg2[MAXLINE], arg3[MAXLINE], content[MAXLINE];

    /* Extract the two arguments */
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
    char sunset[MAXLINE];
    char *r = strchr(buff, ',');
    *r = '\0';
    strcpy(sunset, r+1);

    int countColons = 0;
    int i = 0;
    char gmtTime[5];
    while(countColons < 2){
      if (sunset[i+10] == ':'){
	countColons++;
	if (countColons == 2){
	  break;
      }
      }
      gmtTime[i] = sunset[10+i];
      i++;
    }

    sprintf(content, "%s%s", content, gmtTime);
    //reach out to api and get time back

    host = "api.timezonedb.com";
    port = "80";

    sprintf(content,"%s%s\n",content,arg1);
    
    sprintf(buffer, "GET http://api.timezonedb.com/v2/get-time-zone?key=842Z7KTEY3QI&format=json&by=position&%s&%s\n",arg1,arg2);
     

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    Rio_writen(clientfd, buffer, MAXLINE);
    Rio_readlineb(&rio, buff, MAXLINE);
    
    //sprintf(content, "%sThis is the content from the api:\n%s", content, buff);
    // sprintf(content, "%sThis is char 11: %c\n", content, buff[11]);

    int j = 95;
    int foundTime = 0;
    char zoneDiff[MAXLINE]; 
	
    if (buff[11] != 'O')
       sprintf(content,"%sGetting Candlighting Time Failed\n",content);
    else
      {
	//	sprintf(content,"%sStarting to check letters from%c\n",content,buff[95]);
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
		sprintf(content,"%s%s\n",content,zoneDiff);
	      }
	    else
	      j++;
	  }
      }

    int zone;
    sscanf(zoneDiff, "%d", &zone);

    zone = zone / 60 / 60;

    sprintf(content,"%sZone is: %d\n",content,zone);
    
    //somehow split on colon of time
    //if (minutes > 17)  lightingTime = hours + ":" + str(minutes - 18);
    //else
       //remainder = 18 - minutes
       //lightingTime = str(hours-1) + ":" + (60-remainder)

    /* Make the response body */
    // sprintf(content, "Welcome to Brielle's add.com: ");
    //sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%sThe candlelighting time is: %s\r\n<p>", 
	    content, arg3);
    sprintf(content, "%sThanks for visiting!\r\n", content);
  
    /* Generate the HTTP response */
    printf("Connection: close\r\n");
    printf("Content-length: %d\r\n", (int)strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);

    exit(0);
}
/* $end adder */
