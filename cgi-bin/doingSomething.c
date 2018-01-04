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
    char gmtTime[5];
    for(int i = 0; i < 5; i++){
      if (i == 0 && buff[45] == '\"'){
	gmtTime[i] = '0';
      }
      else{
	gmtTime[i] = buff[45+i];
      }
    
    }

    sprintf(content, "%s%s", content, gmtTime);
    //reach out to api and get time back

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
