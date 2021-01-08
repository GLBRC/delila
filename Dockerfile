FROM ubuntu:focal-20201106
SHELL ["/bin/bash","-c"]

#DEPENDENCIES - REPOSITORY
RUN apt-get update \
	&& DEBIAN_FRONTEND=noninteractive apt-get upgrade -y \
	&& DEBIAN_FRONTEND=noninteractive apt-get install -y gcc-8 g++-8 autoconf apt-utils git build-essential wget curl unzip python3-pip ghostscript nano \
	&& apt-get autoremove -y \
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/* 

RUN cd /opt/ \
	&& wget -q https://alum.mit.edu/www/toms/p2c/p2c-2.01.tar.gz \
	&& tar -xf p2c-2.01.tar.gz \
	&& cd /opt/p2c-2.01 \
	&& make 
	
RUN cd /opt/ \
	&& git clone https://github.com/GLBRC/delila \
	&& sed -i 's@/home/mplace/scripts/delila/@/opt/delila/@g' /opt/delila/delila_pipeline.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/delila_instructions.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/delila_pipeline.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/filter_sites.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/define_site_position.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/merge_books.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/merge_instructions.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/organizing_ri_delila_results.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/removeRI_books.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/removeRI_instructions.py \
	&& sed -i "s@#!/usr/bin/env python@#!/usr/bin/env python3@g" /opt/delila/rename_lib1.py \
	&& cd /opt/delila/src \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' alist.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' catal.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' comp.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' dalvec.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' dbbk.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' delila.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' encode.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' makelogo.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' malign.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' malin.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' ri.c \
	&& sed -i 's@/home/mplace/bin/p2c/src/p2c.h@/opt/p2c-2.01/src/p2c.h@g' rseq.c \
	&& sed -i 's@/home/mplace/bin/p2c/src@/opt/p2c-2.01/src@g' Makefile \
	&& sed -i 's@malin mkdb ri@malin ri@g' Makefile \
	&& make

ENTRYPOINT ["/bin/bash"]



