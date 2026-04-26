#!/bin/bash
# Run docker build (see Dockerfile for details)

cmake --preset release

cmake --build --preset release --target hso_docker_image_engine
