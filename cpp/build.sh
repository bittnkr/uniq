if [ "$1" = "" ]; then
    echo "Missig parameter, use like ./build.sh test"
else
  g++ $1.cpp -lpthread -O3  -o $1
  if [ $? -eq 0 ]; then
      time ./$1
  else
      echo FAIL  
  fi
fi
