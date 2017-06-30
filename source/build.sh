MAIN="main.c"
SRCS=(display dump)
OBJS=( )
OUT="../Builds/bytion"

for i in ${!SRCS[*]}; do
  OBJS[i]="objs/${SRCS[i]}.o";
done;

HELP="Usage: build [<options>] \n
    clear ---------- delete build binarys\n
    check ---------- check for compilation\n
    debug ---------- pass _DEBUG macro into code & compile\n
    help ----------- show current help message"

function BuildIncludes() {
  for name in ${SRCS[@]}; do
    echo "---Building $name---"
    gcc $@ -c inc/$name.c -o ./objs/$name.o
    if [ $? -ne 0 ]; then
      echo "---Building $name Failed---"
      exit 1
    fi;
  done;
}

function RegularBuild() {
  BuildIncludes -O3
  if [ $? -ne 0 ]; then
    exit 1
  fi
  echo "---Building Bytion---"
  gcc -O3 -o $OUT $MAIN ${OBJS[*]}
  if [ $? -eq 0 ]; then
    echo "---Build successfull---"
  fi
}

function  DebugBuild() {
  BuildIncludes -O3 -D_DEBUG
  if [ $? -ne 0 ]; then
    exit 1
  fi
  echo "---Building Bytion---"
  gcc -O3 -D_DEBUG -o $OUT $MAIN ${OBJS[*]} -Wextra
  if [ $? -eq 0 ]; then
    echo "---Debug Build Successfull---"
  fi
}

if [ $# -eq 0 ]; then
  RegularBuild
elif [ $# -eq 1 ]; then
  case "$1" in
    debug)
      DebugBuild
      ;;
    clear)
      rm -i bytion
      ;;
    help)
      echo $HELP
      ;;
    *)
      printf "'$1' is not an option\n"
      echo -e $HELP
      ;;
  esac
else
  echo -e $HELP
fi
