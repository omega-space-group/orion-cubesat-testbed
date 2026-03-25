from pydantic import BaseModel, Field
from typing import List, Tuple

class Detection(BaseModel):
    class_name: str = Field(..., description="Detected object class")
    confidence: float = Field(..., ge=0.0, le=1.0, description="Inference confidence score")
    bounding_box: Tuple[int, int, int, int] = Field(..., description="(x_min, y_min, x_max, y_max)")

class PayloadTelemetry(BaseModel):
    timestamp: float = Field(..., description="Epoch timestamp")
    cpu_temp_c: float = Field(..., description="Jetson CPU temperature")
    detections: List[Detection] = Field(default_factory=list, description="Object detections")
