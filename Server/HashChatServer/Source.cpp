#include "Initialization.h"




int main()
{
	Data_Handler* _Handle = new Data_Handler();
	Initialization* Init = new Initialization();


	Init->Initialize(_Handle);


	return 0;
}