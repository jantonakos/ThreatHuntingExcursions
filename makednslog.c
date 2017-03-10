//Copyright (c) 2017 James L. Antonakos

//This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define FALSE 0
#define TRUE  1
#define SIMPLE 0

// Search for a specific command line parameter
int checkparm(int narg, char **args, char parm[])
{
	int p,found;

	found = FALSE;
	p = 0;
	do
	{
		if(0 == strcmp(args[p],parm))
			found = TRUE;
		else
			p++;
	} while ((p < narg) && !found);
	if(found)
		return(p);
	else
		return(0);
}

// Convert into Microsoft-format domain
// www.yahoo.com --> (3)www(5)yahoo(3)com(0)
void makemsd(char instr[], char *outstr)
{
	int j,jstart,k,p,n;
	char tempstr[6];

	k = 0;
	p = 0;
	jstart = 0;
	do
	{
		j = 0;
		do
		{
			j++;
			p++;
		} while(('.' != instr[p]) && ('\0' != instr[p]));
		sprintf(tempstr,"(%d)",j);
		for(n = 0; n < strlen(tempstr); n++)
		{
			outstr[k] = tempstr[n];
			k++;
		}
		for(n = jstart; n < p; n++)
		{
			outstr[k] = instr[n];
			k++;
		}
		p++;
		jstart = p;
	} while (p < strlen(instr));
	outstr[k] = '(';
	outstr[k+1] = '0';
	outstr[k+2] = ')';
	outstr[k+3] = '\0';
} 

// Return a country code
char *ccode(void)
{
	int cca[] = {0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,0,1,2,3,4,5,0,1,2,3,4,0,1,2,3,0,1,2,0,1,0};

	switch(cca[rand() % 45])
	{
	case 0: return ".ru"; break;  // most frequent
	case 1: return ".cn"; break;
	case 2: return ".ca"; break;
	case 3: return ".tw"; break;
	case 4: return ".bg"; break;
	case 5: return ".uk"; break;
	case 6: return ".in"; break;
	case 7: return ".mx"; break;
	case 8: return ".de"; break;  // least frequent
	}
}

// Return a country code in Microsoft format
char *ccode2(void)
{
	int cca[] = {0,1,2,3,4,5,6,7,8,0,1,2,3,4,5,6,7,0,1,2,3,4,5,6,0,1,2,3,4,5,0,1,2,3,4,0,1,2,3,0,1,2,0,1,0};

	switch(cca[rand() % 45])
	{
	case 0: return "(2)ru(0)"; break;
	case 1: return "(2)cn(0)"; break;
	case 2: return "(2)ca(0)"; break;
	case 3: return "(2)tw(0)"; break;
	case 4: return "(2)bg(0)"; break;
	case 5: return "(2)uk(0)"; break;
	case 6: return "(2)in(0)"; break;
	case 7: return "(2)mx(0)"; break;
	case 8: return "(2)de(0)"; break;
	}
}

