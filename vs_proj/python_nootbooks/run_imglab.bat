set xmlname=positive_train_dataset_crop.xml
set toolkit=d:\DATASET\object_recognition_toolkit
set path=%toolkit%;


::imglab -h
::imglab -c %xmlname% -r %datadir%
imglab %xmlname%
imglab %xmlname% --stats
pause
