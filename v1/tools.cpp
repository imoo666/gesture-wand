#include <Arduino.h>

// 计算两个三维点之间的欧氏距离
float euclideanDistance3D(float point1[3], float point2[3]) {
  float diffX = point1[0] - point2[0];
  float diffY = point1[1] - point2[1];
  float diffZ = point1[2] - point2[2];
  return sqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
}

// 计算向量的点积
float dotProduct(float vec1[3], float vec2[3]) {
    float dot = 0.0;
    for (int i = 0; i < 3; ++i) dot += vec1[i] * vec2[i];
    return dot;
}
// 计算向量的范数（即向量的长度）
float norm(float vec[3]) {
    float sum = 0.0;
    for (int i = 0; i < 3; ++i) sum += vec[i] * vec[i];
    return std::sqrt(sum);
}
// 计算余弦距离
float cosineDistance(float point1[3], float point2[3]) {
    float dot_product = dotProduct(point1, point2);
    float magnitude1 = norm(point1);
    float magnitude2 = norm(point2);
    return 1.0f - (dot_product / (magnitude1 * magnitude2));
}


// 计算两个三维时间序列之间的DTW距离
float calculateDTW(float seq1[][3], int n, float seq2[][3], int m) {
  float D[2][m + 1];

  // 初始化矩阵
  for (int j = 0; j <= m; j++) {
    D[0][j] = D[1][j] = 999999;  // 使用一个非常大的数来替代INFINITY
  }
  D[0][0] = 0;

  // 填充距离矩阵
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      float cost = cosineDistance(seq1[i - 1], seq2[j - 1]);
      D[i % 2][j] = cost + min(min(D[(i - 1) % 2][j], D[i % 2][j - 1]), D[(i - 1) % 2][j - 1]);
    }
  }

  // 返回最优对齐路径的距离
  return D[n % 2][m];
}
void print2DArray(float arr[][3], int rows) {
  Serial.print(" = {");
  for (int i = 0; i < rows; i++) {
    Serial.print("{");
    Serial.print(arr[i][0]);
    Serial.print(", ");
    Serial.print(arr[i][1]);
    Serial.print(", ");
    Serial.print(arr[i][2]);
    Serial.print("}");
    if (i != rows - 1) {
      Serial.print(", ");
    }
  }
  Serial.println("}");
  Serial.println("录制完成");
}
