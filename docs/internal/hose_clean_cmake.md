ENTRY_POINT = HSODrellYanFitter
DY_KINEMATICS ?= kin_drellyan.input
MODEL_PARAMETERS ?= para_hso.input
OUT_DIR ?= output_dir_default

IMAGE_NAME = hso
VERSION    = $(shell cat VERSION)

USER_ID := $(shell id -u)
GROUP_ID := $(shell id -g)

COLOR_OK       := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_OK     ')
COLOR_DIM      := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_DIM    ')
COLOR_SOFT_OK  := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_SOFT_OK')
COLOR_WARNING  := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_WARNING')
COLOR_ERR      := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_ERR    ')
COLOR_MSG      := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_MSG    ')
COLOR_NC       := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_NC     ')


all: help

help:
	@echo "HSO Build System - v$(VERSION)"
	@echo "--------------------------------------"
	@echo "make dev          :  Build with debug symbols/instrumentation"
	@echo "make audit        :  Build with strict linkage/optimization audit"
	@echo "make release      :  Build for production"
	@echo "make docker-build :  Build for production (docker image with all dependencies)"

clean:
	@rm -rf build/


docker-build: verify
#Docker builds the release version
#upon succesful completion of tests on the audit build
	echo "*** [HSO] Building and Verifying in Docker Container."
	docker build -t $(IMAGE_NAME):v$(VERSION) .

docker-run:
	@mkdir -p $(shell pwd)/results
	docker run --rm -u \
	$(shell id -u):$(shell id -g) \
	-v $(shell  pwd)/results:/app/results \
	-v $(shell pwd)/input:/app/input \
	--entrypoint $(ENTRY_POINT) \
	$(IMAGE_NAME):v$(VERSION) \
	/app/input/$(DY_KINEMATICS) \
	/app/input/$(MODEL_PARAMETERS) \
	$(OUT_DIR)

