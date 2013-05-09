#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void usage(char *progname) {
	fprintf(stderr,"Usage: %s [-h|--help] <decrease|increase> <display|keyboard>\n", progname);
	exit(44);
}

void modify(char *action, char *device, char *devpath) {
	FILE *fp;
	int brightness;

	fp=fopen(devpath,"r");
	fscanf(fp,"%d",&brightness);
	fclose(fp);

	fp=fopen(devpath,"w");
	if (strcmp(action,"increase")) { fprintf(fp,"%d",brightness+(brightness+1)*2); }
	else if (strcmp(action,"decrease")) { fprintf(fp,"%d",brightness-(brightness/2)); }
	fclose(fp);
}

main(int argc, char** argv) {
	int i;
	char dpath[] = "/sys/class/backlight/gmux_backlight/brightness";
	char kpath[] = "/sys/class/leds/smc::kbd_backlight/brightness";

	for (i=1;i < argc; i++) {
		if (strcmp(argv[i],"decrease")) {
			if (strcmp(argv[i+1],"display")) {
				modify(argv[i],argv[i+1],dpath);
			}
			else if (strcmp(argv[i+1],"keyboard")) {
				modify(argv[i],argv[i+1],kpath);
			}
		}
		else if (strcmp(argv[i],"increase")) {
			if (strcmp(argv[i+1],"display")) {
				modify(argv[i],argv[i+1],dpath);
			}
			else if (strcmp(argv[i+1],"keyboard")) {
				modify(argv[i],argv[i+1],kpath);
			}
		}
		else { usage(argv[0]); }
	}
}
