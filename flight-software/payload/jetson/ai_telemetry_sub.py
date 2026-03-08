import zenoh
import time
import logging

logging.basicConfig(level=logging.INFO, format='%(asctime)s - [COMMS LINK] - %(message)s')

def listener(sample):
    # Callback function triggered whenever new data arrives
    data = sample.payload.to_string()
    logging.info(f"Received High-Bandwidth Data on '{sample.key_expr}': {data}")

def main():
    logging.info("Initializing Zenoh Middleware Session...")
    conf = zenoh.Config()
    session = zenoh.open(conf)

    topic = "orion/payload/telemetry"
    subscriber = session.declare_subscriber(topic, listener)
    logging.info(f"Listening for Jetson AI telemetry on topic: '{topic}'...")

    try:
        # Keep the main thread alive while the Zenoh listener handles incoming data
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        logging.info("Shutting down Subscriber...")
    finally:
        session.close()

if __name__ == "__main__":
    main()

