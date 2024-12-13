#!/bin/sh

gs -dPDFX -dBATCH -dNOPAUSE -r1200 -dUseCIEColor -dAutoRotatePages=/None -dProcessColorModel=/DeviceCMYK  -sDEVICE=pdfwrite -sOutputFile=$2 ./myPDFX_def_profile.ps $1 
