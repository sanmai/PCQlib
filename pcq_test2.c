/*****************************************************************************
 *
 * FILE:	pacy_test2.c
 * SYNOPSIS:	2nd test program for GPS library
 * DATE:	Thu, Dec 17 1998
 * AUTHOR:	Kouichi ABE (WALL)
 * E-MAIL:	kouichi@etn.yz.yamagata-u.ac.jp
 *
 *****************************************************************************/

#include <stdio.h>
#include "pcq.h"

/*
 *	MAIN FUNCTION
 */
int
main(argc, argv)
	int	argc;
	char	**argv;
{
  gps_info	gpsi;

  if (argc > 1) {
    gps_init(*++argv);
  }
  else {
    gps_init(NULL);
  }

  if (gps_getinfo(&gpsi)) {
    if (gpsi.ns == 'N') {
      fprintf(stdout, "ÀÃ∞ﬁ ");
    }
    else {
      fprintf(stdout, "∆Ó∞ﬁ ");
    }
    fprintf(stdout, " %02d≈Ÿ%02d ¨%02d…√%d\n",
		    gpsi.latitude_deg,
		    gpsi.latitude_min,
		    gpsi.latitude_sec,
		    gpsi.latitude_frac);
    if (gpsi.ew == 'E') {
      fprintf(stdout, "≈Ï∑– ");
    }
    else {
      fprintf(stdout, "¿æ∑– ");
    }
    fprintf(stdout, "%02d≈Ÿ%02d ¨%02d…√%d\n",
		    gpsi.longitude_deg,
		    gpsi.longitude_min,
		    gpsi.longitude_sec,
		    gpsi.longitude_frac);
    fprintf(stdout, "…∏π‚ %d •·°º•»•Î\n", gpsi.altitude);
    fprintf(stdout, "¬Æ≈Ÿ %d km/h\n", gpsi.velocity);
    fprintf(stdout, " ˝∞Ã ");
    if (gpsi.direction == 0) {
      fprintf(stdout, "ÀÃ");
    }
    else if (gpsi.direction == 45) {
      fprintf(stdout, "ÀÃ≈Ï");
    }
    else if (gpsi.direction == 90) {
      fprintf(stdout, "≈Ï");
    }
    else if (gpsi.direction == 135) {
      fprintf(stdout, "≈Ï∆Ó");
    }
    else if (gpsi.direction == 180) {
      fprintf(stdout, "∆Ó");
    }
    else if (gpsi.direction == 225) {
      fprintf(stdout, "∆Ó¿æ");
    }
    else if (gpsi.direction == 270) {
      fprintf(stdout, "¿æ");
    }
    else if (gpsi.direction == 315) {
      fprintf(stdout, "ÀÃ¿æ");
    }
    else {
      fprintf(stdout, "%d ≈Ÿ° ÀÃ§´§Èª˛∑◊≤Û§Í§À°À");
    }
    fputs("\n", stdout);
  }
  gps_quit();

  return 0;
}
