#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"pcq_power.h"

int hgr_power(int onoff, char *devname);

main(int argc, char **argv){
  char devname[254];

  // devname_ptr=devname;
  //  devname_ptr = malloc(sizeof(devname));
  //  bzero(devname, sizeof(devname));
  strcpy(devname,DEVNAME);

  while(argc--){
	*(argv++);
	if(*argv==NULL){
	  printf("Usage: %s [-dev file...][-on][-off]\n", argv[-1]);
	  break;
	}
	else if((strcmp(*argv,"-dev"))==0){
	  // printf("%s\n",*(argv+1));
	  strcpy(devname,*(argv+1));
	  *(argv++);
	  --argc;
	}
	else if ((strcmp(*argv,"-on"))==0){
	  hgr_power(HGRON,devname);
	  break;
	}
	else if((strcmp(*argv,"-off"))==0){
	  hgr_power(HGROFF,devname);
	  break;
	}
	else{
	  printf("Usage: %s [-dev file...][-on][-off]\n",argv[-1]);
	  break;
	}
  }
  return 0;
}
