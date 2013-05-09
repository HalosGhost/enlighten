#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void usage(char *progname) {
	fprintf(stderr,"Usage: %s [-h|--help] <decrease|increase> <display|keyboard>\n", progname);
	exit(44);
}

void perms(char *progname) {
	fprintf(stderr,"%s must be run with root permissions\n",progname);
	exit(45);
}

void modify(char *action, char *device, char *devpath) {
	FILE *fp;
	int brightness;

	fp=fopen(devpath,"r");
	fscanf(fp,"%d",&brightness);
	fclose(fp);

	fp=fopen(devpath,"w");
	if (device[0]=='d') {
		if (action[0]=='d') { fprintf(fp,"%d",brightness-4096); }
		else if (action[0]=='i') { fprintf(fp,"%d",brightness+4096); }
	}
	else if (device[0]=='k') {
		if (action[0]=='d') { fprintf(fp,"%d",brightness-15); }
		else if (action[0]=='i') { fprintf(fp,"%d",brightness+15); }
	}
	fclose(fp);
}

int main(int argc, char** argv) {
	char dpath[] = "/sys/class/backlight/gmux_backlight/brightness";
	char kpath[] = "/sys/class/leds/smc::kbd_backlight/brightness";

	if (argv[1][0]=='d'||argv[1][0]=='i') {
		if (getuid()!=0) { perms(argv[0]); }
		else if (argv[2]) {
			if (argv[2][0]=='d') { modify(argv[1],argv[2],dpath); }
			else if (argv[2][0]=='k') { modify(argv[1],argv[2],kpath); }
			else { usage(argv[0]); }
		}
		else { usage(argv[0]); }
	}
	else { usage(argv[0]); }

	return 0;
}