int main(int argc, char *argv[])
{
	// Ya know what? I like using lots of variables. I try to give them descriptive names
	// but that whole lpiCount is too much. So, you'll see k, and n1, and dk among others.
	int p,k,dr;
	int hr,min,sec,subsec;
	int hrlist[] = {12,1,2,3,4,5,6,7,8,9,10,11,12,1,2,3,4,5,6,7,8,9,10,11};
	int mon,day,yr;
	int monlist[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	char months[][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	int nopci[] = {12,1,2,3,4,5,6,7,8,9,10,11};
	int datemode;
	char tmpstr[4];
	int pcimon;
	int pcimode;
	int today_day;
	int today_mon;
	int today_yr;

	time_t t;
	char timedate[32];

	int sr;
	int ipr;
	int special;
	int n1,n2;
	char exfil[41],exalpha[] = "0123456789abcdef";
	char exalpha2[] = "abcdefghijklmnopqrstuvwxyz0123456789";
	char dga[] = "abcdefghijklmnopqrstuvwxyz";
	char wlist[60][20] = {"welcome","fixture","change","monitor","sleeper","trust","upwards","against","promote","dignity",
		"immediate","after","delay","allow","combine","filter","mixture","transfer","mobile","currency",
		"extra","another","people","help","working","given","schedule","project","next","optional",
		"tool","network","server","network","assist","simple","easy","manual","install","client",
		"question","answer","best","background","sideways","wonder","machine","quick","survey","monkey",
		"sky","green","software","power","planet","universe","download","matrix","shopper","portal"};
	int new1min;
	int new10min;
	int new1hour;
	int bursty;
	int burstknt;

	// This message is base64 encoded and saved in b64str:
	// If you are reading this I assume you have been correctly identifying and decoding the different types of IoCs 
	// I have placed into the fake DNS log. My congratulations for a job well done. Of course I could be wrong, and 
	// you may never read any of this, but I choose to remain positive that you will solve my little mystery. Good 
	// luck and happy threat hunting!
	char b64str[] = "SWYgeW91IGFyZSByZWFkaW5nIHRoaXMgSSBhc3N1bWUgeW91IGhhdmUgYmVlbiBjb3JyZWN0bHkgaWRlbnRpZnlpbmcgYW"
		"5kIGRlY29kaW5nIHRoZSBkaWZmZXJlbnQgdHlwZXMgb2YgSW9DcyBJIGhhdmUgcGxhY2VkIGludG8gdGhlIGZha2UgRE5TIGxvZy4gTXkg"
		"Y29uZ3JhdHVsYXRpb25zIGZvciBhIGpvYiB3ZWxsIGRvbmUuIE9mIGNvdXJzZSBJIGNvdWxkIGJlIHdyb25nLCBhbmQgeW91IG1heSBuZX"
		"ZlciByZWFkIGFueSBvZiB0aGlzLCBidXQgSSBjaG9vc2UgdG8gcmVtYWluIHBvc2l0aXZlIHRoYXQgeW91IHdpbGwgc29sdmUgbXkgbGl0"
		"dGxlIG15c3RlcnkuIEdvb2QgbHVjayBhbmQgaGFwcHkgdGhyZWF0IGh1bnRpbmch";

	char b64[16][40] = {"V2luZG93cyA3LDMyLWJpdCw4IEdC",  // Windows 7,32-bit,8 GB
		"V2luZG93cyBYUC1TUC0xLDMyLWJpdCw0IEdC",          // Windows XP-SP-1,32-bit,4 GB
		"V2luZG93cyAyMDA4UjIsNjQgR0IsSUlT",              // Windows 2008R2,64 GB,IIS
		"V2luZG93cyAyMDEyLDMyIEdC",                      // Windows 2012,32 GB
		"QWNjdDoganNtaXRo",       // Acct: jsmith
		"QWNjdDogc3lzYWRtaW43",   // Acct: sysadmin7
		"QWNjdDogbW1vdXNl",       // Acct: mmouse
		"QWNjdDogc3FsZGJtZ250",   // Acct: sqldbmgnt
		"VklTQTEyMzQ1Njc4OTAxMjM0NTYxMDE5",   // VISA12345678901234561019
		"VklTQTc3Nzc2NjY2NTU1NTQ0NDQwMTE4",   // VISA77776666555544440118
		"TUMtOTg3NjU0MzIxMDk4NzY1NC0wNzIw",   // MC-9876543210987654-0720
		"TUMtMTEyMjMzNDQ1NTY2Nzc4OC0xMTIx",   // MC-1122334455667788-1121
		"Tm8gQVYgZXhpc3Rz",           // No AV exists
		"U29waG9zIEFWIHJ1bm5pbmcu",   // Sophos AV running.
		"QVZHZnJlZSBBViBydW5uaW5n",   // AVGfree AV running
		"RVNFVCBBViBydW5uaW5n"};      // ESET AV running
	int b64pos;
	char bmsg[49];
	int lastburst;
	int noms,ampm;
	int found;
	int verb;
	int goback,back;

	char doms[2000][80];    // This string array is filled with domains read from domains.txt
	int NDOMS;              // This is the number of domains read
	char msdoms[2000][80];  // This string array is populated by converting doms[] into Microsoft domain name format

	long int passes,lineslogged;
	int dots,megs;
	int dlen;
	char ld[12],lt[12],lh1[4],lh2[9],lsr[4],lip[16],lh3[5],lqr[4],lstat[18],ldom[81];
	char startime[32];
	char logline[180];
	FILE *ifile,*ofile;
	char fstr[80],fstr2[80];
	int pv,rv;
	char fname[128];
	char fname1[120];
	char fname2[8];
	int fknt;
	int filesplit;
	int filelimit;
	long int filesize,filemaxsize;
	long int maxlines;
	int linestop;
	int dk;
	int net10,net172,net192;
	int state;
	char malwareip10[8][16],malwareip172[8][16],malwareip192[8][16];
	int s10,s172,s192;
	int plist[6][3] = {{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
	int ppick;
	char spoofip[8][16];

	if(argc == 1)
	{
		printf("Usage: makednslog [-v] [-ofile OUTPUT-FILE] [-noms] [-ampm] [-datemode 1|2] [-goback N] [-pci] "
			"[-split N] [-lines N] [-net 172[,192][,10]]\n");
	}
	fname[0] = '\0';
	strcpy(fname,"dns.log");
	fname1[0] = '\0';
	strcpy(fname1,"dns");
	fname2[0] = '\0';
	strcpy(fname2,"log");

	srand( (unsigned)time( NULL ) );  // Make sure the logs are different every time

	printf("This program creates fake DNS logs.\n\n");

	// Process all command line parameters
	for(pv = 1; pv < argc; pv++)
		printf("p%d: %s\n",pv,argv[pv]);
	printf("Checking parameters...\n");
	
	noms = FALSE;
	if(0 != checkparm(argc,argv,"-noms"))
	{
		noms = TRUE;
		printf("   No Microsoft format.\n");
	}

	ampm = FALSE;
	if(0 != checkparm(argc,argv,"-ampm"))
	{
		ampm = TRUE;
		printf("   12-hour clock.\n");
	}

	datemode = 0;
	pv = checkparm(argc,argv,"-datemode");
	if(0 != pv)
	{
		datemode = atoi(argv[pv+1]);
		if((datemode >= 0) && (datemode <= 2))
			printf("   Date mode = %d\n",datemode);
		else
		{
			printf("Invalid datemode value.\n");
			exit(0);
		}
	}

	time(&t);
	sprintf(timedate,ctime(&t));
	yr = atoi(&timedate[strlen(timedate)-5]);
	today_yr = yr;
	goback = FALSE;
	pv = checkparm(argc,argv,"-goback");
	if(0 != pv)
	{
		goback = TRUE;
		back = atoi(argv[pv+1]);
		if (back > 1)
		{
			printf("   Going back %d months.\n",back);
			back--;
		}
		else
		{
			printf("   Must go back at least 2 months.\n");
			exit(0);
		}
	}
	
	linestop = FALSE;
	maxlines = 500000;
	pv = checkparm(argc,argv,"-lines");
	if(0 != pv)
	{
		linestop = TRUE;
		maxlines = atoi(argv[pv+1]);
		printf("   Maxlines = %ld\n",maxlines);
	}

	verb = FALSE;
	pv = checkparm(argc,argv,"-v");
	if(0 != pv)
	{
		printf("   Verbose-mode enabled.\n");
		verb = TRUE;
	}

	// How many times do you get a chance to create a state machine?
	// This code parses the -net parameter, looking for 10, 172, and/or 192
	//         "1"   "0"   "7"   "9"   "2"   ","
	// State
	//   0      1     5     5     5     5     5
	//   1      5     2     3     4     5     5  "10"
	//   2      5     5     5     5     5     0  
	//   3      5     5     5     5     2     5  "172"
	//   4      5     5     5     5     2     5  "192"
	net10 = FALSE;
	net172 = FALSE;
	net192 = FALSE;
	pv = checkparm(argc,argv,"-net");
	if(0 != pv)
	{
		state = 0;
		n1 = 0;
		do
		{
			if(verb)
				printf("state: %d, n1: %d, argv[pv+1][n]: %c\n",state,n1,argv[pv+1][n1]);
			switch(state) 
			{
			case 0: if('1' == argv[pv+1][n1])
					{
						state = 1;
						n1++;
					}
					else
						state = 5;
					break;
			case 1: if('0' == argv[pv+1][n1])
					{
						net10 = TRUE;
						state = 2;
						n1++;
					}
					else
						if('7' == argv[pv+1][n1])
						{
							state = 3;
							n1++;
						}
						else
							if('9' == argv[pv+1][n1])
							{
								state = 4;
								n1++;
							}
							else
								state = 5;
					break;
			case 2: if(',' == argv[pv+1][n1])
					{
						state = 0;
						n1++;
					}
					else
						state = 5;
					break;
			case 3: if('2' == argv[pv+1][n1])
					{
						net172 = TRUE;
						state = 2;
						n1++;
					}
					else
						state = 5;
					break;
			case 4: if('2' == argv[pv+1][n1])
					{
						net192 = TRUE;
						state = 2;
						n1++;
					}
					else
						state = 5;
					break;
			}
		} while (state != 5);
		if(0 == (net10 || net172 || net192))
		{
			printf("Error extracting network.\n");
			exit(0);
		}
		if(net10)
			printf("   Network 10.x.x.x selected.\n");
		if(net172)
			printf("   Network 172.x.x.x selected.\n");
		if(net192)
			printf("   Network 192.x.x.x selected.\n");
	}
	else
		net10 = TRUE;  // default

	// Pick network octets and populate arrays with "compromised" systems in each network
	// even if only one network is being used (because the code is easier this way).
	s10 = 1 + (rand() % 200);
	s172 = 1 + (rand() % 200);
	s192 = 1 + (rand() % 200);
	if(verb)
	{
		printf("\nAssign IP addresses to \"compromised\" systems.\n");
		printf("\tmalwareip10\tmalwareip172\tmalwareip192\n");
	}
	for(n1 = 0; n1 < 8; n1++)
	{
		sprintf(malwareip10[n1],"10.%d.%d.%d",s10,(1 + (rand() % 200)),(1 + (rand() % 250)));
		sprintf(malwareip172[n1],"172.%d.%d.%d",s172,(1 + (rand() % 200)),(1 + (rand() % 250)));
		sprintf(malwareip192[n1],"192.168.%d.%d",s192,(1 + (rand() % 250)));
		if(verb)
			printf("[%d]\t%s\t%s\t%s\n",n1,malwareip10[n1],malwareip172[n1],malwareip192[n1]);
	}

	// Make up some spoofed IPs
	if(verb)
		printf("\nCreate spoofed IPs.\n");
	for(n1 = 0; n1 < 8; n1++)
	{
		do
		{
			n2 = 4 + (rand() % 220);
		} while ((n2 == 10) && (n2 == 172) && (n2 == 192));
		sprintf(spoofip[n1],"%d.%d.%d.%d",n2,(1+(rand() % 250)),(1+(rand() % 250)),(1+(rand() % 250)));
		if(verb)
			printf("[%d] %s\n",n1,spoofip[n1]);
	}

	pcimode = FALSE;
	pv = checkparm(argc,argv,"-pci");
	if(0 != pv)
	{
		printf("   PCI 90-day log enabled.\n");
		pcimode = TRUE;
	}
	strncpy(tmpstr,&timedate[4],3);
	tmpstr[3] = '\0';
	pcimon = 0;
	found = FALSE;
	do
	{
		if(0 == strcmp(tmpstr,months[pcimon]))
			found = TRUE;
		else 
			pcimon++;
	} while ((pcimon < 12) && !found);
	if(!found)
	{
		printf("Error extracting month from timedate.\n");
		exit(0);
	}
	today_mon = 1 + pcimon;
	mon = nopci[pcimon];  // Default is one month back

	// Sorry this is complicated, but going back even one month could
	// take us to previous year. Argh!
	if(goback || pcimode)
	{
		if(pcimode)
			back = 2;
		do
		{
			if(back > 12)
			{
				back =- 12;
				yr--;
			} 
		}while (back > 11);
		do
		{
			if(back > 0)
			{
				if (mon == 1)
				{
					mon = 12;
					yr--;
				}
				else
					mon--;
				back--;
			}
		} while(back > 0);
		if(verb)
			printf("Going back to %s %d.\n",months[mon-1],yr);
	}

	tmpstr[0] = timedate[8];
	if((timedate[9] >= '0') && (timedate[9] <= '9'))
	{
		tmpstr[1] = timedate[9];
		tmpstr[2] = '\0';
	}
	else
		tmpstr[1] = '\0';
	day = atoi(tmpstr);
	today_day = day;

	// Read all domains from domains.txt and convert into Microsoft format as we go
	printf("Reading domains...\n");
	ifile = fopen("domains.txt","r");
	if(ifile == NULL)
	{
		printf("Error! Can not open domains.txt\n");
		perror(NULL);
		exit(0);
	}
	dk = 0;
	dlen = 0;
	do
	{
		fgets(fstr,80,ifile);
		p = strlen(fstr)-1;
		do
		{
			if(32 > fstr[p])
			{
				fstr[p] = '\0';
				p--;
			}
		} while(32 > fstr[p]);

		if(0 != strcmp(fstr,"/END/"))
		{
			if(verb)
				printf("%s\n",fstr);
			strcpy(doms[dk],fstr);
			makemsd(fstr,&fstr2);
			strcpy(msdoms[dk],fstr2);
			if(dlen < strlen(fstr2))
				dlen = strlen(fstr2);
			if(verb)
				printf("--> %s\n",fstr2);
		}
		dk++;
	} while((0 != strcmp(fstr,"/END/")) && (dk < 2001));
	fclose(ifile);
	dk--;
	NDOMS = dk;
	printf("%d domains read from file.\n",dk);
	if(verb)
		printf("Max MS domain length = %d\n",dlen);

	pv = checkparm(argc,argv,"-ofile");
	if(0 != pv)
	{
		fname[0] = '\0';
		strcpy(fname,argv[pv+1]);
		printf("   Output file: %s\n",fname);
	}

	ofile = fopen(fname,"w");
	if(ofile == NULL)
	{
		printf("Error! Can not open %s\n",fname);
		perror(NULL);
		exit(0);
	}

	// Go through all this nonsense to split filename into name and extension.
	// This makes it easier to add numbers to multiple log files.
	p = strlen(fname);
	found = FALSE;
	do
	{
		p--;
		if('.' == fname[p])
			found = TRUE;
	} while((p > 0) && !found);
	if(found)
	{
		p++;
		k = 0;
		do
		{
			fname2[k] = fname[p];
			k++;
			p++;
		} while('\0' != fname[p]);
		fname2[k] = '\0';
		p -= k;
		p -= 2;
		for(k = 0; k <= p; k++)
			fname1[k] = fname[k];
		fname1[p+1] = '\0';
	}
	else
	{
		strcpy(fname1,fname);
		fname2[0] = '\0';
	}
	printf("LogFile: [%s].[%s]\n",fname1,fname2);
	
	filesplit = FALSE;
	pv = checkparm(argc,argv,"-split");
	if(0 != pv)
	{
		filesplit = TRUE;
		filelimit = atoi(argv[pv+1]);
		printf("   File splitting at %d MB\n",filelimit);
		filemaxsize = 1024*1024*filelimit;
		fknt = 1;
		filesize = 0;
	}

	printf("Generating log, every . equals 100,000 lines.\n");
	
	switch(datemode)
	{
	case 0: sprintf(ld,"%d%02d%02d",yr,mon,day); break;
	case 1: sprintf(ld,"%d-%02d-%02d",yr,mon,day); break;
	case 2: sprintf(ld,"%s %d %d",months[mon-1],day,yr); break;
	}

	lt[0] = '\0';
	strcpy(lt,"20:40:00");  // I was born at 8:40 pm, ok?
	hr = 20;
	min = 40;
	sec = 0;
	if(ampm)
	{
		if(hr < 12)
			sprintf(lt,"%02d:%02d:%02d am",hrlist[hr],min,sec);
		else
			sprintf(lt,"%02d:%02d:%02d pm",hrlist[hr],min,sec);
	}
	else
		sprintf(lt,"%02d:%02d:%02d",hr,min,sec);

	sprintf(startime,"%s, %s",ld,lt);
	lh3[0] = '\0';
	strcpy(lh3,"80FD");

	passes = 0;
	dots = 0;
	megs = 0;
	dr = 0;
	new1min = FALSE;
	new10min = FALSE;
	new1hour = FALSE;
	bursty = FALSE;
	lastburst = 0;
	lineslogged = 0;

	// This begins the main log-entry generating loop.
	// Each pass through the loop generates one or more log entries.
	subsec = 2 + (rand() % 11);
	do
	{
		lstat[0] = '\0';
		strcpy(lstat,"[0001 D NOERROR]");

		lh1[0] = '\0';
		switch(rand() % 4)
		{
		case 0: strcpy(lh1,"710"); break;
		case 1: strcpy(lh1,"8A8"); break;
		case 2: strcpy(lh1,"588"); break;
		case 3: strcpy(lh1,"544"); break;
		}

		lh2[0] = '\0';
		sprintf(lh2,"%04X%04X", rand() % 4096, rand() % 65536);

		lsr[0] = '\0';

//      Not implementing Snd at this time
//      switch(rand() % 2)
		switch(0)
		{
		case 0: strcpy(lsr,"Rcv"); sr = 0; break;
		case 1: strcpy(lsr,"Snd"); sr = 1; break;
		}

		lqr[0] = '\0';
		if(sr == 0)
			strcpy(lqr,"Q");
		else
			strcpy(lqr,"R,Q");

		lip[0] = '\0';

		// Ok, ok. So I don't remember what "ipr" stood for when I named the variable.
		// But when ipr == 5, malware happens !!! Wheeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee.
		if(new1min || new10min || new1hour)  // Beacons ! (not Bacon !)
			ipr = 5;
		else
			ipr = rand() % 100;  // Well, if it wasn't 5 before, maybe it will be now.
		if(ipr == 5)
		{
			if(new1min || new10min || new1hour)
			{
				if(new1min)
				{
					special = 0;
					new1min = FALSE;
				}
				if(new10min)
				{
					special = 1;
					new10min = FALSE;
				}
				if(new1hour)
				{
					special = 2;
					new1hour = FALSE;
				}
			}
			else
				special = 3 + (rand() % 5);

			// At this point, malware rules and "special" is in the range 0..7.
			// That's eight different malware behaviors we can log.

			// See, when I first wrote this code, the only network was 10.x.x.x
			// So, I just assigned random IPs to the eight behaviors.
//			case 0: strcpy(lip,"10.161.1.5"); break;     // Domain: [a-z]{20,36}
//			case 1: strcpy(lip,"10.161.1.18"); break;    // "word"-zone domain
//			case 2: strcpy(lip,"10.161.1.58"); break;    // Domain: [a-f0-9]{32}
//			case 3: strcpy(lip,"10.161.1.102"); break;   // Domain: (p|s|t)([a-f0-9]{4}\-){3}[a-f0-9]{4}
//			case 4: strcpy(lip,"10.161.1.117"); break;   // Domain: [a-f0-9]{40}
//			case 5: strcpy(lip,"10.161.1.160"); break;   // Domain: [a-z0-9]{8,40}
//			case 6: strcpy(lip,"10.161.1.204"); break;   // base64 encoded subdomain
//			case 7: strcpy(lip,"10.161.1.241"); break;   // base64 encoded message snippet subdomain	

			// Now, with up to three networks to choose from, picking a malware IP
			// is more difficult. But I think I came up with a cool way to pick one.

			// First, look at this array: int plist[6][3] = {{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
			// It represents all six permutations of 0, 1, and 2.
			// These numbers represent the three networks (0: net10, 1: net172, 2: net192)
			// Now, we pick a number between 0 and 5 to access a 012 group. Let's pretend it is 4.
			// In the {2,0,1} group (plist[4]), we check the first value (2). If net192 is TRUE, we grab its malware IP.
			// Otherwise, we check the second value (0). If net10 is TRUE, you get the idea.
			// Last, we check the third value (1) and then test net172.
			// Note: we exit the loop when the first TRUE net variable is found.
			// All active networks (in a truly random world) have the same probability of being chosen.
			ppick = FALSE;
			n1 = rand() % 6;
			n2 = 0;
			do
			{
				switch(plist[n1][n2])
				{
				case 0: if(net10)
							{
								strcpy(lip,malwareip10[special]); 
								ppick = TRUE;
							}
						break;
				case 1: if(net172)
							{
								strcpy(lip,malwareip172[special]); 
								ppick = TRUE;
							}
						break;

				case 2: if(net192)
							{
								strcpy(lip,malwareip192[special]); 
								ppick = TRUE;
							}
						break;
				}
				n2++;
			} while (!ppick);
		}
		else
			if(ipr == 22)  // These are spoofed IPs.
				strcpy(lip,spoofip[rand() % 8]);
			else
			{
				ppick = FALSE;  // Now we just pick an ordinary, non-infected system from the available networks.
				n1 = rand() % 6;
				n2 = 0;
				do
				{
					switch(plist[n1][n2])
					{
					case 0: if(net10)
								{
									sprintf(lip,"10.%d.%d.%d",s10,(1 + (rand() % 200)),(1 + (rand() % 250)));
									ppick = TRUE;
								}
							break;
					case 1: if(net172)
								{
									sprintf(lip,"172.%d.%d.%d",s172,(1 + (rand() % 200)),(1 + (rand() % 250))); 
									ppick = TRUE;
								}
							break;
					case 2: if(net192)
								{
									sprintf(lip,"192.168.%d.%d",s192,(1 + (rand() % 250)));
									ppick = TRUE;
								}
							break;
					}
					n2++;
				} while (!ppick);
			}

		lh3[0] = '\0';
		sprintf(lh3,"%04X", rand() % 65536);

		// Generate one of eight different malware domain types.
		ldom[0] = '\0';
		if(ipr == 5)
		{
			switch(special)
			{
			case 0: n2 = 20 + (rand() % 17);  // Domain: [a-z]{20,36}
				for(n1 = 0; n1 < n2; n1++) 
					exfil[n1] = dga[rand() % 26];
				exfil[n2] = '\0';
				if (noms)
					sprintf(ldom,"%s",exfil);
				else
					sprintf(ldom,"(%d)%s",n2,exfil);
				rv = rand() % 6;
				if (noms)
					switch(rv)
					{
					case 0: strcat(ldom,".com"); break; 
					case 1: strcat(ldom,".org"); break;
					case 2: strcat(ldom,".net"); break;
					case 3: strcat(ldom,".biz"); break;
					case 4: strcat(ldom,".info"); break;
					case 5: strcat(ldom,ccode()); break;
					}
				else
					switch(rv)
					{
					case 0: strcat(ldom,"(3)com(0)"); break;
					case 1: strcat(ldom,"(3)org(0)"); break;
					case 2: strcat(ldom,"(3)net(0)"); break;
					case 3: strcat(ldom,"(3)biz(0)"); break;
					case 4: strcat(ldom,"(4)info(0)"); break;
					case 5: strcat(ldom,ccode2()); break;
					}
				lstat[0] = '\0';
				if(5 == (rand() % 20))
					strcpy(lstat,"[0001 D NOERROR]");
				else
					strcpy(lstat,"[8081 D NXDOMAIN]");
				break;
			case 1: n1 = rand() % 60;  // "word"-zone domain
				if (noms)
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"%s%04x-zone.net",wlist[n1],(rand() % 65536)); break;
					case 1: sprintf(ldom,"%s%04x-area.com",wlist[n1],(rand() % 65536));  break;
					case 2: sprintf(ldom,"%s%04x-machine.org",wlist[n1],(rand() % 65536));  break;
					case 3: sprintf(ldom,"%s%04x-service.biz",wlist[n1],(rand() % 65536));  break;
					case 4: sprintf(ldom,"%s%04x-list.de",wlist[n1],(rand() % 65536)); break;
					case 5: sprintf(ldom,"%s%04x-dump.bg",wlist[n1],(rand() % 65536)); break;
					}
				}
				else
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"(%d)%s%04x-zone(3)net(0)",(9+strlen(wlist[n1])),wlist[n1],(rand() % 65536)); break;
					case 1: sprintf(ldom,"(%d)%s%04x-area(3)com(0)",(9+strlen(wlist[n1])),wlist[n1],(rand() % 65536));  break;
					case 2: sprintf(ldom,"(%d)%s%04x-machine(3)org(0)",(12+strlen(wlist[n1])),wlist[n1],(rand() % 65536));  break;
					case 3: sprintf(ldom,"(%d)%s%04x-service(3)biz(0)",(12+strlen(wlist[n1])),wlist[n1],(rand() % 65536));  break;
					case 4: sprintf(ldom,"(%d)%s%04x-list(2)de(0)",(9+strlen(wlist[n1])),wlist[n1],(rand() % 65536)); break;
					case 5: sprintf(ldom,"(%d)%s%04x-dump(2)bg(0)",(9+strlen(wlist[n1])),wlist[n1],(rand() % 65536)); break;
					}
				}
				lstat[0] = '\0';
				if(5 == (rand() % 20))
					strcpy(lstat,"[0001 D NOERROR]");
				else
					strcpy(lstat,"[8081 D NXDOMAIN]");
				break;
			case 2: for(n1 = 0; n1 < 32; n1++) exfil[n1] = exalpha[rand() % 16];  // Domain: [a-f0-9]{32}
				exfil[32] = '\0';
				if (noms)
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"%s.daisyland.net",exfil); break;
					case 1: sprintf(ldom,"%s.shopctr.com",exfil);  break;
					case 2: sprintf(ldom,"%s.biztrack.org",exfil);  break;
					case 3: sprintf(ldom,"%s.baidv.cn",exfil);  break;
					case 4: sprintf(ldom,"%s.kronosya.ru",exfil); break;
					case 5: sprintf(ldom,"%s.bialystok.pl",exfil); break;
					}
				}
				else
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"(32)%s(9)daisyland(3)net(0)",exfil);  break;
					case 1: sprintf(ldom,"(32)%s(7)shopctr(3)com(0)",exfil);  break;
					case 2: sprintf(ldom,"(32)%s(8)biztrack(3)org(0)",exfil);  break;
					case 3: sprintf(ldom,"(32)%s(5)baidv(2)cn(0)",exfil);  break;
					case 4: sprintf(ldom,"(32)%s(8)kronosya(2)ru(0)",exfil); break;
					case 5: sprintf(ldom,"(32)%s(9)bialystok(2)pl(0)",exfil); break;
					}
				}
				break;
			case 3: for(n1 = 0; n1 < 20; n1++) exfil[n1] = exalpha[rand() % 16];  // Domain: (p|s|t)([a-f0-9]{4}\-){3}[a-f0-9]{4}
				exfil[20] = '\0';
				switch(rand() % 3)
				{
				case 0: exfil[0] = 'p'; break;
				case 1: exfil[0] = 's'; break;
				case 2: exfil[0] = 't'; break;
				}
				exfil[5]='-';
				exfil[10]='-';
				exfil[15]='-';
				if (noms)
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"%s.badguy.com",exfil); break;
					case 1: sprintf(ldom,"%s.secret.org",exfil); break;
					case 2: sprintf(ldom,"%s.hidden.net",exfil); break;
					case 3: sprintf(ldom,"%s.sinkhole.biz",exfil); break;
					case 4: sprintf(ldom,"%s.zhishubao.cn",exfil); break;
					case 5: sprintf(ldom,"%s.click.in",exfil); break;
					}
				}
				else
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"(20)%s(6)badguy(3)com(0)",exfil); break;
					case 1: sprintf(ldom,"(20)%s(6)secret(3)org(0)",exfil); break;
					case 2: sprintf(ldom,"(20)%s(6)hidden(3)net(0)",exfil); break;
					case 3: sprintf(ldom,"(20)%s(8)sinkhole(3)biz(0)",exfil); break;
					case 4: sprintf(ldom,"(20)%s(9)zhishubao(2)cn(0)",exfil); break;
					case 5: sprintf(ldom,"(20)%s(5)click(2)in(0)",exfil); break;
					}
				}
				break;			
			case 4: for(n1 = 0; n1 < 40; n1++) exfil[n1] = exalpha[rand() % 16];   // Domain: [a-f0-9]{40}
				exfil[40] = '\0';
				if (noms)
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"%s.jennyhaden.com",exfil); break;
					case 1: sprintf(ldom,"%s.starsearch.net",exfil); break;
					case 2: sprintf(ldom,"%s.allexpensespaid.biz",exfil); break;
					case 3: sprintf(ldom,"%s.dosvidanya.ru",exfil); break;
					case 4: sprintf(ldom,"%s.doubleclack.org",exfil); break;
					case 5: sprintf(ldom,"%s.spy.ro",exfil); break;
					}
				}
				else
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"(40)%s(10)jennyhaden(3)com(0)",exfil); break;
					case 1: sprintf(ldom,"(40)%s(10)starsearch(3)net(0)",exfil); break;
					case 2: sprintf(ldom,"(40)%s(15)allexpensespaid(3)biz(0)",exfil); break;
					case 3: sprintf(ldom,"(40)%s(10)dosvidanya(2)ru(0)",exfil); break;
					case 4: sprintf(ldom,"(40)%s(11)doubleclack(3)org(0)",exfil); break;
					case 5: sprintf(ldom,"(40)%s(3)spy(2)ro(0)",exfil); break;					}
				}
				break;
			case 5: n2 = 8 + (rand() % 33);   // Domain: [a-z0-9]{8,40}
				for(n1 = 0; n1 < n2; n1++) exfil[n1] = exalpha2[rand() % 36];
				exfil[n2] = '\0';
				if (noms)
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"%s.oldbooks.org",exfil); break;
					case 1: sprintf(ldom,"%s.targit.com",exfil); break;
					case 2: sprintf(ldom,"%s.stapels.net",exfil); break;
					case 3: sprintf(ldom,"%s.bestbye.biz",exfil); break;
					case 4: sprintf(ldom,"%s.izvestiia.ru",exfil); break;
					case 5: sprintf(ldom,"%s.huitek.cn",exfil); break;
					}
				}
				else
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"(%d)%s(8)oldbooks(3)org(0)",n2,exfil); break;
					case 1: sprintf(ldom,"(%d)%s(6)targit(3)com(0)",n2,exfil); break;
					case 2: sprintf(ldom,"(%d)%s(7)stapels(3)net(0)",n2,exfil); break;
					case 3: sprintf(ldom,"(%d)%s(7)bestbye(3)biz(0)",n2,exfil); break;
					case 4: sprintf(ldom,"(%d)%s(9)izvestiia(2)ru(0)",n2,exfil); break;
					case 5: sprintf(ldom,"(%d)%s(6)huitek(2)cn(0)",n2,exfil); break;
					}
				}
				break;
			case 6: b64pos = rand() % 16;   // base64 encoded subdomain
				if (noms)
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"%s.petraplace.net",b64[b64pos]); break;
					case 1: sprintf(ldom,"%s.shopctr.com",b64[b64pos]);  break;
					case 2: sprintf(ldom,"%s.biztrack.org",b64[b64pos]);  break;
					case 3: sprintf(ldom,"%s.googel.tw",b64[b64pos]);  break;
					case 4: sprintf(ldom,"%s.yaho.com",b64[b64pos]); break;
					case 5: sprintf(ldom,"%s.merhaba.tr",b64[b64pos]); break;
					}
				}
				else
				{
					switch(rand() % 6)
					{
					case 0: sprintf(ldom,"(%d)%s(10)petraplace(3)net(0)",strlen(b64[b64pos]),b64[b64pos]);  break;
					case 1: sprintf(ldom,"(%d)%s(7)shopctr(3)com(0)",strlen(b64[b64pos]),b64[b64pos]);  break;
					case 2: sprintf(ldom,"(%d)%s(8)biztrack(3)org(0)",strlen(b64[b64pos]),b64[b64pos]);  break;
					case 3: sprintf(ldom,"(%d)%s(6)googel(2)tw(0)",strlen(b64[b64pos]),b64[b64pos]);  break;
					case 4: sprintf(ldom,"(%d)%s(4)yaho(3)com(0)",strlen(b64[b64pos]),b64[b64pos]); break;
					case 5: sprintf(ldom,"(%d)%s(7)merhaba(2)tr(0)",strlen(b64[b64pos]),b64[b64pos]); break;
					}
				}
				break;
			case 7: if(1200 <= (passes - lastburst))   // base64 encoded message snippet subdomain trigger code
					{
						bursty = TRUE;
						burstknt = 5 + (rand() % 10);
						lastburst = passes;
					}
					else  // Or pick an ordinary domain
					{
						if(noms)
							strcpy(ldom,doms[rand() % NDOMS]);  
						else
							strcpy(ldom,msdoms[rand() % NDOMS]);
					}
					break;
			}
		}
		else  // Or pick an ordinary domain
		{
			if(noms)
				strcpy(ldom,doms[rand() % NDOMS]);  
			else
				strcpy(ldom,msdoms[rand() % NDOMS]);
		}

		if(!bursty)
		{
			if(!SIMPLE)
			{
				fprintf(ofile,"%s %s %s PACKET %s UDP %s %s %s %s %s A %s\n",ld,lt,lh1,lh2,lsr,lip,lh3,lqr,lstat,ldom);
				sprintf(logline,"%s %s %s PACKET %s UDP %s %s %s %s %s A %s\n",ld,lt,lh1,lh2,lsr,lip,lh3,lqr,lstat,ldom);
				filesize += strlen(logline);
				lineslogged++;
			}
			else
			{
				fprintf(ofile,"%s %s %s %s %s\n",ld,lt,lip,lstat,ldom);
				lineslogged++;
			}
		}
		else  // base64 encoded message snippet subdomain burst generation
		{
			rv = rand() % 6;
			for(n1 = 0; n1 < burstknt; n1++)
			{
				ldom[0] = '\0';
				bmsg[0] = '\0';
				b64pos = 4 * (rand() % ((strlen(b64str) - 48)/4));
				for(n2 = 0; n2 < 48; n2++)
					bmsg[n2] = b64str[b64pos+n2];
				bmsg[48] = '\0';
				if (noms)
				{
					switch(rv)
					{
					case 0: sprintf(ldom,"%s.axa.biz",bmsg); break;
					case 1: sprintf(ldom,"%s.colorpix.org",bmsg); break;
					case 2: sprintf(ldom,"%s.windosupdate.com",bmsg); break;
					case 3: sprintf(ldom,"%s.travel.id",bmsg); break;
					case 4: sprintf(ldom,"%s.kouponcash.net",bmsg); break;
					case 5: sprintf(ldom,"%s.zelora.sg",bmsg); break;
					}
				}
				else
				{
					switch(rv)
					{
					case 0: sprintf(ldom,"(48)%s(3)axa(3)biz(0)",bmsg); break;
					case 1: sprintf(ldom,"(48)%s(8)colorpix(3)org(0)",bmsg); break;
					case 2: sprintf(ldom,"(48)%s(12)windosupdate(3)com(0)",bmsg); break;
					case 3: sprintf(ldom,"(48)%s(6)travel(2)id(0)",bmsg); break;
					case 4: sprintf(ldom,"(48)%s(10)kouponcash(3)net(0)",bmsg); break;
					case 5: sprintf(ldom,"(48)%s(6)zelora(2)sg(0)",bmsg); break;
					}
				}
				if(!SIMPLE)
				{
					fprintf(ofile,"%s %s %s PACKET %s UDP %s %s %s %s %s A %s\n",ld,lt,lh1,lh2,lsr,lip,lh3,lqr,lstat,ldom);
					sprintf(logline,"%s %s %s PACKET %s UDP %s %s %s %s %s A %s\n",ld,lt,lh1,lh2,lsr,lip,lh3,lqr,lstat,ldom);
					filesize += strlen(logline);
					lineslogged++;
				}
				else
				{
					fprintf(ofile,"%s %s %s %s %s\n",ld,lt,lip,lstat,ldom);
					lineslogged++;
				}
			}
			bursty = FALSE;
		}

		// Check need to split log file
		if((filesize > filemaxsize) && filesplit)
		{
			fclose(ofile);
			if(0 == strlen(fname2))
				sprintf(fname,"%s%d",fname1,fknt);
			else
				sprintf(fname,"%s%d.%s",fname1,fknt,fname2);
			ofile = fopen(fname,"w");
			if(ofile == NULL)
			{
				printf("Error! Can not open %s\n",fname);
				perror(NULL);
				exit(0);
			}
			fknt++;
			filesize = 0;
		}

		// At this point we have generated and written one or more log entries during the current second.
		// A burst will cause multiple log entries.

		// Now we see if we have used up all the sub-seconds for the current second.
		// Even a moderatly large network will exhibit multiple DNS requests each second.
		subsec--;
		if(subsec == 0)
		{
			sec++;
			subsec = 2 + (rand() % 11);
			if(sec == 15)
				new1min = TRUE;   // 1-minute beacons occur at the 15-second mark
			if(sec == 30)
			{
				if(0 == (min % 10))
					new10min = TRUE;   // 10-minute beacons occur at the 30-second mark
			}
			if(sec == 60)
			{
				sec = 0;
				min++;
				if(min == 60)
				{
					min = 0;
					hr++;
					new1hour = TRUE;   // 1-hour beacons occur at the 0-second mark
					if(hr == 24)
					{
						hr = 0;
						day++;
						if(day > monlist[mon])
						{
							day = 1;
							mon++;
							if(mon > 12)
							{
								mon = 1;
								yr++;
							}
						}
						switch(datemode)
						{
						case 0: sprintf(ld,"%d%02d%02d",yr,mon,day); break;
						case 1: sprintf(ld,"%d-%02d-%02d",yr,mon,day); break;
						case 2: sprintf(ld,"%s %d %d",months[mon],day,yr); break;
						}
						if(verb)
							printf("\n%s\n",ld);
					}
				}
			}
			if(ampm)
			{
				if(hr < 12)
					sprintf(lt,"%02d:%02d:%02d am",hrlist[hr],min,sec);
				else
					sprintf(lt,"%02d:%02d:%02d pm",hrlist[hr],min,sec);
			}
			else
				sprintf(lt,"%02d:%02d:%02d",hr,min,sec);
		}
		passes++;

		// Give the user something to look at and see signs of life when large log files are being generated.
		if(0 == (passes % 100000))
		{
			printf(".");
			dots++;
			if(dots == 10)
			{
				dots = 0;
				printf(" ");
				megs++;
				if(0 == (megs % 10))
					printf("\n%dM\n",megs);
			}
		}
	} while (((passes < maxlines) && linestop) || ((!linestop) && ((mon != today_mon) || (day != today_day) || (yr != today_yr))));
	printf("\n");
	printf("Start date/time: %s\n",startime);
	printf("Final date/time: %s, %s\n",ld,lt);
	printf("%ld lines logged.\n",lineslogged);
	fclose(ofile);
	exit(1);
}
