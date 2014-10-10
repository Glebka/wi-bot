#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 777

int open_port(char str[])
{
    int fd = open(str, O_RDWR | O_NOCTTY | O_NONBLOCK); // ?? NDELAY or NONBLOCK?

    if (fd == -1)
    {
        perror("open_port: Unable to open /dev/ttyS0 - ");
    }
    else
        fcntl(fd, F_SETFL, 0);

    struct termios options;
    tcgetattr(fd, &options); //this gets the current options set for the port

    // setting the options

    cfsetispeed(&options, B115200); //input baudrate
    cfsetospeed(&options, B115200); // output baudrate
    options.c_cflag |= (CLOCAL | CREAD); // ?? enable receicer and set local mode
    //options.c_cflag &= ~CSIZE; /* mask the character size bits */
    options.c_cflag |= CS8;    /* select 8 data bits */
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // choosing raw input
    options.c_iflag &= ~INPCK; // disable parity check
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // disable software flow control
    options.c_oflag |= OPOST; // ?? choosing processed output
    options.c_cc[VMIN] = 0; // Wait until x bytes read (blocks!)
    options.c_cc[VTIME] = 0; // Wait x * 0.1s for input (unblocks!)

    // settings for no parity bit
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    tcsetattr(fd, TCSANOW, &options); //set the new options ... TCSANOW specifies all option changes to occur immediately

    return (fd);
}

int main()
{


    int sockfd, newsockfd;
    unsigned char command = 0;
    struct sockaddr_in serv_addr, cli_addr;

    int writeport = open_port("/dev/ttyATH0");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket\n");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR on binding\n");

    listen(sockfd, 5);

    size_t clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);

    if (newsockfd < 0)
        error("ERROR on accept\n");

    while(command != 'X')
    {
        read(newsockfd,&command,1);
        int n = write(writeport, &command, 1);
        if (n < 0)
        {
            fputs("write() of bytes failed!\n", stderr);
            exit(-1);
        }
    }

    //closing ports
    close(writeport);
    exit(0);
}

