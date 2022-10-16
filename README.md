"# pbkderiva" 

Este projeto cria um arquivo com um quantidade de chaves derivadas na curva eliptica a partir de uma chave conhecida.

A variável slices define o número de chaves a ser gerada em cada arquivo.

		int  slices = 2*2*2*2*4096; // 32k

Para compilar o projeto é preciso colocar a biblioteca boost na pasta include do MinGW, para o uso de Big Integers

O nome do arquivo gerado é definido manualmente no código:

	  	std::ofstream pXfile;
	  	pXfile.open ("pxFile.txt");
      
Para utilizar o arquivo gerado no projeto PBKSEARCH, é preciso mudar manulamente o nome do arquivo para o nome definido no código do projeto PBKSEARCH.

