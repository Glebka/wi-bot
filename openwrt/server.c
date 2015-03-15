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
#include <signal.h>

#define PORT 777

int writeport = 0;

void sigPipeHandler()
{
    printf( "Caught SIGPIPE\n" );
}

struct Command
{
  uint16_t code;
  uint16_t hiParam;
  uint16_t loParam;
};

int open_port(char str[])
{
    printf("Opening serial port...\n");
    int fd = open(str, O_RDWR | O_NOCTTY ); // ?? NDELAY or NONBLOCK?

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
    //options.c_cflag |= (CLOCAL | CREAD); // ?? enable receicer and set local mode
    //options.c_cflag &= ~CSIZE; /* mask the character size bits */
    //options.c_cflag |= CS8;    /* select 8 data bits */
    //options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // choosing raw input
    //options.c_iflag &= ~INPCK; // disable parity check
    //options.c_iflag &= ~(IXON | IXOFF | IXANY); // disable software flow control
    //options.c_oflag |= OPOST; // ?? choosing processed output
    //options.c_cc[VMIN] = 0; // Wait until x bytes read (blocks!)
    //options.c_cc[VTIME] = 0; // Wait x * 0.1s for input (unblocks!)

    // settings for no parity bit
    //options.c_cflag &= ~PARENB;
    //options.c_cflag &= ~CSTOPB;
    //options.c_cflag &= ~CSIZE;
    //options.c_cflag |= CS8;
    cfmakeraw( &options );

    tcsetattr(fd, TCSANOW, &options); //set the new options ... TCSANOW specifies all option changes to occur immediately
    printf("Serial port ready.\n");
    return (fd);
}

//! Byte swap unsigned short
uint16_t swap_uint16( uint16_t val ) 
{
    return (val << 8) | (val >> 8 );
}
int writeCommand( int fd, struct Command* cmd )
{
    int writeSoFar = sizeof( struct Command );
    char* ptr = (char*) cmd;
    int written;
    int result = 0;
    while ( writeSoFar > 0 )
    {
        written = write( fd, ptr, writeSoFar );
        if ( written <=0 )
        {
            result = -1;
            break;
        }
        writeSoFar -= written;
        ptr += written;
    }
    return result;
}

int main()
{
    
    signal(SIGPIPE, sigPipeHandler);
    
    int sockfd, newsockfd;
    //unsigned char command = 0;
    struct Command cmd;
    
    struct sockaddr_in serv_addr, cli_addr;
    
    writeport = open_port( "/dev/ttyATH0" );
    
    /*unsigned char byte = 0;
    int counter = 0;
    while ( counter != 0x100 )
    {
        write( writeport, &byte, 1 );
        ++byte;
        ++counter;
    }*/
    
    printf( "Opening socket...\n" );
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error( "ERROR opening socket\n" );

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if ( bind( sockfd, ( struct sockaddr * ) &serv_addr,sizeof( serv_addr ) ) < 0)
        error( "ERROR on binding\n" );

    while( 1 )
    {
        listen(sockfd, 5);
        printf( "Waiting for the client connection...\n" );
        
        cmd.code = swap_uint16(0x00FF);
        cmd.hiParam =  swap_uint16(0x0001);
        cmd.loParam = swap_uint16(0x00FF);
        
        //write( writeport, &cmd, sizeof( struct Command ) );
        writeCommand( writeport, &cmd );
        printf( "Sizeof: %d\n", sizeof( struct Command ) );
        size_t clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);

        if ( newsockfd < 0 )
            error( "ERROR on accept\n" );
    
        printf( "Client was connected.\n" );
        
        cmd.code = 1;
        //char buf[6];
        while( cmd.code > 0 )
        {
            int n = read( newsockfd, &cmd, sizeof( struct Command ) );
            if ( n != sizeof( struct Command ) )
            {
                printf( "Corrupted command from socket! %d\n", n );
                break;
            }
            n = writeCommand( writeport, &cmd );
            if (n == -1 )
            {
                fputs("write() of bytes failed!\n", stderr);
                break;
            }
        }
        printf( "Client was disconnected.\n" );
        close( newsockfd );
        //command = 0;
    }
    
    //closing ports
    close(writeport);
    exit(0);
}

