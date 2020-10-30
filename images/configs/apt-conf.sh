# to remove install installing recommended and suggestes packages
cat > "/etc/apt/apt.conf" <<-__EOF__
APT::Install-Suggests "0";
APT::Install-Recommends "0";
__EOF__
