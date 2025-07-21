# HydraAV Sandbox Docker Image
# Safe environment for dynamic malware analysis

FROM python:3.9-slim

# Set working directory
WORKDIR /sandbox

# Install system dependencies
RUN apt-get update && apt-get install -y \
    procps \
    lsof \
    net-tools \
    curl \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Copy Python requirements and install
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy sandbox files
COPY container_analyzer.py .

# Create directories for analysis
RUN mkdir -p /sandbox/input /sandbox/output /sandbox/logs

# Set environment variables
ENV PYTHONUNBUFFERED=1
ENV SANDBOX_MODE=1

# Run as non-root user for security
RUN useradd -m -s /bin/bash sandbox
RUN chown -R sandbox:sandbox /sandbox
USER sandbox

# Default command
CMD ["python", "container_analyzer.py"] 