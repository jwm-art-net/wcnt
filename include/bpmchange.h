#ifndef BPMCHANGE_H
#define BPMCHANGE_H

/*
	by the time <atbar> is reached bpm will be <tobpm>
*/

class bpmchange
{
 public:
	bpmchange(short bar, double bpm): atbar(bar), tobpm(bpm){};
 	void set_bar(short bar){atbar = bar;}
	void set_bpm(short bpm){tobpm = bpm;}
	short get_bar(){ return atbar;}
	double get_bpm(){ return tobpm;}
	
	bpmchange & operator=(const bpmchange&);
	 
 private:
	short atbar;
	double tobpm;
};

#endif
