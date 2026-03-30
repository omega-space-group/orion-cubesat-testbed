import pytest
from pydantic import ValidationError
from telemetry_schema import PayloadTelemetry, Detection

def test_valid_telemetry():
    data = PayloadTelemetry(
        timestamp=1000.0, cpu_temp_c=40.0,
        detections=[Detection(class_name="test", confidence=0.9, bounding_box=(0,0,1,1))]
    )
    assert data.cpu_temp_c == 40.0

def test_invalid_confidence():
    with pytest.raises(ValidationError):
        Detection(class_name="test", confidence=1.5, bounding_box=(0,0,1,1))
