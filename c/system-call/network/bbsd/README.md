### Use inetd

```bash
$ echo "experiment	18888/tcp" >> /etc/services  # 18888 is port number
$ echo "stream	tcp	nowait	root	[app_inetd's path]	app_inetd" >> /etc/inetd.conf
$ /etc/rc.d/init.d/inet restart
```

### Use xinetd

```bash
$ vi /etc/xinetd.d/experiment

service experiment
{
  id          = experiment
  socket_type = stream
  protocol    = tcp
  user        = root
  wait        = no
  disable     = no
  service     = [app_inetd's path]
}

$ /etc/init.d/xinetd restart
```
