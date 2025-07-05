#!/bin/bash

CONTAINER_VER="1.0"
PRODUCT_NAME="oresat-raw-update"
CERT_NAME="oresat"
ZLIB_IMAGE=""
RAW_IMAGE=""
HASH=""
FILENAME=""

generate_cert () {
  openssl req -x509 -newkey rsa:4096 -noenc -keyout $CERT_NAME.key.pem \
    -out $CERT_NAME.cert.pem -subj "/O=SWUpdate/CN=target" \
    -addext extendedKeyUsage=1.3.6.1.5.5.7.3.4 -addext keyUsage=digitalSignature
}

generate_sw_description () {
  IFS=' ' read HASH FILENAME < $ZLIB_IMAGE.sha256
  rm sw-description*

  echo "Generationg sw-description"
  
  echo "software =" > sw-description
  echo "{" >> sw-description
  echo "  version = "1.0";" >> sw-description
  echo "  images: (" >> sw-description
  echo "    {" >> sw-description
  echo "      filename = \"$FILENAME\";" >> sw-description
  echo "      device = "/dev/mmcblk0p2";" >> sw-description
  echo "      type = "raw";" >> sw-description
  echo "      sha256 = \"$HASH\";" >> sw-description
  echo "      compressed = true;" >> sw-description
  echo "    }" >> sw-description
  echo "  );" >> sw-description
  echo "}" >> sw-description

  cat sw-description
}

generate_cpio () {
  if [ ! -f $CERT_NAME.cert.pem ] || [ ! -f $CERT_NAME.key.pem ]; then
    generate_cert
  fi

  openssl cms -sign -in  sw-description -out sw-description.sig -signer $CERT_NAME.cert.pem \
          -inkey $CERT_NAME.key.pem -outform DER -nosmimecap -binary

  CPIO_FILES="sw-description sw-description.sig $ZLIB_IMAGE"
  
  for i in $CPIO_FILES;do
          echo $i;done | cpio -ov -H crc > ${PRODUCT_NAME}_${CONTAINER_VER}.swu
}

check_files () {
  if [ ! -f "$ZLIB_IMAGE" ] ; then
    echo "$ZLIB_IMAGE does not exist!"
    exit 1
  fi
    
  if [ "${ZLIB_IMAGE: -4}" != ".zst" ]; then
    echo "$ZLIB_IMAGE is not a .zst file"
    exit 1
  fi

  if [ ! -f "$ZLIB_IMAGE.sha256" ] ; then
    echo "$ZLIB_IMAGE.sha256 does not exist!"
    exit 1
  fi

  zstd -t $ZLIB_IMAGE
  if [ $? -ne 0 ]; then
    echo "Error: bad .zst file"
    exit 1	
  fi
}

if [ "$#" -lt 1 ]; then
  echo "not enough arguments"
  exit 1
fi

ZLIB_IMAGE="$1"

check_files

generate_sw_description

generate_cpio

#openssl dgst -sha256 -sign mycert.cert.pem sw-description > sw-description.sig

#if [ "$1" == "-c" ]; then
#  rm oresat-*
#  cp ../images/$IMAGE* .
#  cat $IMAGE-2gb.img.zst.sha256
#  unzstd $IMAGE-4gb.img.zst
#  exit 0
#fi
#
#if grep -qs '/media/chroco/rootfs' /proc/mounts; then
#    echo "It's mounted."
#    lsblk
#    umount /media/chroco/rootfs*
#    sudo dd if=$IMAGE of=/dev/sdb bs=128k status=progress
#else
#    echo "It's not mounted."
#fi


# checkparm () {
#   if [ "$(echo $1|grep ^'\-')" ] ; then
#     echo "E: Need an argument"
# #		usage
#     exit 1
#   fi
# }

#while [ ! -z "$1" ] ; do
#  case $1 in
#  -h|--help)
#    #usage
#    echo "help!"
#    ;;
#  -f|--filename)
#    checkparm $2
#    ZLIB_IMAGE="$2"
#
#    check_files
#    
#    zstd -d -f $ZLIB_IMAGE
#    if [ $? -ne 0 ]; then
#      echo "Error: zstd command failed."
#      exit 1	
#    fi
#
#    RAW_IMAGE=${ZLIB_IMAGE%.*}
#    if [ ! -f $RAW_IMAGE ]; then
#      echo "Error: invalid image"
#      exit 1	
#    fi
#
#    ;;
#  -c)
#    rm oresat-*
#    cp ../images/$IMAGE* .
#    #cat $IMAGE-2gb.img.zst.sha256
#    #unzstd $IMAGE-4gb.img.zst
#    ;;
#  --gen-sw-description)
#    generate_sw_description
#    ;;
#  --create-cert)
#    #create_cert
#    ;;
#  --create-cpio)
#    ;;
#  --scp)
#    #scp ${PRODUCT_NAME}_${CONTAINER_VER}.swu $CERT_NAME.cert.pem debian@192.168.13.10:
#    ;;
##  *)
##    echo "Invalid option"
##    ;;
#  esac
#  shift
#done


