#*******************************************************************************
# Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
#*******************************************************************************
## Process this file with automake to produce Makefile.in

info_TEXINFOS = gto.texi

SUFFIXES = .html

html_docs = gto.html

.texi.html:
	$(HTMLGEN) $<

.PHONY: html
html: $(html_docs)
