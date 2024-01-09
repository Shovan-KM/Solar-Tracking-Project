# Solar-Tracking-Project
Project development of a solar tracking system empowered with machine learning to ensure peak performance. The system utilizes dual servo motors to adjust the panel's position, optimizing sun exposure based on data from four light-dependent resistor (LDR) sensors. A unique feature is the incorporation of an additional LDR for detecting dust on the panel's surface, which, coupled with an automated water pump, enables the system to self-clean and maintain high efficiency.

The integration of machine learning is a standout aspect of this project. I used Arduino Uno to collect data on sun intensity and pump activity, which was then used to train machine learning models. With RandomForest and XGBoost algorithms, I analyzed the data to predict the optimal angle for voltage generation and to determine the timing for solar panel cleaning. Hyperparameter tuning and SMOTE (Synthetic Minority Over-sampling Technique) were applied to enhance model performance.

••••••••••••••
Solar Energy Optimization: Implementing solar tracking for maximum energy capture.
Sensor Integration: Utilizing LDR sensors for sun intensity detection and dust accumulation monitoring.
Automated System Design: Developing an automated cleaning mechanism using a water pump.

Machine Learning: Applied RandomForest and XGBoost for predictive analysis and optimization.
Data Collection: Utilized Arduino Uno for systematic data gathering.
Data Preprocessing: Employed techniques like SMOTE to balance the dataset.
Model Evaluation: Calculated RMSE (Root Mean Squared Error) and used confusion matrices to assess model accuracy.
Hyperparameter Tuning: Improved model performance with GridSearchCV.
