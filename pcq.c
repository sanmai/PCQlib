/*****************************************************************************
 *
 * FILE:	pcq.c
 * SYNOPSIS:	SONY GPS USB library: main module
 * DATE:	Mon Mar 12 18:09 JST 2001
 * EDITOR:  Hirotaka Horie<nandu@ht.sfc.keio.ac.jp>
 * ORIGINAL AUTHOR:	Kouichi ABE (WALL)<kouichi@etn.yz.yamagata-u.ac.jp>
 *****************************************************************************/

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif	/* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include "pcq.h"
#include "pcqinfo.h"
#include "pcq_power.h"

#define DEBUG_GPS

/*****************************************************************************
 *
 *	declaration of local variable
 *
 *****************************************************************************/
static int	gps_fd;
static FILE *	gps_fp;


/*****************************************************************************
 *
 *	declaration of local functions
 *
 *****************************************************************************/
static struct tm *	str2localtm(char *);
/* static struct tmytk *	str2localtmytk(char *); */
static struct tm *	str2localtmytk(char *); 
static int		isleap(int);
static void		getchinfo(channel *, int);
int hgr_power(int onoff, char *n_devname);

/*
 * FUNCTION NAME:	gps_init
 * NOTE:		open the GPS device and set the file pointer
 * ARGUMENTS:		device name
 * RETURN VALUE:	none
 */
void
gps_init(gps_dev)
	char	*gps_dev;
{
  char			dummy[GBUFSIZE];
  gps_info		gpsi;
  int i;


  /* turn USB GPS device on */
  hgr_power(HGRON,DEVNAME);

  /*
   * set GPS device
   */
  if (gps_dev == NULL) {
    gps_dev = GPS_DEVICE;
  }

  /*
   * set terminal attributes 
   */
  gps_fd = open(gps_dev, GPS_OPENMODE);
  if (gps_fd < 0) {
    fprintf(stderr, "GPS: cannot open %s (errno = %d).\n", gps_dev, errno);
    exit(1);
  }

  /*
   * set the file pointer for the GPS device
   */

#ifdef	DEBUG_GPS
  fprintf(stderr, "opened GPS device %s\n", gps_dev);
#endif	/* DEBUG_GPS */

  // if (gps_getinfo(&gpsi) && strcmp(gpsi.version, "SM01")  != 0) {
  if ( (gps_getinfo(&gpsi)==0)&& 
	   (strcmp(gpsi.version, "SM01")||strcmp(gpsi.version, "SM00")==0) )   {
    fprintf(stderr, "This library supports SONY GPS HGR1 or HGR3 only.\n");
	gps_quit();
    exit(1);
  }
  
  signal(SIGINT, (RETSIGTYPE*)gps_quit);	/* for interrupt */
  signal(SIGTERM, (RETSIGTYPE*)gps_quit);	/* for terminate */
}

#define UGPS_SINGLELINE 150-2+1
#define CHAR_CR 13
#define CHAR_LF 10
#define CHAR_SP 32

int read_mygps_line(int gps_fd, char *in_line){
  char line[UGPS_SINGLELINE];
  char *line_ptr;
  char a_buf[2];
  char *a_buf_ptr;
  
  char ccr=CHAR_CR;
  char clf=CHAR_LF;
  char csp=CHAR_SP;

  int flag=1;
  int i=0;

  //a_buf_ptr=(malloc(sizeof(a_buf)));
  bzero(a_buf,sizeof(a_buf));
  a_buf_ptr=a_buf;

  line_ptr=(malloc(sizeof(line)));
  bzero(line,sizeof(line)); 
  line_ptr=line;

  while(flag>0){
	read(gps_fd, a_buf, 1);

	if((a_buf[0]==csp)||(a_buf[0]==NULL)){
	  continue;
	}
	else if(a_buf[0]==ccr){
	  i++;
	  flag=2;
	  continue;
	}
	else if(a_buf[0]==clf){
	  i++;
	  if (flag==2){
		if( ( (strncmp("SM01", line_ptr, 4)==0) || 
			  (strncmp("SM00", line_ptr, 4)==0) )
			&&((i==UGPS_SINGLELINE)==0)){
		  bzero(a_buf,sizeof(a_buf));
		  flag=0; /* terminate while */
		  break;
		}		  
		else{
		  bzero(line,sizeof(line));
		  bzero(a_buf,sizeof(a_buf));
		  i=0;
		  continue;
		}
	  }
	  else{
		bzero(line,sizeof(line));
		bzero(a_buf,sizeof(a_buf));
		i=0;
		continue;
	  }
	}
	else if(i>UGPS_SINGLELINE){
		  bzero(line,sizeof(line));
		  bzero(a_buf,sizeof(a_buf));
		  i=0;
		  continue;
	}
	else{
	  flag=1;
	  strcat(line_ptr,a_buf);
	  bzero(a_buf,sizeof(a_buf));
	  i++;
	}
  }
  //free(a_buf_ptr);
  strcpy(in_line,line_ptr);
  return 1;
  //free(line_ptr);
}

