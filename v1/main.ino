#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// 录制的数据点的数量
int REC_POINT_NUMBER = 100;
// 录制时长
int REC_TIME = 1500;

Adafruit_MPU6050 mpu;
float calculateDTW(float seq1[][3], int n, float seq2[][3], int m);
void print2DArray(float arr[][3], int rows);

struct Action {
  const char* name;
  float (*sequence)[3];
};

Action actions[] = {
  { "三角型", (float[][3]){ { -9.54, 0.37, 3.45 }, { -9.50, 0.13, 3.40 }, { -9.35, 0.46, 3.43 }, { -9.69, 1.32, 3.49 }, { -9.80, 1.46, 3.55 }, { -9.77, 1.03, 3.50 }, { -9.54, 0.60, 3.55 }, { -9.00, 0.07, 3.35 }, { -9.38, 0.24, 3.62 }, { -9.36, 0.57, 3.65 }, { -9.48, 0.62, 3.59 }, { -9.38, 0.22, 3.49 }, { -9.23, -0.06, 3.56 }, { -9.18, 0.00, 3.66 }, { -9.28, 0.26, 3.62 }, { -9.48, 0.19, 3.48 }, { -9.47, 0.32, 3.50 }, { -9.67, 0.18, 3.55 }, { -10.01, 0.38, 3.70 }, { -10.18, 0.39, 3.70 }, { -10.14, 0.38, 3.67 }, { -9.86, 0.62, 3.76 }, { -10.08, 0.66, 3.97 }, { -10.60, 0.06, 3.81 }, { -10.34, 0.06, 3.92 }, { -9.96, 1.13, 4.62 }, { -9.17, 1.90, 5.17 }, { -8.03, 1.39, 4.56 }, { -6.63, 0.98, 4.32 }, { -4.70, 0.44, 3.89 }, { -1.36, 1.32, 4.64 }, { 2.32, 4.68, 6.79 }, { 4.26, 5.70, 7.53 }, { -3.92, 0.14, 2.93 }, { -13.42, -1.70, -0.81 }, { -0.99, 4.96, 1.30 }, { 3.02, 11.09, -0.08 }, { -0.72, 12.34, -1.81 }, { -7.18, 16.78, -4.92 }, { -10.72, 17.45, -7.54 }, { -8.95, 9.59, -9.42 }, { -4.94, 7.07, -6.85 }, { -7.48, 7.20, -7.59 }, { -15.60, 5.51, -9.68 }, { -14.28, 8.13, -9.81 }, { -5.09, 9.11, -6.60 }, { -3.24, 10.18, -3.44 }, { -3.89, 9.30, -2.40 }, { -8.80, 7.23, -3.72 }, { -13.87, 5.84, -4.57 }, { -14.04, 6.19, -5.10 }, { -11.30, 5.71, -6.85 }, { -8.60, 4.76, -8.01 }, { -5.93, 4.69, -7.42 }, { -2.13, 6.25, -4.97 }, { 0.19, 8.37, -1.07 }, { 8.13, 7.33, 1.70 }, { 2.81, 5.55, 2.31 }, { -5.91, 5.99, 0.34 }, { -9.65, 7.42, -1.39 }, { -7.94, 8.84, -0.67 }, { -5.00, 10.17, 0.19 }, { -5.98, 9.04, 0.74 }, { -9.54, 8.58, 0.11 }, { -12.61, 10.18, -0.75 }, { -13.56, 13.29, -0.78 }, { -15.99, 16.57, -0.03 }, { -14.63, 15.66, -0.05 }, { -10.90, 11.11, -1.24 }, { -9.71, 12.90, -2.18 }, { -8.97, 7.23, -3.82 }, { -9.95, -3.64, -4.22 }, { -6.96, -11.54, -3.05 }, { -3.45, -11.44, 0.53 }, { -2.21, -6.40, 3.47 }, { -3.07, -2.29, 4.12 }, { -5.37, -0.33, 4.13 }, { -6.84, -1.35, 3.84 }, { -8.20, -2.92, 4.64 }, { -8.58, -3.74, 5.23 }, { -8.03, -2.26, 5.89 }, { -6.95, -0.58, 5.98 }, { -6.78, 0.51, 6.02 }, { -7.37, -0.19, 5.72 }, { -8.95, -0.98, 5.36 }, { -10.01, -0.83, 5.29 }, { -9.80, -0.09, 5.86 }, { -9.25, -0.15, 5.69 }, { -8.29, 0.22, 5.58 }, { -8.20, 0.66, 5.54 }, { -9.30, 0.40, 5.23 }, { -9.75, 0.06, 5.22 }, { -10.06, -0.24, 5.06 }, { -9.17, -0.01, 5.44 }, { -8.89, 0.30, 5.85 }, { -8.24, 0.74, 5.62 }, { -8.87, 1.21, 5.69 }, { -9.82, 1.27, 5.60 }, { -9.98, 1.22, 5.51 }, { -9.74, 0.67, 5.38 } } },
  { "五角星", (float[][3]){ { -9.45, 1.09, 4.20 }, { -9.24, 1.31, 4.33 }, { -9.07, 1.35, 4.49 }, { -8.94, 1.31, 4.56 }, { -8.77, 1.24, 4.45 }, { -8.77, 1.32, 4.39 }, { -9.01, 1.40, 4.64 }, { -9.00, 1.33, 4.51 }, { -9.03, 1.37, 4.55 }, { -8.96, 1.34, 4.53 }, { -9.10, 1.24, 4.46 }, { -9.19, 1.30, 4.58 }, { -9.40, 1.37, 4.64 }, { -10.04, 0.93, 4.29 }, { -10.91, 0.37, 4.11 }, { -10.14, 0.44, 4.31 }, { -9.04, 1.30, 4.84 }, { -9.30, 2.19, 5.37 }, { -10.02, 1.95, 5.46 }, { -11.13, 0.72, 5.01 }, { -9.47, 0.25, 4.82 }, { -7.87, 0.30, 4.91 }, { -6.58, 1.46, 5.70 }, { -2.73, 3.34, 6.32 }, { 3.95, 5.40, 4.94 }, { 7.84, 5.36, 1.52 }, { 5.18, 7.36, 4.47 }, { -6.48, 2.96, 0.35 }, { -3.39, 5.42, -2.74 }, { -4.06, 9.66, -4.62 }, { -7.46, 10.50, -5.55 }, { -17.14, 13.14, -0.50 }, { -18.18, 14.44, -0.55 }, { -15.86, 10.07, -1.86 }, { -10.35, 4.52, -2.37 }, { -9.06, 3.94, -3.24 }, { -11.43, 4.76, -4.62 }, { -16.06, 7.25, -3.21 }, { -20.11, 7.64, -3.95 }, { -20.64, 8.14, -2.32 }, { -17.64, 7.04, -4.65 }, { -11.35, 7.00, -5.58 }, { -11.31, 7.89, -5.47 }, { -11.95, 12.91, -3.03 }, { -5.23, 14.59, 0.72 }, { 4.83, 14.60, -0.56 }, { 15.16, 16.11, 0.77 }, { 12.26, 12.45, -0.54 }, { 3.52, 9.66, 3.01 }, { -0.35, 18.08, 8.69 }, { 2.08, 22.10, 6.00 }, { 1.80, 20.99, -0.90 }, { -2.87, 9.44, -6.45 }, { -11.18, -2.67, -8.25 }, { -14.82, -1.71, -5.36 }, { -10.19, 1.39, -2.61 }, { -8.81, 2.89, -0.54 }, { -13.30, 0.74, -1.32 }, { -16.21, -0.35, -7.35 }, { -20.81, -0.17, -8.16 }, { -19.09, 2.34, -6.30 }, { -15.18, 4.99, -6.15 }, { -9.94, 4.68, -6.96 }, { -5.62, 5.69, -5.15 }, { -3.35, 8.83, -3.08 }, { -3.56, 12.78, 1.07 }, { -5.15, 17.28, 6.10 }, { -5.25, 20.74, 10.92 }, { -5.78, 24.84, 13.97 }, { -2.89, 24.13, 12.60 }, { 1.59, 19.50, 7.06 }, { -1.38, 18.76, 2.79 }, { -18.37, 20.97, -1.32 }, { -29.20, 16.40, -7.86 }, { -14.98, 7.14, -14.09 }, { -1.34, -1.58, -8.90 }, { 1.14, -8.43, -4.49 }, { -2.18, -5.78, 1.36 }, { -2.50, -2.80, 4.72 }, { -4.62, 0.72, 7.07 }, { -6.71, 2.53, 7.10 }, { -7.56, 2.63, 6.17 }, { -8.89, 1.22, 5.04 }, { -8.83, 0.81, 4.33 }, { -9.98, 1.16, 5.06 }, { -10.35, 2.92, 6.20 }, { -7.78, 6.49, 7.36 }, { -9.21, 7.89, 10.05 }, { -7.41, 6.95, 9.73 }, { -6.71, 4.96, 8.46 }, { -8.28, 2.62, 6.40 }, { -8.57, 2.64, 5.48 }, { -8.55, 3.74, 5.95 }, { -8.77, 4.97, 7.32 }, { -7.41, 5.36, 7.99 }, { -6.62, 4.31, 7.68 }, { -6.30, 2.55, 6.22 }, { -8.43, 1.58, 5.33 }, { -9.95, 1.55, 5.10 }, { -9.33, 1.74, 5.34 } } },
  { "A", (float[][3]){ { -9.51, -0.28, 4.18 }, { -9.32, -0.29, 4.16 }, { -9.16, -0.18, 4.15 }, { -9.11, 0.06, 4.25 }, { -9.17, 0.20, 4.42 }, { -9.31, 0.12, 4.39 }, { -9.42, -0.24, 4.21 }, { -9.46, -0.34, 4.12 }, { -9.44, -0.23, 4.14 }, { -9.37, -0.08, 4.26 }, { -9.25, -0.02, 4.39 }, { -9.41, -0.06, 4.31 }, { -9.44, -0.13, 4.28 }, { -9.31, -0.11, 4.17 }, { -9.67, -0.06, 4.36 }, { -10.03, -0.99, 4.15 }, { -10.80, -1.58, 3.72 }, { -10.38, -0.96, 3.98 }, { -9.55, -0.01, 3.87 }, { -10.18, 0.73, 5.77 }, { -9.82, 1.50, 5.22 }, { -11.03, 0.47, 4.80 }, { -10.31, -0.02, 4.88 }, { -8.83, 0.16, 5.84 }, { -8.42, 0.88, 5.53 }, { -8.52, 1.63, 6.17 }, { -8.14, 1.58, 6.05 }, { -7.58, 1.04, 5.94 }, { -7.27, 1.27, 5.69 }, { -8.39, 0.35, 5.56 }, { -8.73, -0.06, 5.71 }, { -8.06, -0.41, 5.90 }, { -7.20, -0.16, 5.71 }, { -5.07, 0.74, 5.92 }, { 0.15, 4.25, 7.02 }, { 6.01, 3.31, 3.53 }, { 7.76, 3.84, 1.01 }, { -2.10, 2.72, 2.34 }, { -6.23, 3.08, -0.77 }, { -4.93, 7.24, -4.03 }, { -9.22, 9.33, -4.46 }, { -16.63, 11.31, -0.45 }, { -19.39, 7.25, -6.56 }, { -22.33, 0.81, -9.48 }, { -21.75, 1.59, -8.91 }, { -20.84, 8.10, -5.32 }, { -19.63, 13.60, -1.81 }, { -17.44, 12.80, -0.19 }, { -13.57, 6.48, -1.26 }, { -9.12, 1.78, -2.62 }, { -7.77, -1.19, -4.63 }, { -8.78, -0.26, -2.48 }, { -5.41, 1.60, -0.38 }, { 1.00, 0.83, -0.75 }, { 6.35, -3.43, -2.97 }, { 8.78, -2.27, -1.38 }, { 0.87, 2.83, 5.95 }, { -4.47, 3.00, 3.26 }, { -5.02, 4.21, 0.41 }, { -6.64, 8.62, 0.62 }, { -7.66, 12.74, -0.25 }, { -14.85, 16.04, 1.89 }, { -16.32, 15.15, 1.91 }, { -13.18, 10.76, -0.60 }, { -12.46, 10.06, -1.86 }, { -15.57, 13.90, -0.46 }, { -13.71, 13.87, -2.02 }, { -6.36, 6.69, -6.78 }, { -5.03, -3.26, -6.22 }, { -2.84, -7.44, -3.03 }, { -7.50, -7.93, -2.02 }, { -13.41, -8.83, -2.93 }, { -13.71, -11.79, -5.47 }, { -13.90, -14.41, -5.24 }, { -12.01, -11.35, -4.38 }, { -16.81, -8.21, -3.11 }, { -14.05, -0.56, -1.86 }, { -9.79, -0.81, -0.82 }, { -3.93, 0.13, -3.38 }, { -2.60, 3.03, -2.22 }, { -4.38, 8.52, 1.27 }, { -4.72, 10.96, 3.07 }, { -6.20, 12.75, 6.65 }, { -5.96, 10.93, 7.48 }, { -4.61, 8.19, 6.13 }, { -4.75, 6.06, 5.70 }, { -5.43, 4.30, 5.88 }, { -6.44, 3.21, 5.97 }, { -7.76, 2.95, 5.85 }, { -8.78, 3.43, 5.57 }, { -8.53, 3.82, 5.28 }, { -8.69, 3.97, 5.46 }, { -9.44, 3.85, 5.58 }, { -9.40, 3.79, 5.57 }, { -9.33, 3.69, 5.59 }, { -9.59, 3.59, 5.68 }, { -9.72, 2.72, 5.06 }, { -9.59, 2.11, 4.36 }, { -9.42, 1.72, 4.05 }, { -8.60, 1.66, 4.28 } } },
  { "B", (float[][3]){ { -8.95, 0.75, 5.76 }, { -8.84, 0.42, 5.54 }, { -8.62, 0.29, 5.56 }, { -8.58, 0.19, 5.64 }, { -8.84, 0.07, 5.51 }, { -8.98, 0.00, 5.49 }, { -8.86, 0.00, 5.49 }, { -8.77, 0.17, 5.71 }, { -9.04, 0.23, 5.74 }, { -9.12, 0.17, 5.63 }, { -9.19, 0.28, 5.52 }, { -9.33, 0.46, 5.60 }, { -9.22, 0.52, 5.68 }, { -9.07, 0.42, 5.63 }, { -9.11, 0.39, 5.60 }, { -9.66, 0.08, 5.48 }, { -9.40, 0.08, 5.63 }, { -9.02, 0.19, 5.71 }, { -9.25, 0.57, 5.99 }, { -9.94, 0.92, 6.28 }, { -9.45, 0.88, 6.19 }, { -6.27, 0.97, 5.70 }, { -0.26, 3.42, 4.91 }, { -1.22, 2.81, 4.26 }, { -4.64, -1.14, 2.15 }, { -0.99, -2.23, 1.01 }, { 0.18, -0.53, -0.45 }, { -6.66, -3.18, -3.35 }, { -9.27, -3.81, -3.68 }, { -12.63, 4.75, -0.30 }, { -18.58, 7.74, 0.83 }, { -17.87, 8.45, 1.42 }, { -16.41, 8.75, 1.34 }, { -15.38, 8.69, -0.09 }, { -14.66, 9.29, -0.62 }, { -16.63, 8.64, -0.31 }, { -14.76, 5.11, -2.56 }, { -8.83, 3.48, -3.94 }, { -8.40, 3.15, -2.86 }, { -7.58, 3.18, -2.16 }, { -8.88, 3.78, 0.54 }, { -11.59, 2.86, 1.72 }, { -15.22, 0.84, 3.34 }, { -13.74, -3.78, -0.55 }, { -9.77, -3.14, -0.14 }, { -5.99, -1.04, 3.22 }, { -3.37, 0.12, 4.97 }, { -7.68, -1.13, 5.87 }, { -9.14, -3.64, 3.92 }, { -5.85, -3.66, 1.83 }, { -3.45, -3.04, 0.82 }, { -0.31, -0.72, 1.75 }, { 2.99, 2.84, 1.84 }, { 2.64, 3.38, 2.89 }, { -3.82, 0.09, 2.80 }, { -4.19, 1.43, 0.57 }, { -5.64, 9.72, 1.34 }, { -16.92, 21.08, 8.45 }, { -24.64, 23.94, 13.43 }, { -8.33, 18.69, 2.53 }, { -6.71, 7.82, -5.32 }, { -10.93, 3.77, -5.59 }, { -17.63, 1.72, -4.28 }, { -20.22, -3.99, -5.85 }, { -18.37, -11.64, -11.02 }, { -15.14, -14.17, -10.35 }, { -11.96, -10.69, -6.51 }, { -10.11, -6.61, -3.07 }, { -9.76, -4.96, -0.55 }, { -9.22, -3.03, -2.29 }, { -6.31, -1.27, -4.15 }, { -1.20, 1.33, -5.07 }, { 1.66, 3.55, -2.85 }, { 0.90, 8.22, 0.69 }, { -3.78, 10.20, -1.39 }, { -9.56, 15.99, 0.87 }, { -15.12, 20.76, 3.57 }, { -12.84, 19.55, 6.34 }, { -5.50, 14.91, 5.02 }, { 3.04, 9.48, 0.97 }, { -1.88, 4.45, -4.34 }, { -13.62, 1.39, -6.06 }, { -15.45, 2.32, -5.63 }, { -10.80, 5.27, -4.89 }, { -10.03, 4.58, -4.81 }, { -12.34, 0.34, -6.80 }, { -13.44, -2.48, -8.17 }, { -14.98, -1.06, -4.69 }, { -12.97, 3.48, -3.13 }, { -10.29, 4.62, -1.53 }, { -7.95, 2.01, -1.80 }, { -7.61, 0.34, -1.56 }, { -7.15, 0.70, -2.21 }, { -6.04, 5.17, 0.51 }, { -7.15, 7.58, 2.17 }, { -8.30, 7.41, 1.87 }, { -8.69, 2.51, 0.56 }, { -8.16, -0.65, -0.54 }, { -8.57, -1.01, -1.11 }, { -7.63, 1.06, -1.46 } } },
  // 可以在这里添加更多的动作
};


