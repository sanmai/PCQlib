/*****************************************************************************
 *
 * FILE:	pcq_testALL.c
 * SYNOPSIS:	Print all variables
 * DATE:	Febuary 26, 2001
 * AUTHOR:	Hirotaka Horie
 * E-MAIL:	nandu@ht.sfc.keio.ac.jp
 *
 *****************************************************************************/

#include <stdio.h>
#include <time.h>
#include<unistd.h>
#include "pcq.h"

#define ESC     (0x1b)
#define LOOPMODE

/*
 *	MAIN FUNCTION
 */
int
main(argc, argv)
	int	argc;
	char	**argv;
{
  FILE *logfile;
  gps_info gpsi;

  logfile = fopen("./logfile","a");

  if (argc > 1) {
    gps_init(*++argv);
  }
  else {
    gps_init(NULL);
  }

#ifdef  LOOPMODE
  while (gps_getinfo(&gpsi)) {
    fprintf(stderr, "%c[1;1H%c[2J", ESC, ESC);
    gps_print_all(stderr, &gpsi);
	gps_print_all(logfile,&gpsi);
	fprintf(logfile,"#################### ####################\n");
  }
#else
  if (gps_getinfo(&gpsi)) {
    fprintf(stderr, "%c[1;1H%c[2J", ESC, ESC);
    gps_print_all(stderr, &gpsi);
  }
#endif

  gps_quit();
}


gps_print_all(outfp, gpsi)
  FILE *outfp;
  gps_info	*gpsi;
{
  int i=0;

	fprintf(outfp, "GPS receiver version: %s\n", gpsi->version);
	fprintf(outfp, "Position: \n");

    fprintf(outfp, "\t%c  %02dÅÙ%02dÊ¬%02dÉÃ%d\t",
			gpsi->ns,
		    gpsi->latitude_deg,
		    gpsi->latitude_min,
		    gpsi->latitude_sec,
		    gpsi->latitude_frac);
	fprintf(outfp, "n_latitude %lf\n",gpsi->n_latitude);

    fprintf(outfp, "\t%c %02dÅÙ%02dÊ¬%02dÉÃ%d\t",
			gpsi->ew,
		    gpsi->longitude_deg,
		    gpsi->longitude_min,
		    gpsi->longitude_sec,
		    gpsi->longitude_frac);
	fprintf(outfp, "n_longitude %lf\n",gpsi->n_longitude);

    fprintf(outfp, "\taltitude %d [meter]\t", gpsi->altitude);
    fprintf(outfp, "n_altitude %lf\n", gpsi->n_altitude);

    fprintf(outfp, "\tvelocity %d [km/h],\t", gpsi->velocity);
	fprintf(outfp, "direction %d[degree]\n",gpsi->direction);

#ifdef RAWDATA	
	fprintf(outfp, "nowtm:\n");
	fprintf(outfp, "\t%d/%d/%d ",
		   gpsi->nowtm.tm_year, 
		   gpsi->nowtm.tm_mon, 
		   gpsi->nowtm.tm_mday);
	fprintf(outfp, "\t%d:%d:%d \n",
		   gpsi->nowtm.tm_hour, 
		   gpsi->nowtm.tm_min, 
		   gpsi->nowtm.tm_sec);
	fprintf(outfp, "\ttm_wday  %d\n",gpsi->nowtm.tm_wday);
	fprintf(outfp, "\ttm_yday  %d\n",gpsi->nowtm.tm_yday);
	fprintf(outfp, "\ttm_isdst %d\n",gpsi->nowtm.tm_isdst);
	fprintf(outfp, "\ttm_gmtoff %l\n",gpsi->nowtm.tm_gmtoff);
	fprintf(outfp, "\ttm_zone %s\n",gpsi->nowtm.tm_zone);

	fprintf(outfp, "calctm:\n");
	fprintf(outfp, "\t%d/%d/%d ",
		   gpsi->calctm.tm_year, 
		   gpsi->calctm.tm_mon, 
		   gpsi->calctm.tm_mday);
	fprintf(outfp, "\t%d:%d:%d \n",
		   gpsi->calctm.tm_hour, 
		   gpsi->calctm.tm_min, 
		   gpsi->calctm.tm_sec);
	fprintf(outfp, "\ttm_wday  %d\n",gpsi->calctm.tm_wday);
	fprintf(outfp, "\ttm_yday  %d\n",gpsi->calctm.tm_yday);
	fprintf(outfp, "\ttm_isdst %d\n",gpsi->calctm.tm_isdst);
	fprintf(outfp, "\ttm_gmtoff %l\n",gpsi->calctm.tm_gmtoff);
	fprintf(outfp, "\ttm_zone %s\n",gpsi->calctm.tm_zone);
#else
	fprintf(outfp, "nowtm:\t");
	fprintf(outfp, "%24.24s\t", asctime(&(gpsi->nowtm)));
	fprintf(outfp, "calctm:\t");
	fprintf(outfp, "%24.24s", asctime(&(gpsi->calctm)));
	fprintf(outfp, "\n");
#endif /* RAWDATA */
	fprintf(outfp, "measuremode: %d(%dD)\n", gpsi->measuremode,(gpsi->measuremode - 1));
	fprintf(outfp, "map datum: %s\n", gpsi->mapdatum);
	fprintf(outfp, "parity: %c\n",  gpsi->parity);

	fprintf(outfp, "dop %s\t", gpsi->dop);
	fprintf(outfp, "n_dop %d\n", gpsi->n_dop);

	for (i=0;i<NUMBER_OF_SATELITE;i++){
#ifdef RAWDATA	
	  fprintf(outfp, "\n");
	  fprintf(outfp, "number: %d \n",gpsi->ch[i].number);
	  fprintf(outfp, "status: %s \t",&(gpsi->ch[i].status));
	  fprintf(outfp, "prn: %d \n",gpsi->ch[i].prn);
	  fprintf(outfp, "elevation: %d \t",gpsi->ch[i].n_elevation);
	  fprintf(outfp, "%s\n",&(gpsi->ch[i].elevation));
	  fprintf(outfp, "azimuth: %d \t",gpsi->ch[i].n_azimuth);
	  fprintf(outfp, "%s\n",&(gpsi->ch[i].azimuth));
	  fprintf(outfp, "situation: %s ",gpsi->ch[i].situation);
	  fprintf(outfp, "%d ",gpsi->ch[i].n_situation);
	  fprintf(outfp, "%c ",gpsi->ch[i].c_situation);
	  fprintf(outfp, "%d\n",gpsi->ch[i].signal);

#else
	  if(i==(NUMBER_OF_SATELITE/2))
		fprintf(outfp, "\n");
	  if(gpsi->ch[i].number<10)
		fprintf(outfp, " ");
	  fprintf(outfp, "%d:",gpsi->ch[i].number);
	  fprintf(outfp, "%s ",gpsi->ch[i].situation);
	  fflush(outfp);
#endif /* RAWDATA */
	}

    fputs("\n", outfp);


  return 0;
}
