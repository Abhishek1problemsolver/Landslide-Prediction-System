import numpy as np
import pandas as pd
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense, Dropout
from sklearn.preprocessing import MinMaxScaler, LabelEncoder
from sklearn.model_selection import train_test_split
from tensorflow.keras.utils import to_categorical

# Load dataset
df = pd.read_csv('regenerated_landslide_risk_dataset.csv')

# Rename columns to make it clean and usable
df.columns = ['temperature', 'humidity', 'precipitation', 'soil_moisture', 'elevation', 'risk']

# Step 1: Print all unique risk labels (before filtering)
print("Original unique risk labels:", df['risk'].unique())

# Step 2: Filter only the known/valid classes
valid_risks = ['Low', 'Moderate', 'High']
df = df[df['risk'].isin(valid_risks)]

# Step 3: Encode the labels to numbers (Low=1, Moderate=2, High=0 etc.)
label_encoder = LabelEncoder()
df['risk'] = label_encoder.fit_transform(df['risk'])

# Optional: Print the label encoding mapping
print("Encoded classes mapping:", dict(zip(label_encoder.classes_, label_encoder.transform(label_encoder.classes_))))

# Step 4: Define features and target
features = ['temperature', 'humidity', 'precipitation', 'soil_moisture', 'elevation']
target = 'risk'

# Step 5: Normalize features
scaler = MinMaxScaler()
df[features] = scaler.fit_transform(df[features])

# Step 6: Create sequences for LSTM
def create_sequences(data, target_col, time_steps=10):
    X, y = [], []
    for i in range(len(data) - time_steps):
        X.append(data.iloc[i:i+time_steps][features].values)
        y.append(data.iloc[i+time_steps][target_col])
    return np.array(X), np.array(y)

X, y = create_sequences(df, target)

# Step 7: One-hot encode the labels for multi-class classification
y = to_categorical(y, num_classes=3)

# Step 8: Train-test split
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Step 9: Build LSTM Model
model = Sequential([
    LSTM(100, return_sequences=True, input_shape=(X.shape[1], X.shape[2])),
    Dropout(0.2),
    LSTM(50, return_sequences=False),
    Dropout(0.2),
    Dense(25, activation='relu'),
    Dense(3, activation='softmax')  # Softmax for 3 classes
])

# Step 10: Compile the model
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

# Step 11: Train the model
model.fit(X_train, y_train, epochs=50, batch_size=32, validation_data=(X_test, y_test))

# Step 12: Save the model
model.save('landslide_multiclass_model.h5')
print("✅ Multi-Class Landslide Prediction Model Trained and Saved Successfully!")
