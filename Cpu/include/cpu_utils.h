#ifndef CPU_UTILS_H_
#define CPU_UTILS_H_

#include <socket/cliente.h>
#include <commons/string.h>
#include <commons/txt.h>

union Valor {
	uint32_t cuatro_bytes;
	uint64_t ocho_bytes;
	uint32_t dieciseis_bytes [4];
};

typedef struct {
	char nombre[20];
	union Valor valor;
} Registro;

extern Registro AX;
extern Registro BX;
extern Registro CX;
extern Registro DX;

extern Registro EAX;
extern Registro EBX;
extern Registro ECX;
extern Registro EDX;

extern Registro RAX;
extern Registro RBX;
extern Registro RCX;
extern Registro RDX;

extern Registro registros;

Registro buscar_registro(char nombre_registro);

void setear_valor_registro(char nombre_registro,char valor);

TipoInstruccion obtener_tipo_instruccion(char *instruccion);

#endif /* CPU_UTILS_H_ */
