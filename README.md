# Bytion
a byte option program


## bytion -help
```
Usage: ./bytion [Option] file [file1] [file2] [file3] ...

Options:
  -help                                     show this help and exit
  -n [number], -column-number [number]      specify number of output columns
  -m [mode], -mode [mode]                   specify modes for output results
                                            modes can be a string of letters
  -nm, -no-mode                             restore to default mode
  -d, -display                              read files and write them on stdout
  -ds, -display-strings                     read files and write only strings on stdout
  -dm, -dump                                dump file byte into stdout
  -f [pattern], -find [pattern]             find pattern in files (under construction)
  -v, -version                              show version & creator
Modes:
  for display:                              default mode: nothing
  c                                         colorful.
  l                                         show ascii letters.
  d                                         show bytes in decimal.
  for dump:                                 default mode: nothing
  c                                         colorful.
  s                                         print space instead of dot
```
## license
DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