/*
 * FUNCTION NAME:	gps_quit
 * NOTE:		close the file pointer for GPS device
 * ARGUMENTS:		none
 * RETURN VALUE:	none
 */
void
gps_quit(void)
{
  //fclose(gps_fp);
  //if( close(gps_fd))
  //perror("close fail");
  hgr_power(HGROFF,DEVNAME);
  exit(0);
}

/*
 * FUNCTION NAME:	gps_getinfo
 * NOTE:
 * ARGUMENTS:
 * RETURN VALUE:
 */
int
gps_getinfo(gpsi)
	gps_info	*gpsi;
{
  //char		buf[GBUFSIZE];
  char		s[GBUFSIZE];
  struct tm	*tm;
  int		i, j;

  char buf[UGPS_SINGLELINE];
  char *buf_ptr;

  buf_ptr=buf;
  buf_ptr=(malloc(sizeof(buf)));
  bzero(buf,sizeof(buf));

  if (read_mygps_line(gps_fd, buf)){
    /* vendor version */
    sprintf(gpsi->version, "%4.4s", buf);

    /* current date and time */
    sprintf(s, "%15.15s", buf + 4);
    tm = str2localtmytk(s);
    bcopy(tm, &(gpsi->nowtm), sizeof(struct tm));

    /* latitude */
    gpsi->ns	    = buf[19];
    if (gpsi->ns == 'n') {
      gpsi->ns = 'N';
    }
    else if (gpsi->ns == 's') {
      gpsi->ns = 'S';
    }

    sprintf(s, "%2.2s", buf + 20);
    gpsi->latitude_deg = atoi(s);

    sprintf(s, "%2.2s", buf + 22);
    gpsi->latitude_min = atoi(s);

    sprintf(s, "%2.2s", buf + 24);
    gpsi->latitude_sec = atoi(s);

    sprintf(s, "%1.1s", buf + 26);
    gpsi->latitude_frac = atoi(s);

    gpsi->n_latitude = gpsi->latitude_deg +
		       gpsi->latitude_min / 60. +
		       gpsi->latitude_sec / 3600. +
		       gpsi->latitude_frac / 36000.;

    if (gpsi->ns == 'S') {
      gpsi->n_latitude = -(gpsi->n_latitude);
    }

    /* longitude */
    gpsi->ew = buf[27];
    sprintf(s, "%3.3s", buf + 28);
    gpsi->longitude_deg = atoi(s);

    sprintf(s, "%2.2s", buf + 31);
    gpsi->longitude_min = atoi(s);

    sprintf(s, "%2.2s", buf + 33);
    gpsi->longitude_sec = atoi(s);

    sprintf(s, "%1.1s", buf + 35);
    gpsi->longitude_frac = atoi(s);

	// original was placed here... don't know the reason by nandu 
	// moved the following line to just below the comment out of "altitude".
	// sprintf(s, "%5.5s", buf + 36);
    gpsi->n_longitude = gpsi->longitude_deg +
			gpsi->longitude_min / 60. +
			gpsi->longitude_sec / 3600. +
			gpsi->longitude_frac / 36000.;
    if (gpsi->ew == 'W') {
      gpsi->n_longitude = -(gpsi->n_longitude);
    }

    /* altitude */
    sprintf(s, "%5.5s", buf + 36);
    gpsi->altitude = atoi(s);

    /* velocity */
    sprintf(s, "%3.3s", buf + 41);
    gpsi->velocity = atoi(s);

    /* direction */
    sprintf(s, "%3.3s", buf + 44);
    gpsi->direction = atoi(s);

    /* record date and time ? */
    sprintf(s, "%13.13s", buf + 47);
    tm = str2localtm(s);
    bcopy(tm, &(gpsi->calctm), sizeof(struct tm));

    /* dilution of precision (DOP) */
    gpsi->dop = dop_str[buf[60] - 'A'];

    /* measurement mode */
    sprintf(s, "%1.1s", buf + 61);
    gpsi->measuremode = atoi(s);

    /* map datum of position data */
    gpsi->mapdatum = mapdatum_str[buf[62] - 'A'];

    /* satellite information */
    for (i = 0, j = 63; i < NUMBER_OF_SATELITE; i++, j += 5) {
      sprintf(gpsi->ch[i].status, "%5.5s", buf + j);
      getchinfo(&(gpsi->ch[i]), i + 1);
    }

    /* unknown data */
    sprintf(gpsi->rest, "%4.4s", buf + 103+40);

    /* parity */
    gpsi->parity = buf[107+40];

	//free(&buf_ptr);
    return 1;
  }
  return 0;
}

