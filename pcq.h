/*****************************************************************************
 *
 * ORIGINAL_FILE:	pacy.h
 * ORIGINAL SYNOPSIS:	GPS library: header file
 *		(NOTE: This library supports only "SONY PACY-CNV10 (PC-Card)"
 *		 GPS receiver.)
 * ORIGINALDATE:	Thu, Dec 17 1998
 * ORIGINALAUTHOR:	Kouichi ABE (WALL)
 * ORIGINALE-MAIL:	kouichi@etn.yz.yamagata-u.ac.jp
 *
 * Edited by.....
 *
 * FILE:	pcq.h
 * SYNOPSIS:	GPS library: header file
 *		(NOTE: This library supports " PCQ HGR1/HGR3 (USB)"
 *		 GPS receiver.)
 * DATE:	Fri Feb 23
 * AUTHOR:	Hirotaka Horie 
 * E-MAIL:	nandu@ht.sfc.keio.ac.jp
 *****************************************************************************/

#ifndef	_pcq_h
#define	_pcq_h	1

#if	TM_IN_SYS_TIME
#include <sys/time.h>
#else
#include <time.h>
#endif	/* TM_IN_SYS_TIME */

/*****************************************************************************
 *
 *	define	of macros and structures
 *
 *****************************************************************************/
#ifndef	GPS_DEVICE
#define	GPS_DEVICE	"/dev/ugen0.1"
#endif	/* GPS_DEVICE */
#define	GPS_OPENSPEED	(B9600)
#define	GPS_OPENMODE	(O_RDONLY)

#define	GBUFSIZE	512
#define	INVALID		(-999)

#define NUMBER_OF_SATELITE 16

#define RETSIGTYPE void

/* ****************************************************************************
 *************************************************************************** */

typedef struct {	/* ��������ͥ빽¤��	*/
  int	number;		/* ����ͥ��ֹ�		*/
  char	status[6];	/* ����ͥ�ξ���	*/
  int	prn;		/* PRN �ֹ桩		*/
  int	n_elevation;	/* �����ι��١ʿ��͡�	*/
  char	elevation[12];	/* ����: "-(xxx..xxx)"	*/
  int	n_azimuth;	/* ���������̡ʿ��͡�	*/
  char	azimuth[9];	/* ����: "-(xx..xx)"	*/
  char	*situation;	/* �������ȱ����ξ���	*/
  int	n_situation;	/* �������ȱ����ξ���	*/
  char	c_situation;	/* �������ȱ����ξ���	*/
  int	signal;		/* ����ξ���		*/
} channel;

typedef struct {		/* �����Υǡ�����¤��	*/
  char		version[5];	/* �������ΥС������	*/
  struct tm nowtm;		/* ���߻������	*/
  char		ns;		/* �̰ޡ�N�ˡ���ޡ�S��	*/
  char		ew;		/* ���С�W�ˡ���С�E��	*/
  int		latitude_deg;	/* ���١��١�		*/
  int		latitude_min;	/* ���١�ʬ��: 1/60 deg	*/
  int		latitude_sec;	/* ���١��á�: 1/60 min	*/
  int		latitude_frac;	/* ���١�ü����		*/
  double	n_latitude;	/* ���١ʿ��Ͳ���	*/
  int		longitude_deg;	/* ���١��١�		*/
  int		longitude_min;	/* ���١�ʬ��: 1/60 deg	*/
  int		longitude_sec;	/* ���١��á�: 1/60 min	*/
  int		longitude_frac;	/* ���١�ü����		*/
  double	n_longitude;	/* ���١ʿ��Ͳ���	*/
  int		altitude;	/* ɸ��			*/
  int		n_altitude;	/* ɸ��ʿ��Ͳ���	*/
  int		velocity;	/* ®��	km/h	*/
  int		direction;	/* ����			*/
  struct tm	calctm;		/* ¬��������	*/
  char		*dop;		/* ���١�dilution of precision��*/
  int		n_dop;		/* ���٤��ɤ餮��		*/
  int		measuremode;	/* ¬��⡼�ɡ�2-D or 3-D��	*/
  char		*mapdatum;	/* ���������ǡ���		*/
  channel	ch[16];		/* �����Υ���ͥ��16�ġ�	*/
  char		rest[5];	/* �����ʥǡ���			*/
  char		parity;		/* �ѥ�ƥ���either E or 0��	*/
} gps_info;


/*****************************************************************************
 *
 *	declaration of global functions
 *
 *****************************************************************************/
extern void	gps_init(char*);
extern void	gps_quit(void);
extern int	gps_getinfo(gps_info *);
extern void	gps_printinfo(FILE *, gps_info *);

#endif	/* _pcq_h */
