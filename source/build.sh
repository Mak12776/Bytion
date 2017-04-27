gcc -O3 -o bytion main.c -Wextra
if [ $? -eq 0 ]; then
  echo "---Build successfull---"
  exit 0;
fi
