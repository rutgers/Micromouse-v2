#include "bot_config.h"


#if defined(ADAFRUIT_SENSOR_CALIBRATION_USE_EEPROM)
  Adafruit_Sensor_Calibration_EEPROM cal;
#else
  Adafruit_Sensor_Calibration_SDFat cal;
#endif

int surround[3] = {0,0,0};
void checkWalls();


/* Example PD Function */
void PDFunc()
{
    // Here I use the motor encoders for my inputs, but you can also use the ToF sensors
    // or the IMU depending on the situation or your preference
    // Translational PD: motor encoders, ToF (Front)
    // Rotational PD: IMU, ToF (Left and Right)
    int currA = ENCA.readAndReset();
    int currB = ENCB.readAndReset();
    oldErrorX = errorX;
    oldErrorW = errorW;

    // Refer to the Micromouse PID Slides in the S22 Micromouse Transition Docs folder
    // for more details on how the final PD output is determined
    errorX = xSet - (currA + currB);
    errorW = wSet - (currA - currB);
    int outputX = (KpX * errorX) + (KdX * (errorX - oldErrorX));
    int outputW = (KpW * errorW) + (KdW * (errorW - oldErrorW));
    setPWMA(constrain(outputX + outputW, -1023, 1023));
    setPWMB(constrain(outputX - outputW, -1023, 1023));
}

/* Example Orientation Return Function */
// This is basically Adafruits example adjusted for this projects needs
// Call this function in loop() to use
static uint8_t imu_read_counter = 0;
void get_orientation()
{
    if (imu_filter_timestamp < (1000 / FILTER_UPDATE_RATE_HZ)) {
        return;
    }
    imu_filter_timestamp = 0;

    // Fills raw readings from IMU for accel and gyro
    accel->getEvent(&a);
    gyro->getEvent(&g);

    // Calibrates raw values
    // cal.calibrate(a); cal.calibrate(g);

    float gx, gy, gz;

    // Gyroscope needs to be converted from Rad/s to Degree/s
    gx = g.gyro.x * SENSORS_RADS_TO_DPS;
    gy = g.gyro.y * SENSORS_RADS_TO_DPS;
    gz = g.gyro.z * SENSORS_RADS_TO_DPS;

    // Update the SensorFusion filter
    imu_filter.update(gx, gy, gz,
        a.acceleration.x, a.acceleration.y, a.acceleration.z,
        0, 0, 0); // 0's at the end for no magnetometer readings

    // only print the calculated output once in a while
    if (imu_read_counter++ <= PRINT_EVERY_N_UPDATES) {
        return;
    }
    // reset the counter
    imu_read_counter = 0;

    // print the heading, pitch and roll
    float roll, pitch, heading;
    roll = imu_filter.getRoll();
    pitch = imu_filter.getPitch();
    heading = imu_filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(", ");
    Serial.print(pitch);
    Serial.print(", ");
    Serial.println(roll);

    /* Quaternion Output (if you want to more fancy and precise lol)
    
    float qw, qx, qy, qz;
    imu_filter.getQuaternion(&qw, &qx, &qy, &qz);
    Serial.print("Quaternion: ");
    Serial.print(qw, 4);
    Serial.print(", ");
    Serial.print(qx, 4);
    Serial.print(", ");
    Serial.print(qy, 4);
    Serial.print(", ");
    Serial.println(qz, 4);
    */
}

void checkWalls() {
    surround[0] = (L_ToF.readRangeContinuousMillimeters() < 90) ? 1 : 0;
    surround[1] = (F_ToF.read() < 90) ? 1 : 0;
    surround[2] = (R_ToF.readRangeContinuousMillimeters() < 90) ? 1 : 0;
}

void setup()
{
    pinMode(14, OUTPUT);
    pinMode(15, OUTPUT);
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    Wire.begin();
    Wire.setClock(400000);
    setupMotorDriver();
    setupToF();
    setupIMU();

    xSet = 30;
    wSet = 0;

    /* Use only when outputting with bluetooth, otherwise leave commented */
    // bt.begin(115200);
    // delay(100);
    // bt.println("Send ready cmd");
    // bool check = true;
    // while(check) {
    //     if (bt.available()) if (bt.read() == 'a') check = false;
    //     delay(1);
    // }
    // delay(10);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);  // This provides physical indication setup is complete
    delay(2000);

    digitalWrite(STBY, HIGH);
    ENCA.write(0);
    ENCB.write(0);
    PDTimer.begin(PDFunc, 25000);

    /* IMU Orienation Algo Setup */
    // if (!cal.begin()) {
    //     Serial.println("Failed to initialize calibration helper");
    // } else if (! cal.loadCalibration()) {
    //     Serial.println("No calibration loaded/found");
    // }

    // imu_filter.begin(FILTER_UPDATE_RATE_HZ);
    // imu_filter_timestamp = 0;
}

bool stopRecording = false;

void checkWalls() {
    surround[0] = (L_ToF.readRangeContinuousMillimeters() < 90) ? 1 : 0;
    surround[1] = (F_ToF.read() < 90) ? 1 : 0;
    surround[2] = (R_ToF.readRangeContinuousMillimeters() < 90) ? 1 : 0;
}

void loop()
{
    // Sample loop code for the mouse to go straight until less than 80mm from an obstacle
    if (stopRecording) return;
    if (F_ToF.read() < 80) {
        noInterrupts();
        xSet = 0;
        interrupts();
        delay(250);
        PDTimer.end();
        setPWMA(0);
        setPWMB(0);
        stopRecording = true;
    }
}
