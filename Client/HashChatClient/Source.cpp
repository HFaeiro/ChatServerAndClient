#include "Initialization.h";



int main()
{
	Data_Handler* _Handle = new Data_Handler();
	Initialization* _Init = new Initialization();
	//Data_Handler::_sData* sData = new Data_Handler::_sData();
	//Data_Handler::_sData* dData = new Data_Handler::_sData();

	//sData->Type = 0;
	//sData->fU = "Mike";
	//sData->fP = "jones";
	//sData->tU = "BObby";
	//sData->Buffer = "HelloWorld";


	//char * Buffer = (char*)malloc(1024);
	//Buffer = _Handle->serialize(sData, 1024);

	//dData = _Handle->deserialize(Buffer, 1024);

	//_getch();


	_Init->Initialize(_Handle);




	return 0;
}