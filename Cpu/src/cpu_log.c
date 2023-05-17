#include <commons/log.h>

t_log *iniciar_logger_cpu() {
  return log_create("CPU.log", "CPU", 1, LOG_LEVEL_INFO);
}
