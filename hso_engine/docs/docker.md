## Instructions: Docker build and run
### Docker
The user must install Docker first. Upon installation, to avoid the need for `sudo`
run the commands from a terminal:

  ```bash
  # create the group `docker`
  sudo groupadd docker

  # add user name to the group
  sudo usermod -aG docker $USER

  # make the session recognize the group without logging out
  newgrp docker
  ```
The last command is not necessary *after* the user logs out.
### Build
Navigate to the repository's root directory and run the build command:

  ```bash
  #hso:v1.0.0 can be any other custom name for the image
  docker build -t hso:v1.0.0 .
  ```
Docker will install the program and all of its dependencies in an isolated environment, without
interfering with system-wide installations. The first time the image is built, docker
will take longer installing external packages (Minuit2, CUBA, LHAPDF, etc.), but subsequent
builds are significantly faster since they use Docker's layer caching.

### Run
To execute the program and store results in your host machine, use the follwing command.
The -v binds the local `results/` directory to the container's output path
  ```bash
  docker run --rm -v $(pwd)/results:/app/results \
  hso:v1.0.0 input/kin_drellyan.input input/para_hso.input OUTDIRNAME

  ```
Where:
  - `--rm` removes the container instance upon completion
  - `hso:v1.0.0` is the program's name (same as specified at built)
  - `input/kin_drellyan.input` File to setup kinematical cuts and choice of PDF sets (currently only MMHT2014nlo68cl provided)
  - `input/para_hso.input` File  to fix/release/limit the model parameters
  - `OUTDIRNAME` is the name of the directory where the program  outputs results of the fit(`./results/OUTDIRNAME`)
