#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_helper.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include <assert.h>

static TaskHandle_t *task;
static VL53L0X_Dev_t *vl_dev;
static QueueHandle_t readQueue;
static int no_tasks;
static volatile int complete_cnt;
static VL53L0X_RangingMeasurementData_t *meas;

static void start_new_meas() {
	complete_cnt=0;
//	printf("Starting meas\n");
	for (int i=0; i<no_tasks; i++) {
		xTaskNotifyGive(task[i]);
	}
}

static void vl_task_worker(void *arg) {
	int id=(int)arg;
	while(1) {
		//Wait for go signal
//		printf("id %d: waiting\n", id);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//		printf("id %d: measuring\n", id);
		//measure
		VL53L0X_Error status = take_reading(&vl_dev[id], &meas[id]);
		assert(status == VL53L0X_ERROR_NONE);
		int done_ct=__sync_fetch_and_add(&complete_cnt, 1)+1; //sync_fetch_add returns old number, add 1 to get new
//		printf("id %d: %d done\n", id, done_ct);
		if (done_ct==no_tasks) {
			xQueueSend(readQueue, meas, portMAX_DELAY);
			start_new_meas();
		}
	}
}

void vl_task_init(VL53L0X_Dev_t *dev, int cnt) {
	assert(task==NULL);
	task=calloc(sizeof(TaskHandle_t), cnt);
	vl_dev=dev;
	readQueue=xQueueCreate(1, sizeof(VL53L0X_RangingMeasurementData_t)*cnt);
	meas=calloc(sizeof(VL53L0X_RangingMeasurementData_t), cnt);
	no_tasks=cnt;
	complete_cnt=0;
	for (int i=0; i<cnt; i++) {
		printf("%s: Creating worker %d\n", __FUNCTION__, i);
		xTaskCreate(vl_task_worker, "vl_tsk", 8192, (void*)i, 5, &task[i]);
	}
	start_new_meas();
}

void vl_task_get_results(VL53L0X_RangingMeasurementData_t *res) {
	xQueueReceive(readQueue, res, portMAX_DELAY);
}
