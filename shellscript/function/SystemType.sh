SystemType() {
  _HOSTNAME=$(hostname | sed 's/\..*//')

  case $(uname -s) in
    FreeBSD ) echo "FreeBSD" ;;
    Linux   ) echo "Linux"   ;;
    Darwin  ) echo "mac"     ;;
    AIX     ) echo "AIX"     ;;
    HP-UX   ) echo "HP"      ;;
    IRIX    ) echo "SGI"     ;;
    OSF1    ) echo "DECOSF"  ;;
    ULTRIX  ) echo "ULTRIX"  ;;

    SunOS )
      case $(uname -r) in
        4* ) echo "Sun BSD" ;;
        5* ) echo "Solaris" ;;
      esac

      ;;

    ${_HOSTNAME} )
      case $(uname -m) in
        IP*    ) echo "SGI"  ;;
        i386   ) echo "SCD"  ;;
        x86    ) echo "IA32" ;;
        x86_64 ) echo "IA64" ;;
      esac

      ;;
  esac
}
