# 🌍 Landslide Prediction and Early Warning System (LPEWS) Using LSTM Neural Networks

## 📌 Overview
Landslides are catastrophic natural disasters that threaten lives, infrastructure, and ecosystems. The **LPEWS** project leverages **LSTM (Long Short-Term Memory)** neural networks and **IoT sensors** to predict landslide occurrences in real time by analyzing historical and environmental data patterns. The trained model runs on edge devices and triggers timely alerts, enabling proactive disaster management.

## ⚠️ Problem Statement
Traditional landslide monitoring lacks precision and real-time responsiveness. Our system:
- Predicts landslide probability using time-series data (e.g., rainfall, soil moisture, seismic activity).
- Sends early warnings if the risk crosses a threshold.
- Operates independently via low-cost IoT hardware in remote regions.

## 📊 Data Requirements

### 🌦 Meteorological Inputs:
- Rainfall Intensity (mm/hr)
- Rainfall Accumulation (mm/day)
- Humidity (%)
- Wind Speed (m/s)
- Temperature (°C)

### 🌍 Geophysical Inputs:
- Soil Moisture (%)
- Seismic Activity (Hz)
- Slope Angle (°)
- Soil Type & Density

### 📚 Historical Events:
- Records of past landslides with timestamps, location, and severity

## 🧠 Model Architecture (LSTM)

```
Input → LSTM Layers → Dropout → Dense Layers → Output (Risk Probability)
```

### Key Features:
- **Sequential Modeling**: Captures temporal relationships in environmental data.
- **Probability Output**: Risk classified into:
  - Low (0–30%)
  - Moderate (30–70%)
  - High (70–100%)

## 🏋️ Training Process

1. **Preprocessing**: Normalize features, handle missing data, encode soil type.
2. **Loss Function**: 
   - Binary Cross-Entropy for Yes/No prediction
   - Categorical Cross-Entropy for risk category
3. **Optimizer**: Adam for faster convergence
4. **Evaluation**: Accuracy on validation data, tuning hyperparameters

## 🚀 Deployment & Real-Time Monitoring

- **Edge Device**: Raspberry Pi / ESP32 / Jetson Nano
- **Connectivity**: LoRaWAN / MQTT for sensor data streaming
- **Automated Alerts**:
  - SMS, sirens, or messages to government agencies
- **Live Dashboard**: Google Sheets or cloud dashboard integration

## ✅ Benefits

- Real-Time Predictions with LSTM
- Cost-Effective using low-power edge hardware
- Scalable to multiple regions
- High Accuracy due to adaptive learning
- Saves Lives via proactive alerts

## 🔎 Sample Code

```python
# Basic LSTM Model for Landslide Prediction
import numpy as np
import pandas as pd
import tensorflow as tf
from sklearn.preprocessing import MinMaxScaler
from sklearn.model_selection import train_test_split
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense, Dropout

df = pd.read_csv('data.csv')
features = ['rainfall', 'soil_moisture', 'seismic_activity', 'slope_angle', 'temperature', 'humidity']
target = 'landslide_risk'
scaler = MinMaxScaler()
df[features] = scaler.fit_transform(df[features])

def create_sequences(data, target_col, time_steps=10):
    X, y = [], []
    for i in range(len(data) - time_steps):
        X.append(data.iloc[i:i+time_steps][features].values)
        y.append(data.iloc[i+time_steps][target_col])
    return np.array(X), np.array(y)

X, y = create_sequences(df, target)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)

model = Sequential([
    LSTM(100, return_sequences=True, input_shape=(X.shape[1], X.shape[2])),
    Dropout(0.2),
    LSTM(50),
    Dropout(0.2),
    Dense(25, activation='relu'),
    Dense(1, activation='sigmoid')
])

model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])
model.fit(X_train, y_train, epochs=50, batch_size=32, validation_data=(X_test, y_test))
model.save('landslide_prediction_model.h5')
```

## 📡 Data Sources

- [NASA Global Landslide Catalog](https://catalog.data.gov/dataset/global-landslide-catalog)
- [USGS Landslide Hazards Program](https://www.usgs.gov/natural-hazards/landslide-hazards)
- [ISRO Bhuvan Portal](https://bhuvan.nrsc.gov.in/)
- [OpenWeatherMap API](https://openweathermap.org/api)
- [NOAA Climate Data](https://www.ncdc.noaa.gov/)
- [USGS Earthquake API](https://earthquake.usgs.gov/fdsnws/event/1/)
- [Kaggle Datasets](https://www.kaggle.com/)
- [Google Dataset Search](https://datasetsearch.research.google.com/)

## 🛠 Software Requirements

- Python 3.7+
- TensorFlow / Keras
- NumPy, Pandas, Scikit-learn
- Matplotlib, Seaborn

## 📈 Future Enhancements

- Integration with mobile app for alerts
- GIS-based risk mapping
- Model retraining using auto-updated cloud data
- Use of renewable power sources (solar, wind)
- Drone support for aerial scanning

## 🙌 Contribution & Acknowledgement

This project is a collaborative final-year academic research prototype by Abhishek Debnath and Santa Sarkar, under the supervision of Dr. Mitra Barun Sarkar at **NIT Agartala**.
