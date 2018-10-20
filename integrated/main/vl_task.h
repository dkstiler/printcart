#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_helper.h"

void vl_task_init(VL53L0X_Dev_t *dev, int cnt);
void vl_task_get_results(VL53L0X_RangingMeasurementData_t *res);
