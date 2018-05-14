#include "Data_Handler.h"



Data_Handler::Data_Handler()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		uDH[i] = new UserData();
		uDH[i]->_uData = NULL;
		uDH[i]->Logged = NULL;
	}
	ConnectionCounter = 0;
}


Data_Handler::~Data_Handler()
{
}



char * Data_Handler::serialize(_sData * data, int size) /////////////////////////change this.
{
	char* uDat = (char*)malloc(1024);
	memcpy(uDat, &data->Type, 4);
	int ssize = 4;
	int tsize = data->fU.length() + 1;
	for (int i = 0; i < tsize; i++)
		uDat[i + ssize] = data->fU[i];
	ssize += data->fU.length() + 1;
	tsize = data->fP.length() + 1;

	for (int i = 0; i < tsize; i++)
		uDat[i + ssize] = data->fP[i];

	ssize += data->fP.length() + 1;
	tsize = data->tU.length() + 1;

	for (int i = 0; i < tsize; i++)
		uDat[i + ssize] = data->tU[i];


	ssize += data->tU.length() + 1;

	for (int i = 0; i <= (size - ssize); i++)
	{
		uDat[i + ssize] = data->Buffer[i];
	}


	return uDat;
}

Data_Handler::_sData* Data_Handler::deserialize(char * data, size_t size)
{
	_sData* uDat = new _sData();
	memcpy(uDat, data, 4);
	int ssize = 4;

	for (int i = 0; data[i + 4] != '\0'; i++)
		uDat->fU += data[i + 4];
	ssize += uDat->fU.length() + 1;

	for (int i = 0; data[i + ssize] != '\0'; i++)
		uDat->fP += data[i + ssize];
	ssize += uDat->fP.length() + 1;

	for (int i = 0; data[i + ssize] != '\0'; i++)
		uDat->tU += data[i + ssize];
	ssize += uDat->tU.length() + 1;

	for (int i = 0; i < (size - ssize); i++)
		uDat->Buffer[i] = data[i + ssize];


	return uDat;
}
