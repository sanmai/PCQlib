#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <dev/usb/usb.h>
#include "pcq_power.h"

#define DEBUG

int
hgr_emit_order(int fd, byte *command)
{
    int len;
    byte buf[HGR_PACKET_SIZE + 1];
    byte *p;
    struct usb_ctl_request ur;

    p = command;
    len = strlen(command);

    while(len > 0) {
        /* Chop the command with HGR_PACKET_SIZE */
        bzero(buf, sizeof(buf));
        sprintf(buf, "%.8s", p);

        ur.request.bmRequestType = UT_WRITE_CLASS_INTERFACE;	/* 0x21 */
        ur.request.bRequest = 0x09;

        USETW(ur.request.wValue, 0x0100);
        USETW(ur.request.wIndex, 0);
        USETW(ur.request.wLength, HGR_PACKET_SIZE);
        ur.data = buf;
        ur.flags = 0;
        ur.actlen = 0;
  
        if(ioctl(fd, USB_DO_REQUEST, &ur) < 0) {
            perror("Failed hgr_emit_order");
            return -1;
        }
        p += HGR_PACKET_SIZE;
        len -= HGR_PACKET_SIZE;
    }
    return 0;
}

int
hgr_power(int onoff, char *n_devname)
{
    byte *d_name = DEVNAME;
    int cep;	/* Control EndPoint */
    int error = 0;

    struct usb_device_info udi;

	/* if new device name are given... change it!! */
	if(n_devname)
	  d_name=n_devname;

    if ((cep = open(d_name, O_RDWR)) < 0) {
        perror(d_name);
    }

    if (ioctl(cep, USB_GET_DEVICEINFO, &udi) < 0) {
        perror("USB_GET_DEVICEINFO");
        exit(1);
    }

#if 0
    printf("vendor no = 0x%x, product no = 0x%x, rev %s, addr 0x%x\n",
        udi.vendorNo, udi.productNo, udi.release, udi.addr);
#endif

    if ((udi.vendorNo != SONY_VENDOR_ID) ||
        ((udi.productNo!=HGR1_PRODUCT_ID)&&(udi.productNo!=HGR3_PRODUCT_ID))) {
        fprintf(stderr,
                "Unexpected USB device: vendor ID 0x%x product ID 0x%x\n",
                udi.vendorNo, udi.productNo);
    }

	if (onoff==HGRON){
	  error = hgr_emit_order(cep, HGR_POWER_ON);
	}
	else if(onoff==HGROFF){
	  error = hgr_emit_order(cep, HGR_POWER_OFF);
	}
	else{
	  perror("HGR power must be defined by HGRON or HGROFF\n");
	  exit(-1);
	}

	if(close(cep))
	  perror("close endpoint fail");

	return(error);
}
