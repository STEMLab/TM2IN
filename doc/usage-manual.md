# TM2IN - Usage

## Command line

```commandline
TM2IN --input-dir=.input/ --output-dir=.output/ input.tvr
```

만약 short option을 입력할 경우에는 하이픈(-)을 하나만 써줍니다. short option은 대소문자를 구분합니다.

```commandline
TM2IN -I /home/input/something/ -O output_folder/ --no-merge input.3ds 
```
## Options

괄호 안의 알파벳은 short option 입니다.
기울어진 글꼴은 flag option 입니다.

| option | description |
|----------|-------------------------|
| input-dir(I) | input root directory (required) |
| output-dir(O) | output root directory (required) |
| version(v) | output version name (default : "no-version") |
| input-type(r) | |
| *no-merge* | Surface Merging 작업을 하지 않습니다. (Input triangles 그대로 출력) |
| polygonizer | |
| thres1 | |
| thres2 | |
| *output-tvr* | |
| *output-3ds* | |
| *output-tri* | |
| *write-process* | |
| select-arch | |
