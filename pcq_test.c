/*****************************************************************************
 *
 * FILE:	pcq_test.c
 * SYNOPSIS:	test program for GPS library
 * DATE:	Thu, Dec 17 1998
 * OriginalAUTHOR:	Kouichi ABE (WALL)
 * E-MAIL:	kouichi@etn.yz.yamagata-u.ac.jp
 * Editor:	Hirotaka Horie
 * E-MAIL:	nandu@ht.sfc.keio.ac.jp
 *
 *****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "pcq.h"
#include "pcq_power.h"

#define	ESC	(0x1b)
#define LOOPMODE

/*
 *	MAIN FUNCTION
 */
int
main(argc, argv)
	int	argc;
	char	**argv;
{
  gps_info	gpsi;

  /* 
   *  turn GPS device ON 
   */
//  hgr_power(HGRON,DEVNAME);
//  sleep(2);

  if (argc > 1) {
    gps_init(*++argv);
  }
  else {
    gps_init(NULL);
  }

#ifdef	LOOPMODE
  while (gps_getinfo(&gpsi)) {
    fprintf(stderr, "%c[1;1H%c[2J", ESC, ESC);
    gps_printinfo(stderr, &gpsi);
  }
#else
  if (gps_getinfo(&gpsi)) {
    fprintf(stderr, "%c[1;1H%c[2J", ESC, ESC);
    gps_printinfo(stderr, &gpsi);
  }
#endif
  gps_quit();

  /* 
   *  turn GPS device ON 
   */
  hgr_power(HGROFF,DEVNAME);

  return 0;
}
