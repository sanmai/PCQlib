#define SONY_VENDOR_ID	0x54c
#define HGR1_PRODUCT_ID 0x001d
#define HGR3_PRODUCT_ID 0x0040

#define HGR_PACKET_SIZE 8

#define HGR_POWER_ON	"!PUON\r\n"
#define HGR_POWER_OFF	"!PUOFF\r\n"
#define HGR_PC		"!PC\r\n"
#define HGR_GET_ID      "!ID\r\n"
#define HGR_LOG_CLEAR	"!MWDa,0\r\n"

#define DEVNAME		"/dev/ugen0"
#define DEVNAME_EP1	"/dev/ugen0.1"

#define HGRON 0
#define HGROFF 1

typedef unsigned char byte;

#define HGR1_Head SM01
#define HGR3_Head SM00
