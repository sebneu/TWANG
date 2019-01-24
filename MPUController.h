#include "MPU6050.h"

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <wiringPiI2C.h>
#include <errmsg.h>


static const MPUIMU::Gscale_t GSCALE = MPUIMU::GFS_250DPS;
static const MPUIMU::Ascale_t ASCALE = MPUIMU::AFS_2G;

static MPU6050 imu(ASCALE, GSCALE);;

class MPUController {

public:
  void initialize();
  void getMotion6(int *ax, int *ay, int *az, int *gx, int *gy, int *gz);
};


void MPUController::initialize() {
  imu.begin();

  switch (imu.begin()) {

      case MPUIMU::ERROR_IMU_ID:
          errmsg("Bad device ID");
      case MPUIMU::ERROR_SELFTEST:
          errmsg("Failed self-test");
      default:
          printf("MPU6050 online!\n");
  }

  delay(1000);
}


void MPUController::getMotion6(int *ax, int *ay, int *az, int *gx, int *gy, int *gz) {

  // If data ready bit set, all data registers have new data
  if (imu.checkNewData()) {  // check if data ready interrupt
    imu.readAccelerometer(&ax, &ay, &az);
    imu.readGyrometer(&gx, &gy, &gz);
    
    // temperature = imu.readTemperature();
  }
}
