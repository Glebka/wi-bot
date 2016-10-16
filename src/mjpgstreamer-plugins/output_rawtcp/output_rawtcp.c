/*******************************************************************************
#                                                                              #
#      MJPG-streamer allows to stream JPG frames from an input-plugin          #
#      to several output plugins                                               #
#                                                                              #
#      Copyright (C) 2007 Tom Stöveken                                         #
#                                                                              #
# This program is free software; you can redistribute it and/or modify         #
# it under the terms of the GNU General Public License as published by         #
# the Free Software Foundation; version 2 of the License.                      #
#                                                                              #
# This program is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU General Public License for more details.                                 #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with this program; if not, write to the Free Software                  #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    #
#                                                                              #
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <syslog.h>
#include <netinet/in.h>

#include <dirent.h>

#include <utils.h>
#include <mjpg_streamer.h>

#define OUTPUT_PLUGIN_NAME "Raw TCP output plugin"

static pthread_t worker;
static globals *pglobal;
static max_frame_size;
static unsigned char *frame = NULL;
static int input_number = 0;
static int  port;

// listen socket and client socket fdesks
static int sockfd=0, newsockfd=0;

void sigPipeHandler()
{
    OPRINT("Caught SIGPIPE\n");
}

/******************************************************************************
Description.: print a help message
Input Value.: -
Return Value: -
******************************************************************************/
void help(void)
{
    fprintf(stderr, " ---------------------------------------------------------------\n" \
            " Help for output plugin..: "OUTPUT_PLUGIN_NAME"\n" \
            " ---------------------------------------------------------------\n" \
            " The following parameters can be passed to this plugin:\n\n" \
            " [-p | --port ]........: TCP port for this server\n" \
            " ---------------------------------------------------------------\n");
}

/******************************************************************************
Description.: clean up allocated ressources
Input Value.: unused argument
Return Value: -
******************************************************************************/
void worker_cleanup(void *arg)
{
    static unsigned char first_run = 1;

    if(!first_run) {
        DBG("already cleaned up ressources\n");
        return;
    }

    first_run = 0;
    OPRINT("cleaning up ressources allocated by worker thread\n");

    if(frame != NULL) {
        free(frame);
    }
    close( newsockfd );
    close( sockfd );
    
}
/******************************************************************************
Description.: this is the main worker thread
              it loops forever, grabs a fresh frame and stores it to file
Input Value.:
Return Value:
******************************************************************************/
void *worker_thread(void *arg)
{
    int ok = 1, frame_size = 0;
    unsigned char *tmp_framebuffer = NULL;
    
    /* set cleanup handler to cleanup allocated ressources */
    pthread_cleanup_push(worker_cleanup, NULL);
    
    
    
    struct sockaddr_in serv_addr, cli_addr;
    
    OPRINT("Size of int: %d\n", sizeof( int ) );
    
    OPRINT("Opening socket...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        OPRINT("ERROR opening socket\n");
    
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = port;
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        OPRINT("ERROR on binding\n");
        return NULL;
    }
    
    while(ok >= 0 && !pglobal->stop) 
    {
        listen(sockfd, 5);
        OPRINT("Waiting for the client connection...\n");
        size_t clilen = sizeof(cli_addr);
        
        pthread_mutex_lock(&pglobal->in[input_number].db);
        
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
        
        pthread_mutex_unlock(&pglobal->in[input_number].db);

        if (newsockfd < 0)
        {
            OPRINT("ERROR on accept\n");
            return NULL;
        }
    
        OPRINT("Client was connected.\n");
        
        while(ok >= 0 && !pglobal->stop)
        {
            pthread_mutex_lock(&pglobal->in[input_number].db);
            pthread_cond_wait(&pglobal->in[input_number].db_update, &pglobal->in[input_number].db);

            /* read buffer */
            frame_size = pglobal->in[input_number].size;
            /* check if buffer for frame is large enough, increase it if necessary */
            if(frame_size > max_frame_size) 
            {
                DBG("increasing buffer size to %d\n", frame_size);

                max_frame_size = frame_size + (1 << 16);
                if((tmp_framebuffer = realloc(frame, max_frame_size)) == NULL) 
                {
                    pthread_mutex_unlock(&pglobal->in[input_number].db);
                    OPRINT("not enough memory\n");
                    return NULL;
                }

                frame = tmp_framebuffer;
            }
            
            /* copy frame to our local buffer now */
            memcpy(frame, pglobal->in[input_number].buf, frame_size);
            
            /* allow others to access the global buffer again */
            pthread_mutex_unlock(&pglobal->in[input_number].db);
            
            int toWrite = frame_size;
            int written = 0;
            
            written = write( newsockfd, &frame_size, sizeof( int ) );
            if ( written < 0 )
                break;
            do
            {
                written = write( newsockfd, frame, toWrite );
                if ( written < toWrite )
                {
                    OPRINT("Written: %d\nTo write: %d\n", written, toWrite );
                }
                
                toWrite -= written;
            }
            while ( toWrite != 0 && written > 0);
            if ( written < 0 )
                break;
        }
        OPRINT("Client was disconnected.\n");
        close( newsockfd );
        newsockfd = 0;
    }
    close( sockfd );
    sockfd = 0;
    /* cleanup now */
    pthread_cleanup_pop(1);

    return NULL;
}

