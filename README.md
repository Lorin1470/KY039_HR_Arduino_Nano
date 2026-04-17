此 MD 由 GitHub Copilot 生成。

# KY-039 心跳感測器 (Arduino Nano)

![Board](https://img.shields.io/badge/Board-Arduino%20Nano-00979D?logo=arduino&logoColor=white)
![IDE](https://img.shields.io/badge/IDE-Arduino%20IDE-00979D?logo=arduino&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

一個可直接在 Arduino IDE 使用的 KY-039 心跳感測範例，針對「環境光干擾」與「波形向下」情境做了簡單而實用的訊號處理：

- 原始訊號讀取
- EMA 低通濾波
- DC 偏移追蹤
- 交流訊號反轉與置中
- 門檻觸發方波輸出

此專案適合：

- 第一次接觸 KY-039 與 Arduino 的初學者
- 想用 Serial Plotter 觀察心跳波形的人
- 想快速調整門檻值 (THRESHOLD) 做即時偵測的人

## 功能特色

- 輸出 4 條可視化資料軌道，方便在 Serial Plotter 同步觀察
- 用 `ALPHA_SMOOTH` 與 `ALPHA_DC` 分離高頻雜訊與低頻背景光
- 針對常見「心跳呈現波谷」訊號，提供反轉策略
- 以固定採樣間隔 `delay(20)`，約 50Hz 取樣率

## 硬體需求

- Arduino Nano (ATmega328P)
- KY-039 心跳感測模組
- 杜邦線
- USB 連線線材

## 接線方式

目前程式使用 `SENSOR_PIN = A0`，建議接線如下：

| KY-039 | Arduino Nano |
|---|---|
| VCC | 5V |
| GND | GND |
| SIG | A0 |

> 若你的 KY-039 模組標示腳位不同，請以模組實際標示為準。

## 專案結構

- `KY039-HR to Arduino Nano code.ino`: 主要範例程式
- `build/`: 編譯輸出與中介檔（通常不需要手動編輯）

## 快速開始

1. 開啟 Arduino IDE。
2. 載入 `KY039-HR to Arduino Nano code.ino`。
3. 在 `工具 > 開發板` 選擇 Arduino Nano。
4. 在 `工具 > 處理器` 選擇你的 Nano 規格（常見為 ATmega328P 或 Old Bootloader）。
5. 選擇正確的連接埠後上傳。
6. 開啟 `工具 > 序列埠繪圖家 (Serial Plotter)`，鮑率設為 `9600`。

## 輸出資料格式

每一行輸出 4 個數值，以空格分隔：

1. `rawValue`：原始感測值
2. `smoothedValue`：EMA 平滑後訊號
3. `acSignal`：去除 DC 並反轉後的交流訊號（置中到 512）
4. `beatDetected`：門檻觸發方波（0 或 512）

範例（示意）：

```text
522 520 511 0
524 521 514 512
523 521 513 0
```

## 參數調校

可優先調整下列常數：

- `ALPHA_SMOOTH`（預設 `0.2`）
  - 變大：更靈敏，但可能更容易抖動
  - 變小：更平滑，但延遲較明顯
- `ALPHA_DC`（預設 `0.02`）
  - 變大：更快追蹤環境光變化
  - 變小：基準線更穩定
- `THRESHOLD`（預設 `513`）
  - 建議看 `acSignal` 的波峰高度微調
  - 太低：容易誤判
  - 太高：容易漏判

## 疑難排解

- 看不到明顯波形
  - 確認手指與感測器貼合但不要過度施壓
  - 減少環境強光直接照射
  - 檢查 GND 是否共地、接線是否鬆動
- 波形雜訊太大
  - 降低 `ALPHA_SMOOTH`
  - 稍微提高 `THRESHOLD`
- 幾乎一直觸發或完全不觸發
  - 先觀察 `acSignal` 的實際上下界，再調 `THRESHOLD`

## 注意事項

- 此專案僅供教學、實驗與訊號觀察，不作為醫療用途。
- 不同手指、壓力、皮膚狀態、環境光都會影響訊號品質。

## 貢獻方式

歡迎透過 Issue / Pull Request 改善本專案，例如：

- 新增 BPM 計算
- 加入自動門檻調整
- 增加濾波策略（中值濾波、帶通等）

## 授權

本專案採用 MIT License。

完整條款請見 `LICENSE`。

---

## 參考（README 撰寫風格）

本 README 章節設計參考了常見開源專案的組織方式（專案介紹、安裝/使用、疑難排解、貢獻與授權），包含：

- Arduino 官方專案 README
- PulseSensor Playground README
- Adafruit Sensor 類專案 README