/*
 * FUNCTION NAME:	str2localtm
 * NOTE:
 * ARGUMENTS:
 * RETURN VALUE:
 */
static struct tm *
str2localtm(buf)
	char	*buf;
{
  static int		yday[] = {0, 31, 59, 90, 120, 151, 181, 212, 242, 273,
				  303, 334};
  static struct tm	tm;
  int			leap;
  int			f_jst;
  char			s[GBUFSIZE];

  sprintf(s, "%2.2s", buf);
  tm.tm_year  = atoi(s) + 100 ;
  sprintf(s, "%2.2s", buf + 2);
  tm.tm_mon   = atoi(s) - 1;
  sprintf(s, "%2.2s", buf + 4);
  tm.tm_mday  = atoi(s);
  sprintf(s, "%1.1s", buf + 6);
  tm.tm_wday  = atoi(s);
  sprintf(s, "%2.2s", buf + 7);
  tm.tm_hour  = atoi(s);
  sprintf(s, "%2.2s", buf + 9);
  tm.tm_min   = atoi(s);
  sprintf(s, "%2.2s", buf + 11);
  tm.tm_sec   = atoi(s);
  leap	      = isleap(tm.tm_year);
  if (tm.tm_mon > 1) {
    tm.tm_yday = yday[tm.tm_mon] + tm.tm_mday + leap;
  }
  else {
    tm.tm_yday = yday[tm.tm_mon] + tm.tm_mday;
  }
  tm.tm_isdst = -1;

  /* GMT converting to JST */
  f_jst = tm.tm_hour + 9;
  tm.tm_hour = f_jst % 24;
  if (f_jst >= 24) {
    tm.tm_wday = (tm.tm_wday + 1) % 7;
  }

  return &tm;
}

/*
 * FUNCTION NAME:	str2localtmytk
 * NOTE: almost same as str2localtm but solved y2k problem
 * Editer: nandu@ht.sfc.keio.ac.jp
 * ARGUMENTS:
 * RETURN VALUE:
 */
