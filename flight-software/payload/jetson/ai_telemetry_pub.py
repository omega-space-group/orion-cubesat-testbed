import zenoh
import json
import time
import random
import logging

# Configure professional logging
logging.basicConfig(level=logging.INFO, format='%(asctime)s - [JETSON PAYLOAD] - %(message)s')

def main():
    logging.info("Initializing Zenoh Middleware Session...")
    conf = zenoh.Config()
    session = zenoh.open(conf)

    topic = "orion/payload/telemetry"
    publisher = session.declare_publisher(topic)
    logging.info(f"Zenoh Publisher declared on topic: '{topic}'")

    try:
        logging.info("Starting simulated AI inference stream (1 Hz)...")
        while True:
            # Simulate Jetson AI inference data
            telemetry_data = {
                "timestamp": time.time(),
                "model": "yolov8_nano",
                "inference_fps": round(random.uniform(28.0, 32.0), 2),
                "detections": [
                    {"class": "debris", "confidence": round(random.uniform(0.85, 0.99), 2), "bbox": [10, 20, 100, 200]}
                ],
                "core_temp_c": round(random.uniform(40.0, 45.0), 1)
            }
            
            payload = json.dumps(telemetry_data)
            publisher.put(payload)
            logging.info(f"Published: {payload}")
            
            time.sleep(1) # Simulate 1 Hz processing tick
            
    except KeyboardInterrupt:
        logging.info("Shutting down AI Payload Publisher...")
    finally:
        session.close()

if __name__ == "__main__":
    main()
