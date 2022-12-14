DownShift() {
  echo "$@" | tr "[A-Z]" "[a-z]"
}

UpShift() {
  echo "$@" | tr "[a-z]" "[A-Z]"
}
