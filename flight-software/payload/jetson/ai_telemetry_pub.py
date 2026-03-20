import time
import logging
import zenoh
from telemetry_schema import PayloadTelemetry, Detection

logging.basicConfig(level=logging.INFO, format='%(asctime)s - PUB - %(message)s')
logger = logging.getLogger(__name__)

def main():
    conf = zenoh.Config()
    topic = "orion/payload/telemetry"
    
    try:
        session = zenoh.open(conf)
        pub = session.declare_publisher(topic)
        logger.info(f"Zenoh session active. Publishing to {topic}")
        
        while True:
            # Simulate inference data
            data = PayloadTelemetry(
                timestamp=time.time(),
                cpu_temp_c=45.2,
                detections=[Detection(class_name="solar_panel", confidence=0.92, bounding_box=(10, 20, 50, 60))]
            )
            
            payload_json = data.model_dump_json()
            pub.put(payload_json)
            logger.info("Published telemetry payload successfully.")
            
            time.sleep(1)
            
    except Exception as e:
        logger.error(f"Critical error: {e}")
    finally:
        session.close()

if __name__ == "__main__":
    main()
