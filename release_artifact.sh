#!/bin/bash

set -o errexit
set -o nounset
set -x
docker build -t "mde-artifact-docker" .
docker save mde-artifact-docker | gzip > mde-artifact-docker.tar.gz
tar -zcvf thirdparty_sources.tar.gz thirdparty_sources
tar -zcvf mde_artifact.tar.gz \
	thirdparty_sources.tar.gz \
	mde-artifact-docker.tar.gz \
	README.md \
	LICENSE \
	Dockerfile \
	tool.py