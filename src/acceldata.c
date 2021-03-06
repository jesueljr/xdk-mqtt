#include "BCDS_Accelerometer.h"
#include "XdkSensorHandle.h"
#include "acceldata.h"
#include "logging.h"
#include "retcode.h"

static const char ACCEL_LABEL[] = "BMA280 Accelerometer";

static void FillAccelData(SensorData* data, Accelerometer_XyzData_T* meas)
{
    data->numMeas = 1;
    snprintf(data->meas[0].name, SENSOR_NAME_SIZE, "%s", "acceleration");

    snprintf(data->meas[0].value,
             SENSOR_VALUE_SIZE,
             "{\"x\":%f, \"y\":%f, \"z\":%f}",
             (float)meas->xAxisData/1000.0,
             (float)meas->yAxisData/1000.0,
             (float)meas->zAxisData/1000.0);
}

static Retcode_T AccelPrivateInit(void* handle)
{
    return Accelerometer_init((Accelerometer_HandlePtr_T)handle);
}

XDK_Retcode_E AccelInit(void)
{
    return SensorInit(&AccelPrivateInit,
               xdkAccelerometers_BMA280_Handle,
               ACCEL_LABEL);
}

void AccelDeinit(void)
{
    Accelerometer_deInit(xdkAccelerometers_BMA280_Handle);
}

void AccelGetData(SensorData* data)
{
    Retcode_T returnValue = SENSOR_ERROR;
    Accelerometer_XyzData_T getaccelData = {0};

    // Clear data so that a failed sensor read will not be reported
    SensorDataClear(data);

    returnValue = Accelerometer_readXyzLsbValue(xdkAccelerometers_BMA280_Handle, &getaccelData);
    if(SENSOR_SUCCESS == returnValue)
    {
    	TRACE_PRINT("%s Raw Data : x = %ld, y = %ld, z = %ld",
               ACCEL_LABEL,
               (long int)getaccelData.xAxisData,
               (long int)getaccelData.yAxisData,
               (long int)getaccelData.zAxisData);

    }
    else
    {
        WARN_PRINT("%s Raw Data read FAILED", ACCEL_LABEL);
    }

    returnValue = Accelerometer_readXyzGValue(xdkAccelerometers_BMA280_Handle, &getaccelData);
    if(SENSOR_SUCCESS == returnValue)
    {
        FillAccelData(data, &getaccelData);
        TRACE_PRINT("%s Gravity Data : x = %ld mg, y = %ld mg, z = %ld mg",
               ACCEL_LABEL,
               (long int)getaccelData.xAxisData,
               (long int)getaccelData.yAxisData,
               (long int)getaccelData.zAxisData);

    }
    else
    {
        WARN_PRINT("%s Gravity Data read FAILED", ACCEL_LABEL);
    }
}
