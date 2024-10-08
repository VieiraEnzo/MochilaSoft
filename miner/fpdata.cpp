/*----------------------------------
File     : data.cpp
Contents : data set management
----------------------------------*/

#include "fpdata.h"
#include "fpcommon.h"

void Transaction::DoubleTrans(int item)
{
	int* temp = new int[2*item];
	maxlength = 2*item;
	for(int i=0; i<length; i++)
		temp[i] = t[i];
	delete []t;
	t = temp;
}

Data::Data(char *filename)
{
#ifndef BINARY
  in = fopen(filename,"rt");
#else
  in = fopen(filename, "rb");
#endif
}

Data::~Data()
{
  if(in) fclose(in);
}

int Data::isOpen()
{
  if(in) return 1;
  else return 0;
}

Transaction *Data::getNextTransaction(Transaction* Trans)
{
	Trans->length = 0;

  // read list of items
#ifndef BINARY	  
	char c;
	int item, pos;
	do {
		item=0;
		pos=0;
		c = getc(in);
		while((c >= '0') && (c <= '9')) {
			item *=10;
			item += int(c)-int('0');
			c = getc(in);
			pos++;
		}
		if(pos)
		{
			if(Trans->length >= Trans->maxlength)
				Trans->DoubleTrans(Trans->length);

			Trans->t[Trans->length] = item;
			Trans->length++;
		}
	}while(c != '\n' && !feof(in));
	// if end of file is reached, rewind to beginning for next pass
	if(feof(in)){
		rewind(in);
		return 0;
	}
	// Note, also last transaction must end with newline, 
	// else, it will be ignored
#else
	int i, nitem, *buffer=new int;
	fread((char*)buffer, sizeof(int), 1, in);
	if(feof(in))
	{
	    rewind(in);
		return 0;
	}
	fread((char*)buffer, sizeof(int), 1, in);
	fread((char*)buffer, sizeof(int), 1, in);
	nitem=*buffer;
	for(i=0; i<nitem; i++)
	{
		fread((char*)buffer, sizeof(int), 1, in);

		if(Trans->length >= Trans->maxlength)
			Trans->DoubleTrans(Trans->length);

		Trans->t[Trans->length] = *buffer;
		Trans->length++;
	}
#endif
  
	return Trans;
}