float (*rec())[3] {
  Serial.println("2s后开始录制...");
  delay(2000);  // 等待2秒
  Serial.println("启动录制");
  float(*sequence)[3] = new float[REC_POINT_NUMBER][3];  // 用于存储录制的数据
  int delayTime = REC_TIME / REC_POINT_NUMBER;           // 延迟时间
  for (int sequenceLen = 0; sequenceLen < REC_POINT_NUMBER; sequenceLen++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    memcpy(sequence[sequenceLen], new float[3], 3 * sizeof(float));
    sequence[sequenceLen][0] = a.acceleration.x;
    sequence[sequenceLen][1] = a.acceleration.y;
    sequence[sequenceLen][2] = a.acceleration.z;

    delay(delayTime);
  }
  return sequence;
}



void setup() {
  Serial.begin(115200);
  Wire.begin(3, 4);
  mpu.begin();

  // 设置加速度计的量程范围。量程越小，测量精度越高，适用于测量较小的加速度。量程越大，能测量的最大加速度越高，适用于测量较大的加速度。
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  // 设置陀螺仪的量程范围。量程越小，测量精度越高，适用于测量较小的角速度。量程越大，能测量的最大角速度越高，适用于测量较大的角速度。
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // 设置数字低通滤波器的带宽为21Hz。带宽越低，滤除的高频噪声越多，数据越平滑，但会引入更多的延迟。带宽越高，滤除的噪声越少，数据越敏感，但噪声也会增加。
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);
}

void loop() {
  if (Serial.available() <= 0) {
    return;
  }
  char command = Serial.read();
  if (command == '1') {
    float(*sequence)[3] = rec();
    print2DArray(sequence, REC_POINT_NUMBER);
  }
  if (command == '2') {
    float(*sequence)[3] = rec();

    float minDtw = 999999;
    const char* minDtwAction = actions[0].name;

    for (int i = 0; i < sizeof(actions) / sizeof(actions[0]); i++) {
      float dtw = calculateDTW(sequence, REC_POINT_NUMBER, actions[i].sequence, REC_POINT_NUMBER);
      Serial.print("和");
      Serial.print(actions[i].name);
      Serial.print("动作偏差");
      Serial.println(dtw);

      if (dtw < minDtw) {
        minDtw = dtw;
        minDtwAction = actions[i].name;
      }
    }

    if (minDtwAction != "静止") {
      Serial.print("动作识别为：");
      Serial.println(minDtwAction);
    } else {
      Serial.println("未检测到动作");
    }
    Serial.println("-----------");
  }
  delay(1000);
}
