#define SENSOR_PIN A0

// ================= 變數宣告區 =================
float rawValue = 0.0;      // Value 1: 原始感測器訊號 (未處理的雜訊)
float smoothedValue = 0.0; // Value 2: 平滑處理後的訊號 (低通濾波)
float dcTracker = 0.0;     // 追蹤緩慢變動的直流基準線 (超低頻)
float acSignal = 0.0;      // Value 3: 交流心跳訊號 (已經把基準線拉平到 512)
int beatDetected = 0;      // Value 4: 觸發方波 (用來畫圖跟算心跳)

// ================= 參數設定區 =================
// 濾波器參數 (範圍 0.0 ~ 1.0)
const float ALPHA_SMOOTH = 0.2; // 平滑係數：越大波形越靈敏但毛刺多，越小越平滑但有延遲
const float ALPHA_DC = 0.02;    // 基準線係數：用來追蹤環境光的極慢速變化

// 【關鍵參數】觸發心跳方波的門檻值
// 因為目前的環境光讓訊號變微弱，所以設在 513 左右，你可以看 Value 3 (綠線) 的最高點來微調
const int THRESHOLD = 513;      

void setup() {
  Serial.begin(9600);
  
  // 初始化，先抓一筆當前讀值，讓一開始的波形不要從 0 往上爬，避免圖表大爆走
  int initialRead = analogRead(SENSOR_PIN);
  rawValue = initialRead;
  smoothedValue = initialRead;
  dcTracker = initialRead;
}

void loop() {
  // 1. 讀取原始數值
  rawValue = analogRead(SENSOR_PIN);

  // 2. 指數移動平均 (EMA) 低通濾波 -> 消除高頻雜訊
  smoothedValue = (ALPHA_SMOOTH * rawValue) + ((1.0 - ALPHA_SMOOTH) * smoothedValue);

  // 3. 追蹤直流偏移 (DC Offset) -> 抓出緩慢變化的環境光底噪
  dcTracker = (ALPHA_DC * rawValue) + ((1.0 - ALPHA_DC) * dcTracker);

  // 4. 【修改重點：提取並翻轉交流訊號】
  // 因為你的波形是「向下」的 V 字型，所以用 dcTracker (較高) 減去 smoothedValue (較低)
  // 這樣能把向下的波谷翻轉成向上的波峰！然後 +512 將波形置中在圖表中央。
  acSignal = dcTracker - smoothedValue + 512.0;

  // 5. 臨界值判斷 -> 將波形轉換為乾淨的數位方波
  if (acSignal > THRESHOLD) {
    beatDetected = 512; // 輸出 512 讓圖表高度一致，方便觀察
  } else {
    beatDetected = 0;   // 沒達標就躺平輸出 0
  }

  // 6. 按照序列埠繪圖家的格式輸出 (以空格分隔)
  Serial.print(rawValue);
  Serial.print(" ");
  Serial.print(smoothedValue);
  Serial.print(" ");
  Serial.print((int)acSignal);     // 強制轉整數，圖表比較乾淨
  Serial.print(" ");
  Serial.println(beatDetected);    // 印出方波並換行

  // 7. 取樣頻率控制 (20ms 的延遲，大約是 50Hz 的取樣率)
  delay(20); 
}