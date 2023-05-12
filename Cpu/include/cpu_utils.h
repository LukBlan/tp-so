#ifndef CPU_UTILS_H_
#define CPU_UTILS_H_

#include <socket/cliente.h>
#include <commons/string.h>
#include <commons/txt.h>
union Valor{
	uint32_t cuatro_bytes;
	uint64_t ocho_bytes;
	uint32_t dieciseis_bytes [4];
};
struct Registro{
	char nombre[20];
	union Valor valor;
};
extern struct Registro AX;
extern struct Registro BX;
extern struct Registro CX;
extern struct Registro DX;

extern struct Registro EAX;
extern struct Registro EBX;
extern struct Registro ECX;
extern struct Registro EDX;

extern struct Registro RAX;
extern struct Registro RBX;
extern struct Registro RCX;
extern struct Registro RDX;

extern struct Registro registros;

struct Registro buscar_registro (char nombre_registro);

void setear_valor_registro(char nombre_registro,char valor);

TipoInstruccion obtener_tipo_instruccion(char *instruccion);

#endif /* CPU_UTILS_H_ */
