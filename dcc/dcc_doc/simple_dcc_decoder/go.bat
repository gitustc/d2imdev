@echo off
copy /Y ..\..\alleg42.dll .
copy /Y ..\..\msvcr70.dll .
dcc_extract FATRLITA1HTH.dcc > Fatrlita1hth.txt
