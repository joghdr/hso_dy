###########################################
###########################################
## NOTE: User-level parameters            #
###########################################
###########################################
ENTRY_POINT = HSODrellYanFitter
DY_KINEMATICS ?= kin_drellyan.input
MODEL_PARAMETERS ?= para_hso.input
OUT_DIR ?= output_dir_default
############################################

IMAGE_NAME = hso
VERSION    = $(shell cat VERSION)
HSO_ROOT := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
export HSO_ROOT

USER_ID := $(shell id -u)
GROUP_ID := $(shell id -g)


.PHONY: all help clean verify check
.PHONY: dev dev-registry-globals dev-test
.PHONY: audit audit-registry-globals audit-test
.PHONY: release release-registry-globals release-test
.PHONY: docker-build docker-run

MAKEFLAGS += -s


COLOR_OK       := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_OK     ')
COLOR_DIM      := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_DIM    ')
COLOR_SOFT_OK  := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_SOFT_OK')
COLOR_WARNING  := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_WARNING')
COLOR_ERR      := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_ERR    ')
COLOR_MSG      := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_MSG    ')
COLOR_NC       := $(shell bash -c 'source $(HSO_ROOT)/tests/helpers.sh && echo $$COLOR_NC     ')



define run_tests_static
	printf -- "%30s\n" ">>> static analysis <<<" && \
	export HSO_BUILD_DIR="$(1)" && \
	bash tests/static_analysis/sa_test_S1-1.sh && \
	bash tests/static_analysis/sa_test_S2-1.sh && \
	bash tests/static_analysis/sa_test_S3-1.sh && \
	bash tests/static_analysis/sa_test_S5-1.sh && \
	bash tests/static_analysis/sa_test_B1-B2.sh && \
	bash tests/static_analysis/sa_test_B3.sh
endef

define run_tests_validation
	printf -- "%30s\n" ">>> system validation <<<" && \
	export HSO_BUILD_DIR="$(1)" && \
	bash tests/system_validation/sv_test_fit_fast/run.sh
endef

define run_tests
	printf -- "\n----------- %s ------------\n" "Verifying $(1) $$(date +'%Y-%m-%d %H:%M:%S')" && \
	$(call run_tests_static,$(1)) && \
	$(call run_tests_validation,$(1)) && \
	printf "%b[%s]%b\n" "$(COLOR_OK)" "ALL TESTS SUCCEEDED"  "$(COLOR_NC)"
endef



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

verify: audit-test

check: verify

dev:
	cmake --preset dev
	cmake --build --preset dev --target try_build -j$(nproc)

dev-registry-globals: dev
	 @export HSO_BUILD_DIR=build/dev; \
	 bash tests/static_analysis/make_global_registries.sh

dev-test: dev
	if [ -f build/dev/SKIP_TESTS ]; then \
		echo "*** [HSO] Unresolved dependencies. Verification and build disabled."; \
	else \
		$(call run_tests,build/dev); \
	fi

audit:
	printf -- "\n----------- %s ------------\n" "Building build/audit"
	cmake --preset audit
	cmake --build --preset audit --target try_build -j$(nproc)

audit-registry-globals: audit
	 @export HSO_BUILD_DIR=build/audit; \
	 bash tests/static_analysis/make_global_registries.sh

audit-test: audit
	if [ -f build/audit/SKIP_TESTS ]; then \
		echo "*** [HSO] Unresolved dependencies. Verification and build disabled."; \
	else \
		$(call run_tests,build/audit); \
	fi

release:
	cmake --preset release
	cmake --build --preset release  --target try_build -j$(nproc)

release-registry-globals: release
	 @export HSO_BUILD_DIR=build/release; \
	 bash tests/static_analysis/make_global_registries.sh

release-test: release
	if [ -f build/release/SKIP_TESTS ]; then \
		echo "*** [HSO] Unresolved dependencies. Verification and build disabled."; \
	else \
		$(call run_tests,build/release); \
	fi

docker-build: verify
#Docker builds the release version
#upon succesful completion of tests on the audit build
	echo "*** [HSO] Building and Verifying in Docker Container."
	docker build -t $(IMAGE_NAME):v$(VERSION) .
#
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

get-globals:
	@echo "$(shell bash -c 'HSO_ROOT=$(HSO_ROOT) HSO_BUILD_DIR=build/release source tests/static_analysis/get_global_functions.sh; get_globals')" | sed -E "s|( [a-zA-Z0-9_/.-]+\.cpp\.o: )|\n\1|g" | cut -d: -f2-






