#pragma once

#define numcliente 100;
#define TAMNOME 50

typedef enum{
	Regular,
	Esporadico,
	Ocasional
}TipoCliente;

typedef struct {
	int numerocliente;
	char nome[TAMNOME];
	int nif;
	float valorcompras;
	TipoCliente tipocliente;
	int datadia; datames; dataano;
}TCAutomovel;

typedef struct {
	TCAutomovel dados;
	NovoCliente* proximo;
}NovoCliente;