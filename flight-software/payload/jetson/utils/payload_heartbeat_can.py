#!/usr/bin/env python3
"""Send a simple PAYLOAD heartbeat frame over SocketCAN."""

import argparse
import signal
import socket
import struct
import sys
import time


CAN_FRAME_FORMAT = "=IB3x8s"
CAN_ID_PAYLOAD_HEARTBEAT = 0x400
STATUS_BOOT = 1
STATUS_OK = 2
STATUS_ERROR = 3
STATUS_NAMES = {
    STATUS_BOOT: "BOOT",
    STATUS_OK: "OK",
    STATUS_ERROR: "ERROR",
}

def build_can_frame(can_id: int, payload: bytes) -> bytes:
    if len(payload) > 8:
        raise ValueError("CAN payloads cannot exceed 8 bytes")

    data = payload.ljust(8, b"\x00")
    return struct.pack(CAN_FRAME_FORMAT, can_id, len(payload), data)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Send PAYLOAD heartbeat: standard CAN ID 0x400, DLC 1."
    )
    parser.add_argument(
        "-i",
        "--interface",
        default="can0",
        help="SocketCAN interface to use, default: can0",
    )
    parser.add_argument(
        "--boot-seconds",
        type=float,
        default=10.0,
        help="send BOOT before switching to OK, default: 10.0",
    )
    parser.add_argument(
        "--error-at",
        type=float,
        default=30.0,
        help="send one ERROR frame after x seconds, default: 30.0",
    )
    parser.add_argument(
        "-t",
        "--interval",
        type=float,
        default=0.5,
        help="seconds between frames, default: 0.5",
    )
    parser.add_argument(
        "-c",
        "--count",
        type=int,
        default=0,
        help="number of frames to send, default: 0 means forever",
    )
    args = parser.parse_args()

    if args.interval <= 0:
        parser.error("--interval must be greater than zero")
    if args.count < 0:
        parser.error("--count cannot be negative")
    if args.boot_seconds < 0:
        parser.error("--boot-seconds cannot be negative")
    if args.error_at < 0:
        parser.error("--error-at cannot be negative")

    stop = False

    def handle_stop(_signum, _frame):
        nonlocal stop
        stop = True

    signal.signal(signal.SIGINT, handle_stop)
    signal.signal(signal.SIGTERM, handle_stop)

    try:
        with socket.socket(socket.PF_CAN, socket.SOCK_RAW, socket.CAN_RAW) as can_socket:
            can_socket.bind((args.interface,))
            print(
                f"Sending PAYLOAD heartbeat on {args.interface}: "
                f"id=0x{CAN_ID_PAYLOAD_HEARTBEAT:03X} dlc=1 "
                f"BOOT={STATUS_BOOT} OK={STATUS_OK} ERROR={STATUS_ERROR} "
                f"interval={args.interval:.3f}s"
            )

            sent = 0
            error_sent = False
            started_at = time.monotonic()
            next_send = time.monotonic()
            while not stop and (args.count == 0 or sent < args.count):
                elapsed = time.monotonic() - started_at
                if not error_sent and elapsed >= args.error_at:
                    status = STATUS_ERROR
                    error_sent = True
                elif elapsed < args.boot_seconds:
                    status = STATUS_BOOT
                else:
                    status = STATUS_OK

                frame = build_can_frame(CAN_ID_PAYLOAD_HEARTBEAT, bytes([status]))
                can_socket.send(frame)
                sent += 1
                print(
                    f"sent #{sent}: 400#{status:02X} "
                    f"({STATUS_NAMES[status]}, t={elapsed:.1f}s)",
                    flush=True,
                )

                next_send += args.interval
                sleep_time = next_send - time.monotonic()
                if sleep_time > 0:
                    time.sleep(sleep_time)

    except OSError as exc:
        print(f"CAN error on {args.interface}: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
