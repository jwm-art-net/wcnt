#ifndef TIMECHANGE_H
#define TIMECHANGE_H

/*
	by the time <atbar> is reached bpm will be <tobpm>
*/

class timechange
{
 public:
	timechange(short bar, double bpm, char btpb, char btval): 
 		atbar(bar), tobpm(bpm), beatperbar(btpb), beatvalue(btval){};
 	void set_bar(short bar){atbar = bar;}
	void set_bpm(short bpm){tobpm = bpm;}
	short get_bar(){ return atbar;}
	double get_bpm(){ return tobpm;}
	
	timechange & operator=(const timechange&);
	 
 private:
	short atbar;
	double tobpm;
 	char beatperbar;
 	char beatvalue;
};

#endif
