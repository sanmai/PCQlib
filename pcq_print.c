/*****************************************************************************
 *
 * FILE:	pcq_print.c
 * SYNOPSIS:	GPS library: GPS data printng module for SONY-HGR1/HGR3
 * DATE:	2/28/2001
 * OriginalAUTHOR:	Kouichi ABE (WALL)
 * E-MAIL:	kouichi@etn.yz.yamagata-u.ac.jp
 * Editor: Hirotaka Horie
 * E-Mail: nandu@ht.sfc.keio.ac.jp
 *
 *****************************************************************************/

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif	/* HAVE_CONFIG_H */

#include <stdio.h>
#include "pcq.h"

/*****************************************************************************
 *
 *	declaration of local functions
 *
 *****************************************************************************/
static char *	Asctime(struct tm *);


/*
 * FUNCTION NAME:	gps_printinfo
 * NOTE:		show the information of GPS
 * ARGUMENTS:		output sink, gps information structure
 * RETURN VALUE:	none
 */
void
gps_printinfo(outfp, gpsi)
	FILE		*outfp;
	gps_info	*gpsi;
{
  fprintf(outfp, "GPS receiver version: %s\n", gpsi->version);
  fprintf(outfp, "Position: ");
  fprintf(outfp, "(%c%02d:%02d'%02d.%d\", %c%03d:%02d'%02d.%d\", ",
	  gpsi->ns, gpsi->latitude_deg, gpsi->latitude_min,
	  gpsi->latitude_sec, gpsi->latitude_frac,
	  gpsi->ew, gpsi->longitude_deg, gpsi->longitude_min,
	  gpsi->longitude_sec, gpsi->longitude_frac);
  if (gpsi->measuremode != 4) {
    fprintf(outfp, "(");
  }
  fprintf(outfp, "%d[m]", gpsi->altitude);
  if (gpsi->measuremode != 4) {
    fprintf(outfp, ")");
  }
  fprintf(outfp, ")\n");
  fprintf(outfp, "%s\n", Asctime(&(gpsi->nowtm)));
  fprintf(outfp, "velocity %d[km/h], direction %d[deg]\n",
	  gpsi->velocity, gpsi->direction);
  fprintf(outfp, "\tfrom %s\n", Asctime(&(gpsi->calctm)));
  fprintf(outfp, "\n");
  fprintf(outfp, "Dilution of Precision:: %s\n", gpsi->dop);
  fprintf(outfp, "measurement mode: %d (%d dimensional)\n",
	  gpsi->measuremode, gpsi->measuremode - 1);
  fprintf(outfp, "map datum: %s\n", gpsi->mapdatum);
  fprintf(outfp, "???: %s\n", gpsi->rest);
  fprintf(outfp, "parity: %c\n", gpsi->parity);
}

/*
 * FUNCTION NAME:	Asctime
 * NOTE:
 * ARGUMENTS:
 * RETURN VALUE:
 */
static char *
Asctime(t)
	struct tm	*t;
{
  char		*at;
  static char	s[29];
  /* "DOW MON DY HH:MM:SS TZ- YYYY\0" */
  struct tm	*tm;
  time_t	tt;

  at = asctime(t);
  tt = time(0);
  tm = localtime(&tt);
  sprintf(s, "%19.19s GMT %4.4s", at, at + 20);

  return s;
}
