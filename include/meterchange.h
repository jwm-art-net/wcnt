#ifndef METERCHANGE_H
#define METERCHANGE_H


class meterchange
{
 public:
	meterchange(short br, char btpb, char btval): 
 		bar(br), beatperbar(btpb), beatvalue(btval){};
 	void set_bar(short br){bar = bar;}
	void set_beatsperbar(char bpb){beatperbar = bpb;}
	void set_beatvalue(char bv){beatvalue = bv;}
	short get_bar(){ return bar;}
	char get_beatsperbar(){return beatperbar;}
	char get_beatvalue(){return beatvalue;}
	
	meterchange & operator=(const meterchange&);
	 
 private:
	short bar;
 	char beatperbar;
 	char beatvalue;
};

#endif
