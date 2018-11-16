# TM2IN - Usage

## Command line

```commandline
TM2IN --input-dir=.input/ --output-dir=.output/ input.tvr
```

## Options

기울어진 글꼴은 flag option 입니다.

| option | description |
|----------|-------------------------|
| input-dir | input root directory (required) |
| output-dir | output root directory (required) |
| version | output version name (default : "no-version") |
| input-type | the type of input file. (tvr, 3ds, dae) |
| *no-merge* | Surface Merging 작업을 하지 않습니다. (Input triangles 그대로 출력) |
| polygonizer | select Polygonizer. 0 : do nothing, 1 : PCA Polygonize, 2 : Triangle Polygonize, 3 : Divided Polygonize |
| thres1 | Threshold value 1 for merging algorithm |
| thres2 | Threshold value 2 for merging algorithm |
| *output-tvr* | print TVR file |
| *output-3ds* | print 3DS file |
| indoorGML | InFactory server url |
| *output-tri* | print JSON file with triangles |
| *write-process* | record process in file  |
| select-arch | select elements which are remained. 0 : architectural, 1 : non-architectural, 2 : all, 3 : manually choose |
| *do-validation* | do validation or not  |
