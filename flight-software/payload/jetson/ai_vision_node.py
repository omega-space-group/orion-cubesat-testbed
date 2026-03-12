import time
import logging
import cv2
import zenoh
from ultralytics import YOLO
from telemetry_schema import PayloadTelemetry, Detection

logging.basicConfig(level=logging.INFO, format='%(asctime)s - VISION - %(message)s')
logger = logging.getLogger(__name__)

def main():
    # 1. Initialize Zenoh Middleware
    conf = zenoh.Config()
    topic = "orion/payload/telemetry"
    session = zenoh.open(conf)
    pub = session.declare_publisher(topic)
    
    # 2. Initialize AI Model
    logger.info("Loading YOLOv8n model...")
    # 'yolov8n.pt' will be downloaded automatically the first time this runs
    model = YOLO('yolov8n.pt') 
    
    # 3. Initialize Video Stream (0 for default webcam, or path to a test video)
    # Using 0 assumes you have a webcam. If not, we will mock it later.
    cap = cv2.VideoCapture(0)
    
    if not cap.isOpened():
        logger.error("Failed to open camera/video stream.")
        return

    logger.info(f"Vision pipeline active. Publishing to {topic}")

    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                logger.warning("Dropped frame or end of stream.")
                break
                
            start_time = time.time()
            
            # Run inference
            results = model(frame, verbose=False)
            
            # Parse results into our strict schema
            detections_list = []
            for r in results:
                boxes = r.boxes
                for box in boxes:
                    # Extract coordinates, confidence, and class
                    x1, y1, x2, y2 = box.xyxy[0].tolist()
                    conf_score = float(box.conf[0])
                    class_id = int(box.cls[0])
                    class_name = model.names[class_id]
                    
                    # Create validated Detection object
                    det = Detection(
                        class_name=class_name,
                        confidence=conf_score,
                        bounding_box=(int(x1), int(y1), int(x2), int(y2))
                    )
                    detections_list.append(det)
            
            # Create the final telemetry payload
            telemetry = PayloadTelemetry(
                timestamp=time.time(),
                cpu_temp_c=45.0, # Mocked hardware thermal for now
                detections=detections_list
            )
            
            # Serialize and Publish
            pub.put(telemetry.model_dump_json())
            
            latency = (time.time() - start_time) * 1000
            logger.info(f"Processed frame in {latency:.1f}ms. Published {len(detections_list)} detections.")
            
    except KeyboardInterrupt:
        logger.info("Vision node gracefully shutting down...")
    except Exception as e:
        logger.error(f"Critical vision pipeline failure: {e}")
    finally:
        cap.release()
        session.close()

if __name__ == "__main__":
    main()