/*** plugin interface functions ***/
/******************************************************************************
Description.: this function is called first, in order to initialize
              this plugin and pass a parameter string
Input Value.: parameters
Return Value: 0 if everything is OK, non-zero otherwise
******************************************************************************/
int output_init(output_parameter *param)
{
    int i;
    port = htons(778);

    param->argv[0] = OUTPUT_PLUGIN_NAME;

    /* show all parameters for DBG purposes */
    for(i = 0; i < param->argc; i++) {
        DBG("argv[%d]=%s\n", i, param->argv[i]);
    }

    reset_getopt();
    while(1) {
        int option_index = 0, c = 0;
        static struct option long_options[] = {
            {"h", no_argument, 0, 0},
            {"help", no_argument, 0, 0},
            {"p", required_argument, 0, 0},
            {"port", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        c = getopt_long_only(param->argc, param->argv, "", long_options, &option_index);

        /* no more options to parse */
        if(c == -1) break;

        /* unrecognized option */
        if(c == '?') {
            help();
            return 1;
        }

        switch(option_index) {
            /* h, help */
        case 0:
        case 1:
            DBG("case 0,1\n");
            help();
            return 1;
            break;

            /* p, port */
        case 2:
        case 3:
            DBG("case 2,3\n");
            port = htons(atoi(optarg));
            break;
        }
    }

    pglobal = param->global;
    if(!(input_number < pglobal->incnt)) {
        OPRINT("ERROR: the %d input_plugin number is too much only %d plugins loaded\n", input_number, pglobal->incnt);
        return 1;
    }

    OPRINT("TCP port.....: %d\n", ntohs(port));
    OPRINT("input plugin.....: %d: %s\n", input_number, pglobal->in[input_number].plugin);
    return 0;
}

/******************************************************************************
Description.: calling this function stops the worker thread
Input Value.: -
Return Value: always 0
******************************************************************************/
int output_stop(int id)
{
    DBG("will cancel worker thread\n");
    pthread_cancel(worker);
    return 0;
}

/******************************************************************************
Description.: calling this function creates and starts the worker thread
Input Value.: -
Return Value: always 0
******************************************************************************/
int output_run(int id)
{
    DBG("launching worker thread\n");
    pthread_create(&worker, 0, worker_thread, NULL);
    pthread_detach(worker);
    return 0;
}

int output_cmd(int plugin, unsigned int control_id, unsigned int group, int value)
{
    DBG("command (%d, value: %d) for group %d triggered for plugin instance #%02d\n", control_id, value, group, plugin);
    return 0;
}
