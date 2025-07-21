# HydraAV Sandbox

Optional dynamic analysis component for HydraAV.

## Overview

The sandbox provides isolated execution environment for suspicious files using Docker containers.

## Requirements

- Docker 20.0+
- Python 3.9+ (for building)

## Building

```bash
docker build -t hydraav-sandbox .
```

## Usage

The sandbox is automatically called by the main scanner when `--sandbox` flag is used.

## Architecture

- `container_analyzer.py`: Main analysis engine
- `Dockerfile`: Container definition
- `requirements.txt`: Python dependencies

## Security

- Runs as non-root user
- Isolated network and filesystem
- Timeout protection (30 seconds)
- Resource limits 