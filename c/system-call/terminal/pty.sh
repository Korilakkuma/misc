# master
stty -icanon min 1 time 0; cat > /dev/ptyqf

# slave

(stty -icanon min 1 time 0; vi foo) < /dev/ttyqf

# (sleep 1; ./pty_tester > /dev/ttyqf) & grep "failed" < /dev/ptyqf