static struct tm *
str2localtmytk(buf)
	char	*buf;
{
  static int		yday[] = {0, 31, 59, 90, 120, 151, 181, 212, 242, 273,
				  303, 334};
  static struct tm	tm;
  int			leap;
  int			f_jst;
  char			s[GBUFSIZE];
  int year_tmp;

  sprintf(s, "%4.4s", buf);
  year_tmp = atoi(s);
  if (year_tmp>100)
	year_tmp = year_tmp - 1900;
  tm.tm_year  = year_tmp;
  //tm.tm_year  = atoi(s);
  sprintf(s, "%2.2s", buf + 2+2);
  tm.tm_mon   = atoi(s) - 1;
  sprintf(s, "%2.2s", buf + 4+2);
  tm.tm_mday  = atoi(s);
  sprintf(s, "%1.1s", buf + 6+2);
  tm.tm_wday  = atoi(s);
  sprintf(s, "%2.2s", buf + 7+2);
  tm.tm_hour  = atoi(s);
  sprintf(s, "%2.2s", buf + 9+2);
  tm.tm_min   = atoi(s);
  sprintf(s, "%2.2s", buf + 11+2);
  tm.tm_sec   = atoi(s);
  leap	      = isleap(tm.tm_year);
  if (tm.tm_mon > 1) {
    tm.tm_yday = yday[tm.tm_mon] + tm.tm_mday + leap;
  }
  else {
    tm.tm_yday = yday[tm.tm_mon] + tm.tm_mday;
  }
  tm.tm_isdst = -1;

  /* GMT converting to JST */
  f_jst = tm.tm_hour + 9;
  tm.tm_hour = f_jst % 24;
  if (f_jst >= 24) {
    tm.tm_wday = (tm.tm_wday + 1) % 7;
  }

  return &tm;
}

/*
 * FUNCTION NAME:	isleap
 * NOTE:		is this year leap year?
 * ARGUMENTS:		year from 1900
 * RETURN VALUE:	leap year (1) or none (0)
 */
static int
isleap(year)
	int	year;
{
  int	leap;

  leap  = 0;
  /* need to check wether y2k compatible or not */
  if(year<100)
	year+= 100;
  if(year<1900)
  	year += 1900;

  if (year % 4 == 0) {
    leap = 1;			/* leap year */
    if (year % 100 == 0) {
      leap = 0;			/* non-leap year */
      if (year % 400 == 0) {
	leap = 1;		/* special leap year */
      }
    }
  }

  return leap;
}

/*
 * FUNCTION NAME:	getchinfo
 * NOTE:		get the information of current channel
 * ARGUMENTS:
 * RETURN VALUE:	none
 */
static void
getchinfo(ch, chno)
	channel	*ch;
	int	chno;
{
  char		*chp;

  ch->number = chno;
  chp = strchr(prn_str, ch->status[0]);
  if (chp != NULL) {
    ch->prn = (chp - prn_str) + 1;
  }
  else {
    ch->prn = INVALID;
  }

  if ('A' <= ch->status[1] && ch->status[1] <= 'J') {
    sprintf(ch->elevation, "  %s ", elevation_str[ch->status[1] - 'A']);
    ch->n_elevation = elevation_val[ch->status[1] - 'A'];
  }
  else if ('a' <= ch->status[1] && ch->status[1] <= 'j') {
    sprintf(ch->elevation, "-(%s)", elevation_str[ch->status[1] - 'a']);
    ch->n_elevation = -elevation_val[ch->status[1] - 'a'];
  }
  else {
    sprintf(ch->elevation, "???");
    ch->n_elevation = INVALID;
  }

  if ('A' <= ch->status[2] && ch->status[2] <= 'S') {
    sprintf(ch->azimuth, "  %s ", azimuth_str[ch->status[2] - 'A']);
    ch->n_azimuth = azimuth_val[ch->status[2] - 'A'];
  }
  else if ('a' <= ch->status[2] && ch->status[2] <= 's') {
    sprintf(ch->azimuth, "-(%s)", azimuth_str[ch->status[2] - 'a']);
    ch->n_azimuth = -azimuth_val[ch->status[2] - 'a'];
  }
  else {
    sprintf(ch->azimuth, "???");
    ch->n_azimuth = INVALID;
  }

  ch->n_situation = ch->status[3] - 'A';
  ch->situation	  = situation_str[ch->n_situation];
  ch->c_situation = situation_char[ch->n_situation];

  ch->signal = (ch->status[4] - 'A') * 4;
}
