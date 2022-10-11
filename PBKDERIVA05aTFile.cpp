///////////////////////////////
//CRIPTO HEADERS
///////////////////////////////
#include "lib/secp256k1Class3.h" 	// depende de <boost/multiprecision/cpp_int.hpp>

//#include "lib/colADDRc.h" 		// colADDRc.h depende de ripemd160NEWClass.h com <string.h>
#include "lib/hexchar.h"

#include <iostream>
#include <fstream>
///////////////////////////////
///////////////////////////////


////////////////////////////////////////////////////////////////////
//CONVERTE AS CHAVE PRIVADA DE HEX PARA BIGINT
////////////////////////////////////////////////////////////////////

cpp_int chBN(unsigned char chA[])
{
	//unsigned char chA[64], cSHA[33], cSHA2[65];
	cpp_int ichA, tA;

	//converte a chave A em inteiro
	tA = 1;
	ichA = 0;
	for (int i = 63; i>= 0; i--)
	{
		ichA = ichA + tA*hexint (chA[i]); 
		tA = tA*16;
	}
	return (ichA);
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//CLASSE PARA EXECUCAO DAS BUSCAS PELOS ENDERECOS
////////////////////////////////////////////////////////////////////

int naddr;
unsigned char rmd160[10000][20];
int tFlag = 1;
cpp_int cont =0;
cpp_int pointsPBKderiva[100000];


Secp256k1 ECTT;


class pbkColl
{
	public:
	int tNumber, testString;
	cpp_int ichA, ntries;//, Gx, Gy, p;
	cpp_int pvtKeyTest, keyOffSet;

	cpp_int Px, Py;	

	cpp_int m;//para ser usando na soma dos pontos da EC

	int pbkDeriva()
	{
		//cpp_int torder = -1;
		//cpp_int nPBK[10000][2];

		////////////////////////////////////////////////////////////////////////////////////////////////
		cpp_int r, t, aux, newr, newt; // variaveis para o calculo do inverso de um numero no campo p
		////////////////////////////////////////////////////////////////////////////////////////////////

		Secp256k1 EC;


		///////////////////////////////////////////////////////////////////////////////////////////
		// Inicia a variaveis da curva eliptica
		// secp256k1
		///////////////////////////////////////////////////////////////////////////////////////////
		EC.initGvar();
		// Gx = EC.Gx;
		// Gy = EC.Gy;
		// p = EC.p;
		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////



		///////////////////////////////////////////////////////////////////////////////////////////
		// Verifica se a chave publica a ser usada peternce a curva eliptica em questao
		///////////////////////////////////////////////////////////////////////////////////////////


		if (EC.pointVerify(Px, Py) == 0)
		{
			std::cout << "Ponto Nao pertence a curva" << std::endl;
			return (-1);
		}
		else
		{
			std::cout << "Ponto pertence a curva" << std::endl;
		}
		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////


		EC.initGvarNewPoint(Px, Py);

		cpp_int *g, gn[2], gn_anth[2];



		///////////////////////////////////////////////////////////////////////////////////////////
		//Inicializacao do DB de pontos Px derivados
		///////////////////////////////////////////////////////////////////////////////////////////

		for(int i = 0; i < 100000; i++)
		{
			
			pointsPBKderiva[i] = 0;
				
		}

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////
		// Determina a quantidade de chaves que serao derivadas a partir da chave publica original
		// A curva tem simetrica em relação ao ponto central de inicio e de meio
		//
		// Px(PVTKey) onde a chave priva PVTKey eh qualquer numero entre 1 e (n_order - 1) 
		// Px(1) = Px(n_order - 1)
		// Px(n_order/2) = Px((n_order/2) + 1)
		//
		// Px(0) = Px(n_order) = (0, 0)
		//
		// se slices == 4 teremos 3 chaves: 1; n_order/4; 2(n_order/4)
		//	1 => 1 e n_order/2
		//	2 => 1; n_order/4; 2(n_order/4)
		//	4 => 1; n_order/8; 2(n_order/8); 3(n_order/8); 4(n_order/8) 
		//	8 => 1; n_order/16; 2(n_order/16); 3(n_order/16); 4(n_order/16); 5(n_order/16); 6(n_order/16); 7(n_order/16); 8(n_order/16)
		///////////////////////////////////////////////////////////////////////////////////////////

		ichA = 1;
		//int  slices = 8; //4
		//int  slices = 16; //8
		//int slices = 512; //256	
		int  slices = 1024; //512
		//int  slices = 4096; // 2048
		//int  slices = 2*2*4096; // 8k
		//int  slices = 2*2*2*4096; // 16k
		//int  slices = 2*2*2*2*4096; // 32k

		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////




		///////////////////////////////////////////////////////////////////////////////////////////
		// Deriva os Pontos da Curva a Partir da Chave Publica (PBK1) com PVTKey desconhecida
		// igualmente espacados ateh a medade da cuva n_order/2
		// com a inclusão de ambos os extremos: 1*PBK1 e (n_order/2)*PBK1
		///////////////////////////////////////////////////////////////////////////////////////////

		g = EC.secp256k1 (ichA, EC.Gx, EC.Gy);
		gn[0] = g[0];
		gn[1] = g[1];

		pointsPBKderiva[0] = gn[0];

		std::cout << "Px: " << std::endl<< gn[0] << std::endl;
		std::cout << "Py: " << std::endl<< gn[1] << std::endl<< std::endl;


		ichA = 0;	
		int middleP = 0;

	  	std::ofstream pXfile;
	  	pXfile.open ("pxFile.txt");

	  	pXfile << "std::string Xpoints01[100000] = {\n";
		pXfile << "\"" << pointsPBKderiva[0] << "\"" << ",\n";
//		pXfile << EC.dectoint(Xpoints01[0]) << ",\n";
//		Xpoints01[0] = "";//esvazia a variável

	  

		for(int i = 1; i <= slices/2; i++)
		{
			
			ichA = EC.modp((ichA + EC.n_order/slices), EC.p);

			//cpp_int *g, gn[2];
			g = EC.secp256k1 (ichA, EC.Gx, EC.Gy);
			gn[0] = g[0];
			gn[1] = g[1];

			pointsPBKderiva[i] = gn[0];
			pXfile << "\"" << gn[0] << "\"" << ",\n";
//			pXfile << EC.dectoint(Xpoints01[i]) << ",\n";
//			Xpoints01[i] = "";//esvazia a variável

			//std::cout << "Px: " << gn[0] << std::endl;
			std::cout << "Point: " << gn[0] << ": " << i << " de " << slices/2 << std::endl;
			//std::cout << "Py: " << gn[1] << std::endl;
		}

		pXfile << "\"EOF\" };";
		pXfile.close();
		///////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////
	}
};



////////////////////////////////////////////////////////////////////
//Thread Unica 2022
////////////////////////////////////////////////////////////////////

unsigned int singleTHRD(void* data)
{
	//threadSTC *tspec = (threadSTC*) data;
	pbkColl *tspec = (pbkColl*) data;
	//tspec->mT();
	tspec->pbkDeriva();
	return (0);
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
//Funcao MAIN para Iniciar o Sistema
////////////////////////////////////////////////////////////////////

int main()
{
	Secp256k1 EC;  // Variavel necessaria para utilizar parametros da secp256k1
	EC.initGvar(); // Inicia a variaveis da curva eliptica
	int nthreads;

	//Formato SEC precisa ter 64 bytes cada coordenada ttem 32 bytes
	//Verificar o Formato de Minimo 32 bytes na carteira
	//Format SEC 64x2 + 2 = 130 characteres

	//Test key: 7265ED413FFAFBEC04DE374B2D1BE4161EB036E7C9FE1BD1E27EA5D2B05E1CFE
	//Test key - E: 7265ED413FFAFBEC04DE374B2D1BE4161EB036E7C9FE1BD1E27EA5D2B05E1CF0
	//PK TESTE    "049F3D6E36E39CBE628AF9027516D4969C8834F42D5E8E84EA944DFAA9D4F5B1CBD8AFC41DE82C6B1EA4F7EA682BE1DF3D64BFC51399489AC3BBD11DBE0146C51C"

	unsigned char PUBKEY[] = "0411db93e1dcdb8a016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5cb2e0eaddfb84ccf9744464f82e160bfa9b8b64f9d4c03f999b8643f656b412a3";
	unsigned char Px[] = "11db93e1dcdb8a016b49840f8c53bc1eb68a382e97b1482ecad7b148a6909a5c";
	unsigned char Py[] = "b2e0eaddfb84ccf9744464f82e160bfa9b8b64f9d4c03f999b8643f656b412a3";

	//std::cout << "Number of Threads (UP TO 10): "; std::cin >> nthreads;
	std::cout << "Input PUBKEY: ";
	for (int j = 0; j < 130; j++) std::cin >> PUBKEY[j];

	for (int j = 2; j < 66; j++) Px[j-2] = PUBKEY[j];
	for (int j = 66; j < 130; j++) Py[j-66] = PUBKEY[j];


	//threadSTC tspec[nthreads]; // Instacia para manipular threads
	pbkColl tspec;

	unsigned char a[] = "038109007313a5807b2eccc082c8c3fbb988a973cacf1a7df9ce725c31b14776";

//	std::cout << "Input an string up to 64 characteres (0-9 and A-F) (DO NOT INPUT MORE OR LESS THAN 64 CHAR): ";
//	for (int j = 0; j < 64; j++) std::cin >> a[j];

	tspec.Px = chBN(Px);
	tspec.Py = chBN(Py);	

	std::cout << "Px: "<< Px << "\n";
	std::cout << "PxBI: "<< tspec.Px << "\n";
	std::cout << "Py: "<< Py << "\n";
	std::cout << "PyBI: "<< tspec.Py << "\n";
	//std::cin >> tspec[0].ntries;
//	std::cout << "Num Tentativas: ";
//	std::cin >> tspec.ntries;

	//tspec.ntries = 1;
 
	//CONVERTE A CHAVE PRIVADA PARA BIGINT
	//tspec[0].ichA = chBN(a);
	tspec.ichA = chBN(a);
	tspec.pvtKeyTest = tspec.ichA;
	//tspec[0].tNumber = 0;
	tspec.tNumber = 0;


	singleTHRD((void*)&tspec);


	std::cin.get();
	std::cin.get();

	return (0);
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

