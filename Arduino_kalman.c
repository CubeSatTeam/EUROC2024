#include <Wire.h>

int16_t AcX1, AcY1, AcZ1, Tmp1, GyX1, GyY1, GyZ1;
int16_t AcX2, AcY2, AcZ2, Tmp2, GyX2, GyY2, GyZ2;
int16_t VGyX1, VGyY1, VGyZ1, VGyX2, VGyY2, VGyZ2;
double gyro_buffer[9] = {0,0,0,0,0,0,0,0,0};
double RollGyro, PitchGyro, YawGyro;
double PredictionRoll, PredictionPitch;
double UncertaintyRoll = 0;
double KalmanRoll;


//Init parameters to modify

double std_devGyro = 4;  //In degrees per second
double std_devAccell = 3;
double dt = 50;
double timestamp = 0.004;
double KalmanGain; 


bool first;

const byte MPU1 = 0x68;
const byte MPU2 = 0x69;
const double alpha = 0.5;

double AccellPitch, AccellRoll, AccellPitch1, AccellRoll1, AccellPitch2, AccellRoll2;

double FunctionsPitchRoll(double A, double B, double C);

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU1);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  Wire.beginTransmission(MPU2);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(115200);
}

void loop() {
  //Serial.println("\n\nBegin Info mpu1\n");
  Wire.beginTransmission(MPU1);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU1, 14, true); // request a total of 14 registers
  AcX1 = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L) //IMPORTANT The infos read by the accel register have to be divided by 4096 (Ask the vid)
  AcY1 = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ1 = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp1 = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX1 = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L) //IMPORTANT The infos read by the accel register have to be divided by 65.5 (Ask the vid)
  GyY1 = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ1 = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  VGyX1 = (float)GyX1/65.5;
  VGyY1 = (float)GyY1/65.5;
  VGyZ1 = (float)GyZ1/65.5;
/*
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX1);
  Serial.print(" | Y = "); Serial.print(AcY1);
  Serial.print(" | Z = "); Serial.println(AcZ1);
  
  //equation for temperature in degrees C from datasheet
  Serial.print("Temperature: "); Serial.print(Tmp1 / 340.00 + 36.53); Serial.println(" C ");

  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX1);
  Serial.print(" | Y = "); Serial.print(GyY1);
  Serial.print(" | Z = "); Serial.println(GyZ1);
  Serial.println(" ");*/

//  Serial.println("\n\nBegin Info mpu2\n");
  Wire.beginTransmission(MPU2);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU2, 14, true); // request a total of 14 registers
  AcX2 = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY2 = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ2 = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp2 = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX2 = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY2 = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ2 = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  VGyX2 = (float)GyX2/65.5;
  VGyY2 = (float)GyY2/65.5;
  VGyZ2 = (float)GyZ2/65.5;

/*  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX2);
  Serial.print(" | Y = "); Serial.print(AcY2);
  Serial.print(" | Z = "); Serial.println(AcZ2);
  
  //equation for temperature in degrees C from datasheet
  Serial.print("Temperature: "); Serial.print(Tmp2 / 340.00 + 36.53); Serial.println(" C ");


  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX2);
  Serial.print(" | Y = "); Serial.print(GyY2);
  Serial.print(" | Z = "); Serial.println(GyZ2);
  Serial.println(" ");*/

  AccellRoll1 = FunctionsPitchRoll(AcX1, AcY1, AcZ1);   //Calcolo angolo Roll
  AccellPitch1 = FunctionsPitchRoll(AcY1, AcX1, AcZ1);  //Calcolo angolo Pitch
  AccellRoll2 = FunctionsPitchRoll(AcX2, AcY2, AcZ2);   //Calcolo angolo Roll
  AccellPitch2 = FunctionsPitchRoll(AcY2, AcX2, AcZ2);  //Calcolo angolo Pitch

  AccellRoll = AccellRoll1 * alpha + AccellRoll2 * (1 - alpha);
  AccellPitch = AccellPitch1 * alpha + AccellPitch2 * (1 - alpha);



  gyro_buffer[0] = gyro_buffer[3];
  gyro_buffer[1] = gyro_buffer[4];
  gyro_buffer[2] = gyro_buffer[5];
  gyro_buffer[3] = gyro_buffer[6];
  gyro_buffer[4] = gyro_buffer[7];
  gyro_buffer[5] = gyro_buffer[8];
  gyro_buffer[6] = VGyX1;
  gyro_buffer[7] = VGyY1;
  gyro_buffer[8] = VGyZ1;

  if (first){
    RollGyro = AccellRoll1;
    PitchGyro = AccellPitch1;
    KalmanRoll = AccellRoll1;
    first = false;
    }

  RollGyro = RollGyro + 0.05 / 6 * (gyro_buffer[0] + 4*gyro_buffer[3] + gyro_buffer[6]);
  PitchGyro = PitchGyro + 0.05 / 6 * (gyro_buffer[1] + 4*gyro_buffer[4] + gyro_buffer[7]);
  YawGyro = YawGyro + 0.05 / 6 * (gyro_buffer[2] + 4*gyro_buffer[5] + gyro_buffer[8]);

/*
  Serial.print("RollGyro:");
  Serial.print(RollGyro);
  Serial.print("  ");
  Serial.print("PitchGyro:");
  Serial.print(PitchGyro);
  Serial.print("  ");
  Serial.print("YawGyro:");
  Serial.println(YawGyro); */


  PredictionRoll = RollGyro;
  //PredictionPitch = PitchGyro;

  KalmanRoll = KalmanRoll + timestamp * PredictionRoll; //0.004 is a temporary solution, need to fix this better 

  UncertaintyRoll = UncertaintyRoll + timestamp * std_devGyro * std_devGyro;

  KalmanGain = UncertaintyRoll / (UncertaintyRoll + std_devAccell * std_devAccell);

  KalmanRoll = KalmanRoll + KalmanGain * (AccellRoll1 - KalmanRoll);

  UncertaintyRoll = UncertaintyRoll * (1 - KalmanGain);




  Serial.print("  ");
  Serial.print("RollGyro:");
  Serial.print(RollGyro);

  /*
  Serial.print("PitchGyro:");
  Serial.print(PitchGyro);
  Serial.print("  ");
  */


  Serial.print("  ");
  Serial.print("AccellRoll1:");
  Serial.print(AccellRoll1);
  

  Serial.print("  ");
  Serial.print("KalmanRoll:");
  Serial.println(KalmanRoll);
  
  Serial.print("  ");
  Serial.print("KalmanGain:");
  Serial.println(KalmanGain);


  delay(20);
}




double FunctionsPitchRoll(double A, double B, double C) {
  double DatoA, DatoB, Value;
  DatoA = A;
  DatoB = (B * B) + (C * C);
  DatoB = sqrt(DatoB);

  Value = atan2(DatoA, DatoB);
  Value = Value * 180 / 3.14;

  return (int)Value;
}