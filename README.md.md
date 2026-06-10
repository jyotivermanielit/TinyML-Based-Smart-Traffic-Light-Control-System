**Smart Adaptive Traffic Control using TinyML \& Drone IoT Surveillance**



**Overview**



This project presents a Smart Adaptive Traffic Control System that combines TinyML, IoT, and drone-based surveillance to optimize traffic signal timing in real time. A drone-mounted ESP32-CAM captures aerial traffic images and performs on-device traffic density classification using a TinyML model trained with Edge Impulse. The classified traffic density is transmitted wirelessly to an ESP32 DevKit, which dynamically adjusts traffic signal timings based on road congestion levels.



By performing machine learning inference directly on the ESP32-CAM, the system eliminates cloud dependency, reduces latency, and enables fast decision-making for intelligent traffic management applications.



Traffic density is classified into:

* Low Vehicle Density
* Medium Vehicle Density
* High Vehicle Density



**Adaptive Signal Timing**



The green signal duration is dynamically adjusted using:



Green Time = Density Weight × Confidence Score × 30



Where:

Low Density    → Weight = 1

Medium Density → Weight = 2

High Density   → Weight = 3



**System Workflow**

* Drone-mounted ESP32-CAM captures real-time aerial traffic images.
* TinyML model classifies traffic density as Low, Medium, or High.
* Classification result and confidence score are transmitted via Wi-Fi.
* ESP32 DevKit receives the prediction.
* Green signal duration is dynamically adjusted based on traffic density.
* Traffic signals respond in real time to changing road conditions.



**Features**

* Drone-based aerial traffic monitoring
* TinyML traffic density classification on ESP32-CAM
* Real-time edge AI inference
* Adaptive traffic signal timing
* Wi-Fi communication between ESP32 devices
* Low-cost and scalable smart city solution
* Cloud-independent operation with low latency



**Technologies Used**

* ESP32-CAM
* ESP32 DevKit
* Edge Impulse
* TinyML
* Arduino IDE



**Testing Results**



| Metric    | Value  |

| --------- | ------ |

| Accuracy  | 78.79% |

| Precision | 0.88   |

| Recall    | 0.88   |

| F1 Score  | 0.88   |

| ROC AUC   | 0.95   |





**Results**



The system successfully:

* Captured real-time traffic images.
* Classified vehicle density using TinyML.
* Dynamically adjusted green signal duration.
* Communicated wirelessly using HTTP over WiFi.
* Reduced unnecessary waiting time.



**Future Scope**

* Drone-based traffic monitoring.
* Vehicle counting.
* Multi-lane traffic optimization.
* Cloud dashboard integration.
* Smart city deployment.

