# Hadron Structure Oriented approach to TMD phenomenology - Drell-Yan

## Build Instructions
This assumes the user works in a debian based system. Instructions for other
systems are not provided but there should be analogous steps.

### Setting Docker
The user must install Docker first (instructions not provided here). Upon installation, to avoid the need for `sudo` run the commands from a terminal:

  ```bash
  # create the group `docker`
  sudo groupadd docker

  # add $USER to the group
  sudo usermod -aG docker $USER

  # make the session recognize the group without logging out
  newgrp docker
  ```
The last command is not necessary *after* the user logs out. Building and running the code is done within a Docker container, but the user can simply run the bash scripts provided.

### Build
Starting from the repository's root directory, run the following commands from the terminal:

  ```bash
  #go to the engine's directory
  cd hso_engine
  #build the docker image
  bash devtools/hso_build_docker.sh
  ```
The last command contains the instructions for Docker to build containerized executables. Docker will install the program and all of its dependencies in an isolated environment, without interfering with system-wide installations. The first built takes longer since it installs dependencies (Minuit2, CUBA, LHAPDF, etc.), but subsequent builds are significantly faster since they leverage Docker's layer caching. Unless the user needs to modify the source code of the hso engine, the build is needed just once.


