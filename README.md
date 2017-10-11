# Balls-Bridge
## Requirements
* Python
* pip

## Setup
`$ pip install -r requirements.txt`

## Run
`$ python bridge.py`

## Available Environment variables
* BRIDGE_PORT (default 8080)

## Pattern Types

### Formula
contains
 - the formula as a string e.g. sin(x*PI/180) or x*x
 - the minimum y value
 - the maximum y value
 - the minimum x value
 - the maximum x value

example:
```json
{
  "formula": "sin(x)",
  "minX": 90,
  "maxX": 450,
  "maxY": 1,
  "minY": -1
}
```

### Timeline
Is an collection of multiple frames. Each frame has a duration in milliseconds

### Shifting Frame
Is a single frame. The duration of the frame is the time between each shift
