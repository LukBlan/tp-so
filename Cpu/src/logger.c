#include "../include/logger.h"
#include <commons/string.h>

t_log* crearLogger(char* modulo) {
	char* nombreArchivo = string_from_format("%s.log", modulo);
	return log_create(nombreArchivo, modulo, true, LOG_LEVEL_INFO);
}
