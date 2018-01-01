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
	strcpy(arg2, p+1);
    }
    int clientfd;
    char *host, *port;
    rio_t rio;
    char *command;
    char buff[MAXLINE];
    char buffer[MAXLINE];
    host = "api.sunrise-sunset.org";
    port = "80";
    sprintf(buffer, "GET https://api.sunrise-sunset.org/json?%s&%s\n", arg1, arg2);
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    Rio_writen(clientfd, buffer, MAXLINE);
    Rio_readlineb(&rio, buff, MAXLINE);
    sprintf(content, "%sConnected to api: %s", content, buff);

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
	    content, arg2);
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
