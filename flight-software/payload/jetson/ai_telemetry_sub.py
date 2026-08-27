import time
import logging
import zenoh
from telemetry_schema import PayloadTelemetry

logging.basicConfig(level=logging.INFO, format='%(asctime)s - SUB - %(message)s')
logger = logging.getLogger(__name__)

def listener(sample):
    try:
        raw_data = sample.payload.to_string()
        # Strictly validate incoming data against the schema
        validated_data = PayloadTelemetry.model_validate_json(raw_data)
        logger.info(f"Received valid telemetry. Temp: {validated_data.cpu_temp_c}C, Detections: {len(validated_data.detections)}")
    except Exception as e:
        logger.error(f"Data validation failed: {e}")

def main():
    conf = zenoh.Config()
    topic = "orion/payload/telemetry"
    
    try:
        session = zenoh.open(conf)
        _sub = session.declare_subscriber(topic, listener)
        logger.info(f"Listening on {topic}...")
        
        while True:
            time.sleep(1)
            
    except Exception as e:
        logger.error(f"Subscriber error: {e}")
    finally:
        session.close()

if __name__ == "__main__":
    main()
