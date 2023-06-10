#ifndef CPU_UTILS_H
#define CPU_UTILS_H

  #include <commons/string.h>
  #include <commons/txt.h>
  #include <estructuras.h>

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

#endif
