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

typedef struct {	/* 衛星チャネル構造体	*/
  int	number;		/* チャネル番号		*/
  char	status[6];	/* チャネルの状態	*/
  int	prn;		/* PRN 番号？		*/
  int	n_elevation;	/* 衛星の高度（数値）	*/
  char	elevation[12];	/* 高度: "-(xxx..xxx)"	*/
  int	n_azimuth;	/* 衛星の方位（数値）	*/
  char	azimuth[9];	/* 方位: "-(xx..xx)"	*/
  char	*situation;	/* 受信機と衛星の状態	*/
  int	n_situation;	/* 受信機と衛星の状態	*/
  char	c_situation;	/* 受信機と衛星の状態	*/
  int	signal;		/* 信号の状態		*/
} channel;

typedef struct {		/* 衛星のデータ構造体	*/
  char		version[5];	/* 受信機のバージョン	*/
  struct tm nowtm;		/* 現在時刻・日付	*/
  char		ns;		/* 北緯（N）・南緯（S）	*/
  char		ew;		/* 西経（W）・東経（E）	*/
  int		latitude_deg;	/* 緯度（度）		*/
  int		latitude_min;	/* 緯度（分）: 1/60 deg	*/
  int		latitude_sec;	/* 緯度（秒）: 1/60 min	*/
  int		latitude_frac;	/* 緯度（端数）		*/
  double	n_latitude;	/* 緯度（数値化）	*/
  int		longitude_deg;	/* 経度（度）		*/
  int		longitude_min;	/* 経度（分）: 1/60 deg	*/
  int		longitude_sec;	/* 経度（秒）: 1/60 min	*/
  int		longitude_frac;	/* 経度（端数）		*/
  double	n_longitude;	/* 経度（数値化）	*/
  int		altitude;	/* 標高			*/
  int		n_altitude;	/* 標高（数値化）	*/
  int		velocity;	/* 速度	km/h	*/
  int		direction;	/* 方位			*/
  struct tm	calctm;		/* 測定時刻・日付	*/
  char		*dop;		/* 精度（dilution of precision）*/
  int		n_dop;		/* 精度の揺らぎ？		*/
  int		measuremode;	/* 測定モード（2-D or 3-D）	*/
  char		*mapdatum;	/* 高度補正データ		*/
  channel	ch[16];		/* 衛星のチャネル（16個）	*/
  char		rest[5];	/* 不明なデータ			*/
  char		parity;		/* パリティ（either E or 0）	*/
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
